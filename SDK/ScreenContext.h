#pragma once
#include "Tessellator.h"
#include "../Utils/Utils.h"

class ItemRenderer;
class ClientInstance;
class MinecraftGame;
class ItemInHandRenderer;
class ActorBlockRenderer;
class ActorRenderDispatcher;
class BlockActorRenderDispatcher;
class ParticleSystemEngine;
class Tessellator;

class ScreenContext {
public:
	inline float* getColorHolder() {
		return *reinterpret_cast<float**>((uintptr_t)(this) + 0x30);
	}

	inline Tessellator* getTessellator() {
		return *reinterpret_cast<Tessellator**>((uintptr_t)(this) + 0xC0);
	}
};

class BaseActorRenderContext {
private:
	char pad_0x0[0x18];  // 0x00
public:
	ClientInstance* clientInstance;  // 0x18
	MinecraftGame* minecraftGame;  // 0x20
	ScreenContext* screenContext;  // 0x28
	BlockActorRenderDispatcher* blockActorRenderDispatcher;  // 0x30
	Mc_SharedPtr<ActorRenderDispatcher> actorRenderDispatcher;  // 0x38
	ActorBlockRenderer* actorBlockRenderer;  // 0x48
	ItemInHandRenderer* itemInHandRenderer; // 0x50
	ItemRenderer* itemRenderer;  // 0x58
	ParticleSystemEngine* particleSystemEngine;  // 0x60

private:
	char pad_0x68[0x238];  // 0x68
public:
	BaseActorRenderContext(ScreenContext* ScreenCtx, ClientInstance* client, MinecraftGame* game);
};

class ItemRenderer {
public:
	void renderGuiItemNew(BaseActorRenderContext* BaseActorRenderCtx, ItemStack* item, int mode, float x, float y, float opacity, float scale, bool isEnchanted);
	void renderGuiItemInChunk(BaseActorRenderContext* BaseActorRenderCtx, ItemStack* item, int mode, float x, float y, float opacity, float scale, bool isEnchanted);
};