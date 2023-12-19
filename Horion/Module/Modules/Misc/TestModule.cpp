#include "TestModule.h"
#include "../../../Config/ConfigManager.h"
#include "../SDK/Biome.h"
#include <winrt/windows.applicationmodel.core.h>

TestModule::TestModule() : IModule(0, Category::MISC, "For testing purposes") {
	registerColorSetting("Test Color", &this->color, this->color);
	registerVectorSetting("Test Vec2", &this->tv2, this->tv2, {-50, -50}, {50, 50});
	registerVectorSetting("Test Vec2I", &this->tv2i, this->tv2i, {-50, -50}, {50, 50});
	registerVectorSetting("Test Vec3", &this->tv3, this->tv3, {-50, -50, -50}, {50, 50, 50});
	registerVectorSetting("Test Vec3I", &this->tv3i, this->tv3i, {-50, -50, -50}, {50, 50, 50});
}

TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

void TestModule::onTick(GameMode* gm) {

	auto lp = gm->player;
	//this->clientMessageF("is sneaking: %d", (int)lp->isSneaking());
	//this->clientMessageF("is swimming: %d", (int)lp->isSwimming());
	//this->clientMessageF("health: %.3f\nabsorption: %.3f", lp->getHealth(), lp->getAbsorption());
	//this->clientMessageF("wasOnGround: %d, isOnGround: %d", lp->wasOnGround(), lp->isOnGround());

}

void TestModule::onPostRender(MinecraftUIRenderContext* renderCtx) {
	
}

void TestModule::onEnable() {
	
	auto lp = Game.getLocalPlayer();
	if (!lp) {
		this->setEnabled(false);
		return;
	}

	BinaryPacket::CreateAndSend(PacketID::Text, [](BinaryStream& stream) {
		stream.writeUnsignedChar((uint8_t)TextPacket::Type::Chat);
		stream.writeBool(false);
		stream.writeString(Game.getLocalPlayer()->playerName);
		stream.writeString("Test 2");
		stream.writeString(""); // xuid
	});

	this->clientMessageF("local player device id: %s", lp->deviceId.c_str());
	this->clientMessageF("local player build platform from player list: %d", static_cast<int32_t>(lp->getBuildPlatformFromPlayerList()));
	this->clientMessageF("local player name: %s", lp->playerName.c_str());
	this->clientMessageF("curr dimension name: %s", lp->getDimension()->name.c_str());
	this->clientMessageF("curr dimension id: %d", lp->getDimension()->id);
	this->clientMessageF("curr weather addy: %p", lp->getDimension()->weather.get());
	this->clientMessageF("region: %p", Game.getRegion());
	this->clientMessageF("health: %.2f, absorption: %.2f", lp->getHealth(), lp->getAbsorption());
	this->clientMessageF("pos: %.3f, %.3f, %.3f", lp->stateVector->pos.x, lp->stateVector->pos.y, lp->stateVector->pos.z);
	this->clientMessageF("runtime id: %llu", lp->getRuntimeId().value);
	this->clientMessageF("server ip: %s, server port: %i", Game.getRakNetConnector()->serverIp.c_str(), Game.getRakNetConnector()->serverPort);

	Vec3 belowPos{lp->getFeetBlockPos().x, lp->getFeetBlockPos().y - 1, lp->getFeetBlockPos().z};
	auto block = Game.getRegion()->getBlock(belowPos);
	this->clientMessageF("standing on block name: %s", block->blockLegacy->getRawNameStr());

	Game.forEachEntity([this](Entity* a) {
		uint64_t entCount = 0;
		this->clientMessageF("list # (%llu), runtime id: %llu, isPlayer: %d", ++entCount, a->getRuntimeId().value, a->isPlayer());
	});

	//this->clientMessageF("client side gen enabled: %d", Game.getLevel()->isClientSideGenerationEnabled());

	// version checker
	/*auto package = winrt::Windows::ApplicationModel::Package::Current();
	auto packageId = package.Id();
	auto packageVersion = packageId.Version();
	logF("Version: %i.%i.%i", packageVersion.Major, packageVersion.Minor, packageVersion.Build);*/

	//std::unique_ptr<WorldGenerator> worldGenerator;
	//Game.getDimension()->createGenerator(&worldGenerator);
	//this->clientMessageF("world generator: %p", worldGenerator.get());
	auto dim = Game.getDimension();
	auto dimVft = *(void**)dim;
	this->clientMessageF("dim vft: %p", Utils::getStaticAddress(dimVft));

	/*Vec3i output;
	if (BiomeSourceUtil::locateBiome("river", &output, worldGenerator->getBiomeSource(), lp->getFeetBlockPos())) {
		logF("River at (%i ? %i)", output.x, output.z);
	}

	Vec3i output;
	if (worldGenerator->findNearestStructureFeature(StructureFeatureType::STRONGHOLD, lp->getFeetBlockPos(), &output, false))
		logF("Stronghold at (%i ? %i)", output.x, output.z);*/
}