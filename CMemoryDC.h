#pragma once
#include <afxwin.h>

class CMemoryDC :
	public CDC
{
	CSize m_size;
public:
	// 非压缩输出
	void BitTrans(
		int nXDest,		// 目标起点X
		int nYDest,		// 目标起点Y
		int nWidthDest,	// 目标宽度
		int nHeightDest,// 目标高度
		CDC* pDC,		// 目标DC
		int nXSrc,		// 来源起点X
		int nYSrc,		// 来源起点Y
		COLORREF crTrans// 透明色
	);

	// 压缩输出
	void StretchTrans(
		int nXDest,			// 目标起点X
		int nYDest,			// 目标起点Y
		int nWidthDest,     // 目标宽度
		int nHeightDest,    // 目标高度
		CDC* pDC,			// 目标DC
		int nXSrc,			// 来源起点X
		int nYSrc,			// 来源起点Y
		int nWidthSrc,		// 来源宽度
		int nHeightSrc,		// 来源高度
		COLORREF crTrans	// 透明色
	);

	BOOL LoadBitmap(UINT nIDResource, CDC* pDC = NULL); // 通过内部资源ID加载
	BOOL LoadBitmap(LPCTSTR lpszResourceName, CDC* pDC = NULL); // 通过外部文件加载
	BOOL DeleteDC(); // 删除DC
	BOOL Create(int cx, int cy, CDC* pDC = NULL); // 创建一张空白内存画布

	CMemoryDC(int cx, int cy, CDC* pDC = NULL) // 空白DC构造
	{
		Create(cx, cy, pDC);
	}

	CMemoryDC() // 无参构造
	{
		m_size.cx = m_size.cy = 0;
	}

	CMemoryDC(UINT nIDResource, CDC* pDC = NULL) // 资源ID构造
	{
		LoadBitmap(nIDResource, pDC);
	}

	CMemoryDC(LPCTSTR lpszResourceName, CDC* pDC = NULL) // 资源路径构造
	{
		LoadBitmap(lpszResourceName, pDC);
	}

	~CMemoryDC() // 析构函数
	{
		DeleteDC();
	}

	inline int GetWidth() const // 获取图片宽度
	{
		return m_size.cx;
	}

	inline int GetHeight() const // 获取图片高度
	{
		return m_size.cy;
	}

};

