#include "Targethud.h"
#include "../../../Config/ConfigManager.h"
#include "../../../DrawUtils.h"
#include "../../../../Utils/svpng.h"

Targethud::Targethud() : IModule(0, Category::COMBAT, "Informative hud for pvp") {
	registerColorSetting("Primary Color", &color, color);
	registerPositionSetting(&position, position);
}

Targethud::~Targethud() {
}

const char* Targethud::getModuleName() {
	return "TargetHud";
}
static std::vector<Entity*> targetList;
struct CompareTargetEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = Game.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
void Targethud::onTick(GameMode* gm) {
	targetList.clear();
	Game.forEachEntity([&](Entity* ent) {
		if (!Target::isValidTarget(ent, false, false))
			return;
		float dist = (*ent->getPos()).dist(*Game.getLocalPlayer()->getPos());

		if (dist < 7.f)
			targetList.push_back(ent);
	});
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	static auto HudEditormod = moduleMgr->getModule<HudEditor>("HudEditor");
	if (!targetList.empty() || HudEditormod->isEnabled()) {
		Entity* target = HudEditormod->isEnabled() ? Game.getLocalPlayer() : targetList[0];
		if (uniqueID == -1) {
			uniqueID = target->getUniqueId();
		}

		const auto& skin = ((Player*)target)->skin;

		auto wide = ConfigManager::GetRoamingFolderPath();
		std::string path(wide.length(), 0);
		std::transform(wide.begin(), wide.end(), path.begin(), [](wchar_t c) {
			1;
			return (char)c;
		});
		path += "\\Horion\\Assets\\";
		std::filesystem::create_directory(path.c_str());
		filePath = path + "PlayerSkin.png";

		FILE* fp;
		errno_t err = fopen_s(&fp, filePath.c_str(), "wb");
		if (fp) {
			svpng(fp, skin.skinWidth, skin.skinHeight, skin.skinData, 1);
			fclose(fp);
		}
	}
}

void Targethud::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (!Game.getLocalPlayer())
		return;
	targetList.clear();
	Game.forEachEntity([&](Entity* ent) {
		if (!Target::isValidTarget(ent, false, false))
			return;
		float dist = (*ent->getPos()).dist(*Game.getLocalPlayer()->getPos());

		if (dist < 7.f)
			targetList.push_back(ent);
	});
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	static auto HudEditormod = moduleMgr->getModule<HudEditor>("HudEditor");
	Vec4 hudArea = (Vec4(position.x, position.y, position.x + 150.f, position.y + 50.f));

	HudEditormod->addDraggaableItem(DraggableItem(hudArea, &position, "TargetHUD"));

	if (!targetList.empty() || HudEditormod->isEnabled()){
		Entity* target = HudEditormod->isEnabled() ? Game.getLocalPlayer() : targetList[0];
		float distance = target->stateVector->pos.dist(Game.getLocalPlayer()->stateVector->pos);
		char distChar[50];
		sprintf_s(distChar, "%.1f", distance);
		std::string text = target->getNameTag().c_str();
		std::string text2 = "HP: " + std::to_string((int)target->getAttribute(AttributeID::Health)->currentValue) + " | Distance: " + distChar;
		text = Utils::sanitize(text);
		std::string pattern("\n");
		size_t index;
		while ((index = text.find_first_of(pattern)) != std::string::npos) {
			text = text.substr(0, index);
		}
		int64_t id = target->getUniqueId();
		float skinWidth = 43.f;
		float textX = hudArea.x + 56.f;
		DrawUtils::drawRoundRect(Vec4(hudArea.x - 1.f, hudArea.y - 1.f, hudArea.z, hudArea.w), CornerRadius(8), MC_Color(0.f, 0.f, 0.f, 0.85f));
		DrawUtils::drawRoundRectOutline(Vec4(hudArea.x - 1.f, hudArea.y - 1.f, hudArea.z, hudArea.w), CornerRadius(8), MC_Color(color.r, color.g, color.b));
		DrawUtils::drawText(Vec2(textX - 2.f, hudArea.y + 5.f), &text, MC_Color(color.r, color.g, color.b), 1.f, 1.f, DesiredUi::HUD);
		DrawUtils::drawText(Vec2(textX - 2.f, hudArea.y + 15.f), &text2, MC_Color(color.r, color.g, color.b), 1.f, 1.f, DesiredUi::HUD);
		float skinDimension = 33.f, skinOffset = 6.f;
		// armor
		float stackX = textX - 4.f, stackY = hudArea.y + 24.f;
		for (int i = 0; i < 4; i++) {
			ItemStack* stack = target->getArmor(i);
			if (stack->isValid()) {
				float percent = stack->getArmorPercentDura();
				MC_Color percentColor;
				if (percent > 65.f) {
					percentColor = MC_Color(0.1f, 0.95f, 0.1f, 1.f);
				}
				if (percent <= 65.f) {
					percentColor = MC_Color(0.95f, 0.95f, 0.1f, 1.f);
				}
				if (percent <= 35.f) {
					percentColor = MC_Color(0.95f, 0.1f, 0.1f, 1.f);
				}
				DrawUtils::drawItem(stack, Vec2(stackX, stackY), 1.f, 1.f, stack->isEnchanted());
				float percentWidth = DrawUtils::getTextWidth(&std::string(std::to_string(int(percent)) + "%"), 0.7f, DesiredUi::HUD);
				DrawUtils::drawText(Vec2(stackX + (19.f / 2.f) - (percentWidth / 2.f) - 1.5f, stackY + 15.f), &std::string(std::to_string(int(percent)) + "%"), percentColor, 0.7f, 1.f, DesiredUi::HUD);
				stackX += 19.f;
			}
		}
		if (id == uniqueID) {
			float size = 0.125f;
			if (((Player*)target)->skin.skinHeight == 32) {
				size = 0.25f;
			}
			DrawUtils::drawImage(filePath, Vec2(hudArea.x + 10.f, hudArea.y + 10.f), Vec2(30.f, 30.f), Vec2(0.125f, size), Vec2(0.125f, size), true);
			DrawUtils::flushImages();
			DrawUtils::drawImage(filePath, Vec2(hudArea.x + 8.f, hudArea.y + 8.f), Vec2(34.f, 34.f), Vec2(0.625f, size), Vec2(0.125f, size), true);
			DrawUtils::flushImages();
		} else {
			ResourceLocation file(filePath, true);
			renderCtx->unloadTexture(&file);
			uniqueID = -1;
		}
	}
}