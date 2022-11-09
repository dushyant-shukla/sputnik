#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace enginex
{
	enum class FramebufferTextureFormat
	{
		NONE = 0,

		RGBA8, // color

		RED_INTEGER,

		DEPTH24_STENCIL8,

		DEPTH = DEPTH24_STENCIL8 // default
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat textureFormat) : mTextureFormat(textureFormat) {}

		FramebufferTextureFormat mTextureFormat = FramebufferTextureFormat::NONE;

		// TODO: filter/wrap configuration
	};

	struct FramebufferTextureAttachmentSpecification
	{
		FramebufferTextureAttachmentSpecification() = default;
		FramebufferTextureAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachmentSpecifications)
			: mAttachmentSpecifications(attachmentSpecifications) {}

		std::vector<FramebufferTextureSpecification> mAttachmentSpecifications;
	};

	struct FramebufferSpecification
	{
		uint32_t width;
		uint32_t height;
		uint32_t samples = 1;
		bool SwapChainTarget = false;

		FramebufferTextureAttachmentSpecification mAttachments;
	};

	class FrameBuffer
	{
	public:

		virtual  ~FrameBuffer() = default;
		static Ref<FrameBuffer> Create(const FramebufferSpecification& specification);

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearColorAttachment(uint32_t attachmentIndex, int value) = 0;
		virtual void ClearColorAttachment(uint32_t attachmentIndex, const glm::vec4& color) = 0;

		virtual FramebufferSpecification& GetSpecification() = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual uint32_t GetColorAttachmentRendererId(int index = 0) = 0;
		virtual uint32_t GetColorAttachmentRendererId(int index = 0) const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

	private:
	};
}