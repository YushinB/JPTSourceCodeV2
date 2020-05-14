#define GLFW_INCLUDE_VULKAN


#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>


#include "JPTViewBase.h"
//#include "sceneoit.h"


#include "BasicGeometry.h"
#include "Object3DReader.h"

#include <chrono>

#include "GLSLNodeVert.h"
#include "GLSLShadeFrag.h"
#include "GLSLNodeGeo.h"
#include "GLSLNodeGeoMesh.h"

#include "GLSLNodeVertMesh.h"
#include "GLSLShadeFragMesh.h"

#include "GLSLVertArrow.h"
#include "GLSLFragArrow.h"
#include "GLSLGeoArrow.h"

#include "GLSL3DArrowShader.h"

#include "GLSLAxisShader.h"
#include "GLSLTextureShader.h"

#include "GLSLLigtingShader.h"

#include "Preference.h"

#include "texture.h"
#include "StringTool.h"

#include <filesystem>

namespace
{
	const float M_PI = 3.14159265358979323846;

	BOOL IsStringInLine(std::string line, std::string tarString)
	{
		std::size_t found = line.find(tarString);
		if (found != std::string::npos) {
			return true;
		}
		return false;
	}

	std::vector<std::string> spitString(std::string orgStr, char _delim)
	{
		std::vector<std::string> tmp;
		std::istringstream line(orgStr);
		while (line) {
			std::string sline;
			if (getline(line, sline, _delim)) {
				tmp.push_back(sline);
			}
		}
		return tmp;
	}

	long strToLong(std::string strVal)
	{
		try {
			return std::stol(strVal.c_str());
		}
		catch (const std::invalid_argument&) {
			return  -1;
		}

		catch (const std::out_of_range&) {
			return -2;
		}

		catch (const std::exception&) {
			return-3;
		}
		return 0;
	}

	float strToFloat(std::string strVal)
	{
		try {
			return std::stof(strVal.c_str());
		}
		catch (const std::invalid_argument&) {
			return  0.0;
		}

		catch (const std::out_of_range&) {
			return 0.0;
		}

		catch (const std::exception&) {
			return 0.0;
		}
		return 0.0;
	}

	enum MENU_TYPE
	{
		MENU_FRONT,
		MENU_SPOT,
		MENU_BACK,
		MENU_BACK_FRONT,
	};

const GLchar* vertexModelShader = R"glsl(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;
	layout (location = 2) in vec2 aTexCoords;

	out vec2 TexCoords;

	uniform mat4 modelview;
	uniform mat4 projection;
	uniform mat4 RotateMatrix;
	void main()
	{
		TexCoords = aTexCoords;
		vec4 position = RotateMatrix*vec4(aPos, 1.0);    
		gl_Position = projection * modelview * position;
	}
)glsl";

const GLchar* FragModelShader = R"glsl(
	#version 330 core
	out vec4 FragColor;

	in vec2 TexCoords;

	uniform sampler2D texture_diffuse1;

	void main()
	{    
		FragColor = texture(texture_diffuse1, TexCoords);
	}
)glsl";
};

CJPTViewBase::CJPTViewBase()
	:m_pShadeShader(new CShader)
	, m_pShadeShaderMesh(new CShader)
	, m_pArrowShader(new CShader)
	, m_pArrow3dShader(new CShader)
	, m_pTextureShader(new CShader)
	, m_pShaderModelLoader(new CShader)
	, m_pShadeLigting(new CShader)
	, m_pShadeLamp(new CShader)
	, m_CurrentWnd(nullptr)
	, m_renderData(nullptr)
{
	
}

CJPTViewBase::~CJPTViewBase()
{
	if (m_pShadeShader)
		delete m_pShadeShader;
	if (m_CurrentWnd)
		delete m_CurrentWnd;
	if (m_pShadeShaderMesh)
		delete m_pShadeShaderMesh;
	if (m_pArrowShader)
		delete m_pArrowShader;
	if (m_pArrow3dShader)
		delete m_pArrow3dShader;
	if (m_pTextureShader)
		delete m_pTextureShader;
	if (m_pShaderModelLoader)
		delete m_pShaderModelLoader;
	if (m_pShadeLigting)
		delete m_pShadeLigting;
	if (m_pShadeLamp)
		delete m_pShadeLamp;
}

void CJPTViewBase::OnDraw(CDC * pDC)
{
}

