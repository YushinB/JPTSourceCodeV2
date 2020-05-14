
// JPTSourceCodeV2Dlg.cpp : implementation file

#include "stdafx.h"
#include <iostream>
#include <functional>

#include "JPTSourceCodeV2.h"
#include "JPTSourceCodeV2Dlg.h"
#include "afxdialogex.h"
#include "../JPTLib/HashTable.h"
#include "../JPTLib/LinkedList.h"
#include "TestMain.h"
#include "../JPTLib/LogWriter.h"
#include "TemplatePractice.h"
#include "JPTViewBase.h"
//#include "JPTVulkanRender.h"

#include "DebugTool.h"
#include "../include/learnopengl/filesystem.h"

#include "../JPTLib/MyBinary.h"
#include "../JPTLib/StringTool.h"
#include "../JPTLib/BDF_Reader.h"
#include "Preference.h"
#include "../JPTLib/BTS.h"
#include <omp.h>

namespace // private namespaces
{
	ITem* BuildOneTwoThree()
	{
		ITem* Head = new ITem;
		Head->Data = 1;
		Head->Next = new ITem;
		Head->Next->Data = 2;
		Head->Next->Next = new ITem;
		Head->Next->Next->Data = 3;
		Head->Next->Next->Next = NULL;
		return Head;
	}

	int length(ITem* Head)
	{
		auto currentPos = Head;
		int count = 0;
		while (currentPos != NULL)
		{
			count++;
			currentPos = currentPos->Next;
		}
		return count;
	}

	template <typename T, typename C>
	C function()
	{
		C d;
		d.insert(1);
		return d;
	}

};

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CJPTSourceCodeV2Dlg dialog
CJPTSourceCodeV2Dlg::CJPTSourceCodeV2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJPTSourceCodeV2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CLogWriter::GetInstance().initLogWriter("C:/Users/Yushin_2/Documents/Sourcode/Mine/JPTSourceCodeV2/bin/debug/x64/LogFile.txt");
	RenderSettingDlg = nullptr;
	GuiDlg = nullptr;
	ChartControltest = nullptr;
}

void CJPTSourceCodeV2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CJPTSourceCodeV2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GL_TEST, CJPTSourceCodeV2Dlg::OpenGLTest)
	ON_BN_CLICKED(IDC_BUTTON_GUI_TEST3, CJPTSourceCodeV2Dlg::OpenGUITest)
	ON_BN_CLICKED(IDC_BUTTON_CHART_TEST2, CJPTSourceCodeV2Dlg::OpenOtherTest)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_TEST, CJPTSourceCodeV2Dlg::OpenMainTest)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_BDF, CJPTSourceCodeV2Dlg::onImportBDFFile)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_BY, CJPTSourceCodeV2Dlg::onImportBYFile)//onImportObjectFile
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_OBJ_FILE, CJPTSourceCodeV2Dlg::onImportObjectFile)
	//onRenderSettingChange()
	ON_BN_CLICKED(IDC_BUTTON_FILE_SETTING, CJPTSourceCodeV2Dlg::onRenderSettingChange)
	//IDC_BUTTON_CHARTCONTROL
	ON_BN_CLICKED(IDC_BUTTON_CHARTCONTROL, CJPTSourceCodeV2Dlg::onChartControlTest)
	ON_BN_CLICKED(IDC_BUTTON_VULKAN_RENDER, CJPTSourceCodeV2Dlg::onTestVulkanRender)
END_MESSAGE_MAP()


// CJPTSourceCodeV2Dlg message handlers

BOOL CJPTSourceCodeV2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CJPTSourceCodeV2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CJPTSourceCodeV2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.

HCURSOR CJPTSourceCodeV2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CJPTSourceCodeV2Dlg::onImportBDFFile()
{
	CString strFilter = _T("BDF and tsg File (*.bdf;*.tsg)|*.bdf;*.tsg||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, strFilter);
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}

	std::vector<CString> vecPath;
	POSITION pos = fileDlg.GetStartPosition();
	while (pos)
		vecPath.push_back(fileDlg.GetNextPathName(pos));

	BDF_Reader reader;
	reader.SetInputFile(StringTool::ConvertCStringtoString(vecPath[0]));
	reader.Read_BDF_File();
	reader.GetUnivElems(&mUnivElemMap);
	reader.GetUnivNodes(&mUnivNodeMap);
	reader.GetEdgeData(&mEdgeData);
	AfxMessageBox(_T("Import BDF successfully"), MB_OK);
}

