// This file is based on the following file from the LZMA SDK (http://www.7-zip.org/sdk.html):
//   ./CPP/7zip/UI/Client7z/Client7z.cpp
#include "StdAfx.h"
#include "ArchiveExtractToMemoryCallback.h"
#include "PropVariant.h"
#include "FileSys.h"
#include "OutStreamWrapper.h"
#include <comdef.h>

#include "MemoryOutStream.h"

namespace SevenZip
{
namespace intl
{

const TString EmptyFileAlias = _T( "[Content]" );


ArchiveExtractToMemoryCallback::ArchiveExtractToMemoryCallback(const CComPtr< IInArchive >& archiveHandler, std::unordered_map<std::wstring, std::vector<unsigned char> >& result)
  : m_refCount(0)
  , m_archiveHandler(archiveHandler)
  , _resultFiles(result)
{
}

ArchiveExtractToMemoryCallback::~ArchiveExtractToMemoryCallback()
{
}

STDMETHODIMP ArchiveExtractToMemoryCallback::QueryInterface( REFIID iid, void** ppvObject )
{
  if ( iid == __uuidof( IUnknown ) )
  {
    *ppvObject = reinterpret_cast< IUnknown* >( this );
    AddRef();
    return S_OK;
  }

  if (iid == IID_IArchiveExtractCallback)
  {
    *ppvObject = static_cast< IArchiveExtractCallback* >(this);
    AddRef();
    return S_OK;
  }

  if ( iid == IID_ICryptoGetTextPassword )
  {
    *ppvObject = static_cast< ICryptoGetTextPassword* >( this );
    AddRef();
    return S_OK;
  }

  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) ArchiveExtractToMemoryCallback::AddRef()
{
  return static_cast< ULONG >( InterlockedIncrement( &m_refCount ) );
}

STDMETHODIMP_(ULONG) ArchiveExtractToMemoryCallback::Release()
{
  ULONG res = static_cast< ULONG >( InterlockedDecrement( &m_refCount ) );
  if ( res == 0 )
  {
    delete this;
  }
  return res;
}

STDMETHODIMP ArchiveExtractToMemoryCallback::SetTotal( UInt64 size )
{
  return S_OK;
}

STDMETHODIMP ArchiveExtractToMemoryCallback::SetCompleted( const UInt64* completeValue )
{
  return S_OK;
}

STDMETHODIMP ArchiveExtractToMemoryCallback::GetStream( UInt32 index, ISequentialOutStream** outStream, Int32 askExtractMode )
{
  try
  {
    // Retrieve all the various properties for the file at this index.
    GetPropertyFilePath( index );
    if ( askExtractMode != NArchive::NExtract::NAskMode::kExtract )
    {
      return S_OK;
    }

    GetPropertyAttributes( index );
    GetPropertyIsDir( index );
    GetPropertyModifiedTime( index );
    GetPropertySize( index );
  }
  catch ( _com_error& ex )
  {
    return ex.Error();
  }

  if ( m_isDir )
    return S_OK;

  std::wstring relativePath(m_relPath);
  std::vector<unsigned char> & buffer(this->_resultFiles[relativePath]);

  if (m_hasNewFileSize)
    buffer.resize((size_t)m_newFileSize);

  CComPtr< MemoryOutStream > wrapperStream = new MemoryOutStream(buffer);
  *outStream = wrapperStream.Detach();

  return S_OK;
}

STDMETHODIMP ArchiveExtractToMemoryCallback::PrepareOperation( Int32 askExtractMode )
{
  return S_OK;
}

STDMETHODIMP ArchiveExtractToMemoryCallback::SetOperationResult( Int32 operationResult )
{
  return S_OK;
}

STDMETHODIMP ArchiveExtractToMemoryCallback::CryptoGetTextPassword( BSTR* password )
{
  // TODO: support passwords
  return E_ABORT;
}

void ArchiveExtractToMemoryCallback::GetPropertyFilePath( UInt32 index )
{
  CPropVariant prop;
  HRESULT hr = m_archiveHandler->GetProperty( index, kpidPath, &prop );
  if ( hr != S_OK )
  {
    _com_issue_error( hr );
  }

  if ( prop.vt == VT_EMPTY )
  {
    m_relPath = EmptyFileAlias;
  }
  else if ( prop.vt != VT_BSTR )
  {
    _com_issue_error( E_FAIL );
  }
  else
  {
    m_relPath = prop.bstrVal;
  }
}

void ArchiveExtractToMemoryCallback::GetPropertyAttributes( UInt32 index )
{
  CPropVariant prop;
  HRESULT hr = m_archiveHandler->GetProperty( index, kpidAttrib, &prop );
  if ( hr != S_OK )
  {
    _com_issue_error( hr );
  }

  if ( prop.vt == VT_EMPTY )
  {
    m_attrib = 0;
    m_hasAttrib = false;
  }
  else if ( prop.vt != VT_UI4 )
  {
    _com_issue_error( E_FAIL );
  }
  else
  {
    m_attrib = prop.ulVal;
    m_hasAttrib = true;
  }
}

void ArchiveExtractToMemoryCallback::GetPropertyIsDir( UInt32 index )
{
  CPropVariant prop;
  HRESULT hr = m_archiveHandler->GetProperty( index, kpidIsDir, &prop );
  if ( hr != S_OK )
  {
    _com_issue_error( hr );
  }

  if ( prop.vt == VT_EMPTY )
  {
    m_isDir = false;
  }
  else if ( prop.vt != VT_BOOL )
  {
    _com_issue_error( E_FAIL );
  }
  else
  {
    m_isDir = prop.boolVal != VARIANT_FALSE;
  }
}

void ArchiveExtractToMemoryCallback::GetPropertyModifiedTime( UInt32 index )
{
  CPropVariant prop;
  HRESULT hr = m_archiveHandler->GetProperty( index, kpidMTime, &prop );
  if ( hr != S_OK )
  {
    _com_issue_error( hr );
  }

  if ( prop.vt == VT_EMPTY )
  {
    m_hasModifiedTime = false;
  }
  else if ( prop.vt != VT_FILETIME )
  {
    _com_issue_error( E_FAIL );
  }
  else
  {
    m_modifiedTime = prop.filetime;
    m_hasModifiedTime = true;
  }
}

void ArchiveExtractToMemoryCallback::GetPropertySize( UInt32 index )
{
  CPropVariant prop;
  HRESULT hr = m_archiveHandler->GetProperty( index, kpidSize, &prop );
  if ( hr != S_OK )
  {
    _com_issue_error( hr );
  }

  switch ( prop.vt )
  {
  case VT_EMPTY:
    m_hasNewFileSize = false;
    return;
  case VT_UI1: 
    m_newFileSize = prop.bVal;
    break;
  case VT_UI2:
    m_newFileSize = prop.uiVal;
    break;
  case VT_UI4:
    m_newFileSize = prop.ulVal;
    break;
  case VT_UI8:
    m_newFileSize = (UInt64)prop.uhVal.QuadPart;
    break;
  default:
    _com_issue_error( E_FAIL );
  }

  m_hasNewFileSize = true;
}

}
}
