#pragma once
#include "AGButton.h"
#include "AGEdit.h"
#include "DeviceDlg.h"
#include "afxwin.h"

// CEnterChannelDlg �Ի���

class CEnterChannelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterChannelDlg)

public:
	CEnterChannelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEnterChannelDlg();

	CString GetChannelName();

	void SetVideoString(LPCTSTR lpVideoString);

// �Ի�������
	enum { IDD = IDD_ENTERCHANNEL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtntestChannel();
	afx_msg void OnBnClickedBtnjoinChannel();
	afx_msg void OnBnClickedBtnsetChannel();

	afx_msg void OnCbnSelchangeCmbRole();
	DECLARE_MESSAGE_MAP()

protected:
	void InitCtrls();
	void DrawClient(CDC *lpDC);

private:
	CAGEdit			m_ctrChannel;
//	CAGEdit			m_ctrEncKey;
	CAGButton		m_btnTest;
	CAGButton		m_btnJoin;
	CAGButton		m_btnSetup;

	CFont			m_ftChannel;
	CFont			m_ftHead;
	CFont			m_ftDesc;
	CFont			m_ftBtn;

	CDeviceDlg		m_dlgDevice;

	CComboBox		m_ctrRole;
};