void CJPTViewBase::GLRenderScene()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	if (!m_CurrentWnd) {
		m_CurrentWnd = new sf::Window(sf::VideoMode(800, 600), "JPTSourceCodeV2 View", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);
		m_CurrentWnd->setFramerateLimit(60);
		m_CurrentWnd->setVerticalSyncEnabled(true);
		// activate the window
		m_CurrentWnd->setActive(true);
	}
		
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	LoadData();
	PrepairScene();

	if (CPreference::GetInstance().m_bloadTexture)
	{
		if (CPreference::GetInstance().m_texturepath.IsEmpty()){
			m_textureData1.m_path = "D:\\Personal\\JPTSourceCodeV2\\bin\\Common\\texture\\metal.png";
		}
		else {
			m_textureData1.m_path = StringTool::ConvertCStringtoString(CPreference::GetInstance().m_texturepath);
		}
		RenderHelper::LoadTextureData(m_textureData1);

		//m_textureData2.m_path = "D:\\Personal\\JPTSourceCodeV2\\bin\\Common\\texture\\grass.png";
		//RenderHelper::LoadTextureData(m_textureData2);

		// texture 1
		// ---------
		glGenTextures(1, &m_textureData1.textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureData1.textureID);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps

		if (m_textureData1.data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_textureData1.m_width, m_textureData1.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_textureData1.data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	int idx = 0;
	bool running = true;
	m_rotateAngle1 = 0.0;
	m_rotateAngle2 = 0.0;

	while (running)
	{
		sf::Event windowEvent;
		while (m_CurrentWnd->pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::Resized:
				glViewport(0, 0, windowEvent.size.width, windowEvent.size.height);
				break;
			case sf::Event::KeyPressed:
				OnKeyDown((int)windowEvent.key.code);
				break;
			case sf::Event::MouseButtonPressed:
				if (windowEvent.mouseButton.button == sf::Mouse::Right)
				{
					OnRightButtonDowns(windowEvent.mouseButton.x, windowEvent.mouseButton.y);
				}
				else if (windowEvent.mouseButton.button == sf::Mouse::Left)
				{
					OnLeftButtonDowns(windowEvent.mouseButton.x, windowEvent.mouseButton.y);
				}
				else if (windowEvent.mouseButton.button == sf::Mouse::Middle)
				{// midle

				}
				break;
			case sf::Event::MouseButtonReleased:
				if (windowEvent.mouseButton.button == sf::Mouse::Right)
				{
					OnRightButtonUp(windowEvent.mouseButton.x, windowEvent.mouseButton.y);
				}
				else if (windowEvent.mouseButton.button == sf::Mouse::Left)
				{
					OnLeftButtonUp(windowEvent.mouseButton.x, windowEvent.mouseButton.y);
				}
				else if (windowEvent.mouseButton.button == sf::Mouse::Middle)
				{// midle

				}
				break;
			case sf::Event::MouseMoved:
				OnMouseMove(windowEvent.mouseMove.x, windowEvent.mouseMove.y);
				break;
			case sf::Event::MouseWheelMoved:
				OnMouseWhell(windowEvent.mouseWheel.x, windowEvent.mouseWheel.y, windowEvent.mouseWheel.delta);
				break;
			case sf::Event::MouseWheelScrolled:
				OnMouseWhell(windowEvent.mouseWheelScroll.x, windowEvent.mouseWheel.y, windowEvent.mouseWheel.delta);
				break;
			}
		}

		m_ViewCtrl.GLPrepareScene();

		// clear the buffers
		if (CPreference::GetInstance().m_bClippingPlane)
		{
			glEnable(GL_CLIP_PLANE0);
			GLdouble adEquation[4] = { 1.0f, 0.0f, 0.0f, 0.0};
			glClipPlane(GL_CLIP_PLANE0, adEquation);
		}

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		GLDrawBackground();


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (CPreference::GetInstance().m_bCurlingFace) {
			glEnable(GL_CULL_FACE);
		}
		else {
			glDisable(GL_CULL_FACE);
		}

		if (!bModelLoadTest)
		{
			if (CPreference::GetInstance().m_bRenderSurface) {
				GLDrawElememtShader();
			}

			if (bLightingTest) {
				GLDrawLigtingSource();
			}

			if (CPreference::GetInstance().m_bRenderMesh)
			{
				GLDrawLineShader();
			}
			if (CPreference::GetInstance().m_bRenderVector)
			{
				GLDrawVectorField();
			}
			if (CPreference::GetInstance().m_bRender3DVecrtor)
			{
				GLDrawVectorField2();
			}
			if (CPreference::GetInstance().m_bOOBoxTest)
			{
				GLDrawBoxTest();
			}
			if (CPreference::GetInstance().m_bRenderPoints)
			{
				GLDrawNodes();
			}
		}
		else
		{
			GlDrawModelLoader();
		}

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		if (CPreference::GetInstance().m_bCurlingFace) {
			glDisable(GL_CULL_FACE);
		}
		else {
			glEnable(GL_CULL_FACE);
		}

		if (CPreference::GetInstance().m_bClippingPlane)
		{
			glDisable(GL_CLIP_PLANE0);
		}

		m_CurrentWnd->display();

		m_rotateAngle1 = idx * 0.5;
		m_rotateAngle2 = idx * 1;
		if (m_rotateAngle1 > 360)
			m_rotateAngle1 -= 360;
		if (m_rotateAngle2 > 360)
			m_rotateAngle2 -= 360;
		idx++;
	}
}

