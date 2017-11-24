#include "stdafx.h"
#include "FileSys.h"
#include "PathScanner.h"
#include <ShlObj.h>


namespace SevenZip
{
namespace intl
{

class ScannerCallback : public PathScanner::Callback
{
private:

	std::vector< FilePathInfo > m_files;
	bool m_recursive;
	bool m_onlyFirst;

public:

	ScannerCallback( bool recursive, bool onlyFirst = false ) : m_recursive( recursive ), m_onlyFirst( onlyFirst ) {}
	const std::vector< FilePathInfo >& GetFiles() { return m_files; }

	virtual bool ShouldDescend( const FilePathInfo& directory ) { return m_recursive; }
	virtual void ExamineFile( const FilePathInfo& file, bool& exit )
	{
		m_files.push_back( file );
		if ( m_onlyFirst )
		{
			exit = true;
		}
	}
};


class IsEmptyCallback : public PathScanner::Callback
{
private:

	bool m_isEmpty;

public:

	IsEmptyCallback() : m_isEmpty( true ) {}
	bool IsEmpty() const { return m_isEmpty; }

	virtual bool ShouldDescend( const FilePathInfo& directory ) { return true; }
	virtual void ExamineFile( const FilePathInfo& file, bool& exit ) { m_isEmpty = false; exit = true; }
};


TString FileSys::GetPath( const TString& filePath )
{
	// Find the last "\" or "/" in the string and return it and everything before it.
	int index  = filePath.ReverseFind( _T( '\\' ) );
	int index2 = filePath.ReverseFind( _T( '/' ) );

	if ( index2 > index )
	{
		index = index2;
	}

	if ( index >= filePath.GetLength() - 1 )
	{
		// Last char is path sep, the whole thing is a path.
		return filePath;
	}
	else if ( index < 0 )
	{
		// No path sep.
		return TString();
	}
	else
	{
		return filePath.Left( index + 1 );
	}
}

TString FileSys::GetFileName( const TString& filePathOrName )
{
	// Find the last "\" or "/" in the string and return everything after it.
	int index  = filePathOrName.ReverseFind( _T( '\\' ) );
	int index2 = filePathOrName.ReverseFind( _T( '/' ) );

	if ( index2 > index )
	{
		index = index2;
	}

	if ( index >= filePathOrName.GetLength() - 1 )
	{
		// Last char is path sep, no filename.
		return TString();
	}
	else if ( index < 0 )
	{
		// No path sep, return the whole thing.
		return filePathOrName;
	}
	else
	{
		return filePathOrName.Mid( index + 1 );
	}
}

TString FileSys::AppendPath( const TString& left, const TString& right )
{
	if ( left.GetLength() == 0 )
	{
		return right;
	}

	TCHAR lastChar = left[ left.GetLength() - 1 ];
	if ( lastChar == _T( '\\' ) || lastChar == _T( '/' ) )
	{
		return left + right;
	}
	else
	{
		return left + _T( "\\" ) + right;
	}
}

TString FileSys::ExtractRelativePath( const TString& basePath, const TString& fullPath )
{
	if ( basePath.GetLength() >= fullPath.GetLength() )
	{
		return TString();
	}

	if ( basePath != fullPath.Left( basePath.GetLength() ) )
	{
		return TString();
	}

	return fullPath.Mid( basePath.GetLength() );
}

bool FileSys::DirectoryExists( const TString& path )
{
	DWORD attributes = GetFileAttributes( path );

	if ( attributes == INVALID_FILE_ATTRIBUTES )
	{
		return false;
	}
	else
	{
		return ( attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;
	}
}

bool FileSys::IsDirectoryEmptyRecursive( const TString& path )
{
	IsEmptyCallback cb;
	PathScanner::Scan( path, cb );
	return cb.IsEmpty();
}

bool FileSys::CreateDirectoryTree( const TString& path )
{
	int ret = SHCreateDirectoryEx( NULL, path, NULL );
	return ret == ERROR_SUCCESS;
}

std::vector< FilePathInfo > FileSys::GetFile( const TString& filePathOrName )
{
	TString path = FileSys::GetPath( filePathOrName );
	TString name = FileSys::GetFileName( filePathOrName );

	ScannerCallback cb( false, true );
	PathScanner::Scan( path, name, cb );
	return cb.GetFiles();
}

std::vector< FilePathInfo > FileSys::GetFilesInDirectory( const TString& directory, const TString& searchPattern, bool recursive )
{
	ScannerCallback cb( recursive );
	PathScanner::Scan( directory, searchPattern, cb );
	return cb.GetFiles();
}

CComPtr< IStream > FileSys::OpenFileToRead( const TString& filePath )
{
	CComPtr< IStream > fileStream;

	if ( FAILED( SHCreateStreamOnFileEx( filePath, STGM_READ, FILE_ATTRIBUTE_NORMAL, FALSE, NULL, &fileStream ) ) )
	{
		return NULL;
	}

	return fileStream;
}

CComPtr< IStream > FileSys::OpenFileToWrite( const TString& filePath )
{
	CComPtr< IStream > fileStream;

	if ( FAILED( SHCreateStreamOnFileEx( filePath, STGM_CREATE | STGM_WRITE, FILE_ATTRIBUTE_NORMAL, TRUE, NULL, &fileStream ) ) )
	{
		return NULL;
	}

	return fileStream;
}

}
}
