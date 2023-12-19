#include "EntityJesus.h"

EntityJesus::EntityJesus() : IModule(0, Category::WORLD, "Allows Ridable Entites to walk on water") {
}

EntityJesus::~EntityJesus() {
}

const char* EntityJesus::getModuleName() {
	return ("EntityJesus");
}

static std::vector<Entity*> targetList;

void findEntity09(Entity* currentEntity) {
	if (!currentEntity)
		return;

	if (currentEntity->getEntityTypeId() != 23 && currentEntity->getEntityTypeId() != 24 && currentEntity->getEntityTypeId() != 25)
		return;
	//horse = 23
	//donkey = 24
	//mule = 25
	//boat = 90

	targetList.push_back(currentEntity);
}

struct CompareTargetEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = Game.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void EntityJesus::onTick(GameMode* gm) {
	targetList.clear();
	Game.forEachEntity(findEntity09);
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());

	if (!targetList.empty()) {

		if (targetList[0]->hasEnteredWater()) {
			targetList[0]->stateVector->velocity.y = 0.06f;
			targetList[0]->setOnGround(true);
			wasInWater = true;
		} else if (targetList[0]->isInWater() || gm->player->isInLava()) {
			targetList[0]->stateVector->velocity.y = 0.1f;
			targetList[0]->setOnGround(true);
			wasInWater = true;
		} else {
			if (wasInWater) {
				wasInWater = false;
				targetList[0]->stateVector->velocity.x *= 1.2f;
				targetList[0]->stateVector->velocity.x *= 1.2f;
			}
		}
	}

}