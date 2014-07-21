#include "stdafx.h"
#include "UITabLayout.h"

namespace DirectUI
{
	CTabLayoutUI::CTabLayoutUI() : m_nCurSel(-1)
	{
	}

	LPCTSTR CTabLayoutUI::GetClass() const
	{
		return _T("TabLayoutUI");
	}

	LPVOID CTabLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_TABLAYOUT) == 0) 
			return static_cast<CTabLayoutUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	bool CTabLayoutUI::Add(CControlUI* pControl)
	{
		bool ret = CContainerUI::Add(pControl);
		if( !ret ) return ret;

		if(m_nCurSel == -1 && pControl->IsVisible())
		{
			m_nCurSel = GetItemIndex(pControl);
		}
		else
		{
			pControl->SetVisible(false);
		}

		return ret;
	}

	bool CTabLayoutUI::AddAt(CControlUI* pControl, int nIndex)
	{
		bool ret = CContainerUI::AddAt(pControl, nIndex);
		if (!ret) return ret;

		if (m_nCurSel == -1 && pControl->IsVisible())
		{
			m_nCurSel = GetItemIndex(pControl);
		}
		else if (m_nCurSel != -1 && nIndex <= m_nCurSel)
		{
			m_nCurSel += 1;
		}
		else
		{
			pControl->SetVisible(false);
		}

		return ret;
	}

	bool CTabLayoutUI::Remove(CControlUI* pControl)
	{
		if (pControl == NULL) return false;

		int index = GetItemIndex(pControl);
		bool ret = CContainerUI::Remove(pControl);
		if( !ret ) return false;

		if (m_nCurSel == index)
		{
			if (GetCount() > 0)
			{
				m_nCurSel=0;
				GetItemAt(m_nCurSel)->SetVisible(true);
			}
			else m_nCurSel=-1;
			NeedParentUpdate();
		}
		else if (m_nCurSel > index)
		{
			m_nCurSel -= 1;
		}

		return ret;
	}

	void CTabLayoutUI::RemoveAll()
	{
		m_nCurSel = -1;
		CContainerUI::RemoveAll();
		NeedParentUpdate();
	}

	int CTabLayoutUI::GetCurSel() const
	{
		return m_nCurSel;
	}

	bool CTabLayoutUI::SelectItem(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_items.GetSize()) return false;
		if (nIndex == m_nCurSel) return true;

		int iOldSel = m_nCurSel;
		m_nCurSel = nIndex;
		for (int it = 0; it < m_items.GetSize(); it++)
		{
			if (it == nIndex)
			{
				GetItemAt(it)->SetVisible(true);
				GetItemAt(it)->SetFocus();
				SetPos(m_rcItem);
			}
			else GetItemAt(it)->SetVisible(false);
		}

		NeedParentUpdate();

		if (m_pManager != NULL)
		{
			m_pManager->SetNextTabControl();
			m_pManager->SendNotify(this, DUI_MSGTYPE_TABSELECT, m_nCurSel, iOldSel);
		}
		return true;
	}

	bool CTabLayoutUI::SelectItem(CControlUI* pControl)
	{
		int nIndex = GetItemIndex(pControl);
		if (nIndex == -1)
			return false;
		else
			return SelectItem(nIndex);
	}

	void CTabLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("selectedid")) == 0) 
			SelectItem(_ttoi(pstrValue));
		return CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	void CTabLayoutUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);
		rc = m_rcItem;

		// Adjust for inset
		rc.left += m_rcInset.left;
		rc.top += m_rcInset.top;
		rc.right -= m_rcInset.right;
		rc.bottom -= m_rcInset.bottom;

		for (int it = 0; it < m_items.GetSize(); it++) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
			if (!pControl->IsVisible()) continue;
			if (pControl->IsFloat()) 
			{
				SetFloatPos(it);
				continue;
			}

			if (it != m_nCurSel) continue;

			RECT rcPadding = pControl->GetPadding();
			rc.left += rcPadding.left;
			rc.top += rcPadding.top;
			rc.right -= rcPadding.right;
			rc.bottom -= rcPadding.bottom;

			SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
			SIZE sz = pControl->EstimateSize(szAvailable);
			if (sz.cx == 0)
			{
				sz.cx = MAX(0, szAvailable.cx);
			}

			if (sz.cx < pControl->GetMinWidth()) sz.cx = pControl->GetMinWidth();
			if (sz.cx > pControl->GetMaxWidth()) sz.cx = pControl->GetMaxWidth();

			if (sz.cy == 0) 
			{
				sz.cy = MAX(0, szAvailable.cy);
			}

			if (sz.cy < pControl->GetMinHeight()) sz.cy = pControl->GetMinHeight();
			if (sz.cy > pControl->GetMaxHeight()) sz.cy = pControl->GetMaxHeight();

			RECT rcCtrl = { rc.left, rc.top, rc.left + sz.cx, rc.top + sz.cy};
			pControl->SetPos(rcCtrl);
		}
	}
}
