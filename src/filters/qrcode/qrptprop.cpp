/*++

File Name:

qrptprop.cpp

Abstract:

QR Code properties class implementation. The QR Code properties class
is responsible for holding and controling QR Code properties.

--*/

#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "xdstring.h"
#include "qrptprop.h"

using XDPrintSchema::PageQRCode::QRCodeData;
using XDPrintSchema::PageQRCode::EQRCodeOption;
using XDPrintSchema::PageQRCode::DisableQRCode;
using XDPrintSchema::PageQRCode::EnableQRCode;

/*++

Routine Name:

CQRCodePTProperties::CQRCodePTProperties

Routine Description:

Constructor for the CQRCodePTProperties PrintTicket properties class

Arguments:

qrcodeData - Structure containing QR Codeode properties read from the PrintTicket

Return Value:

None

--*/
CQRCodePTProperties::CQRCodePTProperties(
	_In_ CONST QRCodeData& qrcodeData
) :
	m_qrcodeData(qrcodeData)
{
}

/*++

Routine Name:

CQRCodePTProperties::~CQRCodePTProperties

Routine Description:

Default destructor for the CQRCodePTProperties class

Arguments:

None

Return Value:

None

--*/
CQRCodePTProperties::~CQRCodePTProperties()
{
}

/*++

Routine Name:

CQRCodePTProperties::GetQROption

Routine Description:

Method to obtain the QR Code option to identify as enable and disable

Arguments:

pQROption - Enumerated type to indicate the QR Code Option

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTProperties::GetQROption(
	_Out_ EQRCodeOption* pQROption
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pQROption, E_POINTER)))
	{
		*pQROption = m_qrcodeData.QRCodeOption;
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodePTProperties::GetQRScale

Routine Description:

Method to obtain the scale of QR code

Arguments:

pQRScale - Variable which is set to the QR code scale value

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTProperties::GetQRScale(
	_Out_ INT* pQRScale
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pQRScale, E_POINTER)))
	{
		*pQRScale = m_qrcodeData.scale;
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodePTProperties::GetQRScale

Routine Description:

Method to obtain the QR code scale value as a string

Arguments:

pbstrQRSacle - String which is set to contain the QR code scale value

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTProperties::GetQRScale(
	_Outptr_ BSTR* pbstrQRScale
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pbstrQRScale, E_POINTER)))
	{
		*pbstrQRScale = NULL;

		INT qrscale = 0;
		if (SUCCEEDED(hr = GetQRScale(&qrscale)))
		{
			try
			{
				CStringXDW szQRScale;
				szQRScale.Format(L"%i", qrscale);

				*pbstrQRScale = szQRScale.AllocSysString();
			}
			catch (CXDException& e)
			{
				hr = e;
			}
		}
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodePTProperties::GetQROffsetX

Routine Description:

Method to obtain the Offset X of QR code

Arguments:

pQROffsetX - Variable which is set to the QR code offset X value

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTProperties::GetQROffsetX(
	_Out_ INT* pQROffsetX
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pQROffsetX, E_POINTER)))
	{
		*pQROffsetX = m_qrcodeData.offset_x;
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodePTProperties::GetQROffsetX

Routine Description:

Method to obtain the QR code offset X value as a string

Arguments:

pbstrQROffsetX - String which is set to contain the QR Code Offset X value

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTProperties::GetQROffsetX(
	_Outptr_ BSTR* pbstrQROffsetX
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pbstrQROffsetX, E_POINTER)))
	{
		*pbstrQROffsetX = NULL;
		INT qroffsetx = 0;
		if (SUCCEEDED(hr = GetQROffsetX(&qroffsetx)))
		{
			try
			{
				CStringXDW szQROffsetX;
				szQROffsetX.Format(L"%i", qroffsetx);

				*pbstrQROffsetX = szQROffsetX.AllocSysString();
			}
			catch (CXDException& e)
			{
				hr = e;
			}
		}
	}

	ERR_ON_HR(hr);
	return hr;
}


/*++

Routine Name:

CQRCodePTProperties::GetQROffsetY

Routine Description:

Method to obtain the Offset Y of QR code

Arguments:

pQROffsetY - Variable which is set to the QR code offset Y value

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTProperties::GetQROffsetY(
	_Out_ INT* pQROffsetY
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pQROffsetY, E_POINTER)))
	{
		*pQROffsetY = m_qrcodeData.offset_y;
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodePTProperties::GetQROffsetY

Routine Description:

Method to obtain the QR code offset Y value as a string

Arguments:

pbstrQROffsetY - String which is set to contain the QR Code Offset Y value

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTProperties::GetQROffsetY(
	_Outptr_ BSTR* pbstrQROffsetY
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pbstrQROffsetY, E_POINTER)))
	{
		*pbstrQROffsetY = NULL;
		INT qroffsety = 0;
		if (SUCCEEDED(hr = GetQROffsetY(&qroffsety)))
		{
			try
			{
				CStringXDW szQROffsetY;
				szQROffsetY.Format(L"%i", qroffsety);

				*pbstrQROffsetY = szQROffsetY.AllocSysString();
			}
			catch (CXDException& e)
			{
				hr = e;
			}
		}
	}

	ERR_ON_HR(hr);
	return hr;
}