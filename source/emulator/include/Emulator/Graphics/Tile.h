#ifndef EMULATOR_INCLUDE_EMULATOR_GRAPHICS_TILE_H_
#define EMULATOR_INCLUDE_EMULATOR_GRAPHICS_TILE_H_

#include "Kyty/Core/Common.h"

#include "Emulator/Common.h"

#ifdef KYTY_EMU_ENABLED

namespace Kyty::Libs::Graphics {

enum class TileMode
{
	VideoOutLinear,
	VideoOutTiled,
	TextureLinear,
	TextureTiled,
	// RenderTextureLinear,
	// RenderTextureTiled,
};

struct TileSizeAlign
{
	uint32_t size  = 0;
	uint32_t align = 0;
};

void TileInit();
void TileConvertTiledToLinear(void* dst, const void* src, TileMode mode, uint32_t width, uint32_t height, bool neo);
void TileConvertTiledToLinear(void* dst, const void* src, TileMode mode, uint32_t dfmt, uint32_t nfmt, uint32_t width, uint32_t height,
                              uint32_t pitch, uint32_t levels, bool neo);

bool TileGetDepthSize(uint32_t width, uint32_t height, uint32_t pitch, uint32_t z_format, uint32_t stencil_format, bool htile, bool neo,
                      TileSizeAlign* stencil_size, TileSizeAlign* htile_size, TileSizeAlign* depth_size);
void TileGetVideoOutSize(uint32_t width, uint32_t height, uint32_t pitch, bool tile, bool neo, TileSizeAlign* size);
void TileGetTextureSize(uint32_t dfmt, uint32_t nfmt, uint32_t width, uint32_t height, uint32_t pitch, uint32_t levels, uint32_t tile,
                        bool neo, TileSizeAlign* total_size, uint32_t* level_sizes, uint32_t* padded_width, uint32_t* padded_height);

} // namespace Kyty::Libs::Graphics

#endif

#endif /* EMULATOR_INCLUDE_EMULATOR_GRAPHICS_TILE_H_ */
