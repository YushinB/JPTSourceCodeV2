#pragma once

#include "Shader.h"
#include <SFML/Window.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderHelper.h"
#include "VBOManager.h"
#include "BoundBox.h"
#include "OOBoundBox.h"
#include "ViewControl.h"
#include "assimphelper.h"

class CJPTNode;
class CJPTElem;

class CJPTViewBase: public CView
{
public:
	CJPTViewBase();
	virtual ~CJPTViewBase();

	void OnDraw(CDC* pDC);
	void GLRenderScene();

	void SetUnivNodes(std::map<long, std::shared_ptr<CJPTNode>> *sTmp) { mUnivNodeMap = *sTmp; }
	void SetUnivElems(std::map<long, std::shared_ptr<CJPTElem>> *sTmp) { mUnivElemMap = *sTmp;}
	void SetPathFile(const std::vector<CString>& paths) { m_ByPaths = paths; };
	void Set3DObjPathFile(const std::vector<CString>& paths) { m_3DObjectPaths = paths; };

	CString GetError() { return m_strError; }
protected:
	void GLDrawBackground();
	void GLDrawElememtShader();
	void GLDrawLineShader();
	void GLDrawVectorField();
	void GLDrawVectorField2();
	void GLDrawNodes();
	void GLDrawBoxTest();
	void GLTest();
	void GLDrawCameraTest();
	void GLDrawOITtest();
	void GLDrawLigtingSource();
	void GlDrawModelLoader();
protected:
	// event processing
	void OnKeyDown(int code);
	void OnLeftButtonDowns(int x, int y);
	void OnRightButtonDowns(int x, int y);
	void OnLeftButtonUp(int x, int y);
	void OnRightButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseWhell(int x,int y, int deltal);
	void createContextMenu();
protected:
	void PrepairScene();
	void LoadShaders();
	void WGLActivate();
	void WGLDeactivate();
	void LoadData();
	void LoadBDFData();
	void LoadByFile(const CString& path, int& nVertex, std::vector<std::vector<double>>& RangeData);
	void initOITTest();
	glm::mat4 GetModelViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	void GetNormalVectorOfNode(std::vector<long>& connectedElem, std::vector<GLfloat>& normal);

	void GetTextureData(std::shared_ptr<CJPTNode>& Node, double& dX, double& dY);
private:
	CShader* m_pShadeShader;
	CShader* m_pShadeShaderMesh;
	CShader* m_pArrowShader;
	CShader* m_pArrow3dShader;
	CShader* m_pTextureShader;

	CShader* m_pShadeLigting;
	CShader* m_pShadeLamp;

	CShader* m_pShaderModelLoader;

	CString m_strError;

	sf::Window*  m_CurrentWnd;
	std::unique_ptr<CRenderElemData> m_renderData;

	BOOL bLightingTest = TRUE;
	BOOL bModelLoadTest = FALSE;
	float m_LightingColor[3];
	TVector3d m_LightPosition;

	CVBORenderData m_VBOData;

	CVBORenderData m_VBOLigtingData;

	CTextureData m_textureData1;
	CTextureData m_textureData2;

	float m_rotateAngle1;
	float m_rotateAngle2;

	//temp
	std::map<long, std::shared_ptr<CJPTNode>> mUnivNodeMap;
	std::map<long, std::shared_ptr<CJPTElem>> mUnivElemMap;

	// bounding box
	CBoundBox m_cbBox;
	COOBoundBox m_OBBox;
	CViewCtrl m_ViewCtrl;

	std::vector<CString> m_ByPaths;
	std::vector<CString> m_3DObjectPaths;

	CModel m_model;
};
