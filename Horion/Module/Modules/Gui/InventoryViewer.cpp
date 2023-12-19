#include "InventoryViewer.h"
#include "../../../DrawUtils.h"
#include "../../ModuleManager.h"

InventoryViewer::InventoryViewer() : IModule(0x0, Category::GUI, "Shows your inventory on screen") {
	registerPositionSetting(&position, position);
}

InventoryViewer::~InventoryViewer() {
}

const char* InventoryViewer::getModuleName() {
	return ("InventoryViewer");
}

void InventoryViewer::onPostRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	Vec2 windowSize = Game.getGuiData()->windowSize;
	Vec2 windowSizeReal = Game.getGuiData()->windowSizeReal;
	Vec2 mousePosition = *Game.getClientInstance()->getMousePos();
	mousePosition = mousePosition.div(windowSizeReal);
	mousePosition = mousePosition.mul(windowSize);

	float item_pos_x, item_pos_y;

	static auto hudEditorMod = moduleMgr->getModule<HudEditor>("HudEditor");
	hudEditorMod->addDraggaableItem(DraggableItem(Vec4(position.x, position.y, position.x + 9 * 16, position.y + 3 * 16), &position, "InventoryViewer"));

	for (int i = 0; i < 27; i++) {
		auto stack = player->getSupplies()->inventory->getItemStack(i + 9);
		
		item_pos_x = position.x + (i % 9) * 16;
		item_pos_y = position.y + (i / 9) * 16;

		if (stack->isValid()) {
			DrawUtils::drawItem(stack, Vec2(item_pos_x, item_pos_y), 1.f, 1.f, stack->isEnchanted());
			if (stack->count > 1)
				DrawUtils::drawText(Vec2(item_pos_x + 9.f, item_pos_y + 9.f), &std::to_string(stack->count), MC_Color(0.95f, 0.95f, 0.95f), 1.f / 1.3f, 1.f, DesiredUi::HUD);
		}
	}
}