void CJPTViewBase::GLDrawElememtShader()
{
	auto pShader = m_pShadeShader;// ;
	if (bLightingTest)
		pShader = m_pShadeLigting;

	if (pShader->IsValid()){
		// changing uniform variable inside GLSL Program		
		GLint posAttrib = pShader->GetAttrubuteLocation("position");
		m_VBOData.m_ElemVBOData.m_vertices->SetVertextPointer(posAttrib, 3, GL_FALSE, 0);
		glEnableVertexAttribArray(0);

		GLint normalAttrib = pShader->GetAttrubuteLocation("normal");
		m_VBOData.m_ElemVBOData.m_normal->SetVertextPointer(normalAttrib, 3, GL_FALSE, 0);
		glEnableVertexAttribArray(1);

		GLint colAttrib = pShader->GetAttrubuteLocation("color");
		m_VBOData.m_ElemVBOData.m_color->SetVertextPointer(colAttrib, 4, GL_TRUE, 0);
		glEnableVertexAttribArray(2);
		if (CPreference::GetInstance().m_bloadTexture)
		{
			GLint texAttrib = m_pShadeShader->GetAttrubuteLocation("aTexCoord");
			m_VBOData.m_ElemVBOData.m_texture->SetVertextPointer(texAttrib, 2, GL_TRUE, 0);
			glEnableVertexAttribArray(3);
		}


		//// load image, create texture and generate mipmaps
		pShader->Enable();
		
		pShader->alterMat4f("projection", GetProjectionMatrix());
		pShader->alterMat4f("modelview", GetModelViewMatrix());
		
		if (bLightingTest)
		{
			pShader->alterUniform3f("lightColor", m_LightingColor);
			glm::mat4 Rotmat(1.0);
			auto fangle = m_rotateAngle1 *M_PI / 180;
			Rotmat = glm::rotate(Rotmat, fangle, glm::vec3(0.f, 1.f, 0.f));
			pShader->alterMat4f("RotateMatrix", Rotmat);

			glm::mat4 RotmatLight(1.0);
			auto fanglelight = 0.0f;// m_rotateAngle2 *M_PI / 180;
			RotmatLight = glm::rotate(RotmatLight, fanglelight, glm::vec3(0.f, 1.f, 0.f));
			glm::vec4 lightPos((float)m_LightPosition.vec[0], 
				(float)m_LightPosition.vec[1], (float)m_LightPosition.vec[2],1.0);
			lightPos = RotmatLight*lightPos;
			float flightPos[3];
			flightPos[0] = lightPos.x;
			flightPos[1] = lightPos.y;
			flightPos[2] = lightPos.z;
			pShader->alterUniform3f("lightPosition", flightPos);

			float viewpoint[3];
			viewpoint[0] = (float)m_LightPosition.vec[0];
			viewpoint[1] = (float)m_LightPosition.vec[1];
			viewpoint[2] = (float)m_LightPosition.vec[2];
			pShader->alterUniform3f("viewPos", viewpoint);
		}
		else {
			float light[4];
			glGetLightfv(GL_LIGHT0, GL_POSITION, light);
			pShader->alterVec4f("lightDir", light);
		}

		if (CPreference::GetInstance().m_bClippingPlane){
			//GLdouble adEquation[4];
			//glGetClipPlane(GL_CLIP_PLANE0, adEquation);
			GLdouble adEquation[4] = { 1.0f, 0.0f, 0.0f, 0.0 };
			GLfloat faEquation[4];
			for (int i = 0; i < 4; ++i) { faEquation[i] = (float)adEquation[i]; }
			pShader->alterVec4f("ClipPlane", faEquation);
		}

		if (CPreference::GetInstance().m_bloadTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textureData1.textureID);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOData.m_ElemVBOData.m_indices->GetEBOId());
		// bind textures on corresponding texture units
		// render container
		glDrawElements(GL_TRIANGLES, m_VBOData.m_ElemVBOData.m_indices->GetSize(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		if (CPreference::GetInstance().m_bloadTexture)
		{
			glDisableVertexAttribArray(3);
		}

		pShader->Disable();
	}
}

void CJPTViewBase::GLDrawLineShader()
{
	if (m_pShadeShaderMesh->IsValid()) {
		// changing uniform variable inside GLSL Program
		glDepthRange(0.0, 0.9999);
		GLint posAttrib = m_pShadeShaderMesh->GetAttrubuteLocation("position");
		m_VBOData.m_MeshVBOData.m_vertices->SetVertextPointer(posAttrib, 3, GL_FALSE, 0);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLint colAttrib = m_pShadeShaderMesh->GetAttrubuteLocation("color");
		m_VBOData.m_MeshVBOData.m_colorLine->SetVertextPointer(colAttrib, 4, GL_TRUE, 0);
		glEnableClientState(GL_COLOR_ARRAY);
		//// load image, create texture and generate mipmaps
		m_pShadeShaderMesh->Enable();

		m_pShadeShaderMesh->alterMat4f("projection", GetProjectionMatrix());
		m_pShadeShaderMesh->alterMat4f("modelview", GetModelViewMatrix());

		if (CPreference::GetInstance().m_bClippingPlane) {
			//GLdouble adEquation[4];
			//glGetClipPlane(GL_CLIP_PLANE0, adEquation);
			GLdouble adEquation[4] = { 1.0f, 0.0f, 0.0f, 0.0 };
			GLfloat faEquation[4];
			for (int i = 0; i < 4; ++i) { faEquation[i] = (float)adEquation[i]; }
			m_pShadeShaderMesh->alterVec4f("ClipPlane", faEquation);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOData.m_MeshVBOData.m_indicesLine->GetEBOId());
		// bind textures on corresponding texture units
		// render container
		glLineWidth(2.0);
		auto Cnt = m_VBOData.m_MeshVBOData.m_indicesLine->GetSize();
		glDrawElements(GL_LINES, Cnt, GL_UNSIGNED_INT, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDepthRange(0.0, 1.0);
		m_pShadeShaderMesh->Disable();
	}
}

void CJPTViewBase::GLDrawVectorField()
{
	if (m_pArrowShader->IsValid()) {
		
		GLint posAttrib = m_pArrowShader->GetAttrubuteLocation("position");
		m_VBOData.m_MeshVBOData.m_vertices->SetVertextPointer(posAttrib, 3, GL_FALSE, 0);
		glEnableVertexAttribArray(0);

		GLint normalAttrib = m_pArrowShader->GetAttrubuteLocation("normal");
		m_VBOData.m_MeshVBOData.m_normal->SetVertextPointer(normalAttrib, 3, GL_FALSE, 0);
		glEnableVertexAttribArray(1);

		GLint colAttrib = m_pArrowShader->GetAttrubuteLocation("color");
		m_VBOData.m_MeshVBOData.m_color->SetVertextPointer(colAttrib, 4, GL_TRUE, 0);
		glEnableVertexAttribArray(2);

		//// load image, create texture and generate mipmaps
		m_pArrowShader->Enable();

		m_pArrowShader->alterMat4f("projection", GetProjectionMatrix());
		m_pArrowShader->alterMat4f("modelview", GetModelViewMatrix());
		
		auto dScaleFactor = m_ViewCtrl.getscaleFactor();
		GLint scaleLoc = m_pArrowShader->GetUniformLocation("fscale");
		glUniform1f(scaleLoc, (float)dScaleFactor * 0.1f * 1.0f);
		
		// render container
		//glPointSize(2.0);
		int vertexCount = m_VBOData.m_MeshVBOData.m_vertices->GetSize() / 3;
		glDrawArrays(GL_POINTS, 0, vertexCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		m_pArrowShader->Disable();
	}
}

void CJPTViewBase::GLDrawVectorField2()
{
	if (m_pArrow3dShader->IsValid()) {
		// changing uniform variable inside GLSL Program

		GLint posAttrib = m_pArrow3dShader->GetAttrubuteLocation("position");
		m_VBOData.m_MeshVBOData.m_vertices->SetVertextPointer(posAttrib, 3, GL_FALSE, 0);
		glEnableVertexAttribArray(0);

		GLint normalAttrib = m_pArrow3dShader->GetAttrubuteLocation("normal");
		m_VBOData.m_MeshVBOData.m_normal->SetVertextPointer(normalAttrib, 3, GL_FALSE, 0);
		glEnableVertexAttribArray(1);

		GLint colAttrib = m_pArrow3dShader->GetAttrubuteLocation("color");
		m_VBOData.m_MeshVBOData.m_color->SetVertextPointer(colAttrib, 4, GL_TRUE, 0);
		glEnableVertexAttribArray(2);

		//// load image, create texture and generate mipmaps
		m_pArrow3dShader->Enable();

		m_pArrow3dShader->alterMat4f("projection", GetProjectionMatrix());
		m_pArrow3dShader->alterMat4f("modelview", GetModelViewMatrix());
		auto dScaleFactor = m_ViewCtrl.getscaleFactor();
		GLint scaleLoc = m_pArrowShader->GetUniformLocation("fscale");
		glUniform1f(scaleLoc, (float)dScaleFactor * 0.1f * 1.0f);

		// render container
		//glPointSize(2.0);
		int vertexCount = m_VBOData.m_MeshVBOData.m_vertices->GetSize() / 3;
		glDrawArrays(GL_POINTS, 0, vertexCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		m_pArrow3dShader->Disable();
	}
}

void CJPTViewBase::GLDrawNodes()
{
	if (m_pShadeShader->IsValid()) {
		// changing uniform variable inside GLSL Program
		GLint posAttrib = m_pShadeShader->GetAttrubuteLocation("position");
		m_VBOData.m_MeshVBOData.m_vertices->SetVertextPointer(posAttrib, 3, GL_FALSE, 0);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLint colAttrib = m_pShadeShader->GetAttrubuteLocation("color");
		m_VBOData.m_MeshVBOData.m_color->SetVertextPointer(colAttrib, 4, GL_TRUE, 0);
		glEnableClientState(GL_COLOR_ARRAY);
		//// load image, create texture and generate mipmaps
		m_pShadeShader->Enable();

		m_pShadeShader->alterMat4f("projection", GetProjectionMatrix());
		m_pShadeShader->alterMat4f("modelview", GetModelViewMatrix());

		// render container
		//glPointSize(2.0);
		int vertexCount = m_VBOData.m_MeshVBOData.m_vertices->GetSize() / 3;
		glDrawArrays(GL_POINT, 0, vertexCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		m_pShadeShader->Disable();
	}
}

void CJPTViewBase::GLDrawBoxTest()
{
	TVector3d vertex[8];
	m_OBBox.GetVertices(vertex);
	glLineWidth(2.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	{
		glVertex3d(vertex[0].X, vertex[0].Y, vertex[0].Z);
		glVertex3d(vertex[1].X, vertex[1].Y, vertex[1].Z);
		glVertex3d(vertex[2].X, vertex[2].Y, vertex[2].Z);
		glVertex3d(vertex[3].X, vertex[3].Y, vertex[3].Z);
	}
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (int i = 0; i < 4;++i)
	{
		glVertex3d(vertex[i].X, vertex[i].Y, vertex[i].Z);
		glVertex3d(vertex[4 + i].X, vertex[4 + i].Y, vertex[4 + i].Z);
	}
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	{
		glVertex3d(vertex[4].X, vertex[4].Y, vertex[4].Z);
		glVertex3d(vertex[5].X, vertex[5].Y, vertex[5].Z);
		glVertex3d(vertex[6].X, vertex[6].Y, vertex[6].Z);
		glVertex3d(vertex[7].X, vertex[7].Y, vertex[7].Z);
	}
	glEnd();
	glFlush();
}

void CJPTViewBase::GLTest()
{
	if (m_pTextureShader->IsValid()) {

		GLint posAttrib = m_pTextureShader->GetAttrubuteLocation("aPos");
		m_VBOData.m_ElemVBOData.m_vertices->SetVertextPointer(posAttrib, 3, GL_FALSE, 0);
		glEnableVertexAttribArray(0);

		GLint colAttrib = m_pTextureShader->GetAttrubuteLocation("aColor");
		m_VBOData.m_ElemVBOData.m_color->SetVertextPointer(colAttrib, 4, GL_TRUE, 0);
		glEnableVertexAttribArray(1);

		GLint textAttrib = m_pTextureShader->GetAttrubuteLocation("aTexCoord");
		m_VBOData.m_ElemVBOData.m_texture->SetVertextPointer(textAttrib, 2, GL_TRUE, 0);
		glEnableVertexAttribArray(2);


		//// load image, create texture and generate mipmaps
		m_pTextureShader->Enable();

		//m_pShadeShader->alterMat4f("projection", GetProjectionMatrix());
		//m_pShadeShader->alterMat4f("modelview", GetModelViewMatrix());

		//float light[4];
		//glGetLightfv(GL_LIGHT0, GL_POSITION, light);
		//m_pShadeShader->alterVec4f("lightDir", light);

		//if (CPreference::GetInstance().m_bClippingPlane) {
		//	//GLdouble adEquation[4];
		//	//glGetClipPlane(GL_CLIP_PLANE0, adEquation);
		//	GLdouble adEquation[4] = { 1.0f, 0.0f, 0.0f, 0.0 };
		//	GLfloat faEquation[4];
		//	for (int i = 0; i < 4; ++i) { faEquation[i] = (float)adEquation[i]; }
		//	m_pShadeShader->alterVec4f("ClipPlane", faEquation);
		//}

		// load and create a texture 
		// -------------------------
		unsigned int texture1, texture2;
		// texture 1
		// ---------
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps

		if (m_textureData1.data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_textureData1.m_width, m_textureData1.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_textureData1.data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (m_textureData2.data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_textureData2.m_width, m_textureData2.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData2.data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		m_pTextureShader->alterInt("texture1", 0);
		m_pTextureShader->alterInt("texture2", 1);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOData.m_ElemVBOData.m_indices->GetEBOId());
		glDrawElements(GL_TRIANGLES, m_VBOData.m_ElemVBOData.m_indices->GetSize(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		m_pTextureShader->Disable();
	}
}

void CJPTViewBase::GLDrawCameraTest()
{

}

void CJPTViewBase::GLDrawOITtest()
{
}

void CJPTViewBase::GLDrawLigtingSource()
{
	if (m_pShadeLamp->IsValid()) {
		// changing uniform variable inside GLSL Program		
		GLint posAttrib = m_pShadeLamp->GetAttrubuteLocation("position");
		m_VBOLigtingData.m_ElemVBOData.m_vertices->SetVertextPointer(posAttrib, 3, GL_FALSE, 0);
		glEnableVertexAttribArray(0);

		//// load image, create texture and generate mipmaps
		m_pShadeLamp->Enable();

		m_pShadeLamp->alterMat4f("projection", GetProjectionMatrix());
		m_pShadeLamp->alterMat4f("modelview", GetModelViewMatrix());

		glm::mat4 Rotmat(1.0);
		auto fangle = 0.0f;// m_rotateAngle2 *M_PI / 180;
		Rotmat = glm::rotate(Rotmat, fangle, glm::vec3(0.f, 1.f, 0.f));
		m_pShadeLamp->alterMat4f("RotateMatrix", Rotmat);

		m_pShadeLamp->alterUniform3f("ObjectColor", m_LightingColor);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOLigtingData.m_ElemVBOData.m_indices->GetEBOId());
		// bind textures on corresponding texture units
		// render container
		glDrawElements(GL_TRIANGLES, m_VBOLigtingData.m_ElemVBOData.m_indices->GetSize(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);

		m_pShadeLamp->Disable();
	}
}

void CJPTViewBase::GlDrawModelLoader()
{
	if (m_pShaderModelLoader->IsValid()) {
		//// load image, create texture and generate mipmaps
		m_pShaderModelLoader->Enable();

		m_pShaderModelLoader->alterMat4f("projection", GetProjectionMatrix());
		m_pShaderModelLoader->alterMat4f("modelview", GetModelViewMatrix());

		glm::mat4 Rotmat(1.0);
		auto fangle = m_rotateAngle2 *M_PI / 180;
		Rotmat = glm::rotate(Rotmat, fangle, glm::vec3(0.f, 1.f, 0.f));
		m_pShaderModelLoader->alterMat4f("RotateMatrix", Rotmat);

		
		m_model.Draw(m_pShaderModelLoader);

		m_pShaderModelLoader->Disable();
	}
}

void CJPTViewBase::GLDrawBackground()
{
	GLubyte colBack1[4] = { 255,255,255,255 };
	GLubyte colBack2[4] = { 0,0,0,255 };

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
		glPushMatrix();
		{
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
		}
		glPushMatrix();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_STENCIL_TEST);

		glBegin(GL_QUADS);
		{
			//glColor3f( 0.8f, 0.8f, 1.0f );

			glColor4ubv(colBack2);
			glVertex3f(1.0f, -1.0f, 0.0f);	// v5
			glVertex3f(-1.0f, -1.0f, 0.0f);	// v2

			glColor4ubv(colBack1);
			glVertex3f(-1.0f, 1.0f, 0.0f); // v1
			glVertex3f(1.0f, 1.0f, 0.0f);	// v
		}
		glEnd();

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	glPopAttrib();
}

void CJPTViewBase::OnKeyDown(int code)
{
	switch ((sf::Keyboard::Key)code)
	{
	case sf::Keyboard::Num1:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_FRONT);
		break;
	case sf::Keyboard::Num2:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_BACK);
		break;
	case sf::Keyboard::Num3:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_RIGHT);
		break;
	case sf::Keyboard::Num4:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_LEFT);
		break;
	case sf::Keyboard::Num5:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_TOP);
		break;
	case sf::Keyboard::Num6:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_BOTTOM);
		break;
	case sf::Keyboard::Num7:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_USER1);
		break;
	case sf::Keyboard::Num8:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_USER2);
		break;
	case sf::Keyboard::Num9:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_USER3);
		break;
	case sf::Keyboard::Num0:
		m_ViewCtrl.GLViewpoint(CViewCtrl::VIEW_USER4);
		break;
	default:
		break;
	}
}