void CJPTSourceCodeV2Dlg::onImportBYFile()
{
	m_ByPaths.clear();
	CString strFilter = _T("Import by File (*.by)|*.by||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, strFilter);
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}

	POSITION pos = fileDlg.GetStartPosition();
	while (pos)
		m_ByPaths.push_back(fileDlg.GetNextPathName(pos));

	AfxMessageBox(_T("Import *.by File  successfully"), MB_OK);
}

void CJPTSourceCodeV2Dlg::onImportObjectFile()
{
	m_3DObjectPaths.clear();
	CString strFilter = _T("Import obj3d File (*.obj)|*.obj||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, strFilter);
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}

	POSITION pos = fileDlg.GetStartPosition();
	while (pos)
		m_3DObjectPaths.push_back(fileDlg.GetNextPathName(pos));

	AfxMessageBox(_T("Import *.0bj File  successfully"), MB_OK);
}

void CJPTSourceCodeV2Dlg::onRenderSettingChange()
{
	if (RenderSettingDlg) {
		RenderSettingDlg->initDlg();
		if (RenderSettingDlg->IsWindowVisible())
			return;
	}
	RenderSettingDlg.reset(new CGUIRenderSetting(this));
	RenderSettingDlg->Create(CGUIRenderSetting::IDD);
	RenderSettingDlg->initDlg();
	RenderSettingDlg->ShowWindow(SW_SHOW);
}

void CJPTSourceCodeV2Dlg::onChartControlTest()
{
	if (ChartControltest) {
		//ChartControltest->initDlg();
		if (ChartControltest->IsWindowVisible())
			return;
	}
	ChartControltest.reset(new CChartControlDlg(this));
	ChartControltest->Create(CChartControlDlg::IDD);
	//RenderSettingDlg->initDlg();
	ChartControltest->ShowWindow(SW_SHOW);
}

void CJPTSourceCodeV2Dlg::onTestVulkanRender()
{
	//CRenderVulkan render;
	//render.GLRenderScene();
}

void CJPTSourceCodeV2Dlg::logstring(CString str)
{
	SetDlgItemText(IDC_EDIT_TEXT_TEST, str);
}

void CJPTSourceCodeV2Dlg::OpenGLTest()
{
	CJPTViewBase view;
	view.SetUnivElems(&mUnivElemMap);
	view.SetUnivNodes(&mUnivNodeMap);
	view.SetPathFile(m_ByPaths);
	view.Set3DObjPathFile(m_3DObjectPaths);
	view.GLRenderScene();


	// clear;
	m_ByPaths.clear();
	m_3DObjectPaths.clear();
	mUnivElemMap.clear();
	mUnivNodeMap.clear();
	mEdgeData.clear();
}


void CJPTSourceCodeV2Dlg::OpenGUITest()
{
	if (GuiDlg) {
		if (GuiDlg->IsWindowVisible())
			return;
	}
	GuiDlg.reset(new GUIControlTestDlg(this));
	GuiDlg->Create(GUIControlTestDlg::IDD);
	GuiDlg->ShowWindow(SW_SHOW);

	SetDlgItemText(IDC_EDIT_TEXT_TEST, _T("GUI Test Completed"));


	if (StringTool::isScientificNumber("0.00005e-5"))
	{
		SetDlgItemText(IDC_EDIT_TEXT_TEST, _T("Number"));
	}

	//CBinaryReader read("D:/Personal/JPTSourceCodeV2/test.b");
	//int ID = 0;
	//double dID = 0.0;
	//read.binary_read(ID);
	//read.binary_read(dID);
#if 0
	CBinaryWriter writer("D:/Personal/JPTSourceCodeV2/test.b");
	read.binary_write(10);
	read.binary_write(10.0);
#endif
}

void CJPTSourceCodeV2Dlg::OpenOtherTest()
{
	std::string result = "";
	//xsprintf(result, "Product:%, Qty:%, Price is $%\n ", "Shampoo", "XXXXX", 2.65);

	CString log = static_cast<CString>(result.c_str());
	SetDlgItemText(IDC_EDIT_TEXT_TEST, log);
}

struct MyStruct
{
	MyStruct(int a, int b) :_a(a), _b(b) {}
	int _a;
	int _b;
};

bool compairison(const std::shared_ptr<MyStruct>& lhs, const std::shared_ptr<MyStruct>& rhs)
{
	if (lhs->_a != rhs->_a)
	{
		return lhs->_a < rhs->_a;
	}
	else
	{
		return lhs->_b < rhs->_b;
	}
}

