#include "stdafx.h"

#include "MemoryInStream.h"

#include <algorithm>

namespace SevenZip {
  namespace intl {

    MemoryInStream::MemoryInStream(const std::vector<unsigned char>& memory)
      : _buffer(memory)
      , _offset(0)
      , _refCount(0)
    {
    }

    MemoryInStream::~MemoryInStream()
    {
    }

    HRESULT STDMETHODCALLTYPE MemoryInStream::QueryInterface(REFIID iid, void** ppvObject)
    {
      if (iid == __uuidof(IUnknown))
      {
        *ppvObject = reinterpret_cast<IUnknown*>(this);
        AddRef();
        return S_OK;
      }

      if (iid == IID_ISequentialInStream)
      {
        *ppvObject = static_cast<ISequentialInStream*>(this);
        AddRef();
        return S_OK;
      }

      if (iid == IID_IInStream)
      {
        *ppvObject = static_cast<IInStream*>(this);
        AddRef();
        return S_OK;
      }

      if (iid == IID_IStreamGetSize)
      {
        *ppvObject = static_cast<IStreamGetSize*>(this);
        AddRef();
        return S_OK;
      }

      return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE MemoryInStream::AddRef()
    {
      return static_cast<ULONG>(InterlockedIncrement(&_refCount));
    }

    ULONG STDMETHODCALLTYPE MemoryInStream::Release()
    {
      ULONG res = static_cast<ULONG>(InterlockedDecrement(&_refCount));
      if (res == 0)
        delete this;

      return res;
    }

    STDMETHODIMP MemoryInStream::Read(void* data, UInt32 size, UInt32* processedSize)
    {
      if (data == nullptr || processedSize == nullptr)
        return STG_E_INVALIDPOINTER;

      size_t remainBytes = 0;
      if (this->_offset < this->_buffer.size())
        remainBytes = this->_buffer.size() - this->_offset;

      size_t needToRead = min(size, remainBytes);
      bool endOfStream = needToRead != size;
      bool copySuccess = true;

      if (needToRead > 0)
        memcpy_s(data, size, this->_buffer.data() + this->_offset, needToRead);

      if (processedSize != nullptr)
        *processedSize = needToRead;

      this->_offset += needToRead;
      return endOfStream ? S_FALSE : S_OK;
    }

    STDMETHODIMP MemoryInStream::Seek(Int64 offset, UInt32 seekOrigin, UInt64* newPosition)
    {
      uint64_t size = this->_buffer.size();

      if (seekOrigin == STREAM_SEEK_SET) {
        uint64_t realOffset = (uint64_t)offset;
        if (realOffset > size)
          return STG_E_INVALIDFUNCTION;
        
        this->_offset = (size_t)realOffset;
        if (newPosition != nullptr)
          *newPosition = this->_offset;

        return S_OK;
      }

      if (seekOrigin == STREAM_SEEK_CUR) {
        int64_t currentPosition = this->_offset;
        currentPosition += offset;
        if (currentPosition < 0 || currentPosition > size)
          return STG_E_INVALIDFUNCTION;

        this->_offset = (size_t)currentPosition;
        if (newPosition != nullptr)
          *newPosition = this->_offset;

        return S_OK;
      }

      if (seekOrigin == STREAM_SEEK_END) {
        int64_t currentPosition = size;
        currentPosition += offset;
        if (currentPosition < 0 || currentPosition > size)
          return STG_E_INVALIDFUNCTION;

        this->_offset = (size_t)currentPosition;
        if (newPosition != nullptr)
          *newPosition = this->_offset;

        return S_OK;
      }


      return STG_E_INVALIDFUNCTION;
    }

    STDMETHODIMP MemoryInStream::GetSize(UInt64* size)
    {
      if (size == nullptr)
        return STG_E_INVALIDPOINTER;

      *size = this->_buffer.size();
      return S_OK;
    }


  }
}