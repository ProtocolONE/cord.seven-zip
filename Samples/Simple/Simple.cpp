

#include <stdio.h>
#include <tchar.h>
#include "../../SevenZip++/SevenZipCompressor.h"
#include "../../SevenZip++/SevenZipExtractor.h"
#include "../../SevenZip++/CompressionFormat.h"

#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>

int PrintUsage()
{
	_tprintf(_T("Simple.exe [cx] ...\n"));
	_tprintf(_T("  c <archiveName> <targetDirectory>      -- Creates an archive.\n"));
	_tprintf(_T("  x <archiveName> <destinationDirectory> -- Extracts an archive.\n\n"));
	return 0;
}

int CreateArchive(int argc, TCHAR** argv)
{
	if (argc < 4)
	{
		return PrintUsage();
	}

	const TCHAR* archiveName = argv[2];
	const TCHAR* targetDir = argv[3];
	// Note I'm lazily assuming the target is a directory rather than a file.

	SevenZip::SevenZipLibrary lib;
	lib.Load();

	SevenZip::SevenZipCompressor compressor(lib, archiveName);
	compressor.CompressDirectory(targetDir);
	return 0;
}

std::vector<unsigned char> readFile(const TCHAR* filename)
{
  DWORD cbRead;
  HANDLE hFile = ::CreateFile(filename,
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

  DWORD hsize = 0;
  DWORD size = GetFileSize(hFile, &size);

  std::vector<unsigned char> result;
  result.resize(size);

  ::ReadFile(hFile, result.data(), result.size(), &cbRead, NULL);
  ::CloseHandle(hFile);

  return result;
}

int ExtractArchive(int argc, TCHAR** argv)
{
	if (argc < 4)
	{
		return PrintUsage();
	}

	const TCHAR* archiveName = argv[2];
	const TCHAR* destination = argv[3];

	SevenZip::SevenZipLibrary lib;
	lib.Load();

	SevenZip::SevenZipExtractor extractor(lib, archiveName);
	//extractor.ExtractArchive(destination);
	
  std::vector<unsigned char> memoryArchive = readFile(archiveName);

  std::unordered_map<std::wstring, std::vector<unsigned char> > result;
  //SevenZip::SevenZipExtractor extractor(lib);
  //extractor.ExtractArchive(destination);
  //extractor.ExtractArchiveFromMemory(memoryArchive, destination);
  extractor.ExtractArchiveFromMemory(memoryArchive, result);

  std::cout << result[L"update.crc"].data() << std::endl;
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		return PrintUsage();
	}

	try
	{
		switch (argv[1][0])
		{
		case _T('c'):
			return CreateArchive(argc, argv);
		case _T('x'):
			return ExtractArchive(argc, argv);
		default:
			break;
		}
	}
	catch (SevenZip::SevenZipException& ex)
	{
		_tprintf(_T("Error: %s\n"), ex.GetMessage().GetString());
	}

	return PrintUsage();
}
