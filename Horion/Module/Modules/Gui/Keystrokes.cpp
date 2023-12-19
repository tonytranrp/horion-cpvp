#include "Keystrokes.h"
#include "../../../DrawUtils.h"
#include "../../ModuleManager.h"

Keystrokes::Keystrokes() : IModule(0, Category::GUI, "displays your key inputs") {
	registerPositionSetting(&position, position);
}

Keystrokes::~Keystrokes() {
}

const char* Keystrokes::getModuleName() {
	return ("Keystrokes");
}

void Keystrokes::drawKeystroke(char key, const Vec2& pos) {
	std::string keyString = Utils::getKeybindName(key);
	GameSettingsInput* input = Game.getGameSettingsInput();
	if (key == *input->forward.key) {
		Vec4 rectPos(
			pos.x,
			pos.y,
			pos.x + 20.f,
			pos.y + 20.f);
		Vec2 textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString, 1.f, DesiredUi::HUD) / 2.f),
			rectPos.y + 10.f - DrawUtils::getFont(DesiredUi::HUD)->getLineHeight() / 2.f);

		DrawUtils::fillRect(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f, DesiredUi::HUD);
	}
	if (key == *input->spaceBar.key) {
		if (key == *input->spaceBar.key) keyString = "-";
		Vec4 rectPos(
			pos.x,
			pos.y,
			pos.x + 64.f,
			pos.y + 15.f);
		Vec2 textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString, 1.f, DesiredUi::HUD) / 2.f),
			rectPos.y + 7.f - DrawUtils::getFont(DesiredUi::HUD)->getLineHeight() / 2.f);

		DrawUtils::fillRect(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f, DesiredUi::HUD);
	} else {
		Vec4 rectPos(
			pos.x,
			pos.y,
			pos.x + ((key == *input->spaceBar.key) ? 64.f : 20.f),
			pos.y + ((key == *input->spaceBar.key) ? 15.f : 20.f));
		Vec2 textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString, 1.f, DesiredUi::HUD) / 2.f),
			rectPos.y + 10.f - DrawUtils::getFont(DesiredUi::HUD)->getLineHeight() / 2.f);

		DrawUtils::fillRect(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f, DesiredUi::HUD);
	}
}

void Keystrokes::drawLeftMouseKeystroke(Vec2 pos) {
	std::string keyString;
	keyString = "LMB";
	Vec4 rectPos(
		pos.x,
		pos.y + 2,
		pos.x + 31.f,
		pos.y + 22.f);
	DrawUtils::fillRect(rectPos, GameData::GameData::isLeftClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
	Vec2 textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString, 1.f, DesiredUi::HUD) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(DesiredUi::HUD)->getLineHeight() / 2.f);
	DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f, DesiredUi::HUD);
}

void Keystrokes::drawRightMouseKeystroke(Vec2 pos) {
	std::string keyString;
	keyString = "RMB";
	Vec4 rectPos(
		pos.x,
		pos.y + 2,
		pos.x + 31.f,
		pos.y + 22.f);
	DrawUtils::fillRect(rectPos, GameData::GameData::isRightClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
	Vec2 textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString, 1.f, DesiredUi::HUD) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(DesiredUi::HUD)->getLineHeight() / 2.f);
	DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f, DesiredUi::HUD);
}

void Keystrokes::onPostRender(MinecraftUIRenderContext* renderCtx) {

	Vec2 windowSize = Game.getGuiData()->windowSize;
	static auto hudEditorMod = moduleMgr->getModule<HudEditor>("HudEditor");

	if (!(!Game.getLocalPlayer() || !GameData::canUseMoveKeys()) || hudEditorMod->isEnabled()) {
		GameSettingsInput* input = Game.getGameSettingsInput();
		Keystrokes::drawKeystroke(*input->forward.key, Vec2(position.x + 22.f, position.y));
		Keystrokes::drawKeystroke(*input->left.key, Vec2(position.x, position.y + 22.f));
		Keystrokes::drawKeystroke(*input->back.key, Vec2(position.x + 22.f, position.y + 22.f));
		Keystrokes::drawKeystroke(*input->right.key, Vec2(position.x + 44.f, position.y + 22.f));
		Keystrokes::drawKeystroke(*input->spaceBar.key, Vec2(position.x, position.y + 44.f));
		Keystrokes::drawLeftMouseKeystroke(Vec2(position.x, position.y + 59.f));
		Keystrokes::drawRightMouseKeystroke(Vec2(position.x + 33.f, position.y + 59.f));

		Vec4 rect(position.x, position.y, position.x + 64.f, position.y + 81.f);
		hudEditorMod->addDraggaableItem(DraggableItem(rect, &position, "Keystrokes"));
	}
}