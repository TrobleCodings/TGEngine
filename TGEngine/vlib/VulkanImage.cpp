#include "VulkanImage.hpp"

VkBufferImageCopy vlib_buffer_image_copy = {
	0,
	0,
	0,
	{
		VK_IMAGE_ASPECT_COLOR_BIT,
		0,
		0,
		1
	},
	{
		0,
		0,
		0
	},
	{
		0,
		0,
		1
	}
};

VkImageCreateInfo vlib_image_create_info = {
	VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
	nullptr,
	0,
	VK_IMAGE_TYPE_2D,
	VK_FORMAT_UNDEFINED,
	{
		0,
		0,
		1
	},
	1,
	1,
	VK_SAMPLE_COUNT_1_BIT,
	VK_IMAGE_TILING_OPTIMAL,
	VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
	VK_SHARING_MODE_EXCLUSIVE,
	0,
	nullptr,
	VK_IMAGE_LAYOUT_UNDEFINED
};

VkImageViewCreateInfo vlib_image_view_create_info = {
	VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
	nullptr,
	0,
	VK_NULL_HANDLE,
	VK_IMAGE_VIEW_TYPE_2D,
	VK_FORMAT_UNDEFINED,
	{
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY
	},
	{
		VK_IMAGE_ASPECT_COLOR_BIT,
		0,
		1,
		0,
		1
	}
};

VkImageMemoryBarrier vlib_image_memory_barrier = {
	VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
	nullptr,
	0,
	0,
	VK_IMAGE_LAYOUT_UNDEFINED,
	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	VK_QUEUE_FAMILY_IGNORED,
	VK_QUEUE_FAMILY_IGNORED,
	VK_NULL_HANDLE,
	{
		VK_IMAGE_ASPECT_COLOR_BIT,
		0,
		1,
		0,
		1
	}
};

VkImageBlit vlib_image_blit = {
	{
	    VK_IMAGE_ASPECT_COLOR_BIT,
	    0,
	    0,
	    1
	},
	{ {0, 0, 0}, {0, 0, 1} },
	{
	    VK_IMAGE_ASPECT_COLOR_BIT,
	    0,
	    0,
	    1
	},
	{ {0, 0, 0}, { 0, 0, 1 }}
};