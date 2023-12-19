#include "EntityFly.h"
#include "../../ModuleManager.h"

EntityFly::EntityFly() : IModule(0x0, Category::WORLD, "Fly with certain entites, TAB = UP, CTRL = DOWN") {
	registerFloatSetting("speed", &this->speed, this->speed, 0.5f, 10.0f);
}

EntityFly::~EntityFly() {
}

const char* EntityFly::getModuleName() {
	return ("EntityFly");
}

std::string EntityFly::getModeText() {
	char speedChar[50];
	sprintf_s(speedChar, "Speed: %.1f", speed);
	return speedChar;
}

static std::vector<Entity*> targetList;

void findEntity06(Entity* currentEntity) {
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

void EntityFly::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (!player) return;
	targetList.clear();
	Game.forEachEntity(findEntity06);
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());

	if (!targetList.empty()) {
		float yaw = gm->player->viewAnglesPtr->viewAngles.y;
		GameSettingsInput* input = Game.getGameSettingsInput();

		targetList[0]->stateVector->velocity = Vec3(0, 0, 0);

		targetList[0]->stateVector->velocity.y = glideModEffective;

		targetList[0]->setOnGround(true);

		//Vertical
		if (!GameData::canUseMoveKeys()) return;
		glideModEffective = glideMod;
		if (GameData::isKeyDown(VK_TAB))
			glideModEffective += speed;
		if (GameData::isKeyDown(VK_CONTROL))
			glideModEffective -= speed;

		//Horizontal
		bool keyPressed = false;
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
			moveVec.y = glideModEffective;
			moveVec.z = sin(calcYaw) * speed;
			if (keyPressed) {
				targetList[0]->lerpMotion(moveVec);
				//targetList[0]->stateVector->velocity = moveVec; //does the same thing but may be a bool in the future 
				keyPressed = false;
			}
		} 
		static auto AutoMoveMod = moduleMgr->getModule<AutoMove>("AutoMove");
		if (AutoMoveMod->isEnabled()) {
			float calcYaw = (yaw + 90) * (PI / 180);
			float calcPitch = (gm->player->viewAnglesPtr->viewAngles.x) * -(PI / 180);
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * speed;
			moveVec.y = glideModEffective;
			moveVec.z = sin(calcYaw) * speed;
			targetList[0]->lerpMotion(moveVec);
		}
	}
}
