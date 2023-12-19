#include "Freecam.h"

Freecam::Freecam() : IModule('V', Category::MISC, "Move your cam without moving the player") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.10f, 0.50f);
}

Freecam::~Freecam() {
}

const char* Freecam::getModuleName() {
	return ("Freecam");
}

std::string Freecam::getModeText() {
	auto speedStr = Utils::formatFloat(this->speed);
	speedStr.insert(0, "Speed: ");
	return speedStr;
}

void Freecam::onTick(GameMode* gm) {
	auto perspectiveOption = Game.getClientInstance()->getOptions()->getOptionByName("game_thirdperson");
	if (perspectiveOption)
		perspectiveSetting = (int*)&perspectiveOption->maxI;
}

static void* targetAddr = nullptr;
static char ogIns[45] = {};

void Freecam::onEnable() {
	if (!targetAddr)
		targetAddr = (void*)(FindSignature("F3 44 0F 11 6B ?? 44 0F 28 AC 24"));

	Utils::copyBytes(targetAddr, ogIns, sizeof(ogIns));
	Utils::nopBytes(targetAddr, sizeof(ogIns));

	if (!Game.isInGame()) {
		this->setEnabled(false);
		return;
	}
}

void Freecam::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (perspectiveSetting) {
		if (!gotInfo) {
			gotInfo = true;
			oldPerspective = *perspectiveSetting;
		}
		*perspectiveSetting = 1;
	}

	if (!this->cam) return;

	GameSettingsInput* input = Game.getGameSettingsInput();
	if (!input) return;

	auto lp = Game.getLocalPlayer();
	if (!lp) return;

	float yaw = lp->viewAnglesPtr->viewAngles.y;
	bool keyPressed = false;

	if (GameData::isKeyDown(*input->forward.key) && GameData::isKeyDown(*input->back.key))  // Copied from inv move
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
	if (yaw >= 180)
		yaw -= 360.f;


	constexpr float invRadDegrees = PI / 180.f;
	float calcYaw = (yaw + 90.f) * invRadDegrees;
	Vec3 moveVec{
		std::cos(calcYaw) * this->speed,
		0.f,
		std::sin(calcYaw) * this->speed
	};

	if (GameData::isKeyDown(*input->spaceBar.key)) {
		this->cam->cameraPos.y += this->speed;
	}
	else if (GameData::isKeyDown(*input->sneak.key)) {
		this->cam->cameraPos.y -= this->speed;
	}

	if (keyPressed) {
		auto newPos = this->cam->cameraPos.add(moveVec);
		this->cam->cameraPos = std::move(newPos);
	}
}

void Freecam::onDisable() {
	Utils::patchBytes(targetAddr, ogIns, 45);

	if (!Game.isInGame())
		return;

	if (perspectiveSetting && gotInfo) {
		*perspectiveSetting = oldPerspective;
	}
	gotInfo = false;
}