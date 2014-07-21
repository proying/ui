#ifndef __UITABLAYOUT_H__
#define __UITABLAYOUT_H__

#pragma once

namespace DirectUI
{
	class DirectUI_API CTabLayoutUI : public CContainerUI
	{
	public:
		CTabLayoutUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int nIndex);
		bool Remove(CControlUI* pControl);
		void RemoveAll();
		int GetCurSel() const;
		void SetPos(RECT rc);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		virtual bool SelectItem(int nIndex);
		virtual bool SelectItem(CControlUI* pControl);

	protected:
		int m_nCurSel;
	};
}
#endif // __UITABLAYOUT_H__
