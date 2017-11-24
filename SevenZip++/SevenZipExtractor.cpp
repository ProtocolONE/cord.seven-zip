#include "stdafx.h"
#include "SevenZipExtractor.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveOpenCallback.h"
#include "ArchiveExtractCallback.h"
#include "InStreamWrapper.h"

#include "OpenArchiveException.h"
#include "ExtractArchiveException.h"

namespace SevenZip
{

using namespace intl;


CComPtr< IInArchive > GetArchiveReader( const SevenZipLibrary& library, const CompressionFormatEnum& format )
{
	const GUID* guid = NULL;

	switch ( format )
	{
	case CompressionFormat::Zip:
		guid = &CLSID_CFormatZip;
		break;

	default:
		guid = &CLSID_CFormat7z;
		break;
	}
	
	CComPtr< IInArchive > archive;
	library.CreateObject( *guid, IID_IInArchive, reinterpret_cast< void** >( &archive ) );
	return archive;
}


SevenZipExtractor::SevenZipExtractor( const SevenZipLibrary& library, const TString& archivePath )
	: m_library( library )
	, m_archivePath( archivePath )
	, m_format( CompressionFormat::SevenZip )
{
}

SevenZipExtractor::~SevenZipExtractor()
{
}

void SevenZipExtractor::SetCompressionFormat( const CompressionFormatEnum& format )
{
	m_format = format;
}

void SevenZipExtractor::ExtractArchive( const TString& destDirectory )
{
	CComPtr< IStream > fileStream = FileSys::OpenFileToRead( m_archivePath );
	if ( fileStream == NULL )
	{
    throw OpenArchiveException(StrFmt( _T( "Could not open archive \"%s\"" ), m_archivePath.GetString() ));
	}

	ExtractArchive( fileStream, destDirectory );
}

void SevenZipExtractor::ExtractArchive( const CComPtr< IStream >& archiveStream, const TString& destDirectory )
{
	CComPtr< IInArchive > archive = GetArchiveReader( m_library, m_format );
	CComPtr< InStreamWrapper > inFile = new InStreamWrapper( archiveStream );
	CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback();

	HRESULT hr = archive->Open( inFile, 0, openCallback );
	if ( hr != S_OK )
	{
    throw OpenArchiveException(GetCOMErrMsg( _T( "Open archive" ), hr ), hr);
		//throw SevenZipException( GetCOMErrMsg( _T( "Open archive" ), hr ) );
	}

	CComPtr< ArchiveExtractCallback > extractCallback = new ArchiveExtractCallback( archive, destDirectory );

	hr = archive->Extract( NULL, -1, false, extractCallback );
	if ( hr != S_OK ) // returning S_FALSE also indicates error
	{
    throw ExtractArchiveException( GetCOMErrMsg( _T( "Extract archive" ), hr ), hr );
		//throw SevenZipException( GetCOMErrMsg( _T( "Extract archive" ), hr ) );
	}
}

}
