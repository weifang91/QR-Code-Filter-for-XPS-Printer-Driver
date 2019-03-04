#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "xdstring.h"
#include "resource.h"
#include "qrcodectrls.h"
#include "privatedefs.h"


PCSTR CUICtrlQRMerchantIDEdit::m_pszQRMerchantID		= "PageQRCodeMerchantID";
PCSTR CUICtrlQRSecurityKeyEdit::m_pszQRSecurityKey		= "PageQRCodeSecurityKey";
PCSTR CUICtrlQRRedirectModelEdit::m_pszQRRedirectModel	= "PageQRCodeRedirectPrinterModel";
PCSTR CUICtrlQROKBtn::m_pszQROKBtn						= "PageQRCodeOKButton";
PCSTR CUICtrlQROptionCombo::m_pszQROption				= "PageQRCodeOption";
PCSTR CUICtrlQRScaleEdit::m_pszQRScale					= "PageQRCodeScale";
PCSTR CUICtrlQRScaleSpin::m_pszQRScale					= "PageQRCodeScale";
PCSTR CUICtrlQROffsetXEdit::m_pszQROffsetX				= "PageQRCodeOffsetX";
PCSTR CUICtrlQROffsetXSpin::m_pszQROffsetX				= "PageQRCodeOffsetX";
PCSTR CUICtrlQROffsetYEdit::m_pszQROffsetY				= "PageQRCodeOffsetY";
PCSTR CUICtrlQROffsetYSpin::m_pszQROffsetY				= "PageQRCodeOffsetY";
BOOL CUICtrlQROKBtn::gbAccessQRCode						= FALSE;
BOOL CUICtrlQROptionCombo::gbEnableQRCode				= FALSE;

#define QROPTION_DISABLE 0
#define QROPTION_ENABLE 1

