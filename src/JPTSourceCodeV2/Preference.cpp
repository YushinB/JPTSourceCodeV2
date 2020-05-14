#include "stdafx.h"
#include "Preference.h"

CPreference & CPreference::GetInstance()
{
	// TODO: insert return statement here
	// single threading Singleton
	// Data store in Stack

	static CPreference preference;
	return preference;
}

CPreference::~CPreference()
{
}

CPreference::CPreference()
{
	m_bRenderMesh = false;
	m_bRenderSurface = true;
	m_bRenderVector = false;
	m_bRender3DVecrtor = false;
	m_bRenderPoints = false;
	m_bClippingPlane = false;
	m_bCurlingFace = false;
	m_bOOBoxTest = false;
	m_bloadTexture = false;
}