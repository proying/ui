#ifndef __UITEXT_H__
#define __UITEXT_H__

#pragma once

namespace DirectUI
{
	class DirectUI_API CTextUI : public CLabelUI
	{
	public:
		CTextUI();
		~CTextUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		CUIString* GetLinkContent(int nIndex);

		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);

		void PaintText(HDC hDC);

	protected:
		enum { MAX_LINK = 8 };
		int m_nLinks;
		RECT m_rcLinks[MAX_LINK];
		CUIString m_sLinks[MAX_LINK];
		int m_nHoverLink;
	};

} // namespace DirectUI

#endif //__UITEXT_H__