void CJPTViewBase::OnLeftButtonDowns(int x, int y)
{
}

void CJPTViewBase::OnRightButtonDowns(int x, int y)
{
}

void CJPTViewBase::OnLeftButtonUp(int x, int y)
{
}

void CJPTViewBase::OnRightButtonUp(int x, int y)
{
}

void CJPTViewBase::OnMouseMove(int x, int y)
{
}

void CJPTViewBase::OnMouseWhell(int x, int y, int deltal)
{
}

void CJPTViewBase::createContextMenu()
{
}

void CJPTViewBase::PrepairScene()
{
	LoadShaders();
	m_ViewCtrl.GLResize(800, 600);
	//m_ViewCtrl.SetViewMetric(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
	GLDrawBackground();
	glPushMatrix();
	{
		double dRange[6];
		m_cbBox.GetRange(dRange);
		glLoadIdentity();
		
		m_ViewCtrl.GLSetRange(dRange);
		m_ViewCtrl.SetCenter(FALSE);
		m_ViewCtrl.GLCalModelScale();
		
	}
	glPopMatrix();
}

void CJPTViewBase::LoadShaders()
{
	if (GLEW_ARB_shader_objects)
	{
		if (!m_pShadeShader->compile(vertexSource, fragmentSource, GeometrySource))
		{
			m_strError = m_pShadeShader->GetErrorLog();
		}
		if (!m_pShadeShaderMesh->compile(vertexSourceMesh, fragmentSourceMesh, GeometrySourceMesh))
		{
			m_strError = m_pShadeShaderMesh->GetErrorLog();
		}//m_pArrowShader
		if (!m_pArrowShader->compile(vertexArrowShader, fragmentArrowShader, GeoArrowShader))
		{
			m_strError = m_pArrowShader->GetErrorLog();
		}
		//m_pArrow3dShader
		if (!m_pArrow3dShader->compile(vertex3DArrowShader, fragment3DArrowShader, Geo3DArrowShader))
		{
			m_strError = m_pArrow3dShader->GetErrorLog();
		}
		if (!m_pTextureShader->compile(textureVS, textureFS))
		{
			m_strError = m_pTextureShader->GetErrorLog();
		}
		if (!m_pTextureShader->compileFile("shader/oit.vs", "shader/oit.fs"))
		{
			m_strError = m_pTextureShader->GetErrorLog();
		}
		//
		if (!m_pShadeLigting->compile(Lighting_vert, Lighting_frag))
		{
			m_strError = m_pShadeLigting->GetErrorLog();
		}

		if (!m_pShadeLamp->compile(lamp_vert, lamp_frag))
		{
			m_strError = m_pShadeLamp->GetErrorLog();
		}
		if (!m_pShaderModelLoader->compile(vertexModelShader, FragModelShader))
		{
			m_strError = m_pShaderModelLoader->GetErrorLog();
		}
	}
}

