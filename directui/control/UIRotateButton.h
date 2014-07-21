#ifndef __UIROTATEBUTTON_H__
#define __UIROTATEBUTTON_H__

namespace DirectUI
{
	class CRotateButtonUI : public CControlUI
	{
	public:
		CRotateButtonUI();
		~CRotateButtonUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		LPCTSTR GetNormalImage() const;
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage() const;
		void SetHotImage(LPCTSTR pStrImage);
		int	 GetRadius() const;
		void SetRadius(LPCTSTR pStrRadius);
		POINT GetCenterPoint() const;
		void SetCenterPoint(LPCTSTR pStrPoint);

		bool Activate();
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		SIZE EstimateSize(SIZE szAvailable);

		void AdjustAngle(POINT &ptMove);
		void AdjustDragButton(POINT &ptMove);

		void PaintStatusImage(HDC hDC);
	protected:
		int			m_nRadius;
		UINT		m_uButtonState;
		REAL		m_fAngle;
		POINT		m_ptCenter;
		POINT 		m_ptCurrent;
		CUIString	m_strNormalImage;
		CUIString	m_strHotImage;
	};
}

#endif //__UIROTATEBUTTON_H__