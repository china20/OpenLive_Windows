#include "StdAfx.h"
#include "SystemEngineer.h"

#include "HtmlTable.h"
#include "SEHtmlReport.h"

#include "QueryOperatingSystemInfo.h"
#include "SMBQuerySystemInfo.h"

#include "CPUIdInfo.h"
#include "SMBQueryProcessorInfo.h"
#include "SMBQueryCacheInfo.h"

#include "SMBQueryBaseboardInfo.h"
#include "SMBQueryBIOSInfo.h"

#include "SMBMemCtrlInfo.h"
#include "SMBQueryMemModuleInfo.h"
#include "SMBMemDevice.h"
#include "SMBPhyMemArray.h"

#include "QueryVideoControllerInfo.h"
#include "QueryDesktopMonitorInfo.h"

#include "GetMediaInfo.h"

#include "GetDiskInfo.h"
#include "GetCDRomInfo.h"

#include "QueryNetworkAdapterInfo.h"

CSEHtmlReport::CSEHtmlReport(LPCTSTR lpFilePath)
{
	ASSERT(lpFilePath != NULL);
	_tfopen_s(&m_pFileStream, lpFilePath, _T("w+b"));

	CWMIQuery::InitCOMLibrary();
	CWMIQuery::InitCOMSecurity();
}


CSEHtmlReport::~CSEHtmlReport(void)
{
	if(m_pFileStream != NULL)
		fclose(m_pFileStream);

	CWMIQuery::UninitCOMLibrary();
}

void CSEHtmlReport::WriteHead()
{
	ASSERT(m_pFileStream != NULL);

	CStdioFile ReportFile(m_pFileStream);

	CString str;
	CString strInfo;
	SYSTEMTIME LocalTime;

	::GetLocalTime(&LocalTime);

#ifdef _UNICODE
	WORD wUFlag = 0xFEFF;
	ReportFile.Write(&wUFlag, sizeof(WORD));
#endif

	str = _T("<html>\r\n");
	ReportFile.WriteString(str);
	str = _T("<head>\r\n");
	ReportFile.WriteString(str);
	str = _T("<title>SystemEngineerϵͳ��Ϣ����</title>\r\n");
	ReportFile.WriteString(str);
	str = _T("<style type=\"text/css\">\r\n");
	ReportFile.WriteString(str);
	str = _T("<!--\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLECOLUMN {font-size:13px;BACKGROUND-COLOR:#CCCCCC;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLENORMAL {font-size:13px;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLEWARNING {font-size:13px;color:#FF0000;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLEVERIFY {font-size:13px;BACKGROUND-COLOR:#B0C9F1;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLETBITEM {font-size:13px;font-weight:bold;BACKGROUND-COLOR:#E5F0FF;}\r\n");
	ReportFile.WriteString(str);
	str = _T(".STYLETITLE {font-size:14px;font-weight:bold;}\r\n");
	ReportFile.WriteString(str);
	str = _T("-->\r\n");
	ReportFile.WriteString(str);
	str = _T("</style>\r\n");
	ReportFile.WriteString(str);
	str = _T("</head>\r\n");
	ReportFile.WriteString(str);
	str = _T("<body>\r\n");
	ReportFile.WriteString(str);
	str = _T("<p class=\"STYLETITLE\">SystemEngineerϵͳ��Ϣ����</p>\r\n");
	ReportFile.WriteString(str);

	str.Format(_T("����: %d/%d/%d %d:%d <br>\r\n"), 
		LocalTime.wYear,
		LocalTime.wMonth,
		LocalTime.wDay,
		LocalTime.wHour,
		LocalTime.wMinute
		);
	ReportFile.WriteString(str);

	str  = _T("SystemEngineer�汾: ");
	str += SE_VER;
	str += _T("</p>\r\n");
	ReportFile.WriteString(str);
}

void CSEHtmlReport::WriteTail()
{
	CStdioFile ReportFile(m_pFileStream);

	CString str;
	str = _T("</body>\r\n");
	ReportFile.WriteString(str);
	str = _T("</html>\r\n");
	ReportFile.WriteString(str);
}

