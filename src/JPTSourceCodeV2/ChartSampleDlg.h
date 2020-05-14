#pragma once
#include "resource.h"
#include <afx.h>
#include <afxwin.h>
#include "ChartDef.h"
#include "ChartContainer.h"


// CChartSampleDlg dialog

class CChartSampleDlg : public CDialog
{
	DECLARE_DYNAMIC(CChartSampleDlg)

public:
	CChartSampleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChartSampleDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SAMPLECHART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    virtual BOOL OnInitDialog();


protected:
	CChartContainer m_chartContainer;

private:
    ULONG_PTR m_nGdiplusToken;
};
