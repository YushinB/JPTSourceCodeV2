#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class CRenderVulkan
{
public:
	CRenderVulkan();
	~CRenderVulkan();

	void GLRenderScene();

protected:
	void initWindow();
	void mainLoop();
	void initVulkan();
	void cleanup();
	void createInstance();

	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
private:
	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	CString m_buildMessage;
};