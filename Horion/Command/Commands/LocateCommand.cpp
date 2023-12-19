#include <SDK/Biome.h>
#include "LocateCommand.h"

LocateCommand::LocateCommand() : IMCCommand("locate", "Finds the nearest biome or structure", "<structure/biome> <name>") {
	StructureMap["end_city"] = StructureFeatureType::END_CITY;
	StructureMap["nether_fortress"] = StructureFeatureType::NETHER_FORTRESS;
	StructureMap["mineshaft"] = StructureFeatureType::MINESHAFT;
	StructureMap["ocean_monument"] = StructureFeatureType::OCEAN_MONUMENT;
	StructureMap["stronghold"] = StructureFeatureType::STRONGHOLD;
	StructureMap["desert_temple"] = StructureFeatureType::DESERT_TEMPLE;
	StructureMap["village"] = StructureFeatureType::VILLAGE;
	StructureMap["mansion"] = StructureFeatureType::MANSION;
	StructureMap["ship_wreck"] = StructureFeatureType::SHIPWRECK;
	StructureMap["treasure"] = StructureFeatureType::BURIED_TREASURE;
	StructureMap["ocean_ruin"] = StructureFeatureType::OCEAN_RUINS;
	StructureMap["outpost"] = StructureFeatureType::PILLAGER_OUTPOST;
	StructureMap["ruined_portal"] = StructureFeatureType::RUINED_PORTAL;
	StructureMap["bastion"] = StructureFeatureType::BASTION_REMNANT;
	StructureMap["ancient_city"] = StructureFeatureType::ANCIENT_CITY;
}

LocateCommand::~LocateCommand() {
}

namespace {
std::string MakeStringLookNice(std::string Input) {
	std::string output = Input;
	bool hasSpace = true;
	for (unsigned int i = 0; i < output.length(); i++) {
		if (hasSpace) {
			output[i] = std::toupper(output[i]);
			hasSpace = false;
		}
		if (output.at(i) == *"_") {
			output.replace(i, 1, " ");
			hasSpace = true;
		}
	}
	return output;
}
}

bool LocateCommand::execute(std::vector<std::string>* args) {

	auto lp = Game.getLocalPlayer();
	if (!lp) return false;

	std::unique_ptr<WorldGenerator> worldGenerator;
	Game.getDimension()->createGenerator(&worldGenerator);
	if (!worldGenerator) {
		this->clientMessageF("%sSomething went wrong when trying to locate...", RED);
		return false;
	}

	std::string arg1 = args->at(1);
	Utils::toLowercase(arg1);
	Vec3i output;

	if (arg1 == "structure") {
		std::string arg2 = args->at(2);
		Utils::toLowercase(arg2);
		auto convertedString = StructureMap.find(arg2);
		if (convertedString == StructureMap.end()) {
			this->clientMessageF("%sThis structure feature does not exist!", RED);
			return false;
		}

		if (worldGenerator->findNearestStructureFeature(convertedString->second, *lp->getPos(), &output, false))
			clientMessageF("Found %s near (%i, ?, %i)", ::MakeStringLookNice(convertedString->first), output.x, output.z);
		return true;
	}
	if (arg1 == "biome") {
		std::string arg2 = args->at(2);
		Utils::toLowercase(arg2);

		auto coordz = BiomeSourceUtil::locateBiome(arg2, worldGenerator->getBiomeSource(), lp->getFeetBlockPos());
		if (coordz) {
			clientMessageF("Found %s near (%i, ?, %i)", ::MakeStringLookNice(arg2), (*coordz).x, (*coordz).z);
			return true;
		} else {
			this->clientMessageF("%sCould not locate biome!", RED);
			return false;
		}
	}

	return false;
}