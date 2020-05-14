// ChartSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartSampleDlg.h"
#include "afxdialogex.h"


// CChartSampleDlg dialog

IMPLEMENT_DYNAMIC(CChartSampleDlg, CDialog)

CChartSampleDlg::CChartSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartSampleDlg::IDD, pParent)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_nGdiplusToken,
                         &gdiplusStartupInput, NULL);
}

CChartSampleDlg::~CChartSampleDlg()
{
	Gdiplus::GdiplusShutdown(m_nGdiplusToken);
}

void CChartSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STCHARTCONTAINER, m_chartContainer);
}


BEGIN_MESSAGE_MAP(CChartSampleDlg, CDialog)
END_MESSAGE_MAP()

BOOL CChartSampleDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
	//m_chartContainer.CreateChartCtrlWnd(0, WS_POPUP|WS_OVERLAPPEDWINDOW|WS_VISIBLE, CRect(100, 100, 500, 400),NULL, NULL);
	m_chartContainer.SetContainerName(string_t(_T("Demo chart container")));
	std::vector<double> vX;
	vX.push_back(0.0);
	vX.push_back(1.0);
	vX.push_back(2.0);
	vX.push_back(3.0);
	vX.push_back(4.0);
	std::vector<double> vY;
	vY.push_back(0.0);
	vY.push_back(1.0);
	vY.push_back(4.0);
	vY.push_back(9.0);
	vY.push_back(16.0);

	m_chartContainer.AddChart(true,true,_T("TestX"),_T("TestY"),3,Gdiplus::DashStyleSolid,2,1,Gdiplus::Color::Blue,vX,vY,true);
	return TRUE;
}
// CChartSampleDlg message handlers
