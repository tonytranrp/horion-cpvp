#include "MurderTool.h"
#include "../../../DrawUtils.h"
#include "../../../Menu/NotifMgr.h"
MurderTool::MurderTool() : IModule(0, Category::MISC, "Helps you play Murder Mystery on hive") {
	registerFloatSetting("Opacity", &opacity, opacity, 0.1f, 1.f);
}

MurderTool::~MurderTool() {
}

const char* MurderTool::getModuleName() {
	return "MurderTool";
}
static std::vector<Entity*> coinList;
static std::vector<Entity*> targetList;
static std::vector<Entity*> Murder;
void MurderTool::onEnable() {
	if (!coinList.empty())
		coinList.clear();
}
void MurderTool::onPostRender(MinecraftUIRenderContext* renderCtx) {
	auto plr = Game.getLocalPlayer();
	if (!plr) return;
	if (isMurder) {
		if (!targetList.empty()) {
			for (auto& ent : targetList) {
				float dist = ent->getPos()->dist(*Game.getLocalPlayer()->getPos());
				std::string name = "Victim";
				bool hasBow = doesEntityHaveBow(ent);
				if (hasBow) {
					name = ((Player*)ent)->remainingTicksUsingItem > 0 ? "Victim Is Using Bow" : "Victim has Bow";
				}
				DrawUtils::drawName(ent, &name, fmax(0.6f, 3.f / dist), {0.1f, 0.1f, 0.1f}, hasBow ? MC_Color{0.1f, 1.f, 0.85f} : MC_Color{0.1f, 1.f, 0.1f}, opacity, DesiredUi::DEFAULT);
				DrawUtils::Flush();
			}
		}
	}
	if (isMurder) {
		return;
	} 
	if (!coinList.empty()) {
		for (auto& coin : coinList) {
			auto height = coin->aabbShape->aabb.upper.y - coin->aabbShape->aabb.lower.y;
			auto width = coin->aabbShape->aabb.upper.x - coin->aabbShape->aabb.lower.x;
			std::string name = "coin";
			bool hat = (width == 0.75f && height == 1.f && coin->viewAnglesPtr->viewAngles.x);
			if (hat)
				name = "Sherrif Hat";
			float dist = Game.getLocalPlayer()->getPos()->dist(*coin->getPos());
			DrawUtils::drawName(coin, &name, fmax(0.6f, 3.f / dist), {0.1f, 0.1f, 0.1f}, hat ? MC_Color{0.1f, 0.5f, 0.85f} : MC_Color{1.f, 1.f, 0.1f}, opacity, DesiredUi::DEFAULT);
			DrawUtils::Flush();
		}
	}
	if (!Murder.empty()) {
		float dist = Murder[0]->getPos()->dist(*Game.getLocalPlayer()->getPos());
		DrawUtils::drawName(Murder[0], &std::string("Murderder"), fmax(0.6f, 3.f / dist), {0.1f, 0.1f, 0.1f}, {1.f, 0.1f, 0.1f}, opacity);
		DrawUtils::Flush();
	}
}
bool MurderTool::isPlayerMurderer() {
	auto plr = Game.getLocalPlayer();
	if (!plr || !plr->isAlive()) return false;
	int i = 0;
	for (i; i < 9; ++i) {
		auto inv = plr->getSupplies()->inventory;
		auto stack = inv->getItemStack(i);
		if (!stack->isValid())
			continue;
		if (stack->getItem()->getAttackDamage() > 0)
			return true;
	}
	return false;
}
bool MurderTool::doesEntityHaveBow(Entity* entity) {
	auto plr = Game.getLocalPlayer();
	if (!plr || !plr->isAlive()) return false;
	auto* ent = reinterpret_cast<Player*>(entity);
	auto stack = ent->getSelectedItem();
	if (!stack->isValid() || !stack->getItem()->isTool()) {
		return false;
	}
	if (stack->getItem()->name.find("bow") == std::string::npos) {
		return false;
	}
	return true;
}
void MurderTool::onTick(GameMode* c) {
	auto plr = Game.getLocalPlayer();
	if (!plr) return;
	isMurder = isPlayerMurderer();
	targetList.clear();
	if (isMurder) {
		Game.forEachEntity([&](Entity* ent) {
			if (!Target::isValidTarget(ent, false, false)) return;
			targetList.push_back(ent);
		});
	}
	if (isMurder) return;
	coinList.clear();
	Game.forEachEntity([&](Entity* ent) {
		if (!ent) return;
		if (ent == plr) return;
		auto height = ent->aabbShape->aabb.upper.y - ent->aabbShape->aabb.lower.y;
		auto width = ent->aabbShape->aabb.upper.x - ent->aabbShape->aabb.lower.x;
		if (width == 0.75f && height == 1.f && ent->viewAnglesPtr->viewAngles.x)
			coinList.push_back(ent);
		if (ent->getEntityTypeId() == 0 && (height < 0.6 && height > 0.595f) || height < 0.601f && height > 0.6f) {
			coinList.push_back(ent);
		}
		if (ent->getEntityTypeId() != 63) return;
			targetList.push_back(ent);
	});
	if (!Murder.empty()) {
		if (targetList.empty() || !plr->isAlive() || !Murder[0]->isAlive()) {
			Murder.clear(); 
		}
		return;
	}
	for (auto& entity : targetList) {
		auto* ent = reinterpret_cast<Player*>(entity);
		auto stack = ent->getSelectedItem();
		if (!stack->isValid()) continue;
			if (stack->getItem()->getAttackDamage() || stack->getItem()->name.find("nether_star") == 0) {
				Murder.push_back(entity);
				Notif* notif = NotifMgr::makeNotifStatic(800, MC_Color(0.1f, 0.1f, 0.1f));
				notif->setName(MC_Color(1.0f, 1.0f, 1.0f), "Murder Mystery");
				notif->setText(MC_Color(1.0f, 1.0f, 1.0f), "Killer Has been found");
			}
	}
}
