#pragma once
#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_CHECK(x)                                 \
  do {                                              \
    VkResult err = (x);                             \
    if (err) {                                      \
      std::cout << "Vulkan error: " << err << "\n"; \
      std::abort();                                 \
    }                                               \
  } while (0)

struct Backend {
  VkInstance instance{VK_NULL_HANDLE};
  VkSurfaceKHR surface{VK_NULL_HANDLE};
  VkPhysicalDevice physical_device{VK_NULL_HANDLE};
  VkDevice device{VK_NULL_HANDLE};
  VkQueue queue{VK_NULL_HANDLE};
  uint32_t queue_family{0};
  VmaAllocator allocator{VK_NULL_HANDLE};
};

struct Swapchain {
  VkSwapchainKHR handle{VK_NULL_HANDLE};
  std::vector<VkImage> images{};
  VkExtent2D extent{};
  VkFormat format{VK_FORMAT_UNDEFINED};
  VkColorSpaceKHR color_space{VK_COLORSPACE_SRGB_NONLINEAR_KHR};
};

struct ImageCreateInfo {
  VkExtent2D extent{};
  VkFormat format{VK_FORMAT_UNDEFINED};
  VkImageUsageFlags usage{};
  uint32_t mip_levels{1};
  uint32_t array_layers{1};
  VkSampleCountFlagBits samples{VK_SAMPLE_COUNT_1_BIT};
  VkImageType type{VK_IMAGE_TYPE_2D};
  VmaMemoryUsage memory_usage{VMA_MEMORY_USAGE_AUTO};
  bool dedicated{false};
};

struct Image {
  VkImage image{VK_NULL_HANDLE};
  VmaAllocation allocation{VK_NULL_HANDLE};
  VkFormat format{VK_FORMAT_UNDEFINED};
  VkExtent2D extent{};
  VkImageUsageFlags usage{};
};

struct Frames {
  VkCommandPool command_pool{VK_NULL_HANDLE};
  std::vector<VkCommandBuffer> command_buffers{}; // * fif
  std::vector<VkFence> fences{}; // * fif
  std::vector<VkSemaphore> image_acquired{}; // * fif
  std::vector<VkSemaphore> render_complete{}; // * image_count
  uint32_t frames_in_flight{0};
};

inline GLFWwindow* create_window(const int width, const int height, const char* title, const bool resizable = false)
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create window\n";
    std::abort();
  }
  return window;
}

inline void destroy_window(GLFWwindow* window)
{
  if (window) glfwDestroyWindow(window);
  glfwTerminate();
}

