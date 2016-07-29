#pragma once

#include "DlgStatus.h"

typedef struct _REPORT_THREAD_PARAM
{
	HWND		hMainWnd;	// �����ھ��
	LPCTSTR		pReportPath;
	CDlgStatus *pDlgInformation;

} REPORT_THREAD_PARAM, *PREPORT_THREAD_PARAM;

class CSEHtmlReport
{
public:
	CSEHtmlReport(LPCTSTR lpFilePath);
	~CSEHtmlReport(void);

	void WriteHead();
	void WriteTail();

	void SystemInfoReport();		// ϵͳ��Ϣ
	void CPUInfoReport();			// CPU��Ϣ
	void MainBoardInfoReport();		// ������Ϣ
	void MemoryInfoReport();		// �ڴ���Ϣ
	void DisplayDeviceInfoReport();	// �Կ���Ϣ
	void AudioDeviceInfoReport();	// ������Ϣ
	void DiskInfoReport();			// ������Ϣ
	void NetDeviceInfoReport();		// �����豸��Ϣ

	static UINT HtmlReportThreadProc(LPVOID lpParam);
protected:
	void InitialSmartItem();
	CMap<BYTE, BYTE, CString, CString> m_mapSmartItem;

private:
	FILE *m_pFileStream;
};