void CJPTViewBase::WGLActivate()
{
	//wMakeContextCurrent();
}

void CJPTViewBase::WGLDeactivate()
{
}

void CJPTViewBase::LoadData()
{
	if (!bModelLoadTest)
	{
		if (!m_renderData)
			m_renderData.reset(new CRenderElemData());
		if (!mUnivElemMap.empty())
		{
			LoadBDFData();
		}
		else if (!m_ByPaths.empty())
		{
			int nCnt = 0;
			std::vector<std::vector<double>> RangeData(3);
			for (const auto& path : m_ByPaths)
			{
				LoadByFile(path, nCnt, RangeData);
			}

			m_VBOData.m_ElemVBOData.m_vertices->SetData(&m_renderData->vertices[0], (GLuint)m_renderData->vertices.size(), GL_STATIC_DRAW);
			m_VBOData.m_ElemVBOData.m_color->SetData(&m_renderData->color[0], (GLuint)m_renderData->color.size(), GL_STATIC_DRAW);
			m_VBOData.m_ElemVBOData.m_indices->SetData(&m_renderData->indices[0], (GLuint)m_renderData->indices.size(), GL_STATIC_DRAW);
			m_VBOData.m_ElemVBOData.m_normal->SetData(&m_renderData->normal[0], (GLuint)m_renderData->normal.size(), GL_STATIC_DRAW);


			m_VBOData.m_MeshVBOData.m_vertices->SetData(&m_renderData->vertices[0], (GLuint)m_renderData->vertices.size(), GL_STATIC_DRAW);
			m_VBOData.m_MeshVBOData.m_colorLine->SetData(&m_renderData->colorLine[0], (GLuint)m_renderData->colorLine.size(), GL_STATIC_DRAW);
			m_VBOData.m_MeshVBOData.m_color->SetData(&m_renderData->color[0], (GLuint)m_renderData->color.size(), GL_STATIC_DRAW);
			m_VBOData.m_MeshVBOData.m_indicesLine->SetData(&m_renderData->indicesLine[0], (GLuint)m_renderData->indicesLine.size(), GL_STATIC_DRAW);
			m_VBOData.m_MeshVBOData.m_normal->SetData(&m_renderData->normal[0], (GLuint)m_renderData->normal.size(), GL_STATIC_DRAW);

			double dRange[6];
			dRange[0] = *std::min_element(RangeData.at(0).begin(), RangeData.at(0).end());
			dRange[1] = *std::min_element(RangeData.at(1).begin(), RangeData.at(1).end());
			dRange[2] = *std::min_element(RangeData.at(2).begin(), RangeData.at(2).end());
			dRange[3] = *std::max_element(RangeData.at(0).begin(), RangeData.at(0).end());
			dRange[4] = *std::max_element(RangeData.at(1).begin(), RangeData.at(1).end());
			dRange[5] = *std::max_element(RangeData.at(2).begin(), RangeData.at(2).end());

			m_cbBox.SetRange(dRange[0], dRange[1], dRange[2], dRange[3], dRange[4], dRange[5]);
		}
		else if (!m_3DObjectPaths.empty())
		{
			//Object3DReader
			for (const auto& path : m_3DObjectPaths)
			{
				CObj3DReader reader(path);
				reader.Read();
				m_renderData->vertices = reader.m_Vertex;
				m_renderData->color = reader.m_Colors;
				m_renderData->normal = reader.m_Normal;

				std::vector<double> vdX, vdY, vdZ;
				size_t nElem = (size_t)(reader.m_Vertex.size() / 3);
				int NodeCnt = 0;
				for (size_t i = 0; i < nElem; i++)
				{
					vdX.push_back(reader.m_Vertex.at(3 * i));
					vdY.push_back(reader.m_Vertex.at(3 * i + 1));
					vdZ.push_back(reader.m_Vertex.at(3 * i + 2));
				}

				double dRange[6];
				dRange[0] = *std::min_element(vdX.begin(), vdX.end());
				dRange[1] = *std::min_element(vdY.begin(), vdY.end());
				dRange[2] = *std::min_element(vdZ.begin(), vdZ.end());
				dRange[3] = *std::max_element(vdX.begin(), vdX.end());
				dRange[4] = *std::max_element(vdY.begin(), vdY.end());
				dRange[5] = *std::max_element(vdZ.begin(), vdZ.end());

				m_cbBox.SetRange(dRange[0], dRange[1], dRange[2], dRange[3], dRange[4], dRange[5]);

				m_VBOData.m_MeshVBOData.m_vertices->SetData(&m_renderData->vertices[0], (GLuint)m_renderData->vertices.size(), GL_STATIC_DRAW);
				m_VBOData.m_MeshVBOData.m_color->SetData(&m_renderData->color[0], (GLuint)m_renderData->color.size(), GL_STATIC_DRAW);
				m_VBOData.m_MeshVBOData.m_normal->SetData(&m_renderData->normal[0], (GLuint)m_renderData->normal.size(), GL_STATIC_DRAW);
			}
		}
		else {
			TVector3d vTrans(0.0, 0.0, 0.0);
			float color[3];
			color[0] = 1.0f;
			color[1] = 0.5f;
			color[2] = 0.31f;
			RenderHelper::GetCubeRenderData(m_renderData, vTrans, color, 10);

			m_VBOData.m_ElemVBOData.m_vertices->SetData(&m_renderData->vertices[0], (GLuint)m_renderData->vertices.size(), GL_STATIC_DRAW);
			m_VBOData.m_ElemVBOData.m_color->SetData(&m_renderData->color[0], (GLuint)m_renderData->color.size(), GL_STATIC_DRAW);
			m_VBOData.m_ElemVBOData.m_indices->SetData(&m_renderData->indices[0], (GLuint)m_renderData->indices.size(), GL_STATIC_DRAW);
			m_VBOData.m_ElemVBOData.m_normal->SetData(&m_renderData->normal[0], (GLuint)m_renderData->normal.size(), GL_STATIC_DRAW);
			m_VBOData.m_ElemVBOData.m_texture->SetData(&m_renderData->textCoord[0], (GLuint)m_renderData->textCoord.size(), GL_STATIC_DRAW);

			m_VBOData.m_MeshVBOData.m_vertices->SetData(&m_renderData->vertices[0], (GLuint)m_renderData->vertices.size(), GL_STATIC_DRAW);
			m_VBOData.m_MeshVBOData.m_colorLine->SetData(&m_renderData->colorLine[0], (GLuint)m_renderData->colorLine.size(), GL_STATIC_DRAW);
			m_VBOData.m_MeshVBOData.m_color->SetData(&m_renderData->color[0], (GLuint)m_renderData->color.size(), GL_STATIC_DRAW);
			m_VBOData.m_MeshVBOData.m_indicesLine->SetData(&m_renderData->indicesLine[0], (GLuint)m_renderData->indicesLine.size(), GL_STATIC_DRAW);
			m_VBOData.m_MeshVBOData.m_normal->SetData(&m_renderData->normal[0], (GLuint)m_renderData->normal.size(), GL_STATIC_DRAW);

			std::vector<double> vdX, vdY, vdZ;
			size_t nElem = (size_t)(m_renderData->vertices.size() / 3);
			int NodeCnt = 0;
			for (size_t i = 0; i < nElem; i++)
			{
				vdX.push_back(m_renderData->vertices.at(3 * i));
				vdY.push_back(m_renderData->vertices.at(3 * i + 1));
				vdZ.push_back(m_renderData->vertices.at(3 * i + 2));
			}
			if (bLightingTest)
			{
				std::unique_ptr<CRenderElemData> LghtingData = std::make_unique<CRenderElemData>();
				vTrans.SetData(8, 10, 8);

				m_LightingColor[0] = 1.0f;
				m_LightingColor[1] = 1.0f;
				m_LightingColor[2] = 1.0f;
				m_LightPosition = vTrans;

				RenderHelper::GetCubeRenderData(LghtingData, vTrans, m_LightingColor, 1.0);
				m_VBOLigtingData.m_ElemVBOData.m_vertices->SetData(&LghtingData->vertices[0], (GLuint)LghtingData->vertices.size(), GL_STATIC_DRAW);
				m_VBOLigtingData.m_ElemVBOData.m_color->SetData(&LghtingData->color[0], (GLuint)LghtingData->color.size(), GL_STATIC_DRAW);
				m_VBOLigtingData.m_ElemVBOData.m_indices->SetData(&LghtingData->indices[0], (GLuint)LghtingData->indices.size(), GL_STATIC_DRAW);
				m_VBOLigtingData.m_ElemVBOData.m_normal->SetData(&LghtingData->normal[0], (GLuint)LghtingData->normal.size(), GL_STATIC_DRAW);
				m_VBOLigtingData.m_ElemVBOData.m_texture->SetData(&LghtingData->textCoord[0], (GLuint)LghtingData->textCoord.size(), GL_STATIC_DRAW);
				nElem = (size_t)(LghtingData->vertices.size() / 3);
				for (size_t i = 0; i < nElem; i++)
				{
					vdX.push_back(LghtingData->vertices.at(3 * i));
					vdY.push_back(LghtingData->vertices.at(3 * i + 1));
					vdZ.push_back(LghtingData->vertices.at(3 * i + 2));
				}
			}
			double dRange[6];
			dRange[0] = *std::min_element(vdX.begin(), vdX.end());
			dRange[1] = *std::min_element(vdY.begin(), vdY.end());
			dRange[2] = *std::min_element(vdZ.begin(), vdZ.end());
			dRange[3] = *std::max_element(vdX.begin(), vdX.end());
			dRange[4] = *std::max_element(vdY.begin(), vdY.end());
			dRange[5] = *std::max_element(vdZ.begin(), vdZ.end());
			m_cbBox.SetRange(dRange[0], dRange[1], dRange[2], dRange[3], dRange[4], dRange[5]);
		}
	}
	else{
		double dRange[6];
		m_model.loadModel("D:/Personal/JPTSourceCodeV2/resources/objects/cyborg/cyborg.obj");
		m_model.GetRange(dRange);
		m_cbBox.SetRange(dRange[0], dRange[1], dRange[2], dRange[3], dRange[4], dRange[5]);
	}

}

