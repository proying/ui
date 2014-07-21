#ifndef __UIMENUEX_H__
#define __UIMENUEX_H__

namespace DirectUI
{
	class DirectUI_API CShadowWnd : public CWindowWnd
	{
	public:
		CShadowWnd();
	protected:
		LPCTSTR GetWindowClassName() const ;
		UINT GetClassStyle() const ;
		void OnFinalMessage(HWND hWnd);
		void DoPaint();
		HBITMAP CreateShadowBitmap(HDC hDC, int cx, int cy, COLORREF** pBits);
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		bool m_bNeedUpdate;
		CPaintManagerUI m_pm;
	};

	class DirectUI_API CMenuUIEx : public CWindowWnd, public INotifyUI
	{
	public:
		CMenuUIEx();
		~CMenuUIEx();
		void AdjustPostion();
		void Notify(TNotifyUI& msg);
		bool Init(STRINGorID xml, CControlUI* pOwner, POINT pt);

		void CMenuUIEx::ShowWindow(bool bShow = true, bool bTakeFocus = true);
		HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, 
			DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, 
			int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	protected:
		LPCTSTR GetWindowClassName() const;
	    virtual void OnFinalMessage(HWND hWnd);
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		POINT			m_ptPos;
	    CControlUI*		m_pOwner;
		CShadowWnd*		m_pShadowWnd;
		STRINGorID		m_xml;
		CPaintManagerUI m_pm;
	};

}

#endif //__UIMENUEX_H__