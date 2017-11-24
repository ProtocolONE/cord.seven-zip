// This file is based on the following file from the LZMA SDK (http://www.7-zip.org/sdk.html):
//   ./CPP/7zip/Guid.txt
#pragma once


namespace SevenZip
{
namespace intl
{
	// IStream.h

	// {23170F69-40C1-278A-0000-000300010000}
	DEFINE_GUID(IID_ISequentialInStream,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000300010000}
	DEFINE_GUID(IID_ISequentialOutStream,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000300030000}
	DEFINE_GUID(IID_IInStream,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000300040000}
	DEFINE_GUID(IID_IOutStream,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000300060000}
	DEFINE_GUID(IID_IStreamGetSize,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00);

	// ICoder.h
	// {23170F69-40C1-278A-0000-000400040000}
	DEFINE_GUID(IID_ICompressProgressInfo,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00);

	// IPassword.h

	// {23170F69-40C1-278A-0000-000500100000}
	DEFINE_GUID(IID_ICryptoGetTextPassword,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000500110000}
	DEFINE_GUID(IID_ICryptoGetTextPassword2,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x11, 0x00, 0x00);

	// IArchive.h

	// {23170F69-40C1-278A-0000-000600030000}
	DEFINE_GUID(IID_ISetProperties,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000600100000}
	DEFINE_GUID(IID_IArchiveOpenCallback,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x10, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000600200000}
	DEFINE_GUID(IID_IArchiveExtractCallback,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x20, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000600600000}
	DEFINE_GUID(IID_IInArchive,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x60, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000600800000}
	DEFINE_GUID(IID_IArchiveUpdateCallback,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x80, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000600820000}
	DEFINE_GUID(IID_IArchiveUpdateCallback2,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0x82, 0x00, 0x00);

	// {23170F69-40C1-278A-0000-000600A00000}
	DEFINE_GUID(IID_IOutArchive,
		0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, 0xA0, 0x00, 0x00);

	// Handler GUIDs

	// {23170F69-40C1-278A-1000-000110010000}
	DEFINE_GUID(CLSID_CFormatZip,
		0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x01, 0x00, 0x00);

	// {23170F69-40C1-278A-1000-000110070000}
	DEFINE_GUID(CLSID_CFormat7z,
		0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00);
}
}
