#include "RHI/Vulkan/Public/VulkanPipeline.h"

#include "RHI/Vulkan/Public/VulkanDevice.h"
#include "RHI/Vulkan/Public/VulkanMapper.h"
#include "RHI/Vulkan/Public/VulkanShader.h"
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

VkPipeline createGraphicsPipeline(VkDevice logicalDevice, VkPipelineLayout pipelineLayout, const Nexora::PipelineSettings& settings)
{
    const auto colorFormat = Nexora::VulkanMapper::toVkFormat(settings.colorFormat);

    // describe state ->
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

    // create ->
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
        .layout              = pipelineLayout,
    };
    VkPipeline pipeline = VK_NULL_HANDLE;
    NX_VULKAN_CHECK(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));
    return pipeline;
}

VkPipeline createComputePipeline(VkDevice logicalDevice, VkPipelineLayout pipelineLayout, const Nexora::PipelineSettings& settings)
{
    const VkComputePipelineCreateInfo pipelineInfo{
        .sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        .stage  = toShaderStageInfo(*settings.computeShader),
        .layout = pipelineLayout,
    };
    VkPipeline pipeline = VK_NULL_HANDLE;
    NX_VULKAN_CHECK(vkCreateComputePipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));
    return pipeline;
}

}  // namespace

namespace Nexora
{

VulkanPipeline::VulkanPipeline(const VulkanDevice& device, const PipelineSettings& settings)
    : m_device(device),  //
      m_type(settings.type)
{
    const auto logicalDevice = m_device.logicalDevice();

    // create layout ->
    const VkPipelineLayoutCreateInfo layoutInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    };
    NX_VULKAN_CHECK(vkCreatePipelineLayout(logicalDevice, &layoutInfo, nullptr, &m_pipelineLayout));

    // create ->
    switch (m_type)
    {
        case PipelineType::Unknown: break;
        case PipelineType::Graphics: m_pipeline = createGraphicsPipeline(logicalDevice, m_pipelineLayout, settings); break;
        case PipelineType::Compute: m_pipeline = createComputePipeline(logicalDevice, m_pipelineLayout, settings); break;
    }
}

VulkanPipeline::~VulkanPipeline()
{
    const auto logicalDevice = m_device.logicalDevice();

    vkDestroyPipeline(logicalDevice, m_pipeline, nullptr);
    vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
}

PipelineType VulkanPipeline::type() const
{
    return m_type;
}

VkPipeline_T* VulkanPipeline::pipeline() const
{
    return m_pipeline;
}

}  // namespace Nexora
