#include "StdAfx.h"
#include "UIRotateButton.h"

#define PI 3.1415926

namespace DirectUI
{
	CRotateButtonUI::CRotateButtonUI()
		:m_nRadius(1),m_uButtonState(0),m_fAngle(0)
	{
		m_ptCenter.x = m_ptCenter.y = 0;
		m_ptCurrent.x = m_ptCurrent.y = 0;
	}

	CRotateButtonUI::~CRotateButtonUI()
	{
	}

	LPCTSTR CRotateButtonUI::GetClass() const
	{
		return _T("RotateButtonUI");
	}

	UINT CRotateButtonUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	LPVOID CRotateButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_ROTATEBUTTON) == 0)
			return static_cast<CRotateButtonUI*>(this);

		return CControlUI::GetInterface(pstrName);
	}

	LPCTSTR CRotateButtonUI::GetNormalImage() const
	{
		return m_strNormalImage;
	}

	void CRotateButtonUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_strNormalImage = pStrImage;
	}

	LPCTSTR CRotateButtonUI::GetHotImage() const
	{
		return m_strHotImage;
	}

	void CRotateButtonUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_strHotImage = pStrImage;
	}

	int	CRotateButtonUI::GetRadius() const
	{
		return m_nRadius;
	}

	void CRotateButtonUI::SetRadius(LPCTSTR pStrRadius)
	{
		ASSERT(pStrRadius);
		m_nRadius = _ttoi(pStrRadius);
	}

	POINT CRotateButtonUI::GetCenterPoint() const
	{
		return m_ptCenter;
	}

	void CRotateButtonUI::SetCenterPoint(LPCTSTR pStrPoint)
	{
		ASSERT(pStrPoint);
        LPTSTR pstr = NULL;
		m_ptCenter.x = _tcstol(pStrPoint, &pstr, 10);  ASSERT(pstr);    
        m_ptCenter.y = _tcstol(pstr + 1, &pstr, 10);   ASSERT(pstr);   
	}

	SIZE CRotateButtonUI::EstimateSize(SIZE szAvailable)
	{
		if (m_cxyFixed.cy == 0) 
			return CSize(m_cxyFixed.cx, 8);

		return CControlUI::EstimateSize(szAvailable);
	}

	bool CRotateButtonUI::Activate()
	{
		if (!CControlUI::Activate()) return false;
		if (m_pManager) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);

		return true;
	}

	void CRotateButtonUI::DoEvent(TEventUI& event)
	{
		if (!IsMouseEnabled() && 
			event.Type > UIEVENT__MOUSEBEGIN && 
			event.Type < UIEVENT__MOUSEEND) 
		{
			if (m_pParent) 
			{
				m_pParent->DoEvent(event);
			}
			return;
		}

		if (event.Type == UIEVENT_MOUSEMOVE)
		{
			if ((m_uButtonState & UISTATE_CAPTURED) != 0) 
			{
				AdjustDragButton(event.ptMouse);
				Invalidate();
			}
			return;
		}

		if (event.Type == UIEVENT_BUTTONDOWN)
		{
			if (IsEnabled())
			{
				if (::PtInRect(&m_rcItem, event.ptMouse)) 
					m_uButtonState |= UISTATE_CAPTURED;
			}
			return;
		}

		if (event.Type == UIEVENT_BUTTONUP)
		{
			if ((m_uButtonState & UISTATE_CAPTURED) != 0 )
			{
				if (::PtInRect(&m_rcItem, event.ptMouse))
					Activate();

				m_uButtonState &= ~UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}

		if (event.Type == UIEVENT_CONTEXTMENU)
		{
			return;
		}

		if (event.Type == UIEVENT_KEYDOWN)
		{
			if (IsKeyboardEnabled()) 
			{
				if (event.chKey == VK_SPACE || event.chKey == VK_RETURN)  
				{
					Activate();
					return;
				}
			}
		}

		if (event.Type == UIEVENT_SETFOCUS) 
		{
			Invalidate();
		}
		if ( event.Type == UIEVENT_KILLFOCUS) 
		{
			Invalidate();
		}

		CControlUI::DoEvent(event);
	}

	void CRotateButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("radius")) == 0) SetRadius(pstrValue);
		if (_tcscmp(pstrName, _T("centerpoint")) == 0) SetCenterPoint(pstrValue);
		if (_tcscmp(pstrName, _T("normalimage")) == 0) SetNormalImage(pstrValue);
		else if (_tcscmp(pstrName, _T("hotimage")) == 0) SetHotImage(pstrValue);
		else CControlUI::SetAttribute(pstrName, pstrValue);		
	}

	void CRotateButtonUI::AdjustDragButton(POINT &ptMove)
	{
		UINT nXCoord = abs(m_ptCenter.x - ptMove.x);
		UINT nYCoord = abs(m_ptCenter.y - ptMove.y);
		if ((nXCoord * nXCoord) + (nYCoord * nYCoord) < m_nRadius * m_nRadius)
		{
			double fSlope = (ptMove.y - m_ptCurrent.y)/(ptMove.x - m_ptCurrent.x);
			m_fAngle = atan(fSlope) * 180 / PI;

			m_ptCurrent.x = ptMove.x;
			m_ptCurrent.y = ptMove.y;
			return;
		}
		
		double fOutside = sqrt(double((nXCoord * nXCoord) + (nYCoord * nYCoord)));
		UINT nNewX = (nXCoord * m_nRadius)/fOutside;
		UINT nNewY = (nYCoord * m_nRadius)/fOutside;
		ptMove.x = m_ptCenter.x + nNewX;
		ptMove.y = m_ptCenter.y + nNewY;

		double fSlope = double(ptMove.y - m_ptCurrent.y)/(ptMove.x - m_ptCurrent.x);
		m_fAngle = atan(fSlope) * 180 / PI;

		m_ptCurrent.x = ptMove.x;
		m_ptCurrent.y = ptMove.y;
	}

	void CRotateButtonUI::PaintStatusImage(HDC hDC)
	{
		if (m_ptCenter.x == 0 || m_ptCenter.y == 0)
			return;

		Graphics graphics(hDC);
		//REAL	 angle;
		//Matrix matrix;
		//matrix.Rotate(m_fAngle);
		graphics.RotateTransform(45.0f);
		CUIString strImage = CPaintManagerUI::GetResourcePath();
		strImage += m_strNormalImage;
		Image image(strImage);
		int nWidth = image.GetWidth();
		int nHeight = image.GetHeight();
		graphics.DrawImage(&image, m_rcItem.left, m_rcItem.top, image.GetWidth(), image.GetHeight());
		//image.RotateFlip(Rotate90FlipNone);
		//graphics.DrawImage(&image, 0, 0);

		//if (!m_strNormalImage.IsEmpty()) 
		//{
		//	if (!DrawImage(hDC, (LPCTSTR)m_strNormalImage)) 
		//		m_strNormalImage.Empty();
		//}
	}
}