#pragma once


// CJPTPythonIDE dialog

class CJPTPythonIDE : public CDialogEx
{
	DECLARE_DYNAMIC(CJPTPythonIDE)

public:
	CJPTPythonIDE(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJPTPythonIDE();

// Dialog Data
	enum { IDD = IDD_DIALOG_PYTHON_IDE_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
