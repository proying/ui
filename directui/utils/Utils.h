#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once

namespace DirectUI
{
	class CSize;
	class CPoint;
	class CRect;
	class CUIString;
	class CStdPtrArray;
	class CStdStringPtrMap;
	class CWaitCursor;
	class CVariant;

	/////////////////////////////////////////////////////////////////////////////////////
	//STRINGorID

	class DirectUI_API STRINGorID
	{
	public:
		STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
		{ }
		STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
		{ }
		LPCTSTR m_lpstr;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//CPoint - Wrapper for Windows POINT structure.

	class DirectUI_API CPoint : public tagPOINT
	{
	public:
		CPoint();
		CPoint(const POINT& src);
		CPoint(int x, int y);
		CPoint(LPARAM lParam);
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//CSize - Wrapper for Windows SIZE structure.

	class DirectUI_API CSize : public tagSIZE
	{
	public:
		CSize();
		CSize(const SIZE& src);
		CSize(const RECT rc);
		CSize(int cx, int cy);
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//CRect - Wrapper for Windows RECT structure.

	class DirectUI_API CRect : public tagRECT
	{
	public:
		CRect();
		CRect(const RECT& srcRect);
		CRect(LPCRECT lpSrcRect);
		CRect(int nLeft, int nTop, int nRight, int nBottom);

		int GetWidth() const;
		int GetHeight() const;
		void Empty();
		bool IsNull() const;
		void Join(const RECT& rc);
		void ResetOffset();
		void Normalize();
		void Offset(int cx, int cy);
		void Inflate(int cx, int cy);
		void Inflate(LPCRECT lpRect);
		void Deflate(int cx, int cy);
		void Deflate(LPCRECT lpRect);
		void MoveToXY(int cx, int cy);
		void Union(CRect& rc);
		void SetRect(int x1, int y1, int x2, int y2);
		void SetRect(POINT topLeft, POINT bottomRight);
		void SetRectEmpty();
		void CopyRect(LPCRECT lpSrcRect);
		BOOL EqualRect(LPCRECT lpRect) const;

