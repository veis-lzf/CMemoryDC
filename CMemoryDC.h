#pragma once
#include <afxwin.h>

class CMemoryDC :
	public CDC
{
	CSize m_size;
public:
	// ��ѹ�����
	void BitTrans(
		int nXDest,		// Ŀ�����X
		int nYDest,		// Ŀ�����Y
		int nWidthDest,	// Ŀ����
		int nHeightDest,// Ŀ��߶�
		CDC* pDC,		// Ŀ��DC
		int nXSrc,		// ��Դ���X
		int nYSrc,		// ��Դ���Y
		COLORREF crTrans// ͸��ɫ
	);

	// ѹ�����
	void StretchTrans(
		int nXDest,			// Ŀ�����X
		int nYDest,			// Ŀ�����Y
		int nWidthDest,     // Ŀ����
		int nHeightDest,    // Ŀ��߶�
		CDC* pDC,			// Ŀ��DC
		int nXSrc,			// ��Դ���X
		int nYSrc,			// ��Դ���Y
		int nWidthSrc,		// ��Դ���
		int nHeightSrc,		// ��Դ�߶�
		COLORREF crTrans	// ͸��ɫ
	);

	BOOL LoadBitmap(UINT nIDResource, CDC* pDC = NULL); // ͨ���ڲ���ԴID����
	BOOL LoadBitmap(LPCTSTR lpszResourceName, CDC* pDC = NULL); // ͨ���ⲿ�ļ�����
	BOOL DeleteDC(); // ɾ��DC
	BOOL Create(int cx, int cy, CDC* pDC = NULL); // ����һ�ſհ��ڴ滭��

	CMemoryDC(int cx, int cy, CDC* pDC = NULL) // �հ�DC����
	{
		Create(cx, cy, pDC);
	}

	CMemoryDC() // �޲ι���
	{
		m_size.cx = m_size.cy = 0;
	}

	CMemoryDC(UINT nIDResource, CDC* pDC = NULL) // ��ԴID����
	{
		LoadBitmap(nIDResource, pDC);
	}

	CMemoryDC(LPCTSTR lpszResourceName, CDC* pDC = NULL) // ��Դ·������
	{
		LoadBitmap(lpszResourceName, pDC);
	}

	~CMemoryDC() // ��������
	{
		DeleteDC();
	}

	inline int GetWidth() const // ��ȡͼƬ���
	{
		return m_size.cx;
	}

	inline int GetHeight() const // ��ȡͼƬ�߶�
	{
		return m_size.cy;
	}

};

