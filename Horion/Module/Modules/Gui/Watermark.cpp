#include "Watermark.h"
#include "../../../DrawUtils.h"
#include "../../ModuleManager.h"
Watermark::Watermark() : IModule(0, Category::GUI, "Displays a Watermark") {
	registerPositionSetting(&position, position);
}

Watermark::~Watermark() {
}

const char* Watermark::getModuleName() {
	return ("Watermark");
}

void Watermark::onPreRender(MinecraftUIRenderContext* renderCtx) {
	Vec2 windowSize = Game.getGuiData()->windowSize;

	if (Game.getLocalPlayer()) {
		// Draw Horion logo
			constexpr float nameTextSize = 1.5f;
			constexpr float versionTextSize = 0.7f;
			static const float textHeight = (nameTextSize + versionTextSize * 0.7f /* We don't quite want the version string in its own line, just a bit below the name */) * DrawUtils::getFont(DesiredUi::HUD)->getLineHeight();
			constexpr float borderPadding = 1;
			constexpr float margin = 5;

			static std::string name = "Horion";
#ifdef HORION_DEBUG
			static std::string version = "dev";
#elif defined _BETA
			static std::string version = "beta";
#else
			static std::string version = "public";
#endif

			float nameLength = DrawUtils::getTextWidth(&name, nameTextSize, DesiredUi::HUD);
			float fullTextLength = nameLength + DrawUtils::getTextWidth(&version, versionTextSize, DesiredUi::HUD);

			if (position == Vec2(999.f, 999.f))
				position = Vec2(windowSize.x - margin - fullTextLength - borderPadding * 2, windowSize.y - margin - textHeight);

			Vec4 rect = Vec4(
				position.x,
				position.y,
				position.x + fullTextLength + borderPadding * 2,
				position.y + textHeight);

			static auto hudEditorMod = moduleMgr->getModule<HudEditor>("HudEditor");
			hudEditorMod->addDraggaableItem(DraggableItem(rect, &position, "Watermark"));

			static float rcolors[4];
			static float disabledRcolors[4];

			Utils::ApplyRainbow(rcolors);  // Increase Hue of rainbow color array
			disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
			disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
			disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
			disabledRcolors[3] = 1;

			if (rcolors[3] < 1) {
				rcolors[0] = 1;
				rcolors[1] = 0.2f;
				rcolors[2] = 0.2f;
				rcolors[3] = 1;
			}

			Utils::ApplyRainbow(rcolors, 0.00015f);  //0.0015f

			if (hudEditorMod->isEnabled() || GameData::canUseMoveKeys()) {
				DrawUtils::drawRect(rect, MC_Color(rcolors), 1.f, 2.f);
				DrawUtils::fillRect(rect, MC_Color(12, 12, 12), 1.f);
				//Have fun with this example
				//DrawUtils::drawRainbowText(Vec2(rect.x + borderPadding, rect.y), name, 3.f, 1.f, false, nameTextSize, 1.f, DesiredUi::HUD);
				DrawUtils::drawText(Vec2(rect.x + borderPadding, rect.y), &name, MC_Color(rcolors), nameTextSize, 1.f, DesiredUi::HUD);
				DrawUtils::drawText(Vec2(rect.x + borderPadding + nameLength, rect.w - 7), &version, MC_Color(rcolors), versionTextSize, 1.f, DesiredUi::HUD);
			}
	}
}