void CJPTViewBase::LoadBDFData()
{
	if (!m_renderData)
		m_renderData.reset(new CRenderElemData());
	//GetRandomColor
	std::vector<double> vdX, vdY, vdZ;
	std::vector<TVector3d> points(mUnivNodeMap.size());
	int NodeCnt = 0;
	int MaxCnt = mUnivNodeMap.size();
	auto itmapNode = mUnivNodeMap.begin();
	while (itmapNode != mUnivNodeMap.end())
	{
		points.at(NodeCnt).SetData(itmapNode->second->dCoord.X, itmapNode->second->dCoord.Y, itmapNode->second->dCoord.Z);
		m_renderData->vertices.push_back((float)itmapNode->second->dCoord.X);
		vdX.push_back(itmapNode->second->dCoord.X);
		m_renderData->vertices.push_back((float)itmapNode->second->dCoord.Y);
		vdY.push_back(itmapNode->second->dCoord.Y);
		m_renderData->vertices.push_back((float)itmapNode->second->dCoord.Z);
		vdZ.push_back(itmapNode->second->dCoord.Z);
		
		RenderHelper::GetHSVRdColor(m_renderData->color, MaxCnt, NodeCnt);
		RenderHelper::SetColorMeshLine(m_renderData->colorLine);
		auto vElems = itmapNode->second->GetConnectedElem();
		GetNormalVectorOfNode(vElems, m_renderData->normal);
		itmapNode->second->SetFlag(NodeCnt++);
		++itmapNode;
	}

	auto GetIndex = [&](const long& ID) {
		auto itf = mUnivNodeMap.find(ID);
		if (itf != mUnivNodeMap.end()){
			return itf->second->GetFlag();
		}
		return -1l;
	};

	auto itmapElem = mUnivElemMap.begin();
	while (itmapElem != mUnivElemMap.end()){
		auto NodeID = itmapElem->second->GetNodeIndex();
		auto pTri = &_tessTri[itmapElem->second->GetElemType()];
		for (int i = 0; i < pTri->cntTess; ++i){
			m_renderData->indices.push_back(GetIndex(NodeID.at((size_t)pTri->idxTess[i][0])));
			m_renderData->indices.push_back(GetIndex(NodeID.at((size_t)pTri->idxTess[i][1])));
			m_renderData->indices.push_back(GetIndex(NodeID.at((size_t)pTri->idxTess[i][2])));
		}
		auto pTriLine = &_tessLine[itmapElem->second->GetElemType()];
		for (size_t i = 0; i < NodeID.size(); ++i) {
			m_renderData->indicesLine.push_back(GetIndex(NodeID.at((size_t)pTriLine->idxTess[i][0])));
			m_renderData->indicesLine.push_back(GetIndex(NodeID.at((size_t)pTriLine->idxTess[i][1])));
		}
		++itmapElem;
	}
	
	double dRange[6];
	dRange[0] = *std::min_element(vdX.begin(), vdX.end());
	dRange[1] = *std::min_element(vdY.begin(), vdY.end());
	dRange[2] = *std::min_element(vdZ.begin(), vdZ.end());
	dRange[3] = *std::max_element(vdX.begin(), vdX.end());
	dRange[4] = *std::max_element(vdY.begin(), vdY.end());
	dRange[5] = *std::max_element(vdZ.begin(), vdZ.end());

	m_cbBox.SetRange(dRange[0], dRange[1], dRange[2], dRange[3], dRange[4], dRange[5]);
	m_OBBox.ComputeFromPoints(points);

	// caculate Texture data. 
	itmapNode = mUnivNodeMap.begin();
	while (itmapNode != mUnivNodeMap.end())
	{
		double dX, dY;
		GetTextureData(itmapNode->second, dX,dY);
		m_renderData->textCoord.push_back(dX);
		m_renderData->textCoord.push_back(dY);
		++itmapNode;
	}


	m_VBOData.m_ElemVBOData.m_vertices->SetData(&m_renderData->vertices[0], (GLuint)m_renderData->vertices.size(), GL_STATIC_DRAW);
	m_VBOData.m_ElemVBOData.m_color->SetData(&m_renderData->color[0], (GLuint)m_renderData->color.size(), GL_STATIC_DRAW);
	m_VBOData.m_ElemVBOData.m_indices->SetData(&m_renderData->indices[0], (GLuint)m_renderData->indices.size(), GL_STATIC_DRAW);
	m_VBOData.m_ElemVBOData.m_normal->SetData(&m_renderData->normal[0], (GLuint)m_renderData->normal.size(), GL_STATIC_DRAW);
	m_VBOData.m_ElemVBOData.m_texture->SetData(&m_renderData->textCoord[0], (GLuint)m_renderData->textCoord.size(), GL_STATIC_DRAW);

	m_VBOData.m_MeshVBOData.m_vertices->SetData(&m_renderData->vertices[0], (GLuint)m_renderData->vertices.size(), GL_STATIC_DRAW);
	m_VBOData.m_MeshVBOData.m_colorLine->SetData(&m_renderData->colorLine[0], (GLuint)m_renderData->colorLine.size(), GL_STATIC_DRAW);
	m_VBOData.m_MeshVBOData.m_color->SetData(&m_renderData->color[0], (GLuint)m_renderData->color.size(), GL_STATIC_DRAW);
	m_VBOData.m_MeshVBOData.m_indicesLine->SetData(&m_renderData->indicesLine[0], (GLuint)m_renderData->indicesLine.size(), GL_STATIC_DRAW);
	m_VBOData.m_MeshVBOData.m_normal->SetData(&m_renderData->normal[0], (GLuint)m_renderData->normal.size(), GL_STATIC_DRAW);
}

