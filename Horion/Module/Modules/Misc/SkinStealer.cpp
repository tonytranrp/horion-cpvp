#include "SkinStealer.h"

SkinStealer::SkinStealer() : IModule(0, Category::MISC, "Download other people's skins!") {
	registerBoolSetting("Steal Capes", &includeCapes, includeCapes);
#ifdef HORION_DEBUG
	registerBoolSetting("Steal Geometry", &includeGeo, includeGeo);
#endif
}

SkinStealer::~SkinStealer() {
}

const char* SkinStealer::getModuleName() {
	return ("SkinStealer");
}

void SkinStealer::onTick(GameMode* gm) {
	if (!GameData::canUseMoveKeys()) {
		return;
	}

	if (GameData::isRightClickDown()) {

		auto target = Game.getLevel()->getLookingAtEntity();
		if (Game.getLevel()->isLookingAtEntity() && target->isPlayer()) {
			const auto& player = *reinterpret_cast<Player*>(target);
			const auto& skin = player.skin;

			std::string path = Utils::getRoamingStatePath();
			path += "\\Horion\\StolenSkins\\";

			std::filesystem::create_directory(path.c_str());
			std::string text = target->getNameTag().c_str();
			text = Utils::sanitize(text);
			std::string pattern("\n");
			size_t index;
			while ((index = text.find_first_of(pattern)) != std::string::npos)
				text = text.substr(0, index);
			std::string filePath = path + text + "-Skin.png";
			std::string filePath2 = path + text + "-Cape.png";

			FILE* fp;
			errno_t err = fopen_s(&fp, filePath.c_str(), "wb");
			svpng(fp, skin.skinWidth, skin.skinHeight, skin.skinData, 1);
			fclose(fp);

			if (this->includeGeo) {
				std::string filePath3 = path + text + "-Geo.json";
				std::ofstream geoFile;
				std::string out;
				geoFile.open(filePath3);
				geoFile << skin.skinGeometry.toStyledString(&out)->c_str();
				geoFile.close();
			}

			if (this->includeCapes) {
				if (skin.capeWidth > 1) {
					FILE* fp;
					errno_t err = fopen_s(&fp, filePath2.c_str(), "wb");
					svpng(fp, skin.capeWidth, skin.capeHeight, skin.capeData, 1);
					fclose(fp);
				}
			}
			Utils::setClipboardText(path);
			clientMessageF("%sSkin Saved! %sDo WIN+R and paste to get to the skin.", GREEN, GRAY);
		}
	}
}