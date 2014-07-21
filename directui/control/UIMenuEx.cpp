#include "StdAfx.h"
#include "UIMenuEx.h"

namespace DirectUI
{
	//////////////////////////////////////////////////////////////////////////////////
	//// CShadowWnd

	CShadowWnd::CShadowWnd()
	{
	}

	LPCTSTR CShadowWnd::GetWindowClassName() const 
	{ 
		return _T("UIShadow"); 
	}

	UINT CShadowWnd::GetClassStyle() const 
	{ 
		return UI_CLASSSTYLE_FRAME; 
	}

	void CShadowWnd::OnFinalMessage(HWND hWnd) 
	{ 
		delete this; 
	}

	HBITMAP CShadowWnd::CreateShadowBitmap(HDC hDC, int cx, int cy, COLORREF** pBits)
	{
	    LPBITMAPINFO lpbiSrc = NULL;
		lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
		if (lpbiSrc == NULL) return NULL;

		lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiSrc->bmiHeader.biWidth = cx;
		lpbiSrc->bmiHeader.biHeight = cy;
		lpbiSrc->bmiHeader.biPlanes = 1;
		lpbiSrc->bmiHeader.biBitCount = 32;
		lpbiSrc->bmiHeader.biCompression = BI_RGB;
		lpbiSrc->bmiHeader.biSizeImage = cx * cy;
		lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
		lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
		lpbiSrc->bmiHeader.biClrUsed = 0;
		lpbiSrc->bmiHeader.biClrImportant = 0;

		HBITMAP hBitmap = CreateDIBSection(hDC, lpbiSrc, DIB_RGB_COLORS, (void **)pBits, NULL, NULL);
		delete [] lpbiSrc;
		return hBitmap;
	}

	void CShadowWnd::DoPaint()
	{
		RECT rcClient = { 0 };
        ::GetClientRect(m_hWnd, &rcClient);
        DWORD dwWidth = rcClient.right - rcClient.left;
        DWORD dwHeight = rcClient.bottom - rcClient.top;

        HDC hDcPaint = ::GetDC(m_hWnd);
        HDC hDcBackground = ::CreateCompatibleDC(hDcPaint);
        COLORREF* pBackgroundBits;
        HBITMAP hbmpBackground = CreateShadowBitmap(hDcPaint, dwWidth, dwHeight, &pBackgroundBits);
        ::ZeroMemory(pBackgroundBits, dwWidth * dwHeight * 4);
        HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hDcBackground, hbmpBackground);
        CRenderEngine::DrawImageString(hDcBackground, &m_pm, rcClient, rcClient, _T("file='channel_menu_bk.png' corner='40,8,8,8'"), NULL);

        RECT rcWnd = { 0 };
        ::GetWindowRect(m_hWnd, &rcWnd);

        BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
        POINT ptPos   = { rcWnd.left, rcWnd.top };
        SIZE sizeWnd  = { dwWidth, dwHeight };
        POINT ptSrc   = { 0, 0 };
        UpdateLayeredWindow(m_hWnd, hDcPaint, &ptPos, &sizeWnd, hDcBackground, &ptSrc, 0, &bf, ULW_ALPHA);

        ::SelectObject(hDcBackground, hOldBitmap);
        if( hDcBackground != NULL ) ::DeleteDC(hDcBackground);
        if( hbmpBackground != NULL ) ::DeleteObject(hbmpBackground);
        ::ReleaseDC(m_hWnd, hDcPaint);