/*++

Routine Name:

CUICtrlQRMerchantIDEdit::CUICtrlQRMerchantIDEdit

Routine Description:

CUICtrlQRMerchantIDEdit class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRMerchantIDEdit::CUICtrlQRMerchantIDEdit() :
	CUICtrlDefaultEditText(m_pszQRMerchantID,
		IDC_EDIT_MERCHANT_ID,
		MAX_QR_MERCHANT_ID)
{
}

/*++

Routine Name:

CUICtrlQRMerchantIDEdit::~CUICtrlQRMerchantIDEdit

Routine Description:

CUICtrlQRMerchantIDEdit class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRMerchantIDEdit::~CUICtrlQRMerchantIDEdit()
{
}

/*++

Routine Name:

CUICtrlQRSecurityKeyEdit::CUICtrlQRSecurityKeyEdit

Routine Description:

CUICtrlQRSecurityKeyEdit class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRSecurityKeyEdit::CUICtrlQRSecurityKeyEdit() :
	CUICtrlDefaultEditText(m_pszQRSecurityKey,
		IDC_EDIT_SECURITY_KEY,
		MAX_QR_SECURITY_KEY)
{

}

/*++

Routine Name:

CUICtrlQRSecurityKeyEdit::~CUICtrlQRSecurityKeyEdit

Routine Description:

CUICtrlQRSecurityKeyEdit class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRSecurityKeyEdit::~CUICtrlQRSecurityKeyEdit()
{
}

/*++

Routine Name:

CUICtrlQRRedirectModelEdit::CUICtrlQRRedirectModelEdit

Routine Description:

CUICtrlWMTextEdit class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRRedirectModelEdit::CUICtrlQRRedirectModelEdit() :
	CUICtrlDefaultEditText(m_pszQRRedirectModel,
		IDC_EDIT_REDIRECT_MODAL,
		MAX_QR_PRINTER_MODEL)
{
}

/*++

Routine Name:

CUICtrlQRRedirectModelEdit::~CUICtrlQRRedirectModelEdit

Routine Description:

CUICtrlQRRedirectModelEdit class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRRedirectModelEdit::~CUICtrlQRRedirectModelEdit()
{
}

/*++

Routine Name:

CUICtrlQROKBtn::CUICtrlQROKBtn

Routine Description:

CUICtrlQROKBtn class contructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROKBtn::CUICtrlQROKBtn() :
	CUICtrlDefaultBtn(m_pszQROKBtn, IDC_BUTTON_QR_OK)
{
}

/*++

Routine Name:

CUICtrlQROKBtn::~CUICtrlQROKBtn

Routine Description:

CUICtrlQROKBtn class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROKBtn::~CUICtrlQROKBtn()
{
}
/*++
Routine Name:

CUICtrlQROKBtn::CUICtrlQROKBtn

Routine Description:

QR Code OK button OnInit handler.

Arguments:

hDlg - handle to the parent window

Return Value:

S_OK
--*/
HRESULT
CUICtrlQROKBtn::OnInit(
	_In_ CONST HWND hDlg
)
{
	HRESULT hr = S_OK;
	CUICtrlQROptionCombo qrOptionCombo;
	hr = AccessQRCode(hDlg, gbAccessQRCode, qrOptionCombo.gbEnableQRCode);

	ERR_ON_HR(hr);
	return hr;
}
/*++

Routine Name:

CUICtrlQROKBtn::OnBnClicked

Routine Description:

QR OK button clicked handler

Arguments:

hDlg - handle to the parent window

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/

HRESULT
CUICtrlQROKBtn::OnBnClicked(
	_In_ CONST HWND hDlg
)
{
	HRESULT hr = S_OK;
	
	LPTSTR enteredMerchantID = new(std::nothrow) TCHAR[MAX_QR_MERCHANT_ID];
	LPTSTR enteredSecurityKey = new(std::nothrow) TCHAR[MAX_QR_SECURITY_KEY];
	TCHAR correctMerchantID[MAX_QR_MERCHANT_ID] = L"Teaspoon";
	TCHAR correctSecurityKey[MAX_QR_MERCHANT_ID] = L"123456789";

	if (SUCCEEDED(hr) &&
		SUCCEEDED(hr = CHECK_POINTER(enteredMerchantID, E_OUTOFMEMORY))&&
		SUCCEEDED(hr = CHECK_POINTER(enteredSecurityKey, E_OUTOFMEMORY)))
	{
		if (SUCCEEDED(hr = GetDlgItemText(hDlg, IDC_EDIT_MERCHANT_ID, enteredMerchantID, MAX_QR_MERCHANT_ID))&&
			SUCCEEDED(hr = GetDlgItemText(hDlg, IDC_EDIT_SECURITY_KEY, enteredSecurityKey, MAX_QR_SECURITY_KEY)))
		{
			if (wcsncmp(enteredMerchantID, correctMerchantID, MAX_QR_MERCHANT_ID) == 0 &&
				wcsncmp(enteredSecurityKey, correctSecurityKey, MAX_QR_SECURITY_KEY) == 0)
			{
				gbAccessQRCode = TRUE;
			}
			else
			{
				MessageBox(hDlg, L"Invalid Password!! Please try again", NULL, MB_OK);

			}
		}
		else
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
		delete[] enteredMerchantID;
		enteredMerchantID = NULL;
		delete[] enteredSecurityKey;
		enteredSecurityKey = NULL;
	
	}

	CUICtrlQROptionCombo qrOptionCombo;
	hr = AccessQRCode(hDlg, gbAccessQRCode, qrOptionCombo.gbEnableQRCode);

	ERR_ON_HR(hr);
	return hr;
}
/*++

Routine Name:

CUICtrlQROKBtn::AccessQRCode

Routine Description:

Give access to QR code section

Arguments:

hDlg - handle to the parent window
bAccessQRCode - Access or not access QR code

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/

HRESULT
CUICtrlQROKBtn::AccessQRCode(
	_In_ CONST HWND hDlg,
	_In_ BOOL bAccessQRCode,
	_In_ BOOL bEnableQRCode
)
{
	HRESULT hr = S_OK;
	HWND hWnd = NULL;

	if (SUCCEEDED(hr) &&
		SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_GROUP_QRCODE), E_HANDLE)))
	{
		EnableWindow(hWnd, (bAccessQRCode));
		ShowWindow(hWnd, (bAccessQRCode) ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr) &&
		SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_TXT_QRCODE), E_HANDLE)))
	{
		EnableWindow(hWnd, (bAccessQRCode));
		ShowWindow(hWnd, (bAccessQRCode) ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr) &&
		SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_COMBO_QRCODE_OPTION), E_HANDLE)))
	{
		EnableWindow(hWnd, (bAccessQRCode));
		ShowWindow(hWnd, (bAccessQRCode) ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_TXT_QR_SCALE), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
		ShowWindow(hWnd, bAccessQRCode ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_EDIT_QR_SCALE), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
		ShowWindow(hWnd, bAccessQRCode ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_SPIN_QR_SCALE), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
		ShowWindow(hWnd, bAccessQRCode ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_TXT_QROFFX), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
		ShowWindow(hWnd, bAccessQRCode ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_EDIT_QROFFX), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
		ShowWindow(hWnd, bAccessQRCode ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_SPIN_QROFFX), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
		ShowWindow(hWnd, bAccessQRCode ? SW_SHOW : SW_HIDE);
	}

	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_TXT_QROFFY), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
		ShowWindow(hWnd, bAccessQRCode ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_EDIT_QROFFY), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
		ShowWindow(hWnd, bAccessQRCode ? SW_SHOW : SW_HIDE);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_SPIN_QROFFY), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
		ShowWindow(hWnd, bAccessQRCode ? SW_SHOW : SW_HIDE);
	}
	
	ERR_ON_HR(hr);
	return hr;
}

//
// QR Code combo box control
//
/*++

Routine Name:

CUICtrlQROptionCombo::CUICtrlQROptionCombo

Routine Description:

CUICtrlQROptionCombo class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROptionCombo::CUICtrlQROptionCombo() :
	CUICtrlDefaultCombo(m_pszQROption, IDC_COMBO_QRCODE_OPTION)
{
}

/*++

Routine Name:

CUICtrlQROptionCombo::~CUICtrlQROptionCombo

Routine Description:

CUICtrlQROptionCombo class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROptionCombo::~CUICtrlQROptionCombo()
{
}

/*++

Routine Name:

CUICtrlQROptionCombo::OnInit

Routine Description:

This is responsible for initialising the control and is called when
the WM_INITDIALOG message is recieved. This method populates the combo
box with the appropriate option strings.

Arguments:

hDlg - handle to the parent window

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CUICtrlQROptionCombo::OnInit(
	_In_ CONST HWND hDlg
)
{
	HRESULT hr = S_OK;

	//
	// Populate the combo box
	//
	if (SUCCEEDED(hr = AddString(hDlg, g_hInstance, IDS_GPD_DISABLE)))
	{
		hr = AddString(hDlg, g_hInstance, IDS_GPD_ENABLE);
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CUICtrlQROptionCombo::EnableDependentCtrls

Routine Description:

This method is used to enable or disable other controls in the UI based on the
current combo box selection.

Arguments:

hDlg - handle to the parent window
lSel - current combo box selection

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CUICtrlQROptionCombo::EnableDependentCtrls(
	_In_ CONST HWND hDlg,
	_In_ CONST LONG lSel
)
{
	HRESULT hr = S_OK;

	gbEnableQRCode = (lSel == QROPTION_ENABLE);
	hr = EnableQRCode(hDlg, gbEnableQRCode);

	ERR_ON_HR(hr);
	return hr;
}
/*++

Routine Name:

CUICtrlQROptionCombo::EnableQRCode

Routine Description:

Here we are enabling/disabling common QR Code controls based off the current
QR Code option. If a QR Code is enabled we enable the scale,  offsetx and offsety 
controls

Arguments:

hDlg - handle to the parent window
bEnableQRCode - enable or disable QR code

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CUICtrlQROptionCombo :: EnableQRCode(
	_In_ CONST HWND hDlg,
	_In_ BOOL bEnableQRCode
)
{
	HRESULT hr = S_OK;
	HWND hWnd = NULL;

	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_TXT_QR_SCALE), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_EDIT_QR_SCALE), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_SPIN_QR_SCALE), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_TXT_QROFFX), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_EDIT_QROFFX), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_SPIN_QROFFX), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
	}

	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_TXT_QROFFY), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_EDIT_QROFFY), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
	}
	if (SUCCEEDED(hr = CHECK_HANDLE(hWnd = GetDlgItem(hDlg, IDC_SPIN_QROFFY), E_HANDLE)))
	{
		EnableWindow(hWnd, bEnableQRCode);
	}

	ERR_ON_HR(hr);
	return hr;
}

//
// Page QR Code Scale
//

/*++

Routine Name:

CUICtrlQRScaleEdit::CUICtrlQRScaleEdit

Routine Description:

CUICtrlQRScaleEdit class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRScaleEdit::CUICtrlQRScaleEdit() :
	CUICtrlDefaultEditNum(m_pszQRScale,
						  IDC_EDIT_QR_SCALE,
						  qrcodeParamDefIntegers[ePageQRCodeScale].min_length,
						  qrcodeParamDefIntegers[ePageQRCodeScale].max_length,
						  IDC_SPIN_QR_SCALE)
{
}

/*++

Routine Name:

CUICtrlQRScaleEdit::~CUICtrlQRScaleEdit

Routine Description:

CUICtrlQRScaleEdit class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRScaleEdit::~CUICtrlQRScaleEdit()
{
}

/*++

Routine Name:

CUICtrlQRScaleSpin::CUICtrlQRScaleSpin

Routine Description:

CUICtrlQRScaleSpin class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRScaleSpin::CUICtrlQRScaleSpin(
	_In_ CUICtrlDefaultEditNum* pEdit
) :
	CUICtrlDefaultSpin(pEdit)
{
}

/*++

Routine Name:

CUICtrlQRScaleSpin::~CUICtrlQRScaleSpin

Routine Description:

CUICtrlQRScaleSpin class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQRScaleSpin::~CUICtrlQRScaleSpin()
{
}

//
// Page QR Code Offset X
//

/*++

Routine Name:

CUICtrlQROffsetXEdit::CUICtrlQROffsetXEdit

Routine Description:

CUICtrlQROffsetXEdit class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROffsetXEdit::CUICtrlQROffsetXEdit() :
	CUICtrlDefaultEditNum(m_pszQROffsetX,
		IDC_EDIT_QROFFX,
		qrcodeParamDefIntegers[ePageQRCodeOffsetX].min_length,
		qrcodeParamDefIntegers[ePageQRCodeOffsetX].max_length,
		IDC_SPIN_QROFFX)
{
}

/*++

Routine Name:

CUICtrlQROffsetXEdit::~CUICtrlQROffsetXEdit

Routine Description:

CUICtrlQROffsetXEdit class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROffsetXEdit::~CUICtrlQROffsetXEdit()
{
}

/*++

Routine Name:

CUICtrlQROffsetXSpin::CUICtrlQROffsetXSpin

Routine Description:

CUICtrlQROffsetXSpin class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROffsetXSpin::CUICtrlQROffsetXSpin(
	_In_ CUICtrlDefaultEditNum* pEdit
) :
	CUICtrlDefaultSpin(pEdit)
{
}

/*++

Routine Name:

CUICtrlQROffsetXSpin::~CUICtrlQROffsetXSpin

Routine Description:

CUICtrlQROffsetXSpin class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROffsetXSpin::~CUICtrlQROffsetXSpin()
{
}

//
// Page QR Code Offset Y
//

/*++

Routine Name:

CUICtrlWMOffsetYEdit::CUICtrlWMOffsetYEdit

Routine Description:

CUICtrlWMOffsetYEdit class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROffsetYEdit::CUICtrlQROffsetYEdit() :
	CUICtrlDefaultEditNum(m_pszQROffsetY,
		IDC_EDIT_QROFFY,
		qrcodeParamDefIntegers[ePageQRCodeOffsetY].min_length,
		qrcodeParamDefIntegers[ePageQRCodeOffsetY].max_length,
		IDC_SPIN_QROFFY)
{
}

/*++

Routine Name:

CUICtrlWMOffsetYEdit::~CUICtrlWMOffsetYEdit

Routine Description:

CUICtrlWMOffsetYEdit class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROffsetYEdit::~CUICtrlQROffsetYEdit()
{
}

/*++

Routine Name:

CUICtrlWMOffsetYSpin::CUICtrlWMOffsetYSpin

Routine Description:

CUICtrlWMOffsetYSpin class constructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROffsetYSpin::CUICtrlQROffsetYSpin(
	_In_ CUICtrlDefaultEditNum* pEdit
) :
	CUICtrlDefaultSpin(pEdit)
{
}

/*++

Routine Name:

CUICtrlWMOffsetYSpin::~CUICtrlWMOffsetYSpin

Routine Description:

CUICtrlWMOffsetYSpin class destructor

Arguments:

None

Return Value:

None

--*/
CUICtrlQROffsetYSpin::~CUICtrlQROffsetYSpin()
{
}