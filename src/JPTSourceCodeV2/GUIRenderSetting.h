#pragma once


// CGUIRenderSetting dialog

class CGUIRenderSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CGUIRenderSetting)

public:
	CGUIRenderSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIRenderSetting();

// Dialog Data
	enum { IDD = IDD_GUIRENDERSETTING };

	void initDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void LoadTexture();
	DECLARE_MESSAGE_MAP()

	void OnOK();
	void OnCancel();
private:
	BOOL m_bShowSurface;
	BOOL m_bShowMesh;
	BOOL m_bShowEdge;
	BOOL m_bShowVector;
	BOOL m_bShowVector3D;
	BOOL m_bShowPoint;
	BOOL m_bShowClipPlane;
	BOOL m_bShowOOBB;
	BOOL m_bLoadTexture;
	CString m_strpath;
};
