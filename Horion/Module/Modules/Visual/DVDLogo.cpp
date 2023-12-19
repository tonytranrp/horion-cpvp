#include "DVDLogo.h"
#include "../../../DrawUtils.h"
#include "../../../../resource.h"

DVDLogo::DVDLogo() : IModule(0, Category::VISUAL, "DVD Screen Saver") {
}

DVDLogo::~DVDLogo() {
}

const char* DVDLogo::getModuleName() {
	return "DVDLogo";
}

void DVDLogo::onPostRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	if (Game.getLocalPlayer() && GameData::canUseMoveKeys()) {
		float Height = Game.getGuiData()->heightGame;
		float Width = Game.getGuiData()->widthGame;

		//Vertical
		if (positiveY) {
			pos.y += 1.f;
			if (pos.y + 69 > Height) {
				colorR = rand() % 127 + 127;
				colorG = rand() % 127 + 127;
				colorB = rand() % 127 + 127;
				positiveY = false;
			}
		}

		if (!positiveY) {
			pos.y -= 1.f;
			if (pos.y < 0) {
				colorR = rand() % 127 + 127;
				colorG = rand() % 127 + 127;
				colorB = rand() % 127 + 127;
				positiveY = true;
			}
		}

		//Horizontal
		if (positiveX) {
			pos.x += 0.8f;
			if (pos.x + 142 > Width) {
				colorR = rand() % 127 + 127;
				colorG = rand() % 127 + 127;
				colorB = rand() % 127 + 127;
				positiveX = false;
			}
		}

		if (!positiveX) {
			pos.x -= 0.8f;
			if (pos.x < 0) {
				colorR = rand() % 127 + 127;
				colorG = rand() % 127 + 127;
				colorB = rand() % 127 + 127;
				positiveX = true;
			}
		}
		auto dvd = DrawUtils::resourceToTexturePtr(DVD_DATA, "DVD");
		DrawUtils::drawImageFromTexturePtr(dvd, pos, Vec2(142.f, 69.f), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
		DrawUtils::flushImages(MC_Color(colorR, colorG, colorB));
	}
}

void DVDLogo::onEnable() {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	pos.x = Game.getGuiData()->widthGame - Game.getGuiData()->widthGame / 2 - scale / 2;
	pos.y = Game.getGuiData()->heightGame - Game.getGuiData()->heightGame / 2 - scale / 2;
	positiveX = true;
	positiveY = true;
	colorR, colorG, colorB = 255;
}