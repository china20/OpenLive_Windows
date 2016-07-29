// AGEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AGEdit.h"


// CAGEdit

IMPLEMENT_DYNAMIC(CAGEdit, CEdit)

CAGEdit::CAGEdit()
: m_crBorder(RGB(0xC8, 0xC8, 0xC8))
, m_crBack(RGB(0xF5, 0xF5, 0xF5))
, m_crText(RGB(0x00, 0x00, 0x00))
{
	m_penBorder.CreatePen(PS_SOLID, 1, m_crBorder);
	m_brushBack.CreateSolidBrush(m_crBack);
}

CAGEdit::~CAGEdit()
{
	m_penBorder.DeleteObject();
	m_brushBack.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAGEdit, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
//	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CAGEdit ��Ϣ�������
void CAGEdit::SetBalloonTip(LPCTSTR lpszTitle, LPCTSTR lpszText, int ttiIcon)
{
	m_strTitle = lpszTitle;
	m_strText = lpszText;
	m_ttiIcon = ttiIcon;
}

void CAGEdit::SetColor(COLORREF crBorder, COLORREF crBack, COLORREF crText)
{
	if (crBorder != m_crBorder){
		m_penBorder.DeleteObject();
		m_crBorder = crBorder;
		m_penBorder.CreatePen(PS_SOLID, 1, m_crBorder);
	}

	if (crBack != m_crBack){
		m_brushBack.DeleteObject();
		m_crBack = crBack;
		m_brushBack.CreateSolidBrush(m_crBack);
	}

	Invalidate();
}

BOOL CAGEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcWindow;
	
	GetWindowRect(&rcWindow);

	CPen *lpDefaultPen = pDC->SelectObject(&m_penBorder);
	pDC->Rectangle(&rcWindow);
	pDC->SelectObject(lpDefaultPen);

	return CEdit::OnEraseBkgnd(pDC);
}

HBRUSH CAGEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  �ڴ˸��� DC ���κ�����
	pDC->SetBkColor(m_crBack);
	pDC->SetTextColor(m_crText);

	// TODO:  �����Ӧ���ø����Ĵ�������򷵻ط� null ����
	return (HBRUSH)m_brushBack;	//�༭���ɫ��ˢ
}

void CAGEdit::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ShowBalloonTip(m_strTitle, m_strText, m_ttiIcon);

	CEdit::OnMouseHover(nFlags, point);
}


void CAGEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_bTrackMouseEvent) {
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		TrackMouseEvent.hwndTrack = GetSafeHwnd();
		TrackMouseEvent.dwFlags = TME_LEAVE;
		TrackMouseEvent.dwHoverTime = 1;

		ShowBalloonTip(m_strTitle, m_strText, m_ttiIcon);

		m_bTrackMouseEvent = ::TrackMouseEvent(&TrackMouseEvent);
	}

	CEdit::OnMouseMove(nFlags, point);
}


void CAGEdit::OnMouseLeave()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bTrackMouseEvent = FALSE;

	HideBalloonTip();
	CEdit::OnMouseLeave();
}
