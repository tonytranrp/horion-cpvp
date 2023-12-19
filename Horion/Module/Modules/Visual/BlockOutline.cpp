#include "BlockOutline.h"

#include "../../../../Utils/Target.h"
BlockOutline::BlockOutline() : IModule(0x0, Category::VISUAL, "Draws a custom colored outline around blocks you are facing!") {
	registerFloatSetting("Thickness", &this->thickness, this->thickness, 0.f, 0.8f);
	registerFloatSetting("Opacity", &this->opacityVal, this->opacityVal, 0.f, 1.f);
	registerBoolSetting("Outline", &this->doOutline, this->doOutline);
	registerBoolSetting("Rainbow", &this->selectRainbow, this->selectRainbow);
	registerColorSetting("Color", &this->Color, this->Color);
	//registerBoolSetting("FaceHighlight", &this->faceH, this->faceH);
	//registerFloatSetting("FaceOpacity", &this->fOpacity, this->fOpacity, 0.f, 1.f);
}

BlockOutline::~BlockOutline() {
}

const char* BlockOutline::getModuleName() {
	return ("BlockOutline");
}
void BlockOutline::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (!Game.getLocalPlayer() || !GameData::canUseMoveKeys()) return;

	auto level = Game.getLevel();
	auto region = Game.getRegion();
	const auto& hitResult = level->getHitResult();

	AABB aabb{};
	region->getBlock(hitResult.block)->blockLegacy->getOutline(region, &hitResult.block, &aabb);
	MC_Color rainbowColor = DrawUtils::getRainbowWave(8.f, 1.f, 1, true);
	DrawUtils::SetColor(this->Color.r, this->Color.g, this->Color.b, opacityVal);
	if (selectRainbow)
		DrawUtils::SetColor(rainbowColor.r, rainbowColor.g, rainbowColor.b, opacityVal);
	if (hitResult.type == HitResultType::Tile)
		DrawUtils::drawBox(aabb.lower, aabb.upper, (float)fmax(thickness, 1 / (float)fmax(1, Game.getLocalPlayer()->getEyePos().dist(aabb.lower))), doOutline);
}
