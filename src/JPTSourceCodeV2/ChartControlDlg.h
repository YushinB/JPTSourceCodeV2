#pragma once
#include "ChartContainer.h"

// CGUIRenderSetting dialog

class CChartControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChartControlDlg)

public:
	CChartControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChartControlDlg();

	// Dialog Data
	enum { IDD = IDD_PROPPAGE_CHARTCONTROL};

	void initDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void OnOK();
	void OnCancel();
private:
	CChartContainer m_chartTest;

	ULONG_PTR m_nGdiplusToken;
};
