#include "CMemoryDC.h"

void CMemoryDC::BitTrans(int nXDest, int nYDest, int nWidthDest, 
	int nHeightDest, CDC* pDC, int nXSrc, int nYSrc, COLORREF crTrans)
{
	CMemoryDC dcImage(nWidthDest, nHeightDest, pDC);//临时DC
	CBitmap bmpMask;
	bmpMask.CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);            // 创建单色掩码位图
	CDC dcMask;//掩码DC 
	dcMask.CreateCompatibleDC(pDC);
	dcMask.SelectObject(bmpMask);
	//将载入位图的内存DC中的位图，拷贝到临时DC中
	dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, this, nXSrc, nYSrc, SRCCOPY);

	// 设置临时DC的透明色
	dcImage.SetBkColor(crTrans);
	//掩码DC的透明区域为白色其它区域为黑色
	dcMask.BitBlt(0, 0, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCCOPY);

	//临时DC透明区域为黑色，其它区域保持不变
	dcImage.SetBkColor(RGB(0, 0, 0));
	dcImage.SetTextColor(RGB(255, 255, 255));
	dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);

	// 目标DC透明部分保持屏幕不变，其它部分变成黑色
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->BitBlt(nXDest, nYDest, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);
	pDC->BitBlt(nXDest, nYDest, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCPAINT);
}

void CMemoryDC::StretchTrans(int nXDest, int nYDest, int nWidthDest, 
	int nHeightDest, CDC* pDC, int nXSrc, int nYSrc, 
	int nWidthSrc, int nHeightSrc, COLORREF crTrans)
{
	CMemoryDC dcImage(nWidthDest, nHeightDest, pDC);// 临时DC
	CBitmap bmpMask;
	// 创建单色掩码位图
	bmpMask.CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);
	CDC dcMask;
	dcMask.CreateCompatibleDC(pDC);
	dcMask.SelectObject(bmpMask);

	// 将载入位图的内存DC中的位图，拷贝到临时DC中
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, this, nXSrc, nYSrc, SRCCOPY);
	else
		dcImage.StretchBlt(0, 0, nWidthDest, nHeightDest,
			this, nXSrc, nYSrc, nWidthSrc, nHeightSrc, SRCCOPY);

	// 设置临时DC的透明色
	dcImage.SetBkColor(crTrans);
	//掩码DC的透明区域为白色其它区域为黑色
	dcMask.BitBlt(0, 0, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCCOPY);

	// 临时DC透明区域为黑色，其它区域保持不变
	dcImage.SetBkColor(RGB(0, 0, 0));
	dcImage.SetTextColor(RGB(255, 255, 255));
	dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);

	// 目标DC透明部分保持屏幕不变，其它部分变成黑色
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->BitBlt(nXDest, nYDest, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);
	pDC->BitBlt(nXDest, nYDest, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCPAINT);
}

BOOL CMemoryDC::LoadBitmap(UINT nIDResource, CDC* pDC)
{
	CBitmap bitmap;
	if (!bitmap.LoadBitmap(nIDResource))
	{
		m_size.cx = m_size.cy = 0;
		return FALSE;
	}
	BITMAP bm;
	bitmap.GetBitmap(&bm);
	m_size.SetSize(bm.bmWidth, bm.bmHeight);
	CreateCompatibleDC(pDC);
	SelectObject(bitmap);
	return TRUE;
}

BOOL CMemoryDC::LoadBitmap(LPCTSTR lpszResourceName, CDC* pDC)
{
	BITMAP bitmap;
	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, lpszResourceName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!hBitmap)
	{
		m_size.cx = m_size.cy = 0;
		return FALSE;
	}
	::GetObject(hBitmap, sizeof(bitmap), &bitmap);
	m_size.SetSize(bitmap.bmWidth, bitmap.bmHeight);
	CreateCompatibleDC(pDC);
	SelectObject(hBitmap);
	return TRUE;
}

BOOL CMemoryDC::DeleteDC()
{
	if (!GetSafeHdc())
		return TRUE;
	CBitmap* pBitmap = GetCurrentBitmap();
	pBitmap->DeleteObject();
	return CDC::DeleteDC();
}

BOOL CMemoryDC::Create(int cx, int cy, CDC* pDC)
{
	CreateCompatibleDC(pDC);
	CBitmap bitmap;
	if (pDC)
		bitmap.CreateCompatibleBitmap(pDC, cx, cy);
	else
		bitmap.CreateCompatibleBitmap(&CClientDC(NULL), cx, cy);
	m_size.cx = cx;
	m_size.cy = cy;
	SelectObject(bitmap);
	return TRUE;
}
