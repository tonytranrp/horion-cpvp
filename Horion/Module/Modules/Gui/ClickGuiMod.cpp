#include "ClickGuiMod.h"

#include "../../../../Utils/Utils.h"
#include "../../../../Utils/Json.hpp"
#include "../../../Menu/ClickGui.h"

ClickGuiMod::ClickGuiMod() : IModule(VK_INSERT, Category::GUI, "The clickgui - toggle everything by just clicking on it!") {
	registerBoolSetting("Show Tooltips", &this->showTooltips, this->showTooltips);
	font.addEntry(EnumEntry("Smooth", 0));
	font.addEntry(EnumEntry("Mojangles", 1));
	//font.addEntry(EnumEntry("Unicode", 2)); weird
	//font.addEntry(EnumEntry("Galactic", 3)); weird
	font.addEntry(EnumEntry("Ten", 2));
	registerEnumSetting("Font", &font, 0);
	style.addEntry(EnumEntry("Dark Accent", 0));
	style.addEntry(EnumEntry("Dark White", 1));
	style.addEntry(EnumEntry("Classic", 2));
	registerEnumSetting("Style", &style, 0);
}

ClickGuiMod::~ClickGuiMod() {
}

const char* ClickGuiMod::getModuleName() {
	return ("ClickGui");
}

void ClickGuiMod::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (!player) return;
	//int DeathTime = (int)player->getDeathTime();
	//if (this->isEnabled() && DeathTime > 0) {
	//	this->setEnabled(false); //if you die with clickgui enabled, this will disable it so you can click respawn.
	//}
}

void ClickGuiMod::onEnable() {
	Game.getClientInstance()->releaseCursor();
}

bool ClickGuiMod::allowAutoStart() {
	return false;
}

void ClickGuiMod::onDisable() {
	Game.getClientInstance()->grabCursor();
}

void ClickGuiMod::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys())
		Game.getClientInstance()->releaseCursor();
}
void ClickGuiMod::onLoadConfig(void* conf) {
	IModule::onLoadConfig(conf);
	ClickGui::onLoadConfig(conf);
}
void ClickGuiMod::onSaveConfig(void* confVoid) {
	nlohmann::json* conf = reinterpret_cast<nlohmann::json*>(confVoid);
	nlohmann::json obj = {};
	for (auto sett : *this->getSettings()) {
		switch (sett->valueType) {
		case ValueType::FLOAT_T:
			obj.emplace(sett->name, sett->value->_float);
			break;
		case ValueType::DOUBLE_T:
			obj.emplace(sett->name, sett->value->_double);
			break;
		case ValueType::INT64_T:
			obj.emplace(sett->name, sett->value->int64);
			break;
		case ValueType::INT_T:
			obj.emplace(sett->name, sett->value->_int);
			break;
		case ValueType::BOOL_T:
			obj.emplace(sett->name, sett->value->_bool);
			break;
		case ValueType::TEXT_T:
			obj.emplace(sett->name, *sett->value->text);
			break;
		case ValueType::ENUM_T:
			obj.emplace(sett->name, sett->value->_int);
			break;
		}
	}
	ClickGui::onSaveConfig(conf, &obj);

	conf->emplace(this->getRawModuleName(), obj);
}