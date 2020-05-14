#pragma once


// GUIControlTestDlg dialog

class GUIControlTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GUIControlTestDlg)

public:
	GUIControlTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GUIControlTestDlg();

// Dialog Data
//#ifdef AFX_DESIGN_TIME
//	enum { IDD = IDD_DIALOG_GUI_TEST };
//#endif
	enum { IDD = IDD_DIALOG_GUI_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

};
