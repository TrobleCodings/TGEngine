// ConsoleApplication1.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
//

#include "stdafx.h"
#include "Console.h"
#include "Preferences.h"

using namespace std;
using namespace GLVV;

bool debug = true;

void debugOut(uint32_t div_cou,VkPhysicalDevice graka_phy[]) {
	if (debug) {
		for (size_t i = 0; i < div_cou; i++)
		{
			cout << endl;
			cout << "==================================" << endl;
			cout << endl;
			VkPhysicalDevice cdevice = graka_phy[i];
			VkPhysicalDeviceProperties cdivprops;

			vkGetPhysicalDeviceProperties(cdevice, &cdivprops);

			uint32_t av = cdivprops.apiVersion;
			uint32_t dv = cdivprops.driverVersion;

			cout << i + 1 << ". Card " << cdivprops.deviceName << endl;
			cout << "Api version ";
			printVersion(av);
		    cout << endl;
			cout << "Driver version ";
			printVersion(dv);
			cout << endl;
			cout << "Type " << cdivprops.deviceType << endl;
			cout << "ID " << cdivprops.deviceID << endl;

			uint32_t div_qu_prop_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(cdevice, &div_qu_prop_count, nullptr);

			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(cdevice, &features);

			featuresPrint(features);

			vector<VkQueueFamilyProperties> mem_props = {};
			mem_props.resize(div_qu_prop_count);
			vkGetPhysicalDeviceQueueFamilyProperties(cdevice, &div_qu_prop_count, mem_props.data());

			for (size_t s = 0; s < div_qu_prop_count; s++)
			{
				cout << endl;
				VkQueueFamilyProperties prop_mem = mem_props[s];
				cout << s + 1 << ". Queue Family " << prop_mem.queueCount << endl;
				cout << "Flag " << prop_mem.queueFlags << endl;
				cout << "Image transf W" << prop_mem.minImageTransferGranularity.width << " - H" << prop_mem.minImageTransferGranularity.height << " - D" << prop_mem.minImageTransferGranularity.depth << endl;
				cout << "Valid Bits " << prop_mem.timestampValidBits << endl;
			}

		}
		cout << endl;
		cout << "==================================" << endl;
		cout << endl;
	}
}

void debugLayerAndExtensions() {
	uint32_t layer_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	vector<VkLayerProperties> props = {};
	props.resize(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, props.data());	

	if (debug) {
		for (int i = 0; i < layer_count; i++)
		{
			VkLayerProperties cprop = props[i];
			cout << "Found layer property " << cprop.layerName << " - " << cprop.description << endl;
			cout << "Impl_Version ";
			printVersion(cprop.implementationVersion);
			cout << endl;
			cout << "Spec_Version ";
			printVersion(cprop.specVersion);
			cout << endl;

			uint32_t ext_count = 0;
			vkEnumerateInstanceExtensionProperties(cprop.layerName,&ext_count, nullptr);

			vector<VkExtensionProperties> extns = {};
			extns.resize(ext_count);
			vkEnumerateInstanceExtensionProperties(cprop.layerName,&ext_count, extns.data());

			for (int g = 0; g < ext_count; g++)
			{
				cout << endl;
				VkExtensionProperties ex = extns[g];
				cout << g + 1 << ". " << ex.extensionName << " - Version: ";
				printVersion(ex.specVersion);
				cout << endl;
			}

			cout << endl;
		}

		cout << "Extensions: " << endl;

		uint32_t ext_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);

		vector<VkExtensionProperties> extns = {};
		extns.resize(ext_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, extns.data());

		for (int g = 0; g < ext_count; g++)
		{
			cout << endl;
			VkExtensionProperties ex = extns[g];
			cout << g + 1 << ". " << ex.extensionName << " - Version: ";
			printVersion(ex.specVersion);
			cout << endl;
		}

		cout << endl;
	}
}

using namespace InitPrefs;

