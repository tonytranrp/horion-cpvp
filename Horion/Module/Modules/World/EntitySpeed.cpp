#include "EntitySpeed.h"

EntitySpeed::EntitySpeed() : IModule(0, Category::WORLD, "Increases your speed on ridable entites") {
	this->registerFloatSetting("Speed", &this->speed, this->speed, 1.f, 5.f);
}

EntitySpeed::~EntitySpeed() {
}

const char* EntitySpeed::getModuleName() {
	return ("EntitySpeed");
}

std::string EntitySpeed::getModeText() {
	char speedChar[50];
	sprintf_s(speedChar, "Speed: %.1f", speed);
	return speedChar;
}

static std::vector<Entity*> targetList;

void findEntity03(Entity* currentEntity) {
	if (!currentEntity)
		return;

	if (currentEntity->getEntityTypeId() != 23 && currentEntity->getEntityTypeId() != 24 && currentEntity->getEntityTypeId() != 25 && currentEntity->getEntityTypeId() != 90)
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

void EntitySpeed::onTick(GameMode* gm) {
	targetList.clear();
	Game.forEachEntity(findEntity03);
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());

	if (!targetList.empty()) {
		float yaw = gm->player->viewAnglesPtr->viewAngles.y;
		GameSettingsInput* input = Game.getGameSettingsInput();
		bool keyPressed = false;
		if (!GameData::canUseMoveKeys()) return;
		if (GameData::canUseMoveKeys()) {
			if (GameData::isKeyDown(*input->forward.key) && GameData::isKeyDown(*input->back.key))
				return;
			else if (GameData::isKeyDown(*input->forward.key) && GameData::isKeyDown(*input->right.key) && !GameData::isKeyDown(*input->left.key)) {
				yaw += 45.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->forward.key) && GameData::isKeyDown(*input->left.key) && !GameData::isKeyDown(*input->right.key)) {
				yaw -= 45.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->back.key) && GameData::isKeyDown(*input->right.key) && !GameData::isKeyDown(*input->left.key)) {
				yaw += 135.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->back.key) && GameData::isKeyDown(*input->left.key) && !GameData::isKeyDown(*input->right.key)) {
				yaw -= 135.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->forward.key)) {
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->back.key)) {
				yaw += 180.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->right.key) && !GameData::isKeyDown(*input->left.key)) {
				yaw += 90.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->left.key) && !GameData::isKeyDown(*input->right.key)) {
				yaw -= 90.f;
				keyPressed = true;
			}
			float calcYaw = (yaw + 90) * (PI / 180);
			float calcPitch = (gm->player->viewAnglesPtr->viewAngles.x) * -(PI / 180);
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * speed;
			moveVec.y = targetList[0]->stateVector->velocity.y;
			moveVec.z = sin(calcYaw) * speed;
			if (keyPressed) {
				targetList[0]->lerpMotion(moveVec);
				keyPressed = false;
			}
		}
		static auto AutoMoveMod = moduleMgr->getModule<AutoMove>("AutoMove");
		if (AutoMoveMod->isEnabled()) {
			float calcYaw = (yaw + 90) * (PI / 180);
			float calcPitch = (gm->player->viewAnglesPtr->viewAngles.x) * -(PI / 180);
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * speed;
			moveVec.y = targetList[0]->stateVector->velocity.y;
			moveVec.z = sin(calcYaw) * speed;
			targetList[0]->lerpMotion(moveVec);
		}
	}
}
