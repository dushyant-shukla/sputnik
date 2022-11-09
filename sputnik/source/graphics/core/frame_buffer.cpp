#include "engine-x-pch.h"
#include "FrameBuffer.h"
#include "RendererAPI.h"
#include "platform/open-gl/OpenGLFrameBuffer.h"

namespace enginex
{
	Ref<FrameBuffer> FrameBuffer::Create(const FramebufferSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::NONE:
			{
				ENGINE_X_ASSERT(false, "RendererAPI::NONE is currently not supported!");
				return nullptr;
			};

			case RendererAPI::API::OPENGL:
				return CreateRef<OpenGLFrameBuffer>(specification);

			default:
			{
				ENGINE_X_ASSERT(false, "Unknown RenderAPI!");
				return nullptr;
			};
		}
	}
}