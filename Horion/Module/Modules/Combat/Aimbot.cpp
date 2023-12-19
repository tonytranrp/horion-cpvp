#include "Aimbot.h"

Aimbot::Aimbot() : IModule(0, Category::COMBAT, "Automatically aims at the nearest entity") {
	this->registerFloatSetting("Range", &this->range, this->range, 3.f, 8.f);
	this->registerBoolSetting("Require Click", &this->click, this->click);
	this->registerBoolSetting("Only Weapons", &this->sword, this->sword);
	this->registerBoolSetting("Vertical", &this->vertical, this->vertical);
	this->registerFloatSetting("Horizontal Speed", &this->horizontalspeed, this->horizontalspeed, 10.f, 90.f);
	this->registerFloatSetting("Vertical Speed", &this->verticalspeed, this->verticalspeed, 10.f, 90.f);
	this->registerFloatSetting("Horizontal Range", &this->horizontalrange, this->horizontalrange, 20.f, 180.f);
	this->registerFloatSetting("Vertical Range", &this->verticalrange, this->verticalrange, 20.f, 180.f);
	this->registerBoolSetting("Aimlock", &this->lock, this->lock);
	this->registerBoolSetting("Include Mobs", &this->includeMobs, this->includeMobs);
}

Aimbot::~Aimbot() {
}

const char* Aimbot::getModuleName() {
	return ("Aimbot");
}

std::string Aimbot::getModeText() {
	char rangeChar[50];
	sprintf_s(rangeChar, "Range: %.1f", range);
	return rangeChar;
}

struct CompareTargetEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = Game.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

static std::vector<Entity*> targetList;

void Aimbot::onPostRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (!localPlayer || !GameData::canUseMoveKeys())
		return;

	Vec3 origin = Game.getLevelRenderer()->getOrigin();

	//Loop through all our players and retrieve their information
	targetList.clear();
	Game.forEachEntity([&](Entity* ent) {
		if (!Target::isValidTarget(ent, includeMobs, false))
			return;

		float dist = (*ent->getPos()).dist(*Game.getLocalPlayer()->getPos());

		if (dist < range) {
			targetList.push_back(ent);
		}
	});

	if (targetList.size() > 0) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		auto pos = *targetList[0]->getPos();
		if (targetList[0]->getEntityTypeId() == 63)
			pos.y -= (targetList[0]->aabbShape->height / 2);
		else
			pos.y += (targetList[0]->aabbShape->height / 2);
		Vec2 angle = origin.CalcAngle(pos);
		Vec2 appl = angle.sub(localPlayer->viewAnglesPtr->viewAngles).normAngles();
		appl.x = -appl.x;
		if ((appl.x < verticalrange && appl.x > -verticalrange) && (appl.y < horizontalrange && appl.y > -horizontalrange) && GameData::canUseMoveKeys()) {
			PlayerInventoryProxy* supplies = Game.getSupplies();
			ItemStack* item = Game.getInventory()->getItemStack(supplies->selectedHotbarSlot);
			if (sword &&  item->item == NULL)
				return;
			
			if (sword && !(item->getItem()->isWeapon()))
				return;

			if (click && !Game.isLeftClickDown())
				return;

			if (!this->lock) {
				appl.x /= (100.f - verticalspeed);
				appl.y /= (100.f - horizontalspeed);
				if (appl.x >= 1 || appl.x <= -1) appl.div(abs(appl.x));
				if (appl.y >= 1 || appl.y <= -1) appl.div(abs(appl.y));
			}
			if (!vertical)
				appl.x = 0;
			localPlayer->applyTurnDelta(&appl);
		}
	}
}