void CSEHtmlReport::SystemInfoReport()		// ϵͳ��Ϣ
{
	CSMBQuerySystemInfo QuerySystemInfo;
	CQueryOperatingSystemInfo	QueryInfo;
	CHtmlTable Table(m_pFileStream);

	CString str;
	CString strInfo;
	short shTimeZone = 0;

	QuerySystemInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());
	QueryInfo.Create();

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	Table.AddItemTitle(_T("PC��������Ϣ"), STYLE_TBITEM);

	Table.InsertItem(_T("������:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetManufacturer());
	
	Table.InsertItem(_T("����ϵ��:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetFamily());

	Table.InsertItem(_T("��Ʒ�ͺ�:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetProductName());

	Table.InsertItem(_T("��Ʒ�汾:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetVersion());

	Table.InsertItem(_T("��ƷID:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetSerialNumber());

	Table.InsertItem(_T("UUID:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetUUID());

	Table.InsertItem(_T("SKU Number:"), STYLE_NORMAL);
	Table.SetItemText(1, QuerySystemInfo.GetSKUNumber());

	Table.AddItemTitle(_T("����ϵͳ��Ϣ"), STYLE_TBITEM);
	QueryInfo.GetCaption(str);
	Table.InsertItem(_T("����ϵͳ:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetRegisteredUser(str);
	Table.InsertItem(_T("ע���û�:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetWindowsDirectory(str);
	Table.InsertItem(_T("windowsĿ¼:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	
	QueryInfo.GetSystemDirectory(str);
	Table.InsertItem(_T("system32Ŀ¼:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetSerialNumber(str);
	Table.InsertItem(_T("���к�:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	
	QueryInfo.GetBuildNumber(str);
	Table.InsertItem(_T("BuildNumber:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	
	QueryInfo.GetCSDVersion(str);
	Table.InsertItem(_T("�����汾:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	
	QueryInfo.GetBuildType(str);
	Table.InsertItem(_T("�ں�����:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetCountryCode(str);
	Table.InsertItem(_T("������:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetCodeSet(str);
	Table.InsertItem(_T("�ַ������:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%d"), QueryInfo.GetOSLanguage());
	Table.InsertItem(_T("����:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	shTimeZone = QueryInfo.GetCurrentTimeZone();
	if(shTimeZone >=0)
		str.Format(_T("GMT+%d"), shTimeZone/60);
	else
		str.Format(_T("GMT%d"), shTimeZone/60);
	Table.InsertItem(_T("��ǰʱ��:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	QueryInfo.GetInstallDate(str);
	Table.InsertItem(_T("��װʱ��:"), STYLE_NORMAL);
	strInfo.Format(_T("%s-%s-%s %s:%s:%s"), 
		str.Left(4), str.Mid(4, 2), str.Mid(6,2),
		str.Mid(8, 2), str.Mid(10, 2), str.Mid(12, 2));
	Table.SetItemText(1, strInfo);

	QueryInfo.GetLastBootUpTime(str);
	Table.InsertItem(_T("�ϴ�����ʱ��:"), STYLE_NORMAL);
	strInfo.Format(_T("%s-%s-%s %s:%s:%s"), 
		str.Left(4), str.Mid(4, 2), str.Mid(6,2),
		str.Mid(8, 2), str.Mid(10, 2), str.Mid(12, 2));
	Table.SetItemText(1, strInfo);

	Table.AddItemTitle(_T("��ҳ���ڴ���Ϣ"), STYLE_TBITEM);

	QueryInfo.GetTotalVirtualMemorySize(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("�����ڴ��С:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);
	
	QueryInfo.GetFreeVirtualMemory(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("���������ڴ��С:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);

	QueryInfo.GetFreePhysicalMemory(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("���������ڴ��С:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);

	QueryInfo.GetFreeSpaceInPagingFiles(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("��ҳ�ļ����ÿռ�:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);

	QueryInfo.GetSizeStoredInPagingFiles(str);
	strInfo.Format(_T("%sKB"), str);
	Table.InsertItem(_T("�ѷ�ҳ�洢��С:"), STYLE_NORMAL);
	Table.SetItemText(1, strInfo);

	QuerySystemInfo.DetachBuffer();
	QueryInfo.Close();

	Table.Close(TRUE);
}

void CSEHtmlReport::CPUInfoReport()			// CPU��Ϣ
{
	CSMBQueryProcessorInfo	CPUInfo;
	CSMBQueryCacheInfo		CacheInfo;
	WORD					wCacheHandle = 0;
	ULARGE_INTEGER			ProcessorID;

	CString strInfo;
	CString str;
	CHtmlTable Table(m_pFileStream);

	static CONST TCHAR *lpInstrItem[16] = {
		_T("MMX: "),
		_T("SSE: "),
		_T("SSE2: "),
		_T("SSE3: "),
		_T("SSSE3: "),
		_T("SSE4.1: "),
		_T("SSE4.2: "),
		_T("MONITOR/MWAIT: "),
		_T("RDMSR/WRMSR: "),
		_T("SYSENTER/SYSEXIT: "),
		_T("FXSAVE/FXRSTOR: "),
		_T("POPCNT: "),
		_T("CMPXCHG8B: "),
		_T("CMPXCHG16B: "),
		_T("CMOV: "),
		_T("CLFSH: "),
	};

	static CONST TCHAR *lpCommonID[25] = {
		_T("x64 Technology: "),
		_T("xTPR Update Control: "),
		_T("Perf/Debug Capability MSR: "),
		_T("LAHF/SAHF Available In 64-bit Mode: "),
		_T("SYSCALL/SYSRET Available In 64-bit Mode: "),
		_T("Execute Disable Bit Available: "),
		_T("x87 FPU on Chip(FPU): "),
		_T("Virtual-8086 Mode Enhancement(VME): "),
		_T("Debugging Extensions(DE): "),
		_T("Page Size Extensions(PSE): "),
		_T("Time Stamp Counter(TSC): "),
		_T("Physical Address Extensions(PAE): "),
		_T("Machine Check Exception(MCE): "),
		_T("APIC on Chip(APIC): "),
		_T("Memory Type Range Registers(MTRR): "),
		_T("PTE Global Bit(PGE): "),
		_T("Machine Check Architecture(MCA): "),
		_T("Page Attribute Table(PAT): "),
		_T("36-bit Page Size Extension(PSE36): "),
		_T("Processor Serial Number(PSN): "),
		_T("Debug Store(DS): "),
		_T("Thermal Monitor and Clock Ctrl(ACPI)"),
		_T("Self Snoop(SS): "),
		_T("Thermal Monitor(TM): "),
		_T("Pend. Brk. En.(PBE): ")
	};

	static CONST TCHAR *lpIntelTech[5] = {
		_T("Multithreading(HTT): "),
		_T("Virtual Machine Extensions(VME): "),
		_T("Enhanced Intel SpeedStep Technology: "),
		_T("CPL Qualified Debug Store: "),
		_T("Thermal Monitor: ")
	};

	static CONST TCHAR *lpAMDTech[19] = {
		_T("MMXExt: "),
		_T("SSE4A: "),
		_T("3Dnow!: "),
		_T("3DnowExt: "),
		_T("AltMovCr8: "),
		_T("LZCNT: "),
		_T("FFXSR: "),
		_T("RDTSCP: "),

		_T("1GB Page:")
		_T("Nested Paging: "),
		_T("LBR Visualization: "),
		_T("FP128 Optimization: "),

		_T("Core Multi-Processing Legacy Mode(CmpLegacy): "),
		_T("Secure Virtual Machine(SVM): "),
		_T("Extended APIC Register Space(ExtApicSpace): "),

		_T("Misaligned SSE Support Mode Available: "),
		_T("PREFETCH/PREFETCHW Support: "),
		_T("SKINIT/DEV Support:"),
		_T("MOVU, MOVUPS, MOVUPD optimization over MOVL/MOVH, MOVHLPS/MOVHPS, MOVLPD/MOVHPD: ")

	};

	static _IsFunctionSupported pfnIsInstructionsSupported[16] = {
		&CCPUIdInfo::IsMMXSupported,
		&CCPUIdInfo::IsSSESupported,
		&CCPUIdInfo::IsSSE2Supported,
		&CCPUIdInfo::IsSSE3NewSupported,
		&CCPUIdInfo::IsSSSE3Supported,
		&CCPUIdInfo::IsSSE41ExtSupported,
		&CCPUIdInfo::IsSSE42ExtSupported,
		&CCPUIdInfo::IsMonMwaitSupported,
		&CCPUIdInfo::IsMSRSupported,
		&CCPUIdInfo::IsSEPSupported,
		&CCPUIdInfo::IsFXSRSupported,
		&CCPUIdInfo::IsPOPCNTSupported,
		&CCPUIdInfo::IsCX8Supported,
		&CCPUIdInfo::IsCX16Supported,
		&CCPUIdInfo::IsCMOVSupported,
		&CCPUIdInfo::IsCLFSHSupported
	};

	static _IsFunctionSupported pfnIsTechnologySupported[25] = {
		&CCPUIdInfo::Isx64Supported,
		&CCPUIdInfo::IsxTPRUpdateCtrlSupported,
		&CCPUIdInfo::IsPDMSRSupported,
		&CCPUIdInfo::IsLSAHF64Supported,
		&CCPUIdInfo::IsSYS64Supported,
		&CCPUIdInfo::IsEDBSupported,

		&CCPUIdInfo::IsFPUSupported,
		&CCPUIdInfo::IsVMESupported,
		&CCPUIdInfo::IsDESupported,
		&CCPUIdInfo::IsPSESupported,
		&CCPUIdInfo::IsTSCSupported,

		&CCPUIdInfo::IsPAESupported,
		&CCPUIdInfo::IsMCESupported,
		&CCPUIdInfo::IsAPICSupported,
		&CCPUIdInfo::IsMTRRSupported,
		&CCPUIdInfo::IsPGESupported,
		&CCPUIdInfo::IsMCASupported,

		&CCPUIdInfo::IsPATSupported,
		&CCPUIdInfo::IsPSE36Supported,
		&CCPUIdInfo::IsPSNSupported,
		&CCPUIdInfo::IsDSSupported,
		&CCPUIdInfo::IsACPISupported,
		&CCPUIdInfo::IsSSSupported,
		&CCPUIdInfo::IsTMSupported,
		&CCPUIdInfo::IsPBESupported
	};

	static _IsFunctionSupported pfnIsSupportedIntel[5] = {
		&CCPUIdInfo::IsHTTSupported,
		&CCPUIdInfo::IsVMExtSupported,
		&CCPUIdInfo::IsCPLQDSSupported,
		&CCPUIdInfo::IsEISTSupported,
		&CCPUIdInfo::IsThMonSupported
	};

	static _IsFunctionSupported pfnIsSupportedAMD[19] = {
		&CCPUIdInfo::IsEMMXSupported,	
		&CCPUIdInfo::IsSSE4ASupported,	
		&CCPUIdInfo::Is3DnowSupported,
		&CCPUIdInfo::Is3DnowExtSupported,	
		&CCPUIdInfo::IsAltMovCr8Supported,	
		&CCPUIdInfo::IsLZCNTSupported,		
		&CCPUIdInfo::IsFFXSRSupported,	
		&CCPUIdInfo::IsRDTSCPSupported,	

		&CCPUIdInfo::Is1GPageSupported,
		&CCPUIdInfo::IsNestedPagingSupported,
		&CCPUIdInfo::IsLBRVlSupported,	
		&CCPUIdInfo::IsFP128OptSupported,

		&CCPUIdInfo::IsCmpLegacySupported,
		&CCPUIdInfo::IsSVMSupported,
		&CCPUIdInfo::IsExtApicSpaceSupported,

		&CCPUIdInfo::IsMisAliSSESupported,
		&CCPUIdInfo::IsPreRWSupported,
		&CCPUIdInfo::IsSKINITDEVSupported,
		&CCPUIdInfo::IsOptMovSupported
	};

	CONST TCHAR **lppItem = NULL;
	_IsFunctionSupported *lppfn = NULL;
	int nCountAll = 0;

	CPUInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	Table.AddItemTitle(_T("CPU��Ϣ"), STYLE_TBITEM);
	CCPUIdInfo::GetCPUBandString(str);
	Table.InsertItem(_T("��Ʒ�ͺ�:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	Table.InsertItem(_T("����ϵ��:"), STYLE_NORMAL);
	Table.SetItemText(1, CCPUIdInfo::GetCPUInfoDescription());

	Table.InsertItem(_T("��չ��ʾ:"), STYLE_NORMAL);
	Table.SetItemText(1, CCPUIdInfo::GetExtendedCPUInfoDescription());

	CCPUIdInfo::GetCPUIDString(str);
	Table.InsertItem(_T("������:"), STYLE_NORMAL);
	Table.SetItemText(1, str);
	if(str == _T("GenuineIntel")){
		lppItem = lpIntelTech;
		lppfn = pfnIsSupportedIntel;
		nCountAll = 5;
	}
	else{
		lppItem = lpAMDTech;
		lppfn = pfnIsSupportedAMD;
		nCountAll = 19;
	}

	str.Format(_T("%dMHZ"), CPUInfo.GetProcessorMaxSpeed());
	Table.InsertItem(_T("CPU��Ƶ:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%dMHZ"), CPUInfo.GetProcessorCurrentSpeed());
	Table.InsertItem(_T("��ǰ������Ƶ:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%d"), CCPUIdInfo::GetLogicalProcessors());
	Table.InsertItem(_T("�߼�������:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	CPUInfo.GetProcessorId(&ProcessorID);
	str.Format(_T("%08X%08X"), ProcessorID.HighPart, ProcessorID.LowPart);
	Table.InsertItem(_T("CPU����ID:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	wCacheHandle = CPUInfo.GetL1CacheHandle();
	if(wCacheHandle != 0xFFFF){
		CacheInfo.AttachBuffer(CSMBQuerier::GetSMBiosData(), wCacheHandle);

		Table.InsertItem(_T("L1����״̬:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetCacheConfigDescription());

		str.Format(_T("%dKB"), CacheInfo.GetInstalledSize());
		Table.InsertItem(_T("L1�����С:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%dMHZ"), CacheInfo.GetCacheSpeed());
		Table.InsertItem(_T("L1����Ƶ��:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("L1��������:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetTypeDescription());

		Table.InsertItem(_T("L1�������ʽ:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetErrorCorrectionTypeDescription());

		Table.InsertItem(_T("L1����ϵͳ���:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetSystemCacheTypeDescription());

		Table.InsertItem(_T("L1���湤����ʽ:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetAssociativityDescription());

		CacheInfo.DetachBuffer();
	}

	wCacheHandle = CPUInfo.GetL2CacheHandle();
	if(wCacheHandle != 0xFFFF){
		CacheInfo.AttachBuffer(CSMBQuerier::GetSMBiosData(), wCacheHandle);

		Table.InsertItem(_T("L2����״̬:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetCacheConfigDescription());

		str.Format(_T("%dKB"), CacheInfo.GetInstalledSize());
		Table.InsertItem(_T("L2�����С:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%dMHZ"), CacheInfo.GetCacheSpeed());
		Table.InsertItem(_T("L2����Ƶ��:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("L2��������:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetTypeDescription());

		Table.InsertItem(_T("L2�������ʽ:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetErrorCorrectionTypeDescription());

		Table.InsertItem(_T("L2����ϵͳ���:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetSystemCacheTypeDescription());

		Table.InsertItem(_T("L2���湤����ʽ:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetAssociativityDescription());

		CacheInfo.DetachBuffer();
	}

	wCacheHandle = CPUInfo.GetL3CacheHandle();
	if(wCacheHandle != 0xFFFF){
		CacheInfo.AttachBuffer(CSMBQuerier::GetSMBiosData(), wCacheHandle);

		Table.InsertItem(_T("L3����״̬:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetCacheConfigDescription());

		str.Format(_T("%dKB"), CacheInfo.GetInstalledSize());
		Table.InsertItem(_T("L3�����С:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%dMHZ"), CacheInfo.GetCacheSpeed());
		Table.InsertItem(_T("L3����Ƶ��:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("L3��������:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetTypeDescription());

		Table.InsertItem(_T("L3�������ʽ:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetErrorCorrectionTypeDescription());

		Table.InsertItem(_T("L3����ϵͳ���:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetSystemCacheTypeDescription());

		Table.InsertItem(_T("L3���湤����ʽ:"), STYLE_NORMAL);
		Table.SetItemText(1, CacheInfo.GetAssociativityDescription());

		CacheInfo.DetachBuffer();
	}

	str.Format(_T("%dMHZ"), CPUInfo.GetProcessorExternalClock());
	Table.InsertItem(_T("��Ƶ:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%dλ"), CCPUIdInfo::GetPhyAddrBits());
	Table.InsertItem(_T("����Ѱַ���:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%dλ"), CCPUIdInfo::GetVirtAddrBits());
	Table.InsertItem(_T("����Ѱַ���:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	str.Format(_T("%gV"), CPUInfo.GetProcessorVoltage());
	Table.InsertItem(_T("��ǰ��ѹ:"), STYLE_NORMAL);
	Table.SetItemText(1, str);

	Table.InsertItem(_T("CPU���к�:"), STYLE_NORMAL);
	Table.SetItemText(1, CPUInfo.GetSerialNumber());

	Table.InsertItem(_T("CPU Part��:"), STYLE_NORMAL);
	Table.SetItemText(1, CPUInfo.GetPartNumber());

	Table.InsertItem(_T("�������:"), STYLE_NORMAL);
	Table.SetItemText(1, CPUInfo.GetProcessorUpgradeDescription());

	Table.AddItemTitle(_T("��������"), STYLE_TBITEM);
	for(int nIndex = 0; nIndex < 25; nIndex++){
		Table.InsertItem(*(lpCommonID+nIndex), STYLE_NORMAL);
		if(pfnIsTechnologySupported[nIndex]())
			strInfo = _T("֧��");
		else
			strInfo = _T("��֧��");

		Table.SetItemText(1, strInfo);
	}

	Table.AddItemTitle(_T("ָ�����"), STYLE_TBITEM);
	for(int nIndex = 0; nIndex < 16; nIndex++){
		Table.InsertItem(*(lpInstrItem+nIndex), STYLE_NORMAL);
		if(pfnIsInstructionsSupported[nIndex]())
			strInfo = _T("֧��");
		else
			strInfo = _T("��֧��");

		Table.SetItemText(1, strInfo);
	}

	Table.AddItemTitle(_T("ר�м���"), STYLE_TBITEM);
	for(int nIndex = 0; nIndex < nCountAll; nIndex++){
		Table.InsertItem(*(lppItem+nIndex), STYLE_NORMAL);
		if(lppfn[nIndex]())
			strInfo = _T("֧��");
		else
			strInfo = _T("��֧��");

		Table.SetItemText(1, strInfo);
	}

	CPUInfo.DetachBuffer();
	Table.Close(TRUE);
}


void CSEHtmlReport::MainBoardInfoReport()		// ������Ϣ
{
	CSMBQueryBaseboardInfo	QueryBaseboardInfo;	
	CSMBQueryBIOSInfo		QueryBiosInfo;
	BOOL					bSuccess = FALSE;

	CString strInfo;
	CString str;

	ULARGE_INTEGER	uiCharacteristics;
	WORD			wExCharacteristics;
	CHtmlTable		Table(m_pFileStream);

	static CONST TCHAR *pszCharacteristicsItem[28] = {
		_T("ISA����: "),
		_T("MCA����: "),
		_T("EISA����: "),
		_T("PCI����: "),
		_T("PCMCIA�ӿ�: "),
		_T("���弴��: "),
		_T("APM(�߼���Դ����): "),
		_T("BIOS������(Flash): "),
		_T("BIOS��ӳ��: "),
		_T("VL-VESA�ӿ�: "),
		_T("ESCD(��չϵͳ��������): "),
		_T("��������: "),
		_T("������ѡ��: "),
		_T("BIOS ROM is socketed: "),
		_T("PCMCIA����: "),
		_T("EDD(��ǿ����������)�淶: "),
		_T("Int 13h/(NEC)3.5Ӣ��1.2MB����: "),
		_T("Int 13h/(����)3.5Ӣ��1.2MB����: "),
		_T("Int 13h/5.25Ӣ��360KB����: "),
		_T("Int 13h/5.25Ӣ��1.2MB����: "),
		_T("Int 13h/3.5Ӣ��720KB����: "),
		_T("Int 13h/3.5Ӣ��2.88MB����: "),
		_T("Int 5h/��Ļ��ӡ: "),
		_T("Int 9h/8042����: "),
		_T("Int 14h/����: "),
		_T("Int 17h/��ӡ���ӿ�: "),
		_T("Int 10h/CGA&��ɫ��ʾ: "),
		_T("NEC PC-98�淶: ")
	};

	static CONST TCHAR *pszExItem[11] = {
		_T("ACPI: "),
		_T("USB: "),
		_T("AGP: "),
		_T("I2O����: "),
		_T("LS-120����: "),
		_T("ATAPI ZIP����������: "),
		_T("1394����: "),
		_T("SmartBattery(UNOAV���ܵ��ר��): "),
		_T("BIOS Boot Specification: "),
		_T("Function Key-Initiated Network Service Boot: "),
		_T("Targeted Content Distribution: ")
	};

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	bSuccess = QueryBaseboardInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());
	if(bSuccess){
		Table.AddItemTitle(_T("������Ϣ"), STYLE_TBITEM);
		Table.InsertItem(_T("������:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetManufacturer());

		Table.InsertItem(_T("�����ͺ�:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetProduct());

		Table.InsertItem(_T("���к�:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetSerialNumber());

		Table.InsertItem(_T("�汾��:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetVersion());

		Table.InsertItem(_T("�����߱�ǩ:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBaseboardInfo.GetAssetTag());
		QueryBiosInfo.DetachBuffer();
	}

	bSuccess = QueryBiosInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());
	if(bSuccess){
		Table.AddItemTitle(_T("BIOS��Ϣ"), STYLE_TBITEM);
		Table.InsertItem(_T("��������:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetVendor());

		Table.InsertItem(_T("�汾��:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetVersion());

		Table.InsertItem(_T("��������:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetReleaseData());

		str.Format(_T("%dKB"), QueryBiosInfo.GetBIOSRomSize());
		Table.InsertItem(_T("BIOS����:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("SystemBiosVer:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetSystemBIOSVer());

		Table.InsertItem(_T("ECFVer:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryBiosInfo.GetECFVer());

		Table.AddItemTitle(_T("BIOS��������"), STYLE_TBITEM);
		QueryBiosInfo.GetBIOSCharacteristics(&uiCharacteristics);
		for(int nIndex = 0; nIndex < 28; nIndex++){
			Table.InsertItem(*(pszCharacteristicsItem+nIndex), STYLE_NORMAL);
			if(((SMBIOS_BIOS_ISA<<nIndex)&uiCharacteristics.LowPart) != 0)
				strInfo = (_T("֧��"));
			else
				strInfo = (_T("��֧��"));

			Table.SetItemText(1, strInfo);
		}

		Table.AddItemTitle(_T("BIOS��������(��չ)"), STYLE_TBITEM);
		wExCharacteristics = QueryBiosInfo.GetBIOSExCharacteristics();
		for(int nIndex = 0; nIndex < 11; nIndex++){
			Table.InsertItem(*(pszExItem+nIndex), STYLE_NORMAL);
			if(((SMBIOS_BIOSEX_ACPI<<nIndex)&wExCharacteristics) != 0)
				strInfo = (_T("֧��"));
			else
				strInfo = (_T("��֧��"));

			Table.SetItemText(1, strInfo);
		}
		QueryBiosInfo.DetachBuffer();
	}

	Table.Close(TRUE);
}

void CSEHtmlReport::MemoryInfoReport()		// �ڴ���Ϣ
{
	CSMBMemCtrlInfo MemCtrlInfo;
	CSMBMemDevice			MemDevice;
	CSMBPhyMemArray			PhyMemArray;
	ULARGE_INTEGER			uMemSize;

	CHtmlTable Table(m_pFileStream);

	BOOL	bQueryResult = FALSE;
	ULONG   ulIndex = 0;
	WORD    wTypeDetail = 1;
	DWORD   dwValue = 0;
	CString strInfo;
	CString str;

	CONST TCHAR *pszItem[23] = {
		_T("Unknown"),
		_T("Other"),
		_T("SiP"),
		_T("DIP"),
		_T("ZIP"),
		_T("SOJ"),
		_T("Proprietary"),
		_T("SIMM"),
		_T("DIMM"),
		_T("TSOPO"),
		_T("PGA"),
		_T("RIM"),
		_T("SODIMM"),
		_T("SRIMM"),
		_T("SMD"),
		_T("SSMP"),
		_T("QFP"),
		_T("TQFP"),
		_T("SOIC"),
		_T("LCC"),
		_T("PLCC"),
		_T("FPGA"),
		_T("LGA")
	};

	CONST TCHAR *pszMemType[22] =  {
		_T("δ֪����"),
		_T("��������"),
		_T("DRAM"),
		_T("Synchronous DRAM"),
		_T("Cache DRAM"),
		_T("EDO"),
		_T("EDRAM"),
		_T("VRAM"),
		_T("SRAM"),
		_T("RAM"),
		_T("ROM"),
		_T("FLASH"),
		_T("EEPROM"),
		_T("FEPROM"),
		_T("EPROM"),
		_T("CDRAM"),
		_T("3DRAM"),
		_T("SDRAM"),
		_T("SGRAM"),
		_T("RDRAM"),
		_T("DDR"),
		_T("DDR2")
	};

	CONST TCHAR *pszErrorCorrecting[6] =  {
		_T("Other"),
		_T("Unknown"),
		_T("None"),
		_T("Single Bit Error Correcting"),
		_T("Double Bit Error Correcting"),
		_T("Error Scrubbing")
	};

	const TCHAR *pszVoltage[3] = {
		_T("5v"),
		_T("3.3v"),
		_T("2.9v")
	};	

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	bQueryResult = MemCtrlInfo.AttachBuffer(CSMBQuerier::GetSMBiosData());
	if(bQueryResult){
		Table.AddItemTitle(_T("�ڴ��������Ϣ"), STYLE_TBITEM);

		Table.InsertItem(_T("���ģʽ:"), STYLE_NORMAL);
		Table.SetItemText(1, MemCtrlInfo.GetErrorDetectingMethodDescrip());

		Table.InsertItem(_T("����֧��:"), STYLE_NORMAL);
		dwValue = MemCtrlInfo.GetErrorCorrectingCapability();
		str = _T("");
		for(int nIndex = 0; nIndex < 6; nIndex++){
			if(((0x00000001<<nIndex)&dwValue) != 0){
				str += *(pszErrorCorrecting+nIndex);
				str += _T("\\");
			}
		}
		Table.SetItemText(1, str);

		Table.InsertItem(_T("����ʽ����֧��:"), STYLE_NORMAL);
		Table.SetItemText(1, MemCtrlInfo.GetSupportedInterleaveDescrip());

		Table.InsertItem(_T("��ǰ���ý���ʽ����֧��:"), STYLE_NORMAL);
		Table.SetItemText(1, MemCtrlInfo.GetCurrentInterleaveDescrip());

		if(PhyMemArray.AttachBuffer(CSMBQuerier::GetSMBiosData())){
			str.Format(_T("%dMB"), PhyMemArray.GetMaximumCapacity());
			Table.InsertItem(_T("���������֧��:"), STYLE_NORMAL);
			Table.SetItemText(1, str);
			PhyMemArray.DetachBuffer();
		}

		str.Format(_T("%dMB"), MemCtrlInfo.GetMaxMemModuleSize());
		Table.InsertItem(_T("�����������֧��:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		Table.InsertItem(_T("��ѹ֧��:"), STYLE_NORMAL);
		dwValue = MemCtrlInfo.GetMemModuleVoltage();
		str = _T("");
		for(int nIndex = 0; nIndex < 3; nIndex++){
			if(((0x00000001<<nIndex)&dwValue) != 0){
				str += *(pszVoltage+nIndex);
				str += _T("\\");
			}
		}
		Table.SetItemText(1, str);

		str.Format(_T("%d"), MemCtrlInfo.GetMemSlotsCount());
		Table.InsertItem(_T("�����:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		MemCtrlInfo.DetachBuffer();
	}

	if( MemDevice.AttachBuffer(CSMBQuerier::GetSMBiosData()) &&
		PhyMemArray.AttachBuffer(CSMBQuerier::GetSMBiosData()) ){
			do{
				if(MemDevice.GetPhyMemArrayHandle() != PhyMemArray.GetHandle())
					continue;

				MemDevice.GetSize(&uMemSize);
				str.Format(_T("��۱��: %s %s(%dMB)"), MemDevice.GetDeviceLocator(), MemDevice.GetBankLocator(), uMemSize.LowPart);
				Table.AddItemTitle(str, STYLE_TBITEM);

				Table.InsertItem(_T("�����̱��:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetManufacturer());

				Table.InsertItem(_T("��������:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetAssetTag());

				Table.InsertItem(_T("���к�:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetSerialNumber());

				Table.InsertItem(_T("��װ����:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetMemoryTypeDescrip());

				Table.InsertItem(_T("�ڴ�����:"), STYLE_NORMAL);
				Table.SetItemText(1, MemDevice.GetFormFactorDescrip());

				str.Format(_T("%dλ"), MemDevice.GetTotalWidth());
				Table.InsertItem(_T("���߿��:"), STYLE_NORMAL);
				Table.SetItemText(1, str);

				str.Format(_T("%dλ"), MemDevice.GetDataWidth());
				Table.InsertItem(_T("����λ���:"), STYLE_NORMAL);
				Table.SetItemText(1, str);

				str.Format(_T("%dMHZ"), MemDevice.GetSpeed());
				Table.InsertItem(_T("����Ƶ��:"), STYLE_NORMAL);
				Table.SetItemText(1, str);

			} while(MemDevice.FindNext());

			MemDevice.DetachBuffer();
			PhyMemArray.DetachBuffer();
	}

	Table.Close(TRUE);
}

void CSEHtmlReport::DisplayDeviceInfoReport()	// �Կ���Ϣ
{
	CQueryVideoControllerInfo QueryInfo;
	CQueryDesktopMonitorInfo  QueryMonitorInfo;
	BOOL	bQueryResult = TRUE;
	ULONG   ulIndex = 0;

	CONST TCHAR *pszScanMode[4] = {
		_T("����"),
		_T("δ֪"),
		_T("����ɨ��"),
		_T("����ɨ��"),
	};

	CONST TCHAR *pszMemoryType[13] = {
		_T("��������"),
		_T("δ֪����"),
		_T("VRAM"),
		_T("DRAM"),
		_T("SRAM"),
		_T("WRAM"),
		_T("EDO RAM"),
		_T("Burst Synchronous DRAM"),
		_T("Pipelined Burst SRAM"),
		_T("CDRAM"),
		_T("3DRAM"),
		_T("SDRAM"),
		_T("SGRAM")
	};

	CString strInfo;
	CString str;
	CHtmlTable Table(m_pFileStream);

	Table.Open(TRUE);
	Table.SetColumnCount(2);
	bQueryResult = QueryInfo.Create();
	while(bQueryResult) {
		Table.AddItemTitle(_T("�Կ���Ϣ"), STYLE_TBITEM);
		QueryInfo.GetVideoProcessor(str);
		Table.InsertItem(_T("GPU�ͺ�:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetCaption(str);
		Table.InsertItem(_T("�Կ�����:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetAdapterCompatibility(str);
		Table.InsertItem(_T("������:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetAdapterDACType(str);
		Table.InsertItem(_T("DAC�ͺ�:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d M"), QueryInfo.GetAdapterRAM()/1048576);
		Table.InsertItem(_T("�Դ��С:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str = *(pszMemoryType + QueryInfo.GetVideoMemoryType() - 1);
		Table.InsertItem(_T("�Դ�����:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetVideoModeDescription(str);
		Table.InsertItem(_T("��ǰ��ʾģʽ:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d HZ"), QueryInfo.GetCurrentRefreshRate());
		Table.InsertItem(_T("��ǰˢ����:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d HZ"), QueryInfo.GetMaxRefreshRate());
		Table.InsertItem(_T("���ˢ����:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d HZ"), QueryInfo.GetMinRefreshRate());
		Table.InsertItem(_T("��Сˢ����:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str = *(pszScanMode + QueryInfo.GetCurrentScanMode() - 1);
		Table.InsertItem(_T("��ǰɨ��ģʽ:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetInstalledDisplayDrivers(str);
		Table.InsertItem(_T("�Կ�����:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetDriverVersion(str);
		Table.InsertItem(_T("�����汾��:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetDriverDate(strInfo);
		str.Format(_T("%s-%s-%s %s:%s:%s"), 
			strInfo.Left(4), strInfo.Mid(4, 2), strInfo.Mid(6,2),
			strInfo.Mid(8, 2), strInfo.Mid(10, 2), strInfo.Mid(12, 2));
		Table.InsertItem(_T("������������:"), STYLE_NORMAL);
		Table.SetItemText(1, str);
		bQueryResult = QueryInfo.SetQueryIndex(&ulIndex);
	}

	ulIndex = 0;
	Table.AddItemTitle(_T("��ʾ����Ϣ"), STYLE_TBITEM);
	bQueryResult = QueryMonitorInfo.Create();
	while(bQueryResult){

		QueryMonitorInfo.GetDeviceID(str);
		Table.InsertItem(_T("�豸ID:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryMonitorInfo.GetCaption(str);
		Table.InsertItem(_T("�ͺ�:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryMonitorInfo.GetMonitorManufacturer(str);
		Table.InsertItem(_T("������:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%dx%d ����"), 
			QueryMonitorInfo.GetScreenWidth(),
			QueryMonitorInfo.GetScreenHeight());
		Table.InsertItem(_T("���ֱ���:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d���ص�/Ӣ��"), 
			QueryMonitorInfo.GetPixelsPerXLogicalInch());
		Table.InsertItem(_T("X���������ܶ�:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		str.Format(_T("%d���ص�/Ӣ��"), 
			QueryMonitorInfo.GetPixelsPerYLogicalInch());
		Table.InsertItem(_T("Y���������ܶ�:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		bQueryResult = QueryMonitorInfo.SetQueryIndex(&ulIndex);
	}

	QueryInfo.Close();
	QueryMonitorInfo.Close();

	Table.Close(TRUE);
}

void CSEHtmlReport::AudioDeviceInfoReport()		// ������Ϣ
{
	CGetMediaInfo QueryInfo;
	DWORD   dwIndex = 0;

	CString str;
	CHtmlTable Table(m_pFileStream);

	QueryInfo.Create();

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	while(QueryInfo.SetQueryIndex(dwIndex)){
		str.Format(_T("�����ͺ�: %s"), QueryInfo.GetDeviceDesc());
		Table.AddItemTitle(str, STYLE_TBITEM);
		
		Table.InsertItem(_T("�豸����:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryInfo.GetClassDescription());

		Table.InsertItem(_T("������:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryInfo.GetServiceName());

		Table.InsertItem(_T("������:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryInfo.GetManufacturer());

		Table.InsertItem(_T("�豸�ӿ�:"), STYLE_NORMAL);
		Table.SetItemText(1, QueryInfo.GetDeviceInstanceId());
		
		dwIndex++;
	}

	QueryInfo.Close();

	Table.Close(TRUE);
}

void CSEHtmlReport::InitialSmartItem()
{
	if(m_mapSmartItem.GetCount() > 0)
		return;

	m_mapSmartItem.SetAt(0x01, _T("�ײ����ݶ�ȡ������"));
	m_mapSmartItem.SetAt(0x02, _T("��дͨ������"));
	m_mapSmartItem.SetAt(0x03, _T("��Ƭ����ʱ��"));
	m_mapSmartItem.SetAt(0x04, _T("�����ͣ�μ���"));
	m_mapSmartItem.SetAt(0x05, _T("�ض�λ��������"));
	m_mapSmartItem.SetAt(0x06, _T("�ŵ���ȡ����"));
	m_mapSmartItem.SetAt(0x07, _T("Ѱ��������"));
	m_mapSmartItem.SetAt(0x08, _T("Ѱ������"));
	m_mapSmartItem.SetAt(0x09, _T("Ӳ�̼ӵ�ʱ��"));
	m_mapSmartItem.SetAt(0x0a, _T("�����ת����"));
	m_mapSmartItem.SetAt(0x0b, _T("��ͷУ׼����"));
	m_mapSmartItem.SetAt(0x0c, _T("�豸���ؼ���"));
	m_mapSmartItem.SetAt(0x0d, _T("�����ȡ������"));
	m_mapSmartItem.SetAt(0xb7, _T("SATA�������м���"));
	m_mapSmartItem.SetAt(0xb8, _T("�ն�У�����"));
	m_mapSmartItem.SetAt(0xb9, _T("��ͷ�ȶ���"));
	m_mapSmartItem.SetAt(0x07, _T("Ѱ��������"));
	m_mapSmartItem.SetAt(0xbb, _T("���治�ɾ�������"));
	m_mapSmartItem.SetAt(0xbc, _T("ͨ�ų�ʱ"));
	m_mapSmartItem.SetAt(0xbd, _T("��ͷд��߶�"));
	m_mapSmartItem.SetAt(0xbe, _T("�����¶�"));
	m_mapSmartItem.SetAt(0xbf, _T("���ٶȴ�����"));
	m_mapSmartItem.SetAt(0xc0, _T("��Դ�رմ�ͷ�ջؼ���"));
	m_mapSmartItem.SetAt(0xc1, _T("��ͷ��������"));
	m_mapSmartItem.SetAt(0xc2, _T("�¶�"));
	m_mapSmartItem.SetAt(0xc3, _T("Ӳ��ECC�ָ�"));
	m_mapSmartItem.SetAt(0xc4, _T("�ض�λ�¼�����"));
	m_mapSmartItem.SetAt(0xc5, _T("�Ⱥ��ض�����������"));
	m_mapSmartItem.SetAt(0xc6, _T("�޷�У������������"));
	m_mapSmartItem.SetAt(0xc7, _T("UltraDMAͨѶCRC����"));
	m_mapSmartItem.SetAt(0xc8, _T("�����������"));
	m_mapSmartItem.SetAt(0xc9, _T("�߼���ȡ������"));
	m_mapSmartItem.SetAt(0xca, _T("���ݵ�ַ��Ǵ���"));
	m_mapSmartItem.SetAt(0xcb, _T("����ȡ��"));
	m_mapSmartItem.SetAt(0xcc, _T("�߼�ECC����"));
	m_mapSmartItem.SetAt(0xcd, _T("������"));
	m_mapSmartItem.SetAt(0xce, _T("���и߶�"));
	m_mapSmartItem.SetAt(0xcf, _T("��������ӿ��������"));
	m_mapSmartItem.SetAt(0xd0, _T("��ͷ����"));
	m_mapSmartItem.SetAt(0xd1, _T("����ѰַЧ��"));
	m_mapSmartItem.SetAt(0xd3, _T("д������"));
	m_mapSmartItem.SetAt(0xd4, _T("д�������"));
	m_mapSmartItem.SetAt(0xdc, _T("����ƫ��"));
	m_mapSmartItem.SetAt(0xdd, _T("���ټƳ�����"));
	m_mapSmartItem.SetAt(0xde, _T("���ݼ���ʱ��"));
	m_mapSmartItem.SetAt(0xdf, _T("����/ж�����Դ���"));
	m_mapSmartItem.SetAt(0xe0, _T("����Ħ��"));
	m_mapSmartItem.SetAt(0xe1, _T("����/ж��ѭ������"));
	m_mapSmartItem.SetAt(0xe2, _T("��ͷ���غ�ʱ"));
	m_mapSmartItem.SetAt(0xe3, _T("Ť�طŴ����"));
	m_mapSmartItem.SetAt(0xe4, _T("�ϵ���������"));
	m_mapSmartItem.SetAt(0xe6, _T("GMR��ͷ���"));

	m_mapSmartItem.SetAt(0xe7, _T("Ӳ���¶�"));
	m_mapSmartItem.SetAt(0xe8, _T("�;���ʣ��"));
	m_mapSmartItem.SetAt(0xe9, _T("�ӵ�ʱ��"));
	m_mapSmartItem.SetAt(0xf0, _T("��ͷ����ʱ��"));
	m_mapSmartItem.SetAt(0xf1, _T("LBAд������"));
	m_mapSmartItem.SetAt(0xf2, _T("LBA��ȡ����"));
	m_mapSmartItem.SetAt(0xfa, _T("��ȡ����������"));
	m_mapSmartItem.SetAt(0xfe, _T("���ɵ��䱣��"));
}

void CSEHtmlReport::DiskInfoReport()			// ������Ϣ
{
	CGetDiskInfo	GetDiskInfo;
	CGetCDRomInfo	GetCDRomInfo;
	IDSECTOR		IDSector;
	ULONG			ulDiskIndex = 0;
	ULARGE_INTEGER	uLargeInteger;

	LPSMART_INFO	lpSmartInfo = new SMART_INFO[256];
	SIZE_T			nSmartSize = sizeof(SMART_INFO)*256;

	CString strInfo;
	CString str;
	CHtmlTable Table(m_pFileStream);

	Table.Open(TRUE);
	Table.SetColumnCount(2);
	InitialSmartItem();

	Table.AddItemTitle(_T("����洢����Ϣ"), STYLE_TBITEM);
	while(GetDiskInfo.SetQueryIndex(&ulDiskIndex)){
		if(!GetDiskInfo.Create())
			break;

		Table.InsertItem(_T("�����豸��:"), STYLE_NORMAL);
		Table.SetItemText(1, GetDiskInfo.GetCurrentPhysicalDrive());
		
		memset(&IDSector, 0, sizeof(IDSECTOR));
		if(GetDiskInfo.GetIDSector(&IDSector)) {

			Table.InsertItem(_T("�ͺ�:"), STYLE_NORMAL);
			Table.SetItemText(1, CGetDiskInfo::BytesToString((const BYTE*)IDSector.szModelNumber, 40));

			Table.InsertItem(_T("���к�:"), STYLE_NORMAL);
			Table.SetItemText(1, GetDiskInfo.GetSerialNumber());

			Table.InsertItem(_T("�̼���:"), STYLE_NORMAL);
			Table.SetItemText(1, CGetDiskInfo::BytesToString((const BYTE*)IDSector.szFirmwareRev, 8));

			strInfo.Format(_T("%dMB"), IDSector.wBufferSize/2048);
			Table.InsertItem(_T("�����С:"), STYLE_NORMAL);
			Table.SetItemText(1, strInfo);
		}
		
		GetDiskInfo.GetTotalSectors(&uLargeInteger);
		uLargeInteger.QuadPart*= GetDiskInfo.GetBytesPerSector();
		if(uLargeInteger.QuadPart >= 1000000000){
			uLargeInteger.QuadPart /= 1000000000;
			strInfo.Format(_T("%uGB"), uLargeInteger.QuadPart);
		}
		else if(uLargeInteger.QuadPart < 1000000000 && uLargeInteger.QuadPart >= 1000000){
			uLargeInteger.QuadPart /= 1000000;
			strInfo.Format(_T("%uMB"), uLargeInteger);
		}
		else
			strInfo.Format(_T("%uBytes"), uLargeInteger.QuadPart);
		Table.InsertItem(_T("����:"), STYLE_NORMAL);
		Table.SetItemText(1, strInfo);

		GetDiskInfo.GetTotalCylinders(&uLargeInteger);
		strInfo.Format(_T("%u"), uLargeInteger);
		Table.InsertItem(_T("������:"), STYLE_NORMAL);
		Table.SetItemText(1, strInfo);

		GetDiskInfo.GetTotalSectors(&uLargeInteger);
		strInfo.Format(_T("%u"), uLargeInteger);
		Table.InsertItem(_T("������:"), STYLE_NORMAL);
		Table.SetItemText(1, strInfo);

		GetDiskInfo.GetTotalTracks(&uLargeInteger);
		strInfo.Format(_T("%u"), uLargeInteger);
		Table.InsertItem(_T("�Ź���:"), STYLE_NORMAL);
		Table.SetItemText(1, strInfo);

		Table.InsertItem(_T("ÿ����Ź���:"), STYLE_NORMAL);
		strInfo.Format(_T("%d"), GetDiskInfo.GetTracksPerCylinder());
		Table.SetItemText(1, strInfo);

		Table.InsertItem(_T("ÿ�Ź�������:"), STYLE_NORMAL);
		strInfo.Format(_T("%d"), GetDiskInfo.GetSectorsPerTrack());
		Table.SetItemText(1, strInfo);

		Table.InsertItem(_T("ÿ�����ֽ���:"), STYLE_NORMAL);
		strInfo.Format(_T("%d"), GetDiskInfo.GetBytesPerSector());
		Table.SetItemText(1, strInfo);

		if(GetDiskInfo.IsSMARTSupported()){
			strInfo.Format(_T("%d���϶�"), GetDiskInfo.GetCurrentTempurature());
			Table.InsertItem(_T("��ǰ�¶�:"), STYLE_NORMAL);
			Table.SetItemText(1, strInfo);

			memset(lpSmartInfo, 0, nSmartSize);
			if(GetDiskInfo.GetSMARTInfo(lpSmartInfo, &nSmartSize)){
				Table.AddItemTitle(_T("S.M.A.R.T��Ϣ(��ǰֵ/���ֵ/��ֵ)"), STYLE_TBITEM);

				int nSmartCount = nSmartSize/sizeof(SMART_INFO);
				for(int nIndex = 0; nIndex < nSmartCount; nIndex++){
					m_mapSmartItem.Lookup(lpSmartInfo[nIndex].bAttribIndex, str);

					if(lpSmartInfo[nIndex].bWorst < lpSmartInfo[nIndex].bThreshold){
						str += _T("(����):");
						Table.InsertItem(str, STYLE_WARNING);
					}
					else{
						str += _T("(����):");
						Table.InsertItem(str, STYLE_NORMAL);
					}
					strInfo.Format(_T("%d/%d/%d"), lpSmartInfo[nIndex].bCurrent, lpSmartInfo[nIndex].bWorst, lpSmartInfo[nIndex].bThreshold);
					Table.SetItemText(1, strInfo);
				}
			}
		}

		GetDiskInfo.Close();
	}

	delete [] lpSmartInfo;

	Table.AddItemTitle(_T("������Ϣ"), STYLE_TBITEM);
	if(GetCDRomInfo.Create()){
		ulDiskIndex = 0;
		while(GetCDRomInfo.SetQueryIndex(ulDiskIndex)){

			Table.InsertItem(_T("�豸�ͺ�:"), STYLE_NORMAL);
			Table.SetItemText(1, GetCDRomInfo.GetFriendlyName());

			Table.InsertItem(_T("������Ϣ:"), STYLE_NORMAL);
			Table.SetItemText(1, GetCDRomInfo.GetClassDescription());

			Table.InsertItem(_T("������:"), STYLE_NORMAL);
			Table.SetItemText(1, GetCDRomInfo.GetManufacturer());
			ulDiskIndex++;
		}
	}

	GetCDRomInfo.Close();

	Table.Close(TRUE);
}

void CSEHtmlReport::NetDeviceInfoReport()		// �����豸��Ϣ
{
	CQueryNetworkAdapterInfo QueryInfo;

	CString strInfo;
	CString str;
	CHtmlTable Table(m_pFileStream);

	BOOL  bQueryResult = TRUE;
	ULONG ulIndex = 0;

	Table.Open(TRUE);
	Table.SetColumnCount(2);

	Table.AddItemTitle(_T("�����豸��Ϣ"), STYLE_TBITEM);
	bQueryResult = QueryInfo.Create();
	while(bQueryResult){
		QueryInfo.GetDescription(str);
		Table.InsertItem(_T("�豸����:"), STYLE_COLUMN);
		Table.SetItemText(1, str);

		QueryInfo.GetAdapterType(str);
		Table.InsertItem(_T("����������:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetManufacturer(str);
		Table.InsertItem(_T("������:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetNetConnectionID(str);
		Table.InsertItem(_T("��������ID:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetDeviceID(str);
		Table.InsertItem(_T("�豸ID:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetMACAddress(str);
		Table.InsertItem(_T("MAC��ַ:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		QueryInfo.GetPNPDeviceID(str);
		Table.InsertItem(_T("���弴���豸ID:"), STYLE_NORMAL);
		Table.SetItemText(1, str);

		bQueryResult = QueryInfo.SetQueryIndex(&ulIndex);
	}

	QueryInfo.Close();
	Table.Close(TRUE);
}

UINT CSEHtmlReport::HtmlReportThreadProc(LPVOID lpParam)
{
	PREPORT_THREAD_PARAM pThreadParam = (PREPORT_THREAD_PARAM)lpParam;
	CSEHtmlReport Report(pThreadParam->pReportPath);

	::SendMessage(pThreadParam->hMainWnd, WM_THREAD_START, NULL, NULL);
	Report.WriteHead();

	pThreadParam->pDlgInformation->SetInfoText2(_T("ϵͳ��Ϣ���"));
	Report.SystemInfoReport();
	
	pThreadParam->pDlgInformation->SetInfoText2(_T("CPU��Ϣ���"));
	Report.CPUInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("������Ϣ���"));
	Report.MainBoardInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("�ڴ���Ϣ���"));
	Report.MemoryInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("�Կ���Ϣ���"));
	Report.DisplayDeviceInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("������Ϣ���"));
	Report.AudioDeviceInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("������Ϣ"));
	Report.DiskInfoReport();

	pThreadParam->pDlgInformation->SetInfoText2(_T("�����豸��Ϣ"));
	Report.NetDeviceInfoReport();
	
	Report.WriteTail();
	::SendMessage(pThreadParam->hMainWnd, WM_THREAD_FINISHED, NULL, NULL);

	delete pThreadParam;

	return 0;
}