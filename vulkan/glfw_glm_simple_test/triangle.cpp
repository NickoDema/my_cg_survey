#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// #define NO_DEBUG

#include <memory>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>


#ifdef NO_DEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};


class QueueFamily {
public:
    inline void     setFamilyIdx(uint32_t idx) { familyIdx = idx; }
    inline uint32_t getFamilyIdx()             { return familyIdx; }

    inline void setGraphicsAvailable()     { graphicsAvailable = true; }
    inline void setComputationsAvailable() { computationsAvailable = true; }
    inline void setTransferAvailable()     { transferAvailable = true; }
    inline void setSparseMemoryAvailable() { sparseMemoryAvailable = true; }

    inline bool isGraphicsAvailable()     const { return graphicsAvailable; }
    inline bool isComputationsAvailable() const { return computationsAvailable; }
    inline bool isTransferAvailable()     const { return transferAvailable; }
    inline bool isSparseMemoryAvailable() const { return sparseMemoryAvailable; }

    uint32_t familyIdx;
    VkQueueFamilyProperties properties;

    bool graphicsAvailable = false;
    bool computationsAvailable = false;
    bool transferAvailable = false;
    bool sparseMemoryAvailable = false;

};


class TriangleApp {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    QueueFamily graphicsQueueFamily;
    VkDevice device;
    VkQueue graphicsQueue;

    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkaned", nullptr, nullptr);
    }


    void initVulkan() {
        createInstance();
        // setupDebugMessenger();
        pickPhysicalDevice();
        createLogicalDevice();
    }


    void createInstance() {

        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Triangle App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Kolyan Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        VkResult instanceCreationResult;
        instanceCreationResult = vkCreateInstance(&createInfo, nullptr, &instance);

        if (instanceCreationResult == VK_ERROR_INCOMPATIBLE_DRIVER) {
            throw std::runtime_error("cannot find a compatible Vulkan driver\n"
                                     "failed to create instance!");
        } else if (instanceCreationResult != VK_SUCCESS) {
            throw std::runtime_error("unknown error\n"
                                     "failed to create instance!");
        }

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        std::cout << "available extensions:" << std::endl;

        for (const auto& extension : extensions) {
            std::cout << "\t" << extension.extensionName << std::endl;
        }

    }


    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        std::cout << "available validation layers:" << std::endl;

        for (const auto& layer : availableLayers) {
            std::cout << "\t" << layer.layerName << std::endl;
        }

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }


    void pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        std::cout << "available physical devices:" << std::endl;

        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices) {
            int score = rateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0) {
            physicalDevice = candidates.rbegin()->second;
        } else {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }


    int rateDeviceSuitability(VkPhysicalDevice device) {

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        int score = 0;

        std::cout << "\t" << deviceProperties.deviceName << std::endl;

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 10000;
            std::cout << "\t\t" << "type: discrete" << std::endl;
        }

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            score += 5000;
            std::cout << "\t\t" << "type: integrated" << std::endl;
        }

        score += deviceProperties.limits.maxImageDimension2D;
        std::cout << "\t\t" << "max image dimention (2D): " << deviceProperties.limits.maxImageDimension2D << std::endl;

        if (!deviceFeatures.geometryShader) {
            std::cout << "\t\t" << "geometry shader: no" << std::endl;
            return 0;
        } else {
            std::cout << "\t\t" << "geometry shader: yes" << std::endl;
        }

        if (std::shared_ptr<QueueFamily> requiredQueueFamily = findQueueFamilyByOperationType(device, VK_QUEUE_GRAPHICS_BIT)) {
            graphicsQueueFamily = *requiredQueueFamily;
        } else {
            score =  0;
        }

        std::cout << "\t\t" << "total score: " << score << std::endl;
        return score;
    }


    std::shared_ptr<QueueFamily> findQueueFamilyByOperationType (VkPhysicalDevice device, VkQueueFlagBits queueType) {

        std::vector<std::shared_ptr<QueueFamily>> queueFamilies = findQueueFamilies(device);

        // std::shared_ptr<QueueFamily> resultQueueFamily;

        for (const auto queueFamily : queueFamilies) {

            if (queueFamily->properties.queueFlags & queueType) {
                return queueFamily;
                }
        }

        return nullptr;
    }


    std::vector<std::shared_ptr<QueueFamily>> findQueueFamilies (VkPhysicalDevice device) {

        std::vector<std::shared_ptr<QueueFamily>> queueFamilies;

        uint32_t queueFamiliesCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamiliesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, queueFamiliesProperties.data());

        std::cout << "\t\t" << "supported operations: " << std::endl;

        int i = 0;
        for (const auto& queueFamilyProperties : queueFamiliesProperties) {

            QueueFamily queueFamily;
            queueFamily.setFamilyIdx(i);

            if (queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                std::cout << "\t\t\t" << "queue family id: " << i << " graphics" << std::endl;
                queueFamily.setGraphicsAvailable();
            }

            if (queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                std::cout << "\t\t\t" << "queue family id: " << i << " computation" << std::endl;
                queueFamily.setComputationsAvailable();
            }

            if (queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                std::cout << "\t\t\t" << "queue family id: " << i << " transfer" << std::endl;
                queueFamily.setTransferAvailable();
            }

            if (queueFamilyProperties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
                std::cout << "\t\t\t" << "queue family id: " << i << " sparse memory management" << std::endl;
                queueFamily.setSparseMemoryAvailable();
            }
            queueFamily.properties = queueFamilyProperties;
            queueFamilies.push_back(std::make_shared<QueueFamily>(queueFamily));
            i++;
        }

        return queueFamilies;
    }


    void createLogicalDevice() {

        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = graphicsQueueFamily.getFamilyIdx();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = 0;

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(device, graphicsQueueFamily.getFamilyIdx(), 0, &graphicsQueue);
    }


    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }


    void cleanup() {
        vkDestroyDevice(device, nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};


int main() {
    TriangleApp app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
