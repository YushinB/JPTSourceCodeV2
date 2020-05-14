// GUIRenderSetting.cpp : implementation file
//

#include "stdafx.h"
#include "JPTSourceCodeV2.h"
#include "GUIRenderSetting.h"
#include "afxdialogex.h"
#include "Preference.h"

// CGUIRenderSetting dialog

IMPLEMENT_DYNAMIC(CGUIRenderSetting, CDialogEx)

CGUIRenderSetting::CGUIRenderSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GUIRENDERSETTING, pParent)
{

}

CGUIRenderSetting::~CGUIRenderSetting()
{
}
//IDC_BUTTON_LOAD_TEXTURE
//IDC_BUTTON_LOAD_TEXTURE
void CGUIRenderSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SHOW_SURFACE, m_bShowSurface);
	DDX_Check(pDX, IDC_CHECK_SHOW_Mesh, m_bShowMesh);
	DDX_Check(pDX, IDC_CHECK_SHOW_Edge, m_bShowEdge);
	DDX_Check(pDX, IDC_CHECK_SHOW_VECTOR, m_bShowVector);
	DDX_Check(pDX, IDC_CHECK_SHOW_VECTOR3D, m_bShowVector3D);
	DDX_Check(pDX, IDC_CHECK_SHOW_POINTS, m_bShowPoint);
	DDX_Check(pDX, IDC_CHECK_SHOW_CLIPPINGPLAN, m_bShowClipPlane);
	DDX_Check(pDX, IDC_CHECK_SHOW_OOBBOX, m_bShowOOBB);
	DDX_Check(pDX, IDC_CHECK_LOAD_TEXTURE, m_bLoadTexture);
	DDX_Text(pDX, IDC_EDIT_LOAD_TEXTURE, m_strpath);
}

void CGUIRenderSetting::LoadTexture()
{
	CString strFilter = _T("Import texture File (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, strFilter);
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	std::vector<CString> paths;
	POSITION pos = fileDlg.GetStartPosition();
	while (pos)
		paths.push_back(fileDlg.GetNextPathName(pos));
	if (!paths.empty())
	{
		m_strpath = paths.at(0);
		UpdateData(FALSE);
	}
}

void CGUIRenderSetting::OnOK()
{
	UpdateData(TRUE);
	CPreference::GetInstance().m_bRenderSurface = m_bShowSurface;
	CPreference::GetInstance().m_bRenderMesh = m_bShowMesh;
	//CPreference::GetInstance().m_bRenderSurface = m_bShowEdge;
	CPreference::GetInstance().m_bRenderVector = m_bShowVector;
	CPreference::GetInstance().m_bRender3DVecrtor = m_bShowVector3D;
	CPreference::GetInstance().m_bRenderPoints = m_bShowPoint;
	CPreference::GetInstance().m_bClippingPlane = m_bShowClipPlane;
	CPreference::GetInstance().m_bOOBoxTest = m_bShowOOBB;
	CPreference::GetInstance().m_bloadTexture = m_bLoadTexture;
	CPreference::GetInstance().m_texturepath = m_strpath;
	EndDialog(IDOK);
}

void CGUIRenderSetting::OnCancel()
{
	EndDialog(IDOK);
}

void CGUIRenderSetting::initDlg()
{
	m_bShowSurface = CPreference::GetInstance().m_bRenderSurface;
	m_bShowMesh = CPreference::GetInstance().m_bRenderMesh;
	//CPreference::GetInstance().m_bRenderSurface = m_bShowEdge;
	m_bShowVector= CPreference::GetInstance().m_bRenderVector;
	m_bShowVector3D = CPreference::GetInstance().m_bRender3DVecrtor;
	m_bShowPoint = CPreference::GetInstance().m_bRenderPoints;
	m_bShowClipPlane = CPreference::GetInstance().m_bClippingPlane;
	m_bShowOOBB = CPreference::GetInstance().m_bOOBoxTest;
	m_bLoadTexture = CPreference::GetInstance().m_bloadTexture;
	m_strpath = CPreference::GetInstance().m_texturepath;

	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CGUIRenderSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TEXTURE, CGUIRenderSetting::LoadTexture)
END_MESSAGE_MAP()


// CGUIRenderSetting message handlers
