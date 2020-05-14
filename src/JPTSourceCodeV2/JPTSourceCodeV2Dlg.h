
// JPTSourceCodeV2Dlg.h : header file
//

#pragma once
#include "GUIControlTestDlg.h"
#include "GUIRenderSetting.h"
#include "ChartControlDlg.h"

#include <SFML/Window.hpp>
#include "JPTSourceCodeV2Doc.h"
#include <map> 

//class GUIControlTestDlg;
class CJPTNode;
class CJPTElem;
class EdgeData;

// CJPTSourceCodeV2Dlg dialog
class CJPTSourceCodeV2Dlg : public CDialogEx
{
// Construction
public:
	CJPTSourceCodeV2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_JPTSOURCECODEV2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

//	JPTSourceCodeV2Doc* GetOpnDoc() { return (JPTSourceCodeV2Doc *)GetDocument(); }
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OpenGLTest();

	afx_msg void OpenGUITest();
	afx_msg void OpenOtherTest();
	afx_msg void OpenMainTest();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void onImportBDFFile();
	afx_msg void onImportBYFile();
	afx_msg void onImportObjectFile();
	afx_msg void onRenderSettingChange();
	afx_msg void onChartControlTest();
	afx_msg void onTestVulkanRender();
	DECLARE_MESSAGE_MAP()

	//afx_msg void OpenGLTest1();
private:
	void logstring(CString str);
	//void initGL();
	std::unique_ptr<GUIControlTestDlg> GuiDlg;
	std::unique_ptr<CGUIRenderSetting> RenderSettingDlg;
	std::unique_ptr<CChartControlDlg> ChartControltest;

	std::map<long, std::shared_ptr<CJPTNode>> mUnivNodeMap;
	std::map<long, std::shared_ptr<CJPTElem>> mUnivElemMap;
	std::multimap<long, std::shared_ptr<EdgeData>>   mEdgeData;
	std::vector<CString> m_ByPaths;
	std::vector<CString> m_3DObjectPaths;
};
