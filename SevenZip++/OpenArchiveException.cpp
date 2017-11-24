#include "StdAfx.h"
#include "OpenArchiveException.h"

namespace SevenZip
{
  OpenArchiveException::OpenArchiveException()
    : SevenZipException()
  {
  }

  OpenArchiveException::OpenArchiveException(const TString& message)
    : SevenZipException(message)
  {
  }

  OpenArchiveException::OpenArchiveException(HRESULT result)
    : SevenZipException()
    , _result(result)
  {

  }

  OpenArchiveException::OpenArchiveException(const TString& message, HRESULT result)
    : SevenZipException(message)
    , _result(result)
  {

  }

  OpenArchiveException::~OpenArchiveException()
  {
  }

  HRESULT OpenArchiveException::result() const
  {
    return this->_result;
  }

}