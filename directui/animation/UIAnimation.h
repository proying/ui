#ifndef __UIANIMATION_H__
#define __UIANIMATION_H__

#include <vector>
#include <d3d9.h>
#include <math.h>

namespace DirectUI 
{
	class CControlUI;
	class IAnimationUI
	{
	public:
		virtual BOOL StartAnimation(int nElapse, int nTotalFrame, int nAnimationID = 0, BOOL bLoop = FALSE) = 0;
		virtual void StopAnimation(int nAnimationID = 0) = 0;
		virtual BOOL IsAnimationRunning(int nAnimationID) = 0;
		virtual int GetCurrentFrame(int nAnimationID = 0) = 0;
		virtual BOOL SetCurrentFrame(int nFrame, int nAnimationID = 0) = 0;

		virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) = 0;
		virtual void OnAnimationStart(int nAnimationID, BOOL bFirstLoop) = 0;
		virtual void OnAnimationStop(int nAnimationID) = 0;

		virtual void OnAnimationElapse(int nAnimationID) = 0;
	};

	class DirectUI_API CAnimationData
	{
	public:
		CAnimationData(int nElipse, int nFrame, int nID, BOOL bLoop)
		{
			m_bFirstLoop = TRUE;
			m_nCurFrame = 0;
			m_nElapse = nElipse;
			m_nTotalFrame = nFrame;
			m_bLoop = bLoop;
			m_nAnimationID = nID;
		}

	public:
		friend class CAnimationUI;

		int m_nAnimationID;
		int m_nElapse;

		int m_nTotalFrame;
		int m_nCurFrame;

		BOOL m_bLoop;
		BOOL m_bFirstLoop;
	};

	class DirectUI_API CAnimationUI: public IAnimationUI
	{
	public:
		CAnimationUI(CControlUI* pOwner);
		~CAnimationUI();

		virtual BOOL StartAnimation(int nElapse, int nTotalFrame, int nAnimationID = 0, BOOL bLoop = FALSE);
		virtual void StopAnimation(int nAnimationID = 0);
		virtual BOOL IsAnimationRunning(int nAnimationID);
		virtual int GetCurrentFrame(int nAnimationID = 0);
		virtual BOOL SetCurrentFrame(int nFrame, int nAnimationID = 0);

		virtual void OnAnimationStart(int nAnimationID, BOOL bFirstLoop) {};
		virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) {};
		virtual void OnAnimationStop(int nAnimationID) {};

		virtual void OnAnimationElapse(int nAnimationID);

	protected:
		CAnimationData* GetAnimationDataByID(int nAnimationID);

	protected:
		CControlUI* m_pControl;
		typedef std::vector<CAnimationData*> AnimationsVector;
		typedef AnimationsVector::iterator AnimationsIterator;
		AnimationsVector *m_pAnimationsVector;
	};


/////////////////////////////////////////////////////////////////////////////////////
//
	typedef enum
	{
		UIANIMTYPE_FLAT,
		UIANIMTYPE_SWIPE,
	} UITYPE_ANIM;

	class DirectUI_API CAnimJobUI
	{
	public:
		CAnimJobUI(const CAnimJobUI& src);
		CAnimJobUI(UITYPE_ANIM AnimType, DWORD dwStartTick, DWORD dwDuration, COLORREF clrBack, 
			COLORREF clrKey, RECT rcFrom, int xtrans, int ytrans, int ztrans, int alpha, float zrot);

		typedef enum
		{
			INTERPOLATE_LINEAR,
			INTERPOLATE_COS,
		} INTERPOLATETYPE;

		typedef struct PLOTMATRIX
		{
			int xtrans;
			int ytrans;
			int ztrans;
			int alpha;
			float zrot;
		} PLOTMATRIX;

		UITYPE_ANIM AnimType;
		DWORD dwStartTick;
		DWORD dwDuration;
		int iBufferStart;
		int iBufferEnd;
		union
		{
			struct 
			{
				COLORREF clrBack;
				COLORREF clrKey;
				RECT rcFrom;
				PLOTMATRIX mFrom;
				INTERPOLATETYPE iInterpolate;
			} plot;
		} data;
	};

} // namespace DirectUI

#endif // __UIANIMATION_H__