#include "StdAfx.h"
#include "UIBaseDlg.h"

namespace DirectUI
{
	CDialogUI::CDialogUI(CWindowWnd* pParentWnd)
	{
		m_pParentWnd = pParentWnd;
	}

	CDialogUI::~CDialogUI()
	{
	}

	BOOL CDialogUI::Create(HWND hWndTop, LPCTSTR lpszClassName, DWORD dwStyle, DWORD dwExStyle, CWindowWnd* pParentWnd)
	{
		m_pParentWnd = pParentWnd;
		return (CWindowUI::Create(hWndTop, lpszClassName, dwStyle, dwExStyle) != NULL);
	}
}