inline Backend create_backend(GLFWwindow* window, uint32_t api_version = VK_API_VERSION_1_3)
{
  Backend backend{};

  const VkApplicationInfo app_info{.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                   .pApplicationName = "Example",
                                   .pEngineName = "fwrk",
                                   .apiVersion = api_version};

  uint32_t ext_count = 0;
  const char** exts = glfwGetRequiredInstanceExtensions(&ext_count);

  const VkInstanceCreateInfo instance_ci{.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                         .pApplicationInfo = &app_info,
                                         .enabledExtensionCount = ext_count,
                                         .ppEnabledExtensionNames = exts};
  VK_CHECK(vkCreateInstance(&instance_ci, nullptr, &backend.instance));

  glfwCreateWindowSurface(backend.instance, window, nullptr, &backend.surface);
  if (!backend.surface) {
    std::cout << "Failed to create surface\n";
    std::abort();
  }

  uint32_t device_count = 0;
  VK_CHECK(vkEnumeratePhysicalDevices(backend.instance, &device_count, nullptr));
  std::vector<VkPhysicalDevice> devices(device_count);
  VK_CHECK(vkEnumeratePhysicalDevices(backend.instance, &device_count, devices.data()));
  backend.physical_device = devices[0];

  VkPhysicalDeviceProperties2 props{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
  vkGetPhysicalDeviceProperties2(backend.physical_device, &props);
  std::cout << "Selected device: " << props.properties.deviceName << "\n";

  uint32_t qf_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(backend.physical_device, &qf_count, nullptr);
  std::vector<VkQueueFamilyProperties> qfs(qf_count);
  vkGetPhysicalDeviceQueueFamilyProperties(backend.physical_device, &qf_count, qfs.data());
  for (uint32_t i = 0; i < qfs.size(); i++) {
    if (qfs[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      backend.queue_family = i;
      break;
    }
  }
  if (glfwGetPhysicalDevicePresentationSupport(backend.instance, backend.physical_device, backend.queue_family) !=
      GLFW_TRUE) {
    std::cout << "Graphics queue family has no presentation support\n";
    std::abort();
  }

  constexpr float priority = 1.0f;
  VkDeviceQueueCreateInfo queue_ci{.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                   .queueFamilyIndex = backend.queue_family,
                                   .queueCount = 1,
                                   .pQueuePriorities = &priority};

  const std::vector device_exts{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  VkPhysicalDeviceVulkan12Features vk12{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
  vk12.descriptorIndexing = VK_TRUE;
  vk12.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
  vk12.descriptorBindingVariableDescriptorCount = VK_TRUE;
  vk12.runtimeDescriptorArray = VK_TRUE;
  vk12.bufferDeviceAddress = VK_TRUE;

  VkPhysicalDeviceVulkan13Features vk13{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
  vk13.pNext = &vk12;
  vk13.synchronization2 = VK_TRUE;
  vk13.dynamicRendering = VK_TRUE;

  VkPhysicalDeviceFeatures vk10{};
  vk10.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo device_ci{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                               .pNext = &vk13,
                               .queueCreateInfoCount = 1,
                               .pQueueCreateInfos = &queue_ci,
                               .enabledExtensionCount = static_cast<uint32_t>(device_exts.size()),
                               .ppEnabledExtensionNames = device_exts.data(),
                               .pEnabledFeatures = &vk10};
  VK_CHECK(vkCreateDevice(backend.physical_device, &device_ci, nullptr, &backend.device));

  vkGetDeviceQueue(backend.device, backend.queue_family, 0, &backend.queue);
  if (!backend.queue) {
    std::cout << "Failed to get queue\n";
    std::abort();
  }

  VmaVulkanFunctions vk_fns{};
  vk_fns.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
  vk_fns.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

  VmaAllocatorCreateInfo alloc_ci{.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
                                  .physicalDevice = backend.physical_device,
                                  .device = backend.device,
                                  .pVulkanFunctions = &vk_fns,
                                  .instance = backend.instance,
                                  .vulkanApiVersion = api_version};
  VK_CHECK(vmaCreateAllocator(&alloc_ci, &backend.allocator));

  return backend;
}


inline void destroy_backend(Backend& backend)
{
  vmaDestroyAllocator(backend.allocator);
  vkDestroyDevice(backend.device, nullptr);
  vkDestroySurfaceKHR(backend.instance, backend.surface, nullptr);
  vkDestroyInstance(backend.instance, nullptr);
  backend = {};
}

inline Swapchain create_swapchain(const Backend& backend, const VkExtent2D fallback_extent,
                                  const VkFormat format = VK_FORMAT_B8G8R8A8_SRGB,
                                  const VkColorSpaceKHR color_space = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
                                  VkSwapchainKHR old = VK_NULL_HANDLE)
{
  Swapchain swapchain{};
  swapchain.format = format;
  swapchain.color_space = color_space;

  VkSurfaceCapabilitiesKHR caps{};
  VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(backend.physical_device, backend.surface, &caps));

  swapchain.extent = caps.currentExtent;
  if (caps.currentExtent.width == 0xFFFFFFFF) swapchain.extent = fallback_extent;

  const VkSwapchainCreateInfoKHR ci{.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                    .surface = backend.surface,
                                    .minImageCount = caps.minImageCount,
                                    .imageFormat = swapchain.format,
                                    .imageColorSpace = swapchain.color_space,
                                    .imageExtent = swapchain.extent,
                                    .imageArrayLayers = 1,
                                    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                                    .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                                    .presentMode = VK_PRESENT_MODE_FIFO_KHR,
                                    .clipped = VK_TRUE,
                                    .oldSwapchain = old};
  VK_CHECK(vkCreateSwapchainKHR(backend.device, &ci, nullptr, &swapchain.handle));

  uint32_t image_count = 0;
  VK_CHECK(vkGetSwapchainImagesKHR(backend.device, swapchain.handle, &image_count, nullptr));
  swapchain.images.resize(image_count);
  VK_CHECK(vkGetSwapchainImagesKHR(backend.device, swapchain.handle, &image_count, swapchain.images.data()));

  return swapchain;
}

inline void destroy_swapchain(const Backend& backend, Swapchain& swap_chain)
{
  vkDestroySwapchainKHR(backend.device, swap_chain.handle, nullptr);
  swap_chain = {};
}

inline Image create_image(const Backend& backend, const ImageCreateInfo& info)
{
  Image img{};
  img.format = info.format;
  img.extent = info.extent;
  img.usage = info.usage;

  const VkImageCreateInfo ci{.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                             .imageType = info.type,
                             .format = info.format,
                             .extent = {info.extent.width, info.extent.height, 1},
                             .mipLevels = info.mip_levels,
                             .arrayLayers = info.array_layers,
                             .samples = info.samples,
                             .tiling = VK_IMAGE_TILING_OPTIMAL,
                             .usage = info.usage,
                             .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                             .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED};

  VmaAllocationCreateInfo alloc_ci{};
  alloc_ci.usage = info.memory_usage;
  if (info.dedicated) alloc_ci.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

  VK_CHECK(vmaCreateImage(backend.allocator, &ci, &alloc_ci, &img.image, &img.allocation, nullptr));
  return img;
}

inline void destroy_image(const Backend& backend, Image& img)
{
  vmaDestroyImage(backend.allocator, img.image, img.allocation);
  img = {};
}

inline Frames create_frames(const Backend& backend, const uint32_t frames_in_flight, const uint32_t image_count)
{
  Frames frames{};
  frames.frames_in_flight = frames_in_flight;
  frames.fences.resize(frames_in_flight);
  frames.image_acquired.resize(frames_in_flight);
  frames.render_complete.resize(image_count);
  frames.command_buffers.resize(frames_in_flight);

  constexpr VkSemaphoreCreateInfo sem_ci{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
  constexpr VkFenceCreateInfo fence_ci{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                                       .flags = VK_FENCE_CREATE_SIGNALED_BIT};

  for (uint32_t i = 0; i < frames_in_flight; i++) {
    VK_CHECK(vkCreateFence(backend.device, &fence_ci, nullptr, &frames.fences[i]));
    VK_CHECK(vkCreateSemaphore(backend.device, &sem_ci, nullptr, &frames.image_acquired[i]));
  }
  for (auto& sem: frames.render_complete) {
    VK_CHECK(vkCreateSemaphore(backend.device, &sem_ci, nullptr, &sem));
  }

  const VkCommandPoolCreateInfo pool_ci{.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                        .queueFamilyIndex = backend.queue_family};
  VK_CHECK(vkCreateCommandPool(backend.device, &pool_ci, nullptr, &frames.command_pool));

  const VkCommandBufferAllocateInfo cb_ai{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                                          .commandPool = frames.command_pool,
                                          .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                          .commandBufferCount = frames_in_flight};
  VK_CHECK(vkAllocateCommandBuffers(backend.device, &cb_ai, frames.command_buffers.data()));

  return frames;
}

inline void resize_frames(const Backend& backend, Frames& frames, const uint32_t image_count)
{
  for (auto& sem: frames.render_complete) vkDestroySemaphore(backend.device, sem, nullptr);
  frames.render_complete.resize(image_count);
  constexpr VkSemaphoreCreateInfo sem_ci{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
  for (auto& sem: frames.render_complete) VK_CHECK(vkCreateSemaphore(backend.device, &sem_ci, nullptr, &sem));
}

inline void destroy_frames(const Backend& backend, Frames& frames)
{
  vkDestroyCommandPool(backend.device, frames.command_pool, nullptr);
  for (auto& f: frames.fences) vkDestroyFence(backend.device, f, nullptr);
  for (auto& sem: frames.image_acquired) vkDestroySemaphore(backend.device, sem, nullptr);
  for (auto& sem: frames.render_complete) vkDestroySemaphore(backend.device, sem, nullptr);
  frames = {};
}

inline std::vector<uint32_t> read_spv(const char* path)
{
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) return {};
  const std::streamsize size = file.tellg();
  if (size % 4 != 0) return {};
  std::vector<uint32_t> code(static_cast<size_t>(size) / 4);
  file.seekg(0);
  file.read(reinterpret_cast<char*>(code.data()), size);
  return code;
}

inline VkShaderModule load_shader(VkDevice device, const char* path)
{
  auto code = read_spv(path);
  const VkShaderModuleCreateInfo ci{.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                                    .codeSize = code.size() * sizeof(uint32_t),
                                    .pCode = code.data()};
  VkShaderModule module = VK_NULL_HANDLE;
  VK_CHECK(vkCreateShaderModule(device, &ci, nullptr, &module));
  return module;
}

inline VkPipeline create_default_pipeline(VkDevice device, VkPipelineLayout layout, VkShaderModule vert,
                                          VkShaderModule frag, const VkFormat color_format, const VkFormat depth_format)
{
  std::array stages{VkPipelineShaderStageCreateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                                                    .stage = VK_SHADER_STAGE_VERTEX_BIT,
                                                    .module = vert,
                                                    .pName = "main"},
                    VkPipelineShaderStageCreateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                                                    .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                                                    .module = frag,
                                                    .pName = "main"}};

  VkPipelineVertexInputStateCreateInfo vertex_input{.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
  VkPipelineInputAssemblyStateCreateInfo input_assembly{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
  VkPipelineViewportStateCreateInfo viewport{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, .viewportCount = 1, .scissorCount = 1};
  std::array dynamic_states{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamic{.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                                           .dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
                                           .pDynamicStates = dynamic_states.data()};
  VkPipelineDepthStencilStateCreateInfo depth_stencil{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
      .depthTestEnable = VK_TRUE,
      .depthWriteEnable = VK_TRUE,
      .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL};
  VkPipelineRenderingCreateInfo rendering{.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
                                          .colorAttachmentCount = 1,
                                          .pColorAttachmentFormats = &color_format,
                                          .depthAttachmentFormat = depth_format};
  VkPipelineColorBlendAttachmentState blend_att{.colorWriteMask = 0xF};
  VkPipelineColorBlendStateCreateInfo color_blend{.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                                                  .attachmentCount = 1,
                                                  .pAttachments = &blend_att};
  VkPipelineRasterizationStateCreateInfo raster{.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                                                .lineWidth = 1.0f};
  VkPipelineMultisampleStateCreateInfo multisample{.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                                                   .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT};

  VkGraphicsPipelineCreateInfo ci{.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                                  .pNext = &rendering,
                                  .stageCount = static_cast<uint32_t>(stages.size()),
                                  .pStages = stages.data(),
                                  .pVertexInputState = &vertex_input,
                                  .pInputAssemblyState = &input_assembly,
                                  .pViewportState = &viewport,
                                  .pRasterizationState = &raster,
                                  .pMultisampleState = &multisample,
                                  .pDepthStencilState = &depth_stencil,
                                  .pColorBlendState = &color_blend,
                                  .pDynamicState = &dynamic,
                                  .layout = layout};
  VkPipeline pipeline = VK_NULL_HANDLE;
  VK_CHECK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &ci, nullptr, &pipeline));
  return pipeline;
}

inline VkPipelineLayout create_pipeline_layout(VkDevice device)
{
  constexpr VkPipelineLayoutCreateInfo ci{.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
  VkPipelineLayout layout = VK_NULL_HANDLE;
  VK_CHECK(vkCreatePipelineLayout(device, &ci, nullptr, &layout));
  return layout;
}
