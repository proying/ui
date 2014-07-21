#include "StdAfx.h"
#include "UIAnimation.h"
#include <algorithm>

#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#ifndef PI
   #define PI 3.1415926535897932384626433832795029L
#endif


namespace DirectUI 
{
	CAnimationUI::CAnimationUI(CControlUI* pOwner)
	{
		ASSERT(pOwner != NULL);
		m_pControl = pOwner;
		m_pAnimationsVector = new AnimationsVector;
	}

	CAnimationUI::~CAnimationUI()
	{
		if (m_pAnimationsVector)
		{
			AnimationsIterator it = m_pAnimationsVector->begin();
			for (; it != m_pAnimationsVector->end(); ++it)
			{
				delete *it;
			}

			m_pAnimationsVector->clear();
			delete m_pAnimationsVector;
		}
	}

	BOOL CAnimationUI::StartAnimation(int nElapse, int nTotalFrame, int nAnimationID, BOOL bLoop)
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if( NULL != pData 
			|| nElapse <= 0
			|| nTotalFrame <= 0
			|| NULL == m_pControl )
		{
			ASSERT(FALSE);
			return FALSE;
		}

		CAnimationData* pAnimation = new CAnimationData(nElapse, nTotalFrame, nAnimationID, bLoop);
		if (!pAnimation) return FALSE;
		
		if(m_pControl->GetManager()->SetTimer(m_pControl, nAnimationID, nElapse))
		{
			m_pAnimationsVector->push_back(pAnimation);
			return TRUE;
		}

		return FALSE;
	}

	void CAnimationUI::StopAnimation(int nAnimationID)
	{
		if (!m_pControl) return;

		if (nAnimationID != 0)
		{
			CAnimationData* pData = GetAnimationDataByID(nAnimationID);
			if (pData)
			{
				m_pControl->GetManager()->KillTimer(m_pControl, nAnimationID);
				m_pAnimationsVector->erase(std::remove(m_pAnimationsVector->begin(), m_pAnimationsVector->end(), pData), m_pAnimationsVector->end());
				delete pData; pData = NULL;
				return;
			}
		}
		else
		{
			int nCount = m_pAnimationsVector->size();
			for (int i = 0; i < nCount; ++i)
			{
				m_pControl->GetManager()->KillTimer(m_pControl, (*m_pAnimationsVector)[i]->m_nAnimationID);
			}

			m_pAnimationsVector->clear();
		}
	}

	BOOL CAnimationUI::IsAnimationRunning(int nAnimationID)
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		return (pData != NULL);
	}

	int CAnimationUI::GetCurrentFrame(int nAnimationID/* = 0*/)
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if (!pData)
		{
			ASSERT(FALSE);
			return -1;
		}

		return pData->m_nCurFrame;
	}

	BOOL CAnimationUI::SetCurrentFrame(int nFrame, int nAnimationID/* = 0*/)
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if( NULL == pData)
		{
			ASSERT(FALSE);
			return FALSE;
		}

		if(nFrame >= 0 && nFrame <= pData->m_nTotalFrame)
		{
			pData->m_nCurFrame = nFrame;
			return TRUE;
		}
		else
		{
			ASSERT(FALSE);
		}
		return FALSE;
	}

	void CAnimationUI::OnAnimationElapse(int nAnimationID)
	{
		if (!m_pControl) return;

		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if (NULL == pData) return;

		int nCurFrame = pData->m_nCurFrame;
		if (nCurFrame == 0)
		{
			OnAnimationStart(nAnimationID, pData->m_bFirstLoop);
			pData->m_bFirstLoop = FALSE;
		}

		OnAnimationStep(pData->m_nTotalFrame, nCurFrame, nAnimationID);

		if (nCurFrame >= pData->m_nTotalFrame)
		{
			OnAnimationStop(nAnimationID);
			if(pData->m_bLoop)
			{
				pData->m_nCurFrame = 0;
			}
			else
			{
				m_pControl->GetManager()->KillTimer(m_pControl, nAnimationID);
				m_pAnimationsVector->erase(std::remove(m_pAnimationsVector->begin(), m_pAnimationsVector->end(), pData), m_pAnimationsVector->end());
				delete pData; pData = NULL;
			}
		}

		if (pData)
		{
			++(pData->m_nCurFrame);
		}
	}

	CAnimationData* CAnimationUI::GetAnimationDataByID(int nAnimationID)
	{
		CAnimationData* pRet = NULL;
		int nCount = m_pAnimationsVector->size();
		for (int i=0; i<nCount; ++i)
		{
			if ((*m_pAnimationsVector)[i]->m_nAnimationID == nAnimationID)
			{
				pRet = (*m_pAnimationsVector)[i];
				break;
			}
		}

		return pRet;
	}


/////////////////////////////////////////////////////////////////////////////////////
//
//

	CAnimJobUI::CAnimJobUI(const CAnimJobUI& src)
	{   
	   *this = src;
	}

	CAnimJobUI::CAnimJobUI(UITYPE_ANIM AnimType, DWORD dwStartTick, DWORD dwDuration, COLORREF clrBack, COLORREF clrKey, RECT rcFrom, int xtrans, int ytrans, int ztrans, int alpha, FLOAT zrot)
	{
	   this->AnimType = AnimType;
	   this->dwStartTick = dwStartTick;
	   this->dwDuration = dwDuration;
	   data.plot.clrBack = clrBack;
	   data.plot.clrKey = clrKey;
	   data.plot.rcFrom = rcFrom;
	   data.plot.mFrom.xtrans = xtrans;
	   data.plot.mFrom.ytrans = ytrans;
	   data.plot.mFrom.ztrans = ztrans;
	   data.plot.mFrom.alpha = alpha;
	   data.plot.mFrom.zrot = zrot;
	   data.plot.iInterpolate = INTERPOLATE_COS;
	}


} // namespace DirectUI