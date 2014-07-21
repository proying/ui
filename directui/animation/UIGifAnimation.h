#ifndef __UIGIFANIMATION_H__
#define __UIGIFANIMATION_H__

namespace DirectUI 
{
	class DirectUI_API IGifAnimationCallback
	{
	public:
		virtual void OnFrame() = 0;
	};

	class DirectUI_API CGifAnimation
	{
	public:
		#pragma pack(1)

		enum GIFBlockTypes
		{
			BLOCK_UNKNOWN,
			BLOCK_APPEXT,
			BLOCK_COMMEXT,
			BLOCK_CONTROLEXT,
			BLOCK_PLAINTEXT,
			BLOCK_IMAGE,
			BLOCK_TRAILER
		};

		enum ControlExtValues
		{
			GCX_PACKED_DISPOSAL, 
			GCX_PACKED_USERINPUT,
			GCX_PACKED_TRANSPCOLOR
		};

		enum LSDPackedValues 
		{
			LSD_PACKED_GLOBALCT,
			LSD_PACKED_CRESOLUTION,
			LSD_PACKED_SORT,
			LSD_PACKED_GLOBALCTSIZE
		};

		enum IDPackedValues 
		{
			ID_PACKED_LOCALCT,
			ID_PACKED_INTERLACE,
			ID_PACKED_SORT,
			ID_PACKED_LOCALCTSIZE
		};

		struct TGIFHeader
		{
			char   m_cSignature[3]; //"GIF数据头"
			char   m_cVersion[3];   //版本 "87a" or "89a"
		};

		struct TGIFLSDescriptor
		{
			WORD          m_wWidth;         
			WORD          m_wHeight;      
			unsigned char m_cPacked;       
			unsigned char m_cBkIndex;       
			unsigned char m_cPixelAspect;  
			inline int    GetPackedValue(enum LSDPackedValues Value);
		};

	    struct TGIFAppExtension 
		{
			unsigned char m_cExtIntroducer;     
			unsigned char m_cExtLabel;         
			unsigned char m_cBlockSize;         
			char          m_cAppIdentifier[8];  
			char          m_cAppAuth[3];  
		};

		struct TGIFControlExt 
		{
			unsigned char m_cExtIntroducer; // (0x21)
			unsigned char m_cControlLabel;  // (0xF9)
			unsigned char m_cBlockSize;     // fixed value of 4
			unsigned char m_cPacked;        // packed field
			WORD          m_wDelayTime;     // delay time
			unsigned char m_cTColorIndex;   // transparent color index
			unsigned char m_cBlockTerm;     // block terminator (0x00)
			inline int    GetPackedValue(enum ControlExtValues Value);
		};

		struct TGIFCommentExt 
		{
			unsigned char m_cExtIntroducer; // extension introducer (0x21)
			unsigned char m_cCommentLabel;  // comment extension label (0xFE)
		};

		struct TGIFPlainTextExt
		{
			unsigned char m_cExtIntroducer;     // extension introducer (0x21)
			unsigned char m_cPlainTextLabel;    // text extension label (0x01)
			unsigned char m_cBlockSize;         // fixed value of 12
			WORD          m_wLeftPos;           // text grid left position
			WORD          m_wTopPos;            // text grid top position
			WORD          m_wGridWidth;         // text grid width
			WORD          m_wGridHeight;        // text grid height
			unsigned char m_cCellWidth;         // character cell width
			unsigned char m_cCellHeight;        // character cell height
			unsigned char m_cFgColor;           // text foreground color index
			unsigned char m_cBkColor;           // text background color index
		};

		struct TGIFImageDescriptor // image descriptor block
		{
			unsigned char m_cImageSeparator;    // image separator byte (0x2C)
			WORD          m_wLeftPos;           // image left position
			WORD          m_wTopPos;            // image top position
			WORD          m_wWidth;             // image width
			WORD          m_wHeight;            // image height
			unsigned char m_cPacked;            // packed field
			inline int    GetPackedValue(enum IDPackedValues Value);
		};
		#pragma pack()
		struct TFrame 
		{
			TImageInfo* m_pImage;       // pointer to one frame image
			SIZE        m_frameSize;
			SIZE        m_frameOffset;
			UINT        m_nDelay;       // delay (in 1/100s of a second)
			UINT        m_nDisposal;    // disposal method
		};
		typedef std::vector<TFrame>     VTFRAME;

	public:
		CGifAnimation(IGifAnimationCallback *pCallback);
		~CGifAnimation();

