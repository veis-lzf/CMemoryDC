#include "CMemoryDC.h"

void CMemoryDC::BitTrans(int nXDest, int nYDest, int nWidthDest, 
	int nHeightDest, CDC* pDC, int nXSrc, int nYSrc, COLORREF crTrans)
{
	CMemoryDC dcImage(nWidthDest, nHeightDest, pDC);//��ʱDC
	CBitmap bmpMask;
	bmpMask.CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);            // ������ɫ����λͼ
	CDC dcMask;//����DC 
	dcMask.CreateCompatibleDC(pDC);
	dcMask.SelectObject(bmpMask);
	//������λͼ���ڴ�DC�е�λͼ����������ʱDC��
	dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, this, nXSrc, nYSrc, SRCCOPY);

	// ������ʱDC��͸��ɫ
	dcImage.SetBkColor(crTrans);
	//����DC��͸������Ϊ��ɫ��������Ϊ��ɫ
	dcMask.BitBlt(0, 0, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCCOPY);

	//��ʱDC͸������Ϊ��ɫ���������򱣳ֲ���
	dcImage.SetBkColor(RGB(0, 0, 0));
	dcImage.SetTextColor(RGB(255, 255, 255));
	dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);

	// Ŀ��DC͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->BitBlt(nXDest, nYDest, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);
	pDC->BitBlt(nXDest, nYDest, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCPAINT);
}

void CMemoryDC::StretchTrans(int nXDest, int nYDest, int nWidthDest, 
	int nHeightDest, CDC* pDC, int nXSrc, int nYSrc, 
	int nWidthSrc, int nHeightSrc, COLORREF crTrans)
{
	CMemoryDC dcImage(nWidthDest, nHeightDest, pDC);// ��ʱDC
	CBitmap bmpMask;
	// ������ɫ����λͼ
	bmpMask.CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);
	CDC dcMask;
	dcMask.CreateCompatibleDC(pDC);
	dcMask.SelectObject(bmpMask);

	// ������λͼ���ڴ�DC�е�λͼ����������ʱDC��
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, this, nXSrc, nYSrc, SRCCOPY);
	else
		dcImage.StretchBlt(0, 0, nWidthDest, nHeightDest,
			this, nXSrc, nYSrc, nWidthSrc, nHeightSrc, SRCCOPY);

	// ������ʱDC��͸��ɫ
	dcImage.SetBkColor(crTrans);
	//����DC��͸������Ϊ��ɫ��������Ϊ��ɫ
	dcMask.BitBlt(0, 0, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCCOPY);

	// ��ʱDC͸������Ϊ��ɫ���������򱣳ֲ���
	dcImage.SetBkColor(RGB(0, 0, 0));
	dcImage.SetTextColor(RGB(255, 255, 255));
	dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);

	// Ŀ��DC͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ
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
