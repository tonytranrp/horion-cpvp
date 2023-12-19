#include "HealthCheck.h"

HealthCheck::HealthCheck() : IModule(0x0, Category::PLAYER, "A health check that can be used with crystalaura ect...") {
	this->registerIntSetting("Health Slider", &this->HealthSlider, this->HealthSlider, 0, 20);
}

HealthCheck::~HealthCheck() {
}

const char* HealthCheck::getModuleName() {
	return ("HealthCheck");
}

std::string HealthCheck::getModeText() {
	return "HP: " + std::to_string(HealthSlider);
}

bool sent = false;
void HealthCheck::onTick(GameMode* gm) {
	auto PlayerHealth = Game.getLocalPlayer()->getAttribute(AttributeID::Health)->currentValue;

	if (moduleMgr->getModule<HealthCheck>("HealthCheck")->isEnabled()) {
		if (PlayerHealth < HealthSlider && !sent) {
			Notif* notif = NotifMgr::makeNotifStatic(2 * 60, MC_Color(0.0f, 0.0f, 0.0f));
			notif->setName(MC_Color(1.0f, 1.0f, 1.0f), "HealthCheck");
			notif->setText(MC_Color(1.0f, 1.0f, 1.0f), "Your health is low!");
			notif->setDefTexture(Notif::DefTexture::SWarn);
			sent = true;
		} else if (PlayerHealth > HealthSlider) sent = false;
	}
}