void CJPTSourceCodeV2Dlg::OpenMainTest()
{
	struct CustomCompare
	{
		bool operator()(const std::shared_ptr<MyStruct>& lhs, const std::shared_ptr<MyStruct>& rhs)const
		{
			if (lhs->_a != rhs->_a)
			{
				return lhs->_a < rhs->_a;
			}
			else
			{
				return lhs->_b < rhs->_b;
			}
		}
	};

	auto compairlamda = [&](const std::shared_ptr<MyStruct>& lhs, const std::shared_ptr<MyStruct>& rhs) ->bool{
		if (lhs->_a != rhs->_a)
		{
			return lhs->_a < rhs->_a;
		}
		else
		{
			return lhs->_b < rhs->_b;
		}
	};

	//std::set<std::shared_ptr<MyStruct>, CustomCompare> testcase;
	//std::set<std::shared_ptr<MyStruct>, decltype(compairison)*> testcase(compairison);
	std::set<std::shared_ptr<MyStruct>, decltype(compairlamda)> testcase(compairlamda);

	std::vector<std::shared_ptr<MyStruct>> testcase1;
	for (int i = 0; i < 100; ++i)
	{
		auto a = std::rand() % 10 + 1;
		auto b = std::rand() % 10 + 1;
		testcase1.push_back(std::make_shared<MyStruct>(a, b));
	}

	testcase.insert(testcase1.begin(), testcase1.end());
	auto test6 = std::make_shared<MyStruct>(5, 5);
	if (testcase.end() != testcase.find(test6))
	{
		CString strOut;
		strOut.Format(_T("hello(%d) world(%d) \n"), 1, 1);
	}

	static long num_steps = 100000;
	static const long NThreads = 2;

	int i, nthreads; double pi, sum[NThreads];
	double step = 1.0 / (double)num_steps;

	auto Pool = [&](int threadID) {
		CString strOut;
		strOut.Format(_T("hello(%d) world(%d) \n"), threadID, threadID);
		return strOut;
	};

	auto normalPiCal = [&]() {
		int i; double x, pi, sum = 0.0;
		step = 1.0 / (double)num_steps;
		for (i = 0; i< num_steps; i++) {
			x = (i + 0.5)*step;
			sum = sum + 4.0 / (1.0 + x*x);
		}
		pi = step * sum;
		return pi;
	};

#if  0
	auto ParallelPiCal = [&](int threadID) {
		int i; double x, pi, sum = 0.0;
		step = 1.0 / (double)num_steps;
		for (i = threadID * (num_steps/ 4l); i< (threadID + 1) * (num_steps / 4l); i++) {
			x = (i + 0.5)*step;
			sum = sum + 4.0 / (1.0 + x*x);
		}
		pi = step * sum;
		return pi;
	};

	auto ParallelPiCal2 = [&](int threadID) {
		int i; double x, pi, sum = 0.0;
		step = 1.0 / (double)num_steps;
		for (i = threadID * (num_steps / 4l); i< (threadID + 1) * (num_steps / 4l); i++) {
			x = (i + 0.5)*step;
			sum = sum + 4.0 / (1.0 + x*x);
		}
		pi = step * sum;
		return pi;
	};
#endif //  0


//	double dValParallel = 0.0;
//#pragma omp parallel num_threads(NThreads)
//	{
//		int i, id, nthrds;
//		double x;
//		id = omp_get_thread_num();
//		nthrds = omp_get_num_threads();
//		if (id == 0) nthreads = nthrds;
//		for (i = id, sum[id] = 0.0; i< num_steps; i = i + nthrds) {
//			x = (i + 0.5)*step;
//			sum[id] += 4.0 / (1.0 + x*x);
//		}
//	}
//	for (i = 0, dValParallel = 0.0; i<nthreads; i++)dValParallel += sum[i] * step;
//	double fsum = 0.0;
//	double dValParallel = 0.0;
//#pragma omp parallel
//	{
//		double x = 0.0;
//#pragma omp for reduction (+:fsum)
//		for (i = 0; i < num_steps; i++) {
//			x = (i + 0.5)*step;
//			fsum += 4.0 / (1.0 + x*x);
//		}
//	}
//
//
//	dValParallel = fsum* step;
//	auto dValPiNormal = normalPiCal();
//	CString res;
//	res.Format(_T("PiNormal = %f  --- Pi_paralle = %f -- tmp = %f"), dValPiNormal, dValParallel, tmp);
//	SetDlgItemText(IDC_EDIT_TEXT_TEST, res);
//	return;
}
