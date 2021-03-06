#include "../../public/pipeline/Framebuffer.hpp"
#include "../../public/pipeline/window/Window.hpp"
#include "../../public/pipeline/RenderPass.hpp"
#include "../../public/pipeline/Swapchain.hpp"
#include "../../public/pipeline/PrePipeline.hpp"

VkFramebuffer* frameBuffer;
VkImageView* imageView;

void createFramebuffer() {
	frameBuffer = new VkFramebuffer[imageCount];
	imageView = new VkImageView[imageCount];

	VkFramebufferCreateInfo frameBufferCreateInfo;
	frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.pNext = nullptr;
	frameBufferCreateInfo.flags = 0;
	frameBufferCreateInfo.renderPass = renderPass;
	frameBufferCreateInfo.attachmentCount = 3;
	frameBufferCreateInfo.pAttachments = VK_NULL_HANDLE;
	frameBufferCreateInfo.width = tge::win::mainWindowWidth;
	frameBufferCreateInfo.height = tge::win::mainWindowHeight;
	frameBufferCreateInfo.layers = 1;

	VkImageView views[3];
	views[0] = colorImageView;
	views[1] = depthImageView;

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = VK_NULL_HANDLE;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = usedSurfaceFormat.format;
	// All channels are their identity, therefore R = R, G = G, B = B, A = A => for color channel mapping
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	for (size_t i = 0; i < imageCount; i++) {
		imageViewCreateInfo.image = swapchainImages[i];
		CHECKFAIL(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &imageView[i]));

		views[2] = imageView[i];
		frameBufferCreateInfo.pAttachments = views;

		CHECKFAIL(vkCreateFramebuffer(device, &frameBufferCreateInfo, nullptr, &frameBuffer[i]));
	}
}

void destroyFrameBuffer() {
	for (size_t i = 0; i < imageCount; i++) {
		vkDestroyFramebuffer(device, frameBuffer[i], nullptr);
		vkDestroyImageView(device, imageView[i], nullptr);
	}
}