		const TImageInfo* GetCurImage();
		const TImageInfo* LoadGIF(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
		const TImageInfo* LoadGIF(const LPBYTE pData, DWORD dwSize, DWORD mask = 0);
		SIZE GetSize() const;
		size_t GetFrameCount() const;
		COLORREF GetBkColor() const;
		void SetCurrentFrame(long nFrame);

		bool Play();   
		void Stop(); 
		void UnLoad();
		bool IsPlaying() const;
    
	protected:
		int GetNextBlockLen() const;
		BOOL SkipNextBlock();
		BOOL SkipNextGraphicBlock();
		void ResetDataPointer();
		enum GIFBlockTypes GetNextBlock() const;
		UINT GetSubBlocksLen(UINT nStartingOffset) const;
		LPBYTE GetNextGraphicBlock(UINT* pBlockLen, UINT* pDelay, SIZE* pBlockSize, SIZE* pBlockOffset, UINT* pDisposal);

	#ifdef GIF_TRACING
		void EnumGIFBlocks();
		void WriteDataOnDisk(CString szFileName, HGLOBAL hData, DWORD dwSize);
	#endif // GIF_TRACING

	private:
		unsigned char*			m_pRawData;           // 解析时用于暂存动画文件数据 解析结束后重置无效
		UINT					m_nDataSize;          // GIF文件大小
		TGIFHeader*				m_pGIFHeader;         // GIF文件头
		TGIFLSDescriptor*		m_pGIFLSDescriptor;   // 逻辑屏幕标识符
		UINT					m_nGlobalCTSize;      // 全局颜色列表大小
		SIZE					m_PictureSize;        // 图像尺寸
		COLORREF				m_clrBackground;      // 背景色
		volatile long			m_nCurrFrame;         // 当前帧索引值
		UINT					m_nCurrOffset;        // 块读取偏移量
		VTFRAME*				m_pvFrames;           // 帧数组

		HANDLE					m_hThread;
		HANDLE					m_hDrawEvent;         // 绘图事件 代表当前帧已经被读取用于绘制 （防止由于渲染速度慢而导致跳帧）
		volatile bool			m_bExitThread;
		IGifAnimationCallback*	m_pCallback;

		DWORD ThreadAnimation();
		static DWORD __stdcall _ThreadAnimation(LPVOID pParam);
	};

	class DirectUI_API CGifAnimationUI : public IGifAnimationCallback
	{
	public:
		virtual ~CGifAnimationUI();

		bool Play(LPCTSTR bitmap);
		void Stop(LPCTSTR bitmap);

		/**
		 * 绘图函数的增强版 替代原有CControlUI::DrawImage 和 CRenderEngine::DrawImageString函数
		 * Note: 被PaintXXImage调用
		 */
		bool DrawImageEx(HDC hDC, CPaintManagerUI* pManager, const RECT& rcPaint, LPCTSTR pStrImage);
		bool DrawImageEx(HDC hDC, CPaintManagerUI* pManager, const RECT& rc, const RECT& rcPaint, LPCTSTR pStrImage, 
			LPCTSTR pStrModify = NULL);
		/**
		 * 绘图函数的增强版 替代原有DuiLib::DrawImage函数
		 * Note: 包括判断文件类型、初始化读取GIF内容等
		 */
		bool CGifAnimationUI::DrawAniImage(HDC hDC, CPaintManagerUI* pManager, const RECT& rc, const RECT& rcPaint, const CUIString& strImageName,\
			const CUIString& strImageResType, DWORD dwMask);
		bool DrawAniImage(HDC hDC, CPaintManagerUI* pManager, const RECT& rc, const RECT& rcPaint, const CUIString& strImageName, \
			const CUIString& strImageResType, RECT rcItem, RECT rcBmpPart, RECT rcCorner, DWORD dwMask, BYTE bFade, \
			bool bHole, bool bTiledX, bool bTiledY);

 		const TImageInfo* AddAniImage(LPCTSTR bitmap, LPBYTE pData, DWORD dwSize, DWORD mask = 0);
  
	protected:
		const TImageInfo* GetAniImage(LPCTSTR bitmap);
		const TImageInfo* AddAniImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
   
	protected:
		CStdStringPtrMap    m_mAniHash;
	};

	class DirectUI_API CLabelEx : public CLabelUI, public CGifAnimationUI
	{
	public:
		CLabelEx();
		~CLabelEx(){};

		LPCTSTR GetClass() const;
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoEvent(TEventUI& event);

		void PaintBkImage(HDC hDC);
		void OnFrame();
		void SetNotify(DWORD dwIndex, INotifyUI* pNotify);
	protected:
		bool		m_bPlaying;
		DWORD		m_dwIndex;
		INotifyUI*	m_pNotify;
	};
}

#endif //__UIGIFANIMATION_H__