#include "NoRender.h"

NoRender::NoRender() : IModule(0x0, Category::VISUAL, "Stop certain animations & unrender objects/entites") {
	registerBoolSetting("Fire", &this->noFire, this->noFire);
	registerBoolSetting("Weather", &this->noWeather, this->noWeather);
	registerBoolSetting("Nausea", &this->noNausea, this->noNausea);
	registerBoolSetting("Entities", &this->noEntities, this->noEntities);
	registerBoolSetting("Block Entities", &this->noBlockEntities, this->noBlockEntities);
	registerBoolSetting("Particles", &this->noParticles, this->noParticles);
}

NoRender::~NoRender() {
}

const char* NoRender::getModuleName() {
	return ("NoRender");
}

void NoRender::onTick(GameMode* gm) {
	auto options = Game.getClientInstance()->getOptions();
	auto renderWeatherOption = options->getOptionByTranslatedName("options.dev_disableRenderWeather");
	if (renderWeatherOption)
		noWeatherSetting = (bool*)&renderWeatherOption->valueI;

	auto renderEntitiesOption = options->getOptionByTranslatedName("options.dev_disableRenderEntities");
	if (renderEntitiesOption)
		noEntitySetting = (bool*)&renderEntitiesOption->valueI;

	auto renderBlockEntitiesOption = options->getOptionByTranslatedName("options.dev_disableRenderBlockEntities");
	if (renderBlockEntitiesOption)
		noBlockEntitySetting = (bool*)&renderBlockEntitiesOption->valueI;

	auto renderParticlesOption = options->getOptionByTranslatedName("options.dev_disableRenderParticles");
	if (renderParticlesOption)
		noParticleSetting = (bool*)&renderParticlesOption->valueI;

	if (noWeatherSetting)
		*noWeatherSetting = noWeather;
	if (noEntitySetting)
		*noEntitySetting = noEntities;
	if (noBlockEntitySetting)
		*noBlockEntitySetting = noBlockEntities;
	if (noParticleSetting)
		*noParticleSetting = noParticles;
}

void NoRender::onDisable() {
	if (noWeatherSetting)
		*noWeatherSetting = 0;
	if (noEntitySetting)
		*noEntitySetting = 0;
	if (noBlockEntitySetting)
		*noBlockEntitySetting = 0;
	if (noParticleSetting)
		*noParticleSetting = 0;
}