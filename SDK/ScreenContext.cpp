#include "ScreenContext.h"

BaseActorRenderContext::BaseActorRenderContext(ScreenContext *ScreenCtx, ClientInstance *client, MinecraftGame *game) {
	memset(this, 0, sizeof(BaseActorRenderContext));
	using BaseActorRenderContext_t = __int64(__fastcall *)(BaseActorRenderContext *, ScreenContext *, ClientInstance *, MinecraftGame *);
	static BaseActorRenderContext_t BaseActorRenderContext_constructor = reinterpret_cast<BaseActorRenderContext_t>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 4C 24 ? 57 48 83 EC ? 49 8B F8 48 8B DA 48 8B F1 48 8D 05 ? ? ? ? 48 89 01"));
	BaseActorRenderContext_constructor(this, ScreenCtx, client, game);
}
void ItemRenderer::renderGuiItemNew(BaseActorRenderContext *BaseActorRenderCtx, ItemStack *item, int mode, float x, float y, float opacity, float scale, bool isEnchanted) {
	using renderGuiItemNew_t = void(__fastcall *)(ItemRenderer *, BaseActorRenderContext *, ItemStack *, int, float, float, bool, float, float, float);
	static renderGuiItemNew_t renderGuiItemNew = reinterpret_cast<renderGuiItemNew_t>(FindSignature("40 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 0F 29 ? 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 45 8B E1 49 8B D8"));
	item->setShowPickUp(false);
	renderGuiItemNew(this, BaseActorRenderCtx, item, mode, x, y, isEnchanted, opacity, 0, scale);
}

void ItemRenderer::renderGuiItemInChunk(BaseActorRenderContext *BaseActorRenderCtx, ItemStack *item, int mode, float x, float y, float opacity, float scale, bool isEnchanted) {
	using renderGuiItemInChunk_t = void(__fastcall *)(ItemRenderer *, BaseActorRenderContext *, int, ItemStack *, float, float, bool, float, float, float);
	static renderGuiItemInChunk_t renderGuiItem = reinterpret_cast<renderGuiItemInChunk_t>(FindSignature("48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 58 FC FF FF 48 81 EC 70 04 00 00 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 98 44 0F 29 48 88 44 0F 29 90 78 FF FF FF 44 0F 29 98 68 FF FF FF 44 0F 29 A0 58 FF FF FF 44 0F 29 A8 48 FF FF FF 48 8B 05 C7 F3 05 03"));

	renderGuiItem(this, BaseActorRenderCtx, 11, item, x, y, isEnchanted, opacity, scale, 1);
}