        m_bNeedUpdate = false;
	}

	LRESULT CShadowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_CREATE) SetTimer(m_hWnd, 9000, 10, NULL);
        else if (uMsg == WM_SIZE) m_bNeedUpdate = true;
        else if (uMsg == WM_CLOSE) KillTimer(m_hWnd, 9000);
        else if (uMsg == WM_TIMER) 
		{
            if (LOWORD(wParam) == 9000 && m_bNeedUpdate) 
			{
                if(!::IsIconic(m_hWnd)) 
					DoPaint();
            }
        }

        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }


	//////////////////////////////////////////////////////////////////////////////////
	//// CMenuUIEx

	CMenuUIEx::CMenuUIEx()
		:m_pOwner(0), 
		m_pShadowWnd(0),
		m_xml(_T(""))
	{
	}

	CMenuUIEx::~CMenuUIEx()
	{
	}

	LPCTSTR CMenuUIEx::GetWindowClassName() const 
	{ 
		return _T("MenuWnd"); 
	}

    void CMenuUIEx::OnFinalMessage(HWND hWnd) 
	{
		m_pm.RemoveNotifier(this);
		m_pm.ReapObjects(m_pm.GetRoot());

		delete this; 
	}

	void CMenuUIEx::Notify(TNotifyUI& msg)
    {
        if (msg.sType == _T("itemselect")) 
		{
            Close();
        }
        else if (msg.sType == _T("itemclick")) 
		{
			LPCTSTR lpsszMsg = msg.pSender->GetName();        
            if (m_pOwner) m_pOwner->GetManager()->SendNotify(m_pOwner, lpsszMsg, 0, 0, true);
        }
    }

	HWND CMenuUIEx::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
    {
        if (!m_pShadowWnd) 
			m_pShadowWnd = new CShadowWnd;

        dwExStyle |= WS_EX_TOOLWINDOW;
        m_pShadowWnd->Create(hwndParent, _T(""), WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS, WS_EX_LAYERED | WS_EX_TOOLWINDOW, x, y, cx, cy, NULL);

        return CWindowWnd::Create(hwndParent, pstrName, dwStyle, dwExStyle, x, y, cx, cy, hMenu);
    }

	void CMenuUIEx::ShowWindow(bool bShow, bool bTakeFocus)
    {
        if (m_pShadowWnd)
			m_pShadowWnd->ShowWindow(bShow, false);

        CWindowWnd::ShowWindow(bShow, bTakeFocus);
    }

	bool CMenuUIEx::Init(STRINGorID xml, CControlUI* pOwner, POINT pt)
	{
		if (!pOwner) 
			return false;

		m_xml = xml;
		m_ptPos = pt;
		m_pOwner = pOwner;

		HWND hWndPaint = pOwner->GetManager()->GetPaintWindow();
		if (!::IsWindow(hWndPaint))
			return false;

		HWND hWndParent = Create(hWndPaint, NULL, WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		while (::GetParent(hWndParent)) 
			hWndParent = ::GetParent(hWndParent);

		::ShowWindow(m_hWnd, SW_SHOW);
		::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);

		return true;
	}

	 void CMenuUIEx::AdjustPostion() 
	 {
        CRect rcWnd;
        GetWindowRect(m_hWnd, &rcWnd);
        int nWidth = rcWnd.GetWidth();
        int nHeight = rcWnd.GetHeight();
        rcWnd.left = m_ptPos.x;
        rcWnd.top = m_ptPos.y;
        rcWnd.right = rcWnd.left + nWidth;
        rcWnd.bottom = rcWnd.top + nHeight;
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CRect rcWork = oMonitor.rcWork;

        if (rcWnd.bottom > rcWork.bottom) 
		{
            if (nHeight >= rcWork.GetHeight()) 
			{
                rcWnd.top = 0;
                rcWnd.bottom = nHeight;
            }
            else 
			{
                rcWnd.bottom = rcWork.bottom;
                rcWnd.top = rcWnd.bottom - nHeight;
            }
        }

        if (rcWnd.right > rcWork.right) 
		{
            if (nWidth >= rcWork.GetWidth())
			{
                rcWnd.left = 0;
                rcWnd.right = nWidth;
            }
            else 
			{
                rcWnd.right = rcWork.right;
                rcWnd.left = rcWnd.right - nWidth;
            }
        }

        ::SetWindowPos(m_hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
    }

	LRESULT CMenuUIEx::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        m_pm.Init(m_hWnd);
        CDialogBuilder builder;

        CControlUI* pRoot = builder.Create(m_xml, (UINT)0, NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
		if (pRoot == NULL) return 0;

        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        m_pm.SetRoundCorner(3, 3);

        CListUI* pList = static_cast<CListUI*>(m_pOwner);
        int nSel = pList->GetCurSel();
        if (nSel < 0) 
		{
            CControlUI* pControl = m_pm.FindControl(_T("menu_Delete"));
            if (pControl) pControl->SetEnabled(false);
        }

        AdjustPostion();
        return 0;
    }

    LRESULT CMenuUIEx::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (m_pShadowWnd) 
			m_pShadowWnd->Close();

        bHandled = FALSE;
        return 0;
    }

	LRESULT CMenuUIEx::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if ((HWND)wParam == m_hWnd) bHandled = TRUE;
        else if (m_pShadowWnd != NULL && (HWND)wParam == m_pShadowWnd->GetHWND()) 
		{
            CWindowWnd::HandleMessage(uMsg, wParam, lParam);
            ::SetFocus(m_hWnd);
            bHandled = TRUE;
        }
        else 
		{
            Close();
            bHandled = FALSE;
        }

        return 0;
    }

	LRESULT CMenuUIEx::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (wParam == VK_ESCAPE) Close();
        return 0;
    }

    LRESULT CMenuUIEx::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (!::IsIconic(*this))
		{
            CRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            HRGN hRgn = ::CreateRectRgn(rcWnd.left + 8, rcWnd.top + 8, rcWnd.right - 8, rcWnd.bottom - 8);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        if (m_pShadowWnd) 
		{
            RECT rcWnd = { 0 };
            ::GetWindowRect(m_hWnd, &rcWnd );
            ::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right -rcWnd.left,
                rcWnd.bottom -rcWnd.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
        }

        bHandled = FALSE;
        return 0;
    }

    LRESULT CMenuUIEx::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (m_pShadowWnd) 
		{
            RECT rcWnd = { 0 };
            ::GetWindowRect(m_hWnd, &rcWnd );
            ::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right -rcWnd.left,
                rcWnd.bottom -rcWnd.top, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
        }

        bHandled = FALSE;
        return 0;
    }

    LRESULT CMenuUIEx::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch (uMsg) 
		{
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break; 
		case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEWHEEL:    break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_MOVE:          lRes = OnMove(uMsg, wParam, lParam, bHandled); break;
		default:			   bHandled = FALSE;
		}
		if (bHandled) return lRes;
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

}