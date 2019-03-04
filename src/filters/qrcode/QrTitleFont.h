/*++

File Name:

    QrTitleFont.h

Modified from:

    wmfont.h

Abstract:

QR code's TITLE font definition. The CQrTitleFont class is responsible
for managing the font resource for the TITLE of QR code. This implements
the IResWriter interface so that the font can be added to the resource
cache.

Known Issues:

The QR code's TITLE font does not yet use the Uniscript interface to retrieve glyph
indices.

--*/

#pragma once

#include "rescache.h"

class CQrTitleFont : public IResWriter
{
public:
	CQrTitleFont(
		VOID
	);

	~CQrTitleFont();

	HRESULT
		WriteData(
			_In_ IPartBase*         pResource,
			_In_ IPrintWriteStream* pStream
		);

	HRESULT
		GetKeyName(
			_Outptr_ BSTR* pbstrKeyName
		);

	HRESULT
		GetResURI(
			_Outptr_ BSTR* pbstrResURI
		);

private:
	HRESULT
		SetFont(
			VOID
		);

	VOID
		UnsetFont(
			VOID
		);

private:
	HDC                  m_hDC;

	HFONT                m_hFont;

	HFONT                m_hOldFont;

	CComBSTR             m_bstrFaceName;

};