void initVulkan() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Engine Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "TEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vector<char*> pon = {
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_VALVE_steam_overlay",
	};

	vector<VkLayerProperties> lay_props;
	uint32_t layprop_cou = 0;
	vkEnumerateInstanceLayerProperties(&layprop_cou, nullptr);
	lay_props.resize(layprop_cou);
	vkEnumerateInstanceLayerProperties(&layprop_cou, lay_props.data());

	int xc = 0;

	vector<char*> val_pon = {};

	for (size_t gh = 0; gh < pon.size(); gh++)
	{
		string point = pon[gh];
		for (size_t cf = 0; cf < layprop_cou; cf++)
		{
			string name = lay_props[cf].layerName;
			if (name == point) {
				val_pon.resize(xc + 1);
				val_pon[xc] = pon[gh];
				cout << "Found valid layer " << pon[gh] << endl;
				xc++;
				break;
			}
		}
	}

	if(debug)cout << endl;

	debugLayerAndExtensions();

	uint32_t exten_cout_used = 0;
	const char** exts = glfwGetRequiredInstanceExtensions(&exten_cout_used);

	VkInstanceCreateInfo InscreatInfo = {};
	InscreatInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InscreatInfo.pApplicationInfo = &appInfo;
	if (val_pon.size() > 0) {
		InscreatInfo.enabledLayerCount = val_pon.size();
		InscreatInfo.ppEnabledLayerNames = val_pon.data();
	}
	if (exten_cout_used > 0) {
		InscreatInfo.enabledExtensionCount = exten_cout_used;
		InscreatInfo.ppEnabledExtensionNames = exts;
	}

	handel("Creating Instance", vkCreateInstance(&InscreatInfo, nullptr, &instance));

	handel("Creating Surface", glfwCreateWindowSurface(instance,window,nullptr,&KHR));

	uint32_t div_cou = 0;
	handel("Get graphic card count", vkEnumeratePhysicalDevices(instance, &div_cou, nullptr));

	if (debug)cout << "Found " << div_cou << " graphic card(s)" << endl;

	vector<VkPhysicalDevice> graka_phy = {};
	graka_phy.resize(div_cou);

	handel("Get physical graphic card", vkEnumeratePhysicalDevices(instance, &div_cou, graka_phy.data()));

	debugOut(div_cou, graka_phy.data());


	VkPhysicalDevice c_div = graka_phy[0];

	VkSurfaceCapabilitiesKHR khrcaps;
	handel("Get Surface capabilities",vkGetPhysicalDeviceSurfaceCapabilitiesKHR(c_div, KHR, &khrcaps));

	if (debug) {
		cout << "Max Image Size supported " << khrcaps.maxImageExtent.width << "|" << khrcaps.maxImageExtent.height << endl << endl;
	}

	uint32_t suf_form_cout = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(c_div,KHR,&suf_form_cout,nullptr);
	
	vector<VkSurfaceFormatKHR> formats = {};
	formats.resize(suf_form_cout);
	vkGetPhysicalDeviceSurfaceFormatsKHR(c_div, KHR, &suf_form_cout, formats.data());

	if (debug) cout << "Formats Provided: " << endl;

	VkSurfaceFormatKHR form;
	bool validformat = false;

	for (size_t foc = 0; foc < suf_form_cout; foc++)
	{
		VkSurfaceFormatKHR c_form = formats[foc];
		if (c_form.format == PREFERRED_FORMAT) {
			validformat = true;
			form = c_form;
			break;
		}
		if (debug) {
			cout << endl << "Format: " << c_form.format << endl;
			cout << "Color Space " << c_form.colorSpace << endl;
		}
	}
	if (!validformat) {
		ERROR("Format not Valid", -2)
	}
	cout << endl;

	uint32_t Present_mode_count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(c_div,KHR,&Present_mode_count,nullptr);

	vector<VkPresentModeKHR> khr_present_mode = {};
	khr_present_mode.resize(Present_mode_count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(c_div,KHR,&Present_mode_count, khr_present_mode.data());

	bool ismodevalid = false;

	for (size_t prm = 0; prm < Present_mode_count; prm++)
	{
		VkPresentModeKHR cpm = khr_present_mode[prm];
		if (cpm == PRESENT_MODE_USED) {
			ismodevalid = true;
			if(!debug)break;
		}
		if(debug)cout << "Present Mode " << cpm << endl;
	}

	if (!ismodevalid) {
		ERROR("Present mode not available", -1)
	}

	VkSwapchainCreateInfoKHR swap_chain_creat_info = {};
	swap_chain_creat_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swap_chain_creat_info.surface = KHR;
	if (khrcaps.minImageCount > PREFERRED_MINIMAL_FRAME_COUNT) {
		swap_chain_creat_info.minImageCount = khrcaps.minImageCount;
	} else {
		swap_chain_creat_info.minImageCount = PREFERRED_MINIMAL_FRAME_COUNT;
	}
	actual_image_count = swap_chain_creat_info.minImageCount;
	swap_chain_creat_info.imageFormat = PREFERRED_FORMAT;
	swap_chain_creat_info.imageColorSpace = form.colorSpace;
	swap_chain_creat_info.imageExtent = WINDOW_SIZE;
	swap_chain_creat_info.imageSharingMode = PREFERRED_SHARING_MODE;
	swap_chain_creat_info.imageArrayLayers = 1;
	swap_chain_creat_info.imageUsage = IMAGE_USAGE;
	swap_chain_creat_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swap_chain_creat_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swap_chain_creat_info.presentMode = PRESENT_MODE_USED;
    swap_chain_creat_info.clipped = VK_NULL_HANDLE;

	VkPhysicalDeviceFeatures features = {};

	float arra[] = { 1.0F ,1.0F ,1.0F ,1.0F };

	VkDeviceQueueCreateInfo queue_create_info = {};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = 0;
	queue_create_info.queueCount = 1;
	queue_create_info.pQueuePriorities = arra;

	VkDeviceCreateInfo div_create_info = {};
	div_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	div_create_info.pQueueCreateInfos = &queue_create_info;
	div_create_info.queueCreateInfoCount = 1;
	div_create_info.pEnabledFeatures = &features;

	handel("Create Device", vkCreateDevice(c_div, &div_create_info, nullptr, &cdevice));

	if (debug)featuresPrint(features);

	VkQueue queue;
	vkGetDeviceQueue(cdevice, 0, 0, &queue);
}

void initFrame() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mon);
	if(debug)cout << mode->width << " - " << mode->height << endl;
	if (AUTO_SIZE) {
		WINDOW_SIZE = { static_cast<uint32_t>(mode->width),static_cast<uint32_t>(mode->height)};
	}
	window = glfwCreateWindow(WINDOW_SIZE.width, WINDOW_SIZE.height, FRAME_TITEL,nullptr,nullptr);
	glfwSetWindowPos(window, 0, 0);
}

int draw() {
	glfwPollEvents();
	if (glfwWindowShouldClose(window))return 1;

	return 0;
}

void shutdown(int i) {
	vkDeviceWaitIdle(cdevice);

	vkDestroyDevice(cdevice, nullptr);
	vkDestroySurfaceKHR(instance,KHR,nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	exit(i);
}

int main()
{
	initFrame();
	initVulkan();
	int xcv;
	while (true) {
		xcv = draw();
		if (xcv != 0) {
			break;
		}
	}
	shutdown(xcv);
	return 0;
}