void CJPTViewBase::LoadByFile(const CString & path, int& nVertex, std::vector<std::vector<double>>& RangeData)
{
	std::ifstream infile;
	infile.open(path, std::ios::in);
	std::string line;
	bool bVertex = false;
	bool bnormal = false;
	while (std::getline(infile, line)) {
		if (IsStringInLine(line, "#Vertex")){
			bVertex = true;
		}
		if (IsStringInLine(line, "#Normal")) {
			bnormal = true;
			bVertex = false;
		}
		if (bVertex){
			auto dVals = spitString(line, ',');
			if (dVals.size() >= 3){
				m_renderData->vertices.push_back(strToFloat(dVals.at(0)));
				m_renderData->vertices.push_back(strToFloat(dVals.at(1)));
				m_renderData->vertices.push_back(strToFloat(dVals.at(2)));

				RangeData.at(0).push_back(strToFloat(dVals.at(0)));
				RangeData.at(1).push_back(strToFloat(dVals.at(1)));
				RangeData.at(2).push_back(strToFloat(dVals.at(2)));

				RenderHelper::GetHSVRdColor(m_renderData->color, 1);
				RenderHelper::GetHSVRdColor(m_renderData->colorLine, 1);

				m_renderData->indices.push_back(nVertex);
				nVertex++;
			}
		}
		if (bnormal)
		{
			auto dVals = spitString(line, ',');
			if (dVals.size() >= 3) {
				m_renderData->normal.push_back(strToFloat(dVals.at(0)));
				m_renderData->normal.push_back(strToFloat(dVals.at(1)));
				m_renderData->normal.push_back(strToFloat(dVals.at(2)));
			}
		}
	}
	for (size_t i = 0; i < (size_t)m_renderData->vertices.size()/9; i++)
	{
		m_renderData->indicesLine.push_back(3 * i);
		m_renderData->indicesLine.push_back(3 * i + 1);
		m_renderData->indicesLine.push_back(3 * i + 1);
		m_renderData->indicesLine.push_back(3 * i + 2);
		m_renderData->indicesLine.push_back(3 * i + 2);
		m_renderData->indicesLine.push_back(3 * i);
	}
}