		CRect operator +(LPCRECT lpRect) const;
		CRect operator -(LPCRECT lpRect) const;
		void operator =(const RECT& srcRect);
		BOOL operator ==(const RECT& rect) const;
		BOOL operator !=(const RECT& rect) const;

	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DirectUI_API CStdPtrArray
	{
	public:
		CStdPtrArray(int iPreallocSize = 0);
		CStdPtrArray(const CStdPtrArray& src);
		~CStdPtrArray();

		void Empty();
		void Resize(int iSize);
		bool IsEmpty() const;
		int Find(LPVOID nIndex) const;
		bool Add(LPVOID pData);
		bool SetAt(int nIndex, LPVOID pData);
		bool InsertAt(int nIndex, LPVOID pData);
		bool Remove(int nIndex);
		int GetSize() const;
		LPVOID* GetData();

		LPVOID GetAt(int nIndex) const;
		LPVOID operator[] (int nIndex) const;

	protected:
		LPVOID* m_ppVoid;
		int m_nCount;
		int m_nAllocated;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DirectUI_API CStdValArray
	{
	public:
		CStdValArray(int iElementSize, int iPreallocSize = 0);
		~CStdValArray();

		void Empty();
		bool IsEmpty() const;
		bool Add(LPCVOID pData);
		bool Remove(int nIndex);
		int GetSize() const;
		LPVOID GetData();

		LPVOID GetAt(int nIndex) const;
		LPVOID operator[] (int nIndex) const;

	protected:
		LPBYTE m_pVoid;
		int m_iElementSize;
		int m_nCount;
		int m_nAllocated;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DirectUI_API CUIString
	{
	public:
		enum { MAX_LOCAL_STRING_LEN = MAX_PATH };

		CUIString();
		CUIString(const TCHAR ch);
		CUIString(const CUIString& src);
		CUIString(LPCTSTR lpsz, int nLen = -1);
		~CUIString();

		void Empty();
		int GetLength() const;
		bool IsEmpty() const;
		TCHAR GetAt(int nIndex) const;
		void Append(LPCTSTR pstr);
		void Assign(LPCTSTR pstr, int nLength = -1);
		LPCTSTR GetData() const;

		void SetAt(int nIndex, TCHAR ch);
		operator LPCTSTR() const;

		TCHAR operator[] (int nIndex) const;
		const CUIString& operator=(const CUIString& src);
		const CUIString& operator=(const TCHAR ch);
		const CUIString& operator=(LPCTSTR pstr);
#ifdef _UNICODE
		const CUIString& CUIString::operator=(LPCSTR lpStr);
		const CUIString& CUIString::operator+=(LPCSTR lpStr);
#else
		const CUIString& CUIString::operator=(LPCWSTR lpwStr);
		const CUIString& CUIString::operator+=(LPCWSTR lpwStr);
#endif
		CUIString operator+(const CUIString& src) const;
		CUIString operator+(LPCTSTR pstr) const;
		const CUIString& operator+=(const CUIString& src);
		const CUIString& operator+=(LPCTSTR pstr);
		const CUIString& operator+=(const TCHAR ch);

		bool operator == (LPCTSTR str) const;
		bool operator != (LPCTSTR str) const;
		bool operator <= (LPCTSTR str) const;
		bool operator <  (LPCTSTR str) const;
		bool operator >= (LPCTSTR str) const;
		bool operator >  (LPCTSTR str) const;

		int Compare(LPCTSTR pstr) const;
		int CompareNoCase(LPCTSTR pstr) const;

		void MakeUpper();
		void MakeLower();

		CUIString Left(int nLength) const;
		CUIString Mid(int iPos, int nLength = -1) const;
		CUIString Right(int nLength) const;

		int Find(TCHAR ch, int iPos = 0) const;
		int Find(LPCTSTR pstr, int iPos = 0) const;
		int ReverseFind(TCHAR ch) const;
		int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

		int __cdecl Format(LPCTSTR pstrFormat, ...);
		int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

	protected:
		LPTSTR m_pstr;
		TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	struct TITEM
	{
		CUIString Key;
		LPVOID Data;
		struct TITEM* pPrev;
		struct TITEM* pNext;
	};

	class DirectUI_API CStdStringPtrMap
	{
	public:
		CStdStringPtrMap(int nSize = 83);
		~CStdStringPtrMap();

		void Resize(int nSize = 83);
		LPVOID Find(LPCTSTR key, bool optimize = true) const;
		bool Insert(LPCTSTR key, LPVOID pData);
		LPVOID Set(LPCTSTR key, LPVOID pData);
		bool Remove(LPCTSTR key);
		void RemoveAll();
		int GetSize() const;
		LPCTSTR GetAt(int nIndex) const;
		LPCTSTR operator[] (int nIndex) const;

	protected:
		TITEM** m_aT;
		int m_nBuckets;
		int m_nCount;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class DirectUI_API CWaitCursor
	{
	public:
		CWaitCursor();
		~CWaitCursor();

	protected:
		HCURSOR m_hOrigCursor;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CVariant : public VARIANT
	{
	public:
		CVariant() 
		{ 
			VariantInit(this); 
		}
		CVariant(int i)
		{
			VariantInit(this);
			this->vt = VT_I4;
			this->intVal = i;
		}
		CVariant(float f)
		{
			VariantInit(this);
			this->vt = VT_R4;
			this->fltVal = f;
		}
		CVariant(LPOLESTR s)
		{
			VariantInit(this);
			this->vt = VT_BSTR;
			this->bstrVal = s;
		}
		CVariant(IDispatch *disp)
		{
			VariantInit(this);
			this->vt = VT_DISPATCH;
			this->pdispVal = disp;
		}

		~CVariant() 
		{ 
			VariantClear(this); 
		}
	};

}// namespace DirectUI

#endif // __UTILS_H__