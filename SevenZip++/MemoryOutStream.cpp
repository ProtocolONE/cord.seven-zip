#include "stdafx.h"
#include "MemoryOutStream.h"

#include <stdint.h>

namespace SevenZip
{
namespace intl
{

  MemoryOutStream::MemoryOutStream(std::vector<unsigned char> & result)
	: m_refCount( 0 )
  , _buffer(result)
  , _offset(0)
{
}

MemoryOutStream::~MemoryOutStream()
{
}

HRESULT STDMETHODCALLTYPE MemoryOutStream::QueryInterface( REFIID iid, void** ppvObject )
{ 
	if ( iid == __uuidof( IUnknown ) )
	{
		*ppvObject = static_cast< IUnknown* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_ISequentialOutStream )
	{
		*ppvObject = static_cast< ISequentialOutStream* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_IOutStream )
	{
		*ppvObject = static_cast< IOutStream* >( this );
		AddRef();
		return S_OK;
	}
	
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE MemoryOutStream::AddRef()
{
	return static_cast< ULONG >( InterlockedIncrement( &m_refCount ) );
}

ULONG STDMETHODCALLTYPE MemoryOutStream::Release()
{
	ULONG res = static_cast< ULONG >( InterlockedDecrement( &m_refCount ) );
	if ( res == 0 )
	{
		delete this;
	}
	return res;
}

STDMETHODIMP MemoryOutStream::Write( const void* data, UInt32 size, UInt32* processedSize )
{
  if (this->_buffer.size() < this->_offset + size) {
    this->_buffer.resize(this->_offset + size);
  }
  memcpy_s(this->_buffer.data() + this->_offset, this->_buffer.size() - this->_offset,
    data, size);

  if (processedSize != nullptr)
    *processedSize = size;

  return S_OK;
}

STDMETHODIMP MemoryOutStream::Seek( Int64 offset, UInt32 seekOrigin, UInt64* newPosition )
{
  int64_t size = this->_buffer.size();

  if (seekOrigin == STREAM_SEEK_SET) {
    uint64_t realOffset = (uint64_t)offset;
    if (realOffset > (uint64_t)size)
      return STG_E_INVALIDFUNCTION;

    this->_offset = (size_t)realOffset;
    if (newPosition != nullptr)
      *newPosition = this->_offset;

    return S_OK;
  }

  if (seekOrigin == STREAM_SEEK_CUR) {
    int64_t currentPosition = this->_offset;
    currentPosition += offset;
    if (currentPosition < 0 || currentPosition > size)
      return STG_E_INVALIDFUNCTION;

    this->_offset = (size_t)currentPosition;
    if (newPosition != nullptr)
      *newPosition = this->_offset;

    return S_OK;
  }

  if (seekOrigin == STREAM_SEEK_END) {
    int64_t currentPosition = size;
    currentPosition += offset;
    if (currentPosition < 0 || currentPosition > size)
      return STG_E_INVALIDFUNCTION;

    this->_offset = (size_t)currentPosition;
    if (newPosition != nullptr)
      *newPosition = this->_offset;

    return S_OK;
  }


  return STG_E_INVALIDFUNCTION;
}

STDMETHODIMP MemoryOutStream::SetSize( UInt64 newSize )
{
  _buffer.resize((uint32_t)newSize);
  return S_OK;
}

}
}
