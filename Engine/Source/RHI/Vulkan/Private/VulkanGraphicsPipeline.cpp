#include "RHI/Vulkan/Public/VulkanGraphicsPipeline.h"

#include "RHI/Vulkan/Public/VulkanDevice.h"
#include "RHI/Vulkan/Public/VulkanShader.h"
#include "RHI/Vulkan/Public/VulkanMapper.h"
#include "RHI/Vulkan/Public/VulkanUtility.h"

namespace
{

VkPipelineShaderStageCreateInfo toShaderStageInfo(const Nexora::IRHIShader& shader)
{
    const auto& vulkanShader = static_cast<const Nexora::VulkanShader&>(shader);
    return {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = Nexora::VulkanMapper::toVkShaderStage(vulkanShader.stage()),
        .module = vulkanShader.handle(),
        .pName  = "main",
    };
}

}  // namespace

namespace Nexora
{

VulkanGraphicsPipeline::VulkanGraphicsPipeline(const VulkanDevice& device, const GraphicsPipelineSettings& settings) : m_device(device)
{
    const auto logicalDevice = m_device.logicalDevice();
    const auto colorFormat   = VulkanMapper::toVkFormat(settings.colorFormat);

    // create layout ->
    const VkPipelineLayoutCreateInfo layoutInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    };
    NX_VULKAN_CHECK(vkCreatePipelineLayout(logicalDevice, &layoutInfo, nullptr, &m_pipelineLayout));

    // create ->
    const VkPipelineShaderStageCreateInfo shaderStages[] = {
        toShaderStageInfo(*settings.vertexShader),
        toShaderStageInfo(*settings.fragmentShader),
    };
    const VkPipelineVertexInputStateCreateInfo vertexInputInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    };
    const VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
        .sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    };
    const VkPipelineViewportStateCreateInfo viewportInfo{
        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount  = 1,
    };
    const VkPipelineRasterizationStateCreateInfo rasterizationInfo{
        .sType       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode    = VK_CULL_MODE_NONE,
        .frontFace   = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .lineWidth   = 1.0f,
    };
    const VkPipelineMultisampleStateCreateInfo multisampleInfo{
        .sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    };
    const VkPipelineColorBlendAttachmentState colorBlendAttachment{
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };
    const VkPipelineColorBlendStateCreateInfo colorBlendInfo{
        .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments    = &colorBlendAttachment,
    };
    const VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };
    const VkPipelineDynamicStateCreateInfo dynamicInfo{
        .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates    = dynamicStates,
    };
    const VkPipelineRenderingCreateInfo renderingInfo{
        .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .colorAttachmentCount    = 1,
        .pColorAttachmentFormats = &colorFormat,
    };
    const VkGraphicsPipelineCreateInfo pipelineInfo{
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext               = &renderingInfo,
        .stageCount          = 2,
        .pStages             = shaderStages,
        .pVertexInputState   = &vertexInputInfo,
        .pInputAssemblyState = &inputAssemblyInfo,
        .pViewportState      = &viewportInfo,
        .pRasterizationState = &rasterizationInfo,
        .pMultisampleState   = &multisampleInfo,
        .pColorBlendState    = &colorBlendInfo,
        .pDynamicState       = &dynamicInfo,
        .layout              = m_pipelineLayout,
    };
    NX_VULKAN_CHECK(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
    const auto logicalDevice = m_device.logicalDevice();

    vkDestroyPipeline(logicalDevice, m_pipeline, nullptr);
    vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
}

VkPipeline_T* VulkanGraphicsPipeline::pipeline() const
{
    return m_pipeline;
}

}  // namespace Nexora
