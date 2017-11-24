#include "stdafx.h"
#include "SevenZipException.h"


namespace SevenZip
{

TString StrFmt( const TCHAR* format, ... )
{
	TString result;
	TCHAR*	buffer;
	int		sz;
	va_list	args;

	va_start( args, format );

	sz		= _vsctprintf( format, args ) + 1;
	buffer	= result.GetBuffer( sz );
	sz		= _vsntprintf_s( buffer, sz, _TRUNCATE, format, args );
	result	.ReleaseBuffer( sz );

	va_end( args );

	return result;
}

TString GetWinErrMsg( const TString& contextMessage, DWORD lastError )
{
	// TODO: use FormatMessage to get the appropriate message from the 
	return StrFmt( _T( "%s: GetLastError = %lu" ), contextMessage.GetString(), lastError );
}

TString GetCOMErrMsg( const TString& contextMessage, HRESULT lastError )
{
	// TODO: use FormatMessage to get the appropriate message from the 
	return StrFmt( _T( "%s: HRESULT = 0x%08X" ), contextMessage.GetString(), lastError );
}

SevenZipException::SevenZipException()
{
}

SevenZipException::SevenZipException( const TString& message )
	: m_message( message )
{
}

SevenZipException::~SevenZipException()
{
}

const TString& SevenZipException::GetMessage() const
{
	return m_message;
}

}
