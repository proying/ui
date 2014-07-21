#ifndef __UIBASE_DIALOG_H__
#define __UIBASE_DIALOG_H__

namespace DirectUI
{
	class DirectUI_API CDialogUI : public CWindowUI
	{
	public:
		virtual ~CDialogUI();
		CDialogUI(CWindowWnd* pParentWnd = NULL);
		virtual BOOL Create(HWND hWndTop = NULL,  //模态对话框不能为NULL
				LPCTSTR lpszWindowName = _T("UIDialog"),				
				DWORD dwStyle = UI_CLASSSTYLE_DIALOG, 
				DWORD dwExStyle = UI_WNDSTYLE_DIALOG, 
				CWindowWnd* pParentWnd = NULL);
	protected:
		CWindowWnd	*m_pParentWnd;
	};
}

#endif //__UIBASE_DIALOG_H__
