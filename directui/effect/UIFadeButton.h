#ifndef __UIFADEBUTTON_H__
#define __UIFADEBUTTON_H__

namespace DirectUI 
{
	class CButtonUI;
	class DirectUI_API CFadeButtonUI: 
		public CButtonUI, 
		public CAnimationUI
	{
	public:
		CFadeButtonUI();
		virtual ~CFadeButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void DoEvent(TEventUI& event);
		void SetFadeEffect(BOOL hasFadeEffect);

		void OnTimer(int nTimerID);
		void PaintStatusImage(HDC hDC);

		virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) {}
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
		virtual void OnAnimationStop(INT nAnimationID) {}
		
	protected:
		enum
		{
			FADE_IN_ID       = 8,
			FADE_OUT_ID      = 9,
			FADE_ELLAPSE     = 10,
			FADE_FRAME_COUNT = 50,
		};		

		BYTE        m_bFadeAlpha;
		BOOL        m_bMouseHove;
		BOOL        m_bMouseLeave;
		CUIString		m_sOldImage;
		CUIString		m_sNewImage;
		CUIString		m_sLastImage;
	private:
		static CUIString UpdateFadeAttribute(const CUIString& imageAttribute, int fadeAlpha);

	private:
		BOOL m_hasFadeEffect;
	};

} // namespace DuiLib

#endif // __UIFADEBUTTON_H__