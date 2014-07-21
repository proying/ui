#ifndef __UIANIMATIONTABLAYOUT_H__
#define __UIANIMATIONTABLAYOUT_H__

namespace DirectUI
{

	class DirectUI_API CAnimationTabLayoutUI: 
		  public CTabLayoutUI, 
		  public CAnimationUI
	{
	public:
		CAnimationTabLayoutUI();
		~CAnimationTabLayoutUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		virtual bool SelectItem(int nIndex);
		void AnimationSwitch();
		void DoEvent(TEventUI& event);
		void OnTimer(int nTimerID);

		virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) {}
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
		virtual void OnAnimationStop(INT nAnimationID);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		bool m_bIsVerticalDirection;
		int m_nPositiveDirection;
		RECT m_rcCurPos;
		RECT m_rcItemOld;
		CControlUI* m_pCurrentControl;
		bool m_bControlVisibleFlag;
		enum
		{
			TAB_ANIMATION_ID = 1,

			TAB_ANIMATION_ELLAPSE = 1,
			TAB_ANIMATION_FRAME_COUNT = 15,
		};
	};

}

#endif //__UIANIMATIONTABLAYOUT_H__