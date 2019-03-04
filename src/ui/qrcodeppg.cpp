#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "xdexcept.h"
#include "xdstring.h"
#include "resource.h"
#include "qrcodeppg.h"
#include "qrcodectrls.h"


/*++

Routine Name:

CQrCodePropPage::CQrCodePropPage

Routine Description:

CQrCodePropPage class constructor.
Creates a handler class object for every control on the qr code property page.
Each of these handlers is stored in a collection.

Arguments:

None

Return Value:

None
Throws CXDException(HRESULT) on an error

--*/
CQrCodePropPage::CQrCodePropPage()
{
	
	HRESULT hr = S_OK;

	try
	{
		CUIControl* pControl = new(std::nothrow) CUICtrlQRMerchantIDEdit();

		if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)))
		{
			hr = AddUIControl(IDC_EDIT_MERCHANT_ID, pControl);
		}

		if (SUCCEEDED(hr))
		{
			pControl = new(std::nothrow) CUICtrlQRSecurityKeyEdit(); 
			if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)))
			{
				hr = AddUIControl(IDC_EDIT_SECURITY_KEY, pControl);
			}
		}

		if (SUCCEEDED(hr))
		{
			pControl = new(std::nothrow) CUICtrlQRRedirectModelEdit();
			if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)))
			{
				hr = AddUIControl(IDC_EDIT_REDIRECT_MODAL, pControl);
			}
		}

		if (SUCCEEDED(hr))
		{
			pControl = new(std::nothrow) CUICtrlQROKBtn();
			if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)))
			{
				hr = AddUIControl(IDC_BUTTON_QR_OK, pControl);
			}
		}

		if (SUCCEEDED(hr))
		{
			pControl = new(std::nothrow) CUICtrlQROptionCombo();
			if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)))
			{
				hr = AddUIControl(IDC_COMBO_QRCODE_OPTION, pControl);
			}
		}

		if (SUCCEEDED(hr))
		{
			pControl = new(std::nothrow) CUICtrlQRScaleEdit();
			if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)) &&
				SUCCEEDED(hr = AddUIControl(IDC_EDIT_QR_SCALE, pControl)))
			{
				pControl = new(std::nothrow) CUICtrlQRScaleSpin(reinterpret_cast<CUICtrlDefaultEditNum *>(pControl));
				if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)))
				{
					hr = AddUIControl(IDC_SPIN_QR_SCALE, pControl);
				}
			}
		}

		if (SUCCEEDED(hr))
		{
			pControl = new(std::nothrow) CUICtrlQROffsetXEdit();
			if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)) &&
				SUCCEEDED(hr = AddUIControl(IDC_EDIT_QROFFX, pControl)))
			{
				pControl = new(std::nothrow) CUICtrlQROffsetXSpin(reinterpret_cast<CUICtrlDefaultEditNum *>(pControl));
				if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)))
				{
					hr = AddUIControl(IDC_SPIN_QROFFX, pControl);
				}
			}
		}

		if (SUCCEEDED(hr))
		{
			pControl = new(std::nothrow) CUICtrlQROffsetYEdit();
			if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)) &&
				SUCCEEDED(hr = AddUIControl(IDC_EDIT_QROFFY, pControl)))
			{
				pControl = new(std::nothrow) CUICtrlQROffsetYSpin(reinterpret_cast<CUICtrlDefaultEditNum *>(pControl));
				if (SUCCEEDED(hr = CHECK_POINTER(pControl, E_OUTOFMEMORY)))
				{
					hr = AddUIControl(IDC_SPIN_QROFFY, pControl);
				}
			}
		}

	}
	catch (CXDException& e)
	{
		hr = e;
	}

	if (FAILED(hr))
	{
		DestroyUIComponents();
		throw CXDException(hr);
	}
}

/*++

Routine Name:

CQrCodePropPage::~CQrCodePropPage

Routine Description:

CQrCodePropPage class destructor

Arguments:

None

Return Value:

None

--*/
CQrCodePropPage::~CQrCodePropPage()
{
}

/*++

Routine Name:

CQrCodePropPage::InitDlgBox

Routine Description:

Provides the base class with the data required to intialise the dialog box.

Arguments:

ppszTemplate - Pointer to dialog box template to be intialised.
ppszTitle    - Pointer to dialog box title to be intialised.

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQrCodePropPage::InitDlgBox(
	_Out_ LPCTSTR* ppszTemplate,
	_Out_ LPCTSTR* ppszTitle
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(ppszTemplate, E_POINTER)) ||
		SUCCEEDED(hr = CHECK_POINTER(ppszTitle, E_POINTER)))
	{
		*ppszTemplate = MAKEINTRESOURCE(IDD_QRCODE);
		*ppszTitle = MAKEINTRESOURCE(IDS_QRCODE);
	}

	ERR_ON_HR(hr);
	return hr;
}