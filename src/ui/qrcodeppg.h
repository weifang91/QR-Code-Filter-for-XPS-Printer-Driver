#pragma once

#include "precomp.h"
#include "docppg.h"

class CQrCodePropPage : public CDocPropPage
{
public:
	CQrCodePropPage();
	
	virtual ~CQrCodePropPage();

	HRESULT
		InitDlgBox(
			_Out_ LPCTSTR* ppszTemplate,
			_Out_ LPCTSTR* ppszTitle
		);
};

