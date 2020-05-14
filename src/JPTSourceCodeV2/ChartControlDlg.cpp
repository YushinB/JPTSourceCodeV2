#include "stdafx.h"
#include "JPTSourceCodeV2.h"
#include "ChartControlDlg.h"
#include "afxdialogex.h"
#include "Preference.h"
//IDD_PROPPAGE_CHARTCONTROL

IMPLEMENT_DYNAMIC(CChartControlDlg, CDialogEx)

CChartControlDlg::CChartControlDlg(CWnd * pParent)
	:CDialogEx(CChartControlDlg::IDD,pParent)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_nGdiplusToken,
		&gdiplusStartupInput, NULL);
}

CChartControlDlg::~CChartControlDlg()
{
	Gdiplus::GdiplusShutdown(m_nGdiplusToken);
}

void CChartControlDlg::initDlg()
{
}

void CChartControlDlg::DoDataExchange(CDataExchange * pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE_CHART_CONTROL, m_chartTest);
}

void CChartControlDlg::OnOK()
{
	EndDialog(IDOK);
}

void CChartControlDlg::OnCancel()
{
}

BEGIN_MESSAGE_MAP(CChartControlDlg, CDialogEx)
END_MESSAGE_MAP()