void CJPTViewBase::initOITTest()
{

}

// delete later
glm::mat4 CJPTViewBase::GetModelViewMatrix()
{
	return glm::mat4(m_ViewCtrl.GetViewMatrix().adMV[0], m_ViewCtrl.GetViewMatrix().adMV[1],
					 m_ViewCtrl.GetViewMatrix().adMV[2], m_ViewCtrl.GetViewMatrix().adMV[3],
					 m_ViewCtrl.GetViewMatrix().adMV[4], m_ViewCtrl.GetViewMatrix().adMV[5],
					 m_ViewCtrl.GetViewMatrix().adMV[6], m_ViewCtrl.GetViewMatrix().adMV[7],
					 m_ViewCtrl.GetViewMatrix().adMV[8], m_ViewCtrl.GetViewMatrix().adMV[9],
					 m_ViewCtrl.GetViewMatrix().adMV[10], m_ViewCtrl.GetViewMatrix().adMV[11],
					 m_ViewCtrl.GetViewMatrix().adMV[12], m_ViewCtrl.GetViewMatrix().adMV[13],
					 m_ViewCtrl.GetViewMatrix().adMV[14], m_ViewCtrl.GetViewMatrix().adMV[15]);
}

glm::mat4 CJPTViewBase::GetViewMatrix()
{
	return glm::mat4(m_ViewCtrl.GetViewMatrix().aiVP[0], m_ViewCtrl.GetViewMatrix().aiVP[1],
		m_ViewCtrl.GetViewMatrix().aiVP[2], m_ViewCtrl.GetViewMatrix().aiVP[3],
		m_ViewCtrl.GetViewMatrix().aiVP[4], m_ViewCtrl.GetViewMatrix().aiVP[5],
		m_ViewCtrl.GetViewMatrix().aiVP[6], m_ViewCtrl.GetViewMatrix().aiVP[7],
		m_ViewCtrl.GetViewMatrix().aiVP[8], m_ViewCtrl.GetViewMatrix().aiVP[9],
		m_ViewCtrl.GetViewMatrix().aiVP[10], m_ViewCtrl.GetViewMatrix().aiVP[11],
		m_ViewCtrl.GetViewMatrix().aiVP[12], m_ViewCtrl.GetViewMatrix().aiVP[13],
		m_ViewCtrl.GetViewMatrix().aiVP[14], m_ViewCtrl.GetViewMatrix().aiVP[15]);
}

glm::mat4 CJPTViewBase::GetProjectionMatrix()
{
	return glm::mat4(m_ViewCtrl.GetViewMatrix().adPJ[0], m_ViewCtrl.GetViewMatrix().adPJ[1],
					m_ViewCtrl.GetViewMatrix().adPJ[2], m_ViewCtrl.GetViewMatrix().adPJ[3],
					m_ViewCtrl.GetViewMatrix().adPJ[4], m_ViewCtrl.GetViewMatrix().adPJ[5],
					m_ViewCtrl.GetViewMatrix().adPJ[6], m_ViewCtrl.GetViewMatrix().adPJ[7],
					m_ViewCtrl.GetViewMatrix().adPJ[8], m_ViewCtrl.GetViewMatrix().adPJ[9],
					m_ViewCtrl.GetViewMatrix().adPJ[10], m_ViewCtrl.GetViewMatrix().adPJ[11],
					m_ViewCtrl.GetViewMatrix().adPJ[12], m_ViewCtrl.GetViewMatrix().adPJ[13],
					m_ViewCtrl.GetViewMatrix().adPJ[14], m_ViewCtrl.GetViewMatrix().adPJ[15]);
}

void CJPTViewBase::GetNormalVectorOfNode(std::vector<long>& connectedElem, std::vector<GLfloat>& normal)
{
	TVector3d vnormal;
	for (const auto& ElemID : connectedElem)
	{
		auto itmap = mUnivElemMap.find(ElemID);
		if (itmap != mUnivElemMap.end()) {
			vnormal += itmap->second->GetNormal();
		}
	}

	vnormal /= (double)connectedElem.size();

	normal.push_back((float)vnormal.X);
	normal.push_back((float)vnormal.Y);
	normal.push_back((float)vnormal.Z);
}

void CJPTViewBase::GetTextureData(std::shared_ptr<CJPTNode>& Node, double & dX, double & dY)
{
	double corner1[3];
	double corner2[3];

	m_cbBox.GetMin(corner1);
	m_cbBox.GetMax(corner2);

	// codinate of texture from 0-1;
	dX = (Node->dCoord.vec[0] - corner1[0])/(corner2[0] - corner1[0]);
	dY = (Node->dCoord.vec[1] - corner1[1]) / (corner2[1] - corner1[1]);
}

