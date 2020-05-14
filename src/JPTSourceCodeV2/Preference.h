#pragma once

class CPreference
{
public:
	static	CPreference& GetInstance();
	virtual ~CPreference();

	bool m_bRenderMesh;
	bool m_bRenderSurface;
	bool m_bRenderVector;
	bool m_bRender3DVecrtor;
	bool m_bRenderPoints;
	bool m_bClippingPlane;
	bool m_bCurlingFace;
	bool m_bOOBoxTest;
	bool m_bloadTexture;
	CString m_texturepath;
private:
	CPreference();
};