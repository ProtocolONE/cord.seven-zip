#pragma once

#include "SevenZipException.h"
namespace SevenZip
{
  class OpenArchiveException : public SevenZipException
  {
  public:
    OpenArchiveException();
    OpenArchiveException(const TString& message);

    OpenArchiveException(HRESULT result);
    OpenArchiveException(const TString& message, HRESULT result);
    virtual ~OpenArchiveException();

    HRESULT result() const;

  private:
    HRESULT _result;
  };
}