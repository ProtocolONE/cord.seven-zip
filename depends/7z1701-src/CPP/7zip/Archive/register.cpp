#include "StdAfx.h"

#include "register.h"

#define DECLARE_REG_GETTER(nm) namespace nm { extern void* __stdcall getRegInfo(); }
#define CALL_REG_GETTER(nm) NArchive::nm::getRegInfo();

#define REGISTER_HASHER_GETTER_NAME(x) CRegHasherGetter_ ## x
#define CALL_HASHER_GETTER_NAME(x) \
  extern void* __stdcall REGISTER_HASHER_GETTER_NAME(x)(); \
  (CRegHasherGetter_ ## x)();

struct CCRCTableInit;
extern struct CCRCTableInit g_CRCTableInit;

void* initCrc() {
  return (void*)&g_CRCTableInit;
}

#define REGISTER_CODEC_GETTER_NAME(x) CRegisterCodecGetter ## x

#define DECLARE_CODEC_GETTER(nm, x) namespace nm { extern void* __stdcall REGISTER_CODEC_GETTER_NAME(x)(); }
#define CALL_CODEC_GETTER_NAME(nm, x) NCompress:: ## nm ## ::CRegisterCodecGetter ## x();

#define DECLARE_CODEC_GETTER2(x) extern void* __stdcall REGISTER_CODEC_GETTER_NAME(x)();
#define CALL_CODEC_GETTER_NAME2(x) NCompress::CRegisterCodecGetter ## x();

namespace NArchive {
  DECLARE_REG_GETTER(N7z);
  //DECLARE_REG_GETTER(NCab);
  //DECLARE_REG_GETTER(NIso);
  //DECLARE_REG_GETTER(NNsis);
  //DECLARE_REG_GETTER(NTar);
  //DECLARE_REG_GETTER(NWim);
  //DECLARE_REG_GETTER(NZip);
}

namespace NCompress {
  DECLARE_CODEC_GETTER(NLzma, LZMA);
  DECLARE_CODEC_GETTER(NLzma2, LZMA2);
  //DECLARE_CODEC_GETTER(NBZip2, BZip2);
  //DECLARE_CODEC_GETTER(NDeflate, Deflate);
  //DECLARE_CODEC_GETTER(NDeflate, Deflate64);
  //DECLARE_CODEC_GETTER(NPpmd, PPMD);
  
  DECLARE_CODEC_GETTER2(Copy);
}

void registerAll()
{
  CALL_REG_GETTER(N7z);
  //CALL_REG_GETTER(NCab);
  //CALL_REG_GETTER(NIso);
  //CALL_REG_GETTER(NNsis);
  //CALL_REG_GETTER(NTar);
  //CALL_REG_GETTER(NWim);
  //CALL_REG_GETTER(NZip);

  CALL_HASHER_GETTER_NAME(CCrcHasher);
  CALL_HASHER_GETTER_NAME(CXzCrc64Hasher);
  //CALL_HASHER_GETTER_NAME(CSha1Hasher);
  CALL_HASHER_GETTER_NAME(CSha256Hasher);

  CALL_CODEC_GETTER_NAME(NLzma, LZMA);
  CALL_CODEC_GETTER_NAME(NLzma2, LZMA2);
  //CALL_CODEC_GETTER_NAME(NBZip2, BZip2);
  //CALL_CODEC_GETTER_NAME(NDeflate, Deflate);
  //CALL_CODEC_GETTER_NAME(NDeflate, Deflate64);
  //CALL_CODEC_GETTER_NAME(NPpmd, PPMD);

  CALL_CODEC_GETTER_NAME2(Copy);

  //NBz2::registerArcBZip2();
  //NGz::registerArcGZip();
  //NLzma::NLzmaAr::registerArcLzma();
  //NLzma::NLzma86Ar::registerArcLzma86();
  //NSplit::registerArcSplit();
  //NXz::registerArcxz();
  //NZ::registerArcZ();
  
  //registerArc7z();
  //registerArcCab();
  //registerArcTar();
  //registerArcZip();
  //registerCodecBCJ2();
  //registerCodecBCJ();
  //registerCodecBCJ();
  //registerCodecByteSwap();
  //registerCodecBZip2();
  //registerCodecCopy();
  //registerCodecDeflate64();
  //registerCodecDeflate();
  //registerCodecDelta();
  //registerCodecLZMA2();
  //registerCodecLZMA();
  //registerCodecPPMD();
  //registerCodec7zAES();
  
  initCrc();
}

