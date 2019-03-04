/*++

File Name:

qrcodectrls.h

Abstract:

Definition of the qr code specific UI controls.

--*/
#pragma once

#include "uictrl.h"

class CUICtrlQRMerchantIDEdit : public CUICtrlDefaultEditText
{
public:
	CUICtrlQRMerchantIDEdit();

	virtual ~CUICtrlQRMerchantIDEdit();

private:
	static PCSTR m_pszQRMerchantID;
};

class CUICtrlQRSecurityKeyEdit : public CUICtrlDefaultEditText
{
public:
	CUICtrlQRSecurityKeyEdit();

	virtual ~CUICtrlQRSecurityKeyEdit();

private:
	static PCSTR m_pszQRSecurityKey;
};

class CUICtrlQRRedirectModelEdit : public CUICtrlDefaultEditText
{
public:
	CUICtrlQRRedirectModelEdit();

	virtual ~CUICtrlQRRedirectModelEdit();

private:
	static PCSTR m_pszQRRedirectModel;
};

class CUICtrlQROKBtn : public CUICtrlDefaultBtn
{
public:
	CUICtrlQROKBtn();

	~CUICtrlQROKBtn();

	HRESULT
		OnInit(
			_In_ CONST HWND hDlg
		);

	HRESULT
		OnBnClicked(
			_In_ CONST HWND hDlg
		);

	HRESULT
		AccessQRCode(
			_In_ CONST HWND hDlg,
			_In_ BOOL gbAccessQRCode,
			_In_ BOOL bEnableQRCode
		);

private:
	static BOOL gbAccessQRCode;
	static PCSTR m_pszQROKBtn;
};

class CUICtrlQROptionCombo : public CUICtrlDefaultCombo
{
public:
	CUICtrlQROptionCombo();

	virtual ~CUICtrlQROptionCombo();

	HRESULT
		OnInit(
			_In_ CONST HWND hDlg
		);

private:
	HRESULT
		EnableDependentCtrls(
			_In_ CONST HWND hDlg,
			_In_ CONST LONG lSel
		);
	HRESULT
		EnableQRCode(
			_In_ CONST HWND hDlg,
			_In_ BOOL bEnableQRCode
		);

public:
	static BOOL gbEnableQRCode;
private:
	static PCSTR m_pszQROption;
};

class CUICtrlQRScaleEdit : public CUICtrlDefaultEditNum
{
public:
	CUICtrlQRScaleEdit();

	virtual ~CUICtrlQRScaleEdit();

private:
	static PCSTR m_pszQRScale;
};

class CUICtrlQRScaleSpin : public CUICtrlDefaultSpin
{
public:
	CUICtrlQRScaleSpin(
		_In_ CUICtrlDefaultEditNum* pEdit
	);

	virtual ~CUICtrlQRScaleSpin();

private:
	static PCSTR m_pszQRScale;
};

class CUICtrlQROffsetXEdit : public CUICtrlDefaultEditNum
{
public:
	CUICtrlQROffsetXEdit();

	virtual ~CUICtrlQROffsetXEdit();

private:
	static PCSTR m_pszQROffsetX;
};

class CUICtrlQROffsetXSpin : public CUICtrlDefaultSpin
{
public:
	CUICtrlQROffsetXSpin(
		_In_ CUICtrlDefaultEditNum* pEdit
	);

	virtual ~CUICtrlQROffsetXSpin();

private:
	static PCSTR m_pszQROffsetX;
};

class CUICtrlQROffsetYEdit : public CUICtrlDefaultEditNum
{
public:
	CUICtrlQROffsetYEdit();

	virtual ~CUICtrlQROffsetYEdit();

private:
	static PCSTR m_pszQROffsetY;
};

class CUICtrlQROffsetYSpin : public CUICtrlDefaultSpin
{
public:
	CUICtrlQROffsetYSpin(
		_In_ CUICtrlDefaultEditNum* pEdit
	);

	virtual ~CUICtrlQROffsetYSpin();

private:
	static PCSTR m_pszQROffsetY;
};