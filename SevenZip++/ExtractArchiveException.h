#pragma once

#include "SevenZipException.h"

namespace SevenZip
{

  class ExtractArchiveException : public SevenZipException
  {
  public:

    ExtractArchiveException();
    ExtractArchiveException(const TString& message);

    ExtractArchiveException(HRESULT result);
    ExtractArchiveException(const TString& message, HRESULT result);
    virtual ~ExtractArchiveException();
    HRESULT result() const;

  private:
    HRESULT _result;
  };

}
