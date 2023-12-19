#include "HudEditor.h"
#include "../../ModuleManager.h"

HudEditor::HudEditor() : IModule(0x0, Category::GUI, "Edit HUD element positions") {
}

HudEditor::~HudEditor() {
}

const char* HudEditor::getModuleName() {
	return ("HudEditor");
}

void HudEditor::onEnable() {
	if (!Game.getLocalPlayer())
		this->setEnabled(false);
	static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>("ClickGui");
	if (clickGuiModule->isEnabled())
		clickGuiModule->setEnabled(false);
	Game.getClientInstance()->releaseCursor();
}

void HudEditor::addDraggaableItem(DraggableItem item) {
	if (std::count(itemList.begin(), itemList.end(), item)) {
		itemList.erase(std::remove(itemList.begin(), itemList.end(), item), itemList.end());
		itemList.emplace_back(item);
	} else
		itemList.emplace_back(item);
}

void HudEditor::removeDraggaableItem(DraggableItem item) {
	itemList.erase(std::remove(itemList.begin(), itemList.end(), item), itemList.end());
}

void HudEditor::Render() {
	if (!Game.getLocalPlayer())
		return;
	auto windowSize = Game.getGuiData()->windowSize;
	Vec2 mousePosition = *Game.getClientInstance()->getMousePos();
	mousePosition = mousePosition.div(Game.getGuiData()->windowSizeReal);
	mousePosition = mousePosition.mul(windowSize);
	float x = windowSize.x / 2, y = 5, width = DrawUtils::getTextWidth(&std::string("Exit"), 1.5f, DesiredUi::HUD), height = DrawUtils::getFontHeight(1.5f, DesiredUi::HUD);
	bool hovered = DrawUtils::isMouseOver(Vec4(x - (width / 2), y, x + (width / 2), y + height));
	MC_Color circleColor = hovered ? MC_Color(1.f, 0.2f, 0.2f, 1.f) : MC_Color(1.f, 1.f, 1.f, 1.f);
	DrawUtils::drawText(Vec2(x - (width / 2), y), &std::string("Exit"), MC_Color(circleColor), 1.5f, 1.f, DesiredUi::HUD);
	if (Game.isKeyDown(VK_ESCAPE) || hovered && Game.isLeftClickDown() || !Game.getLocalPlayer()->isAlive()) {
		this->setEnabled(false);
	}

	for (auto& item : itemList) {
		Vec4 rect(item.areaPos.x - 2.f, item.areaPos.y - 2.f, item.areaPos.z + 2.f, item.areaPos.w + 2.f);
		DrawUtils::drawRect(rect, MC_Color(1.f, 1.f, 1.f), 1.f);
		if (DrawUtils::isMouseOver(rect) || item.name == currentDragging) {
			Vec2 windowSize = Game.getGuiData()->windowSize;
			std::string text = item.name;
			float textWidth = DrawUtils::getTextWidth(&text, 1.f, DesiredUi::HUD);
			float textHeight = DrawUtils::getFontHeight(1.f, DesiredUi::HUD);
			Vec2 textPos = Vec2(
				(windowSize.x / 2.f) - (textWidth / 2.f),
				textHeight + 14.f);
			Vec4 rectPos = Vec4(
				textPos.x - 2.f,
				textPos.y - 2.f,
				textPos.x + textWidth + 2.f,
				textPos.y + textHeight + 3.5f);
			DrawUtils::drawRoundRect(rectPos, CornerRadius(2), MC_Color(0.f, 0.f, 0.f, 0.65f));
			DrawUtils::drawRoundRect({rectPos.x, rectPos.w - 0.5f, rectPos.z, rectPos.w}, CornerRadius(0, 0, 1, 1), MC_Color(1.f, 1.f, 1.f), 0.5f);
			DrawUtils::drawText(textPos, &text, MC_Color(1.f, 1.f, 1.f), 1.f, 1.f, DesiredUi::HUD);  
		}
		if (DrawUtils::isMouseOver(item.areaPos)) {
			DrawUtils::fillRect(rect, MC_Color(1.f, 1.f, 1.f), 0.50f);
			mouseAlreadyOver = item.name;
		} else
			mouseAlreadyOver = "";
		if (currentDragging.empty()) {
			if (DrawUtils::isMouseOver(rect) && item.name == mouseAlreadyOver) {
				DrawUtils::fillRect(rect, MC_Color(1.f, 1.f, 1.f), 0.50f);
				if (!Game.isLeftClickDown()) {
					targetVec.x = mousePosition.x - item.pos->x;
					targetVec.y = mousePosition.y - item.pos->y;
				}
				if (Game.isLeftClickDown()) {
					currentDragging = item.name;
				}
			}
		} else if (item.name == currentDragging) {
			if (Game.isLeftClickDown()) {
				DrawUtils::fillRect(rect, MC_Color(1.f, 1.f, 1.f), 0.50f);
				item.pos->x = mousePosition.x - targetVec.x;
				item.pos->y = mousePosition.y - targetVec.y;
				if (item.pos->x <= 1.f)
					item.pos->x = 2.f;
				if (item.pos->y <= 1.f)
					item.pos->y = 2.f;

				float wdith = item.areaPos.z - item.areaPos.x;
				float height = item.areaPos.w - item.areaPos.y;
				if (item.pos->x + wdith >= windowSize.x - 1.f)
					item.pos->x = windowSize.x - 2.f - wdith;
				if (item.pos->y + height >= windowSize.y - 1.f)
					item.pos->y = windowSize.y - 2.f - height;
			} else {
				currentDragging = "";
			}
		}
	}
}

void HudEditor::onDisable() {
	Game.getClientInstance()->grabCursor();
}
