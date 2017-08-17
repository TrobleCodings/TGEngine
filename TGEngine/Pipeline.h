#pragma once

#include "stdbase.h"
#include "Window.h"
#include "RenderPass.h"
#include "Swapchain.h"
#include "Vertex.h"
#include "VertexBuffer.h"

namespace Pipeline {

	struct Pipe
	{
		std::vector<VkPipelineShaderStageCreateInfo>* shader;
		std::vector<VkFramebuffer>* frame_buffer;
		std::vector<VkCommandBuffer>* command_buffer;
		VkCommandPool* command_pool;
		Window* window;
		VkPipeline* pipeline;
		Device* device;
		RenderPass* render_pass;
		VkSemaphore* available;
		VkSemaphore* end;
		Swapchain* swapchain;
		VertexBuffer* buffer;
	};

	/*
	 * Creates a Pipeline
	 */
	void createPipeline(Pipe* pipeline);

	/*
	 * Destroys the Pipeline
	 */
	void destroyPipeline(Pipe* pipeline);
}