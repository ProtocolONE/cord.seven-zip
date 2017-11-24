#pragma once


#include "SevenZipLibrary.h"
#include "CompressionFormat.h"

#include <unordered_map>

namespace SevenZip
{
	class SevenZipExtractor
	{
	private:

		const SevenZipLibrary& m_library;
		TString m_archivePath;
		CompressionFormatEnum m_format;

	public:

    SevenZipExtractor(const SevenZipLibrary& library);
		SevenZipExtractor( const SevenZipLibrary& library, const TString& archivePath );
		virtual ~SevenZipExtractor();

		void SetCompressionFormat( const CompressionFormatEnum& format );

		virtual void ExtractArchive( const TString& directory );
    void ExtractArchiveFromMemory(std::vector<unsigned char> buffer, const TString& destDirectory);
    void ExtractArchiveFromMemory(std::vector<unsigned char> buffer, std::unordered_map<std::wstring, std::vector<unsigned char> >& result);

	private:

		void ExtractArchive( const CComPtr< IStream >& archiveStream, const TString& directory );
	};
}
