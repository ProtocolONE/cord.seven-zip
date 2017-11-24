#pragma once
#include <7zip/IStream.h>
#include <vector>
#include <stdint.h>

namespace SevenZip {
  namespace intl {

    class MemoryInStream : public IInStream, public IStreamGetSize
    {
    private:
      long  _refCount;
      std::vector<unsigned char> _buffer;
      size_t _offset;

    public:
      MemoryInStream(const std::vector<unsigned char>& memory);
      virtual ~MemoryInStream();

      STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject);
      STDMETHOD_(ULONG, AddRef)();
      STDMETHOD_(ULONG, Release)();

      // ISequentialInStream
      STDMETHOD(Read)(void* data, UInt32 size, UInt32* processedSize);

      // IInStream
      STDMETHOD(Seek)(Int64 offset, UInt32 seekOrigin, UInt64* newPosition);

      // IStreamGetSize
      STDMETHOD(GetSize)(UInt64* size);
    };

  }
}