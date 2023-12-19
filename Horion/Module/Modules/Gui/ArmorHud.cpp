#include "ArmorHud.h"

#include "../../../DrawUtils.h"
#include "../../ModuleManager.h"

ArmorHud::ArmorHud() : IModule(0, Category::GUI, "Displays ArmorHud") {
	registerPositionSetting(&position, position);
	registerBoolSetting("Vertical", &this->Vertical, this->Vertical);
}

ArmorHud::~ArmorHud() {
}

const char* ArmorHud::getModuleName() {
	return ("ArmorHud");
}

void ArmorHud::onPostRender(MinecraftUIRenderContext* renderCtx) {

	if (!Game.getLocalPlayer()) return;

	static float constexpr scale = 1.f;
	static float constexpr opacity = 1.f;
	static float constexpr spacing = scale + 15.f;

	LocalPlayer* player = Game.getLocalPlayer();

	Vec2 windowSize = Game.getGuiData()->windowSize;
	
	float xVal = position.x;
	float yVal = position.y;
	Vec4 rect;
	static auto hudEditorMod = moduleMgr->getModule<HudEditor>("HudEditor");
	if (!Vertical)
		rect = Vec4(position.x, position.y, position.x + (spacing * 5.f), position.y + spacing);
	else
		rect = Vec4(position.x, position.y, position.x + spacing, position.y + (spacing * 5.f));
	hudEditorMod->addDraggaableItem(DraggableItem(rect, &position, "ArmorHud"));

	for (int i = 0; i < 4; i++) {
		ItemStack* stack = player->getArmor(i);
		if (stack->isValid()) {
			DrawUtils::drawItem(stack, Vec2(xVal, yVal), opacity, scale, stack->isEnchanted());
			if (Vertical) {
				std::string durability = std::to_string(stack->getDurability()) + "/" + std::to_string(stack->getMaxDurability());
				float textWidth = DrawUtils::getTextWidth(&durability, scale, DesiredUi::HUD);
				if (position.x < windowSize.x / 2.f)
					DrawUtils::drawText(Vec2(xVal + spacing + 1.f, yVal + (spacing / 3) - scale), &durability, MC_Color(1.f, 1.f, 1.f), scale, 1.f, DesiredUi::HUD);
				else
					DrawUtils::drawText(Vec2(xVal - textWidth - 1.f, yVal + (spacing / 3) - scale), &durability, MC_Color(1.f, 1.f, 1.f), scale, 1.f, DesiredUi::HUD);
			}

			if (!Vertical)
				xVal += scale * spacing;
			else
				yVal += scale * spacing;
		}
	}
	PlayerInventoryProxy* supplies = Game.getSupplies();
	ItemStack* item = Game.getInventory()->getItemStack(supplies->selectedHotbarSlot);

	//x += scale * spacing;
	if (item->isValid()) {
		DrawUtils::drawItem(item, Vec2(xVal, yVal), opacity, scale, item->isEnchanted());
		if (Vertical) {
			std::string textStr = "";
			if (item->getMaxDurability() == 0) { //prevent items about to break to become the naughty
				textStr = std::to_string(item->count) + "/" + std::to_string(item->getItem()->maxStackSize);
			} else {
				textStr = std::to_string(item->getDurability()) + "/" + std::to_string(item->getMaxDurability());
			}
			float textWidth = DrawUtils::getTextWidth(&textStr, scale, DesiredUi::HUD);
			if (position.x < windowSize.x / 2.f)
				DrawUtils::drawText(Vec2(xVal + spacing + 1.f, yVal + (spacing / 3) - scale), &textStr, MC_Color(1.f, 1.f, 1.f), scale, 1.f, DesiredUi::HUD);
			else
				DrawUtils::drawText(Vec2(xVal - textWidth - 1.f, yVal + (spacing / 3) - scale), &textStr, MC_Color(1.f, 1.f, 1.f), scale, 1.f, DesiredUi::HUD);
		}
	}
}