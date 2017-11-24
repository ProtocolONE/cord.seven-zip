#pragma once


#include <7zip/IStream.h>


namespace SevenZip
{
namespace intl
{
  class MemoryOutStream : public IOutStream
  {
  private:

    long  m_refCount;
    std::vector<unsigned char>& _buffer;
    size_t _offset;
  public:

    MemoryOutStream(std::vector<unsigned char> & result);
    virtual ~MemoryOutStream();

    STDMETHOD(QueryInterface)( REFIID iid, void** ppvObject );
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // ISequentialOutStream
    STDMETHOD(Write)( const void* data, UInt32 size, UInt32* processedSize );

    // IOutStream
    STDMETHOD(Seek)( Int64 offset, UInt32 seekOrigin, UInt64* newPosition );
    STDMETHOD(SetSize)( UInt64 newSize );
  };
}
}
