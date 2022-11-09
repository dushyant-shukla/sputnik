#include "engine-x-pch.h"
#include "Texture.h"
#include "Renderer.h"
#include "platform/open-gl/OpenGLTexture.h"

namespace enginex
{
	Ref<Texture2D> Texture2D::Create(const std::string path)
	{
		switch (Renderer::GetActiveAPI())
		{
		case RendererAPI::API::NONE:
		{
			ENGINE_X_ASSERT(false, "RendererAPI::NONE is currently not supported!");
			return nullptr;
		};

		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLTexture2D>(path);

		default:
		{
			ENGINE_X_ASSERT(false, "Unknown RenderAPI!");
			return nullptr;
		};
		}
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetActiveAPI())
		{
		case RendererAPI::API::NONE:
		{
			ENGINE_X_ASSERT(false, "RendererAPI::NONE is currently not supported!");
			return nullptr;
		};

		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLTexture2D>(width, height);

		default:
		{
			ENGINE_X_ASSERT(false, "Unknown RenderAPI!");
			return nullptr;
		};
		}
	}
}