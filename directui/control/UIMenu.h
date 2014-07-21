#ifndef __UIMENU_H__
#define __UIMENU_H__

#include "pattern\observer.h"

namespace DirectUI
{
	struct ContextMenuParam
	{
		// 1: remove all
		// 2: remove the sub menu
		WPARAM wParam;
		HWND hWnd;
	};

	enum MenuAlignment
	{
		eMenuAlignment_Left = 1 << 1,
		eMenuAlignment_Top = 1 << 2,
		eMenuAlignment_Right = 1 << 3,
		eMenuAlignment_Bottom = 1 << 4,
	};

	typedef class CObserver<BOOL, ContextMenuParam> ContextMenuObserver;
	typedef class CReceiver<BOOL, ContextMenuParam> ContextMenuReceiver;

	extern ContextMenuObserver s_context_menu_observer;

	class CMenuWnd;
	class DirectUI_API CMenuUI : public CListUI
	{
	public:
		CMenuUI();
		~CMenuUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		virtual void DoEvent(TEventUI& event);
		virtual bool Add(CControlUI* pControl);
		virtual bool AddAt(CControlUI* pControl, int iIndex);

		virtual int GetItemIndex(CControlUI* pControl) const;
		virtual bool SetItemIndex(CControlUI* pControl, int iIndex);
		virtual bool Remove(CControlUI* pControl);

		bool OnNotify(void *pNotify);
		void SetParent(CMenuWnd *pWnd);

		SIZE EstimateSize(SIZE szAvailable);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void SetMenuAlign(DWORD dwAlignment);
		DWORD GetMenuAlign() const;
		void SetMenuAlpha(BYTE ucAlpha);
		BYTE GetMenuAlpha() const;
		void SetMenuColorkey(BYTE ucColorkey);
		BYTE GetMenuColorkey() const;
	protected:
		DWORD	m_dwAlignment;
		BYTE	m_bAlpha;
		BYTE	m_cColorkey;
		CMenuWnd* m_pWindow;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	// MenuElementUI
	class DirectUI_API CMenuItemUI : public CListContainerElementUI
	{
		friend CMenuWnd;
	public:
		CMenuItemUI();
		~CMenuItemUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void DoPaint(HDC hDC, const RECT& rcPaint);
		void DrawItemText(HDC hDC, const RECT& rcItem);

		SIZE EstimateSize(SIZE szAvailable);
		bool Activate();
		void DoEvent(TEventUI& event);

		CMenuWnd* GetMenuWnd();
		void CreateMenuWnd();

	protected:
		CMenuWnd* m_pWindow;
	};

	class DirectUI_API CMenuWnd 
		: public CWindowWnd, 
		  public ContextMenuReceiver
	{
	public:
		CMenuWnd(HWND hParent = NULL);
		void Init(INotifyUI *pNotify, CMenuItemUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point);
		LPCTSTR GetWindowClassName() const;
		void OnFinalMessage(HWND hWnd);
		void Notify(TNotifyUI *msg);
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		BOOL Receive(ContextMenuParam param);
		BOOL UpdateMenuItem(LPCTSTR lpszName, LPCTSTR lpszElement, LPCTSTR lpszInfo);
	protected:
		HWND m_hParent;
		POINT m_BasedPoint;
		STRINGorID m_xml;
		CUIString m_sType;
		CPaintManagerUI m_pm;
		CMenuItemUI* m_pOwner;
		CMenuUI* m_pMenu;
		INotifyUI	*m_pNotify;
	};

	class CMenuBuilderCallback: public IDialogBuilderCallback
	{
		CControlUI* CreateControl(LPCTSTR pstrClass)
		{
			if (_tcsicmp(pstrClass, DUI_CTR_MENU) == 0)
			{
				return new CMenuUI();
			}
			else if (_tcsicmp(pstrClass, DUI_CTR_MENUITEM) == 0)
			{
				return new CMenuItemUI();
			}
			return NULL;
		}
	};

} // namespace DuiLib

#endif // __UIMENU_H__
