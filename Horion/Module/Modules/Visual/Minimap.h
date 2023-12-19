#pragma once

#include "../../../../SDK/MinecraftUIRenderContext.h"
#include "../Module.h"
class Minimap : public IModule {
	int delay = 20;
	int yLevel = 320;

public:
	bool chunkBorders = true;
	bool players = true;

	static constexpr int map_size = 128;                 // 128 by 128 map
	static constexpr int map_max = map_size * map_size;  // map size squared

	struct BlockContainer {
		Vec3i worldPos;
		bool exists;
		float color[4];

		BlockContainer() {
			memset(this, 0, sizeof(BlockContainer));
		}

		BlockContainer(float* color) {
			exists = true;
			this->color[0] = color[0];
			this->color[1] = color[1];
			this->color[2] = color[2];
			this->color[3] = color[3];
		}
	};

	std::array<BlockContainer, map_max> blocks;

	Minimap();
	~Minimap(){};

	const char* getModuleName() override { return "Minimap"; }

	void doMinimap();

	void onTick(GameMode* gm) override;
	void onPreRender(MinecraftUIRenderContext* ctx) override;
};
