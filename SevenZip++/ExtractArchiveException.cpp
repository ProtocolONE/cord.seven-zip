#include "StdAfx.h"
#include "ExtractArchiveException.h"

namespace SevenZip
{

  ExtractArchiveException::ExtractArchiveException()
    : SevenZipException()
  {
  }

  ExtractArchiveException::ExtractArchiveException(const TString& message)
    : SevenZipException(message)
  {
  }

  ExtractArchiveException::ExtractArchiveException(HRESULT result)
    : SevenZipException()
    , _result(result)
  {

  }

  ExtractArchiveException::ExtractArchiveException(const TString& message, HRESULT result)
    : SevenZipException(message)
    , _result(result)
  {

  }

  ExtractArchiveException::~ExtractArchiveException()
  {
  }

  HRESULT ExtractArchiveException::result() const
  {
    return this->_result;
  }
}
