#ifndef EMULATOR_INCLUDE_EMULATOR_GRAPHICS_OBJECTS_RENDERTEXTURE_H_
#define EMULATOR_INCLUDE_EMULATOR_GRAPHICS_OBJECTS_RENDERTEXTURE_H_

#include "Kyty/Core/Common.h"

#include "Emulator/Common.h"
#include "Emulator/Graphics/Objects/GpuMemory.h"

#ifdef KYTY_EMU_ENABLED

namespace Kyty::Libs::Graphics {

struct GraphicContext;
struct VulkanMemory;

enum class RenderTextureFormat : uint64_t
{
	R8G8B8A8Unorm
};

class RenderTextureObject: public GpuObject
{
public:
	static constexpr int PARAM_FORMAT = 0;
	static constexpr int PARAM_WIDTH  = 1;
	static constexpr int PARAM_HEIGHT = 2;
	static constexpr int PARAM_TILED  = 3;
	static constexpr int PARAM_NEO    = 4;
	static constexpr int PARAM_PITCH  = 5;

	explicit RenderTextureObject(RenderTextureFormat pixel_format, uint32_t width, uint32_t height, bool tiled, bool neo, uint32_t pitch)
	{
		params[PARAM_FORMAT] = static_cast<uint64_t>(pixel_format);
		params[PARAM_WIDTH]  = width;
		params[PARAM_HEIGHT] = height;
		params[PARAM_TILED]  = tiled ? 1 : 0;
		params[PARAM_NEO]    = neo ? 1 : 0;
		params[PARAM_PITCH]  = pitch;
		check_hash           = true;
		type                 = Graphics::GpuMemoryObjectType::RenderTexture;
	}

	void* Create(GraphicContext* ctx, const uint64_t* vaddr, const uint64_t* size, int vaddr_num, VulkanMemory* mem) const override;
	bool  Equal(const uint64_t* other) const override;

	[[nodiscard]] write_back_func_t GetWriteBackFunc() const override { return nullptr; };
	[[nodiscard]] delete_func_t     GetDeleteFunc() const override;
	[[nodiscard]] update_func_t     GetUpdateFunc() const override;
};

} // namespace Kyty::Libs::Graphics

#endif // KYTY_EMU_ENABLED

#endif /* EMULATOR_INCLUDE_EMULATOR_GRAPHICS_OBJECTS_RENDERTEXTURE_H_ */
