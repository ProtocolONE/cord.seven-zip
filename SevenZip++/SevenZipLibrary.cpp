#include "stdafx.h"
#include "SevenZipLibrary.h"


#define STATIC_7ZIP 1

#ifdef STATIC_7ZIP
#include <7zip/Archive/register.h>

EXTERN_C UINT32 __stdcall CreateObject(const GUID *clsid, const GUID *iid, void **outObject);

static void initArchives()
{
  registerAll();
}

#endif;


namespace SevenZip
{

#ifndef STATIC_7ZIP
const TString DefaultLibraryPath = _T( "7za.dll" );
#endif 

SevenZipLibrary::SevenZipLibrary()
	: m_dll( NULL )
	, m_func( NULL )
{
}

SevenZipLibrary::~SevenZipLibrary()
{
#ifndef STATIC_7ZIP
	Free();
#endif
}

void SevenZipLibrary::Load()
{
#ifdef STATIC_7ZIP
  //typedef UINT32 (WINAPI * CreateObjectFunc)( const GUID* clsID, const GUID* interfaceID, void** outObject );
  //  HRESULT (__stdcall *)(const GUID *,const GUID *,void **)
  m_func = ::CreateObject;
  initArchives();
#else
	Load( DefaultLibraryPath );
#endif
}

void SevenZipLibrary::Load( const TString& libraryPath )
{
#ifdef STATIC_7ZIP
  Load();
#else
	Free();
	m_dll = LoadLibrary( libraryPath );
	if ( m_dll == NULL )
	{
		throw SevenZipException( GetWinErrMsg( _T( "LoadLibrary" ), GetLastError() ) );
	}

	m_func = reinterpret_cast< CreateObjectFunc >( GetProcAddress( m_dll, "CreateObject" ) );
	if ( m_func == NULL )
	{
		Free();
		throw SevenZipException( _T( "Loaded library is missing required CreateObject function" ) );
	}
#endif
}

void SevenZipLibrary::Free()
{
#ifndef STATIC_7ZIP
	if ( m_dll != NULL )
	{
		FreeLibrary( m_dll );
		m_dll = NULL;
		m_func = NULL;
	}
#endif
}

void SevenZipLibrary::CreateObject( const GUID& clsID, const GUID& interfaceID, void** outObject ) const
{
	if ( m_func == NULL )
	{
		throw SevenZipException( _T( "Library is not loaded" ) );
	}

	HRESULT hr = m_func( &clsID, &interfaceID, outObject );
	if ( FAILED( hr ) )
	{
		throw SevenZipException( GetCOMErrMsg( _T( "CreateObject" ), hr ) );
	}
}

}
