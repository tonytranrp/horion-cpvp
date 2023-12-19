#include "ModuleManager.h"

#include "../../Utils/Json.hpp"
#include "../../Utils/Logger.h"
#include "Modules/Player/Damage.h"

using json = nlohmann::json;

ModuleManager::ModuleManager(GameData* gameData) {
	this->gameData = gameData;
}

ModuleManager::~ModuleManager() {
	initialized = false;
	auto lock = this->lockModuleListExclusive();
	this->moduleList.clear();
}

void ModuleManager::initModules() {
	logF("Initializing modules");
	{
		auto lock = this->lockModuleListExclusive();

		this->moduleList.push_back(std::make_shared<Aimbot>());
		this->moduleList.push_back(std::make_shared<AirJump>());
		this->moduleList.push_back(std::make_shared<AntiBot>());
		this->moduleList.push_back(std::make_shared<AntiCrystal>());
		this->moduleList.push_back(std::make_shared<AntiEffect>());
		this->moduleList.push_back(std::make_shared<AntiImmobile>());
		this->moduleList.push_back(std::make_shared<AntiVoid>());
		this->moduleList.push_back(std::make_shared<ArmorHud>());
		this->moduleList.push_back(std::make_shared<Arraylist>());
		this->moduleList.push_back(std::make_shared<AutoArmor>());
		this->moduleList.push_back(std::make_shared<AutoClicker>());
		this->moduleList.push_back(std::make_shared<AutoDisconnect>());
		this->moduleList.push_back(std::make_shared<AutoFish>());
		this->moduleList.push_back(std::make_shared<AutoMine>());
		this->moduleList.push_back(std::make_shared<AutoMove>());
		this->moduleList.push_back(std::make_shared<AutoRespawn>());
		this->moduleList.push_back(std::make_shared<AutoSneak>());
		this->moduleList.push_back(std::make_shared<AutoSprint>());
		this->moduleList.push_back(std::make_shared<AutoTool>());
		this->moduleList.push_back(std::make_shared<Bhop>());
		this->moduleList.push_back(std::make_shared<Blink>());
		this->moduleList.push_back(std::make_shared<BlockOutline>());
		this->moduleList.push_back(std::make_shared<BlockReach>());
		this->moduleList.push_back(std::make_shared<BoneESP>());
		this->moduleList.push_back(std::make_shared<BowAimbot>());
		this->moduleList.push_back(std::make_shared<BowSpam>());
		this->moduleList.push_back(std::make_shared<Breadcrumbs>());
		this->moduleList.push_back(std::make_shared<CameraNoClip>());
		this->moduleList.push_back(std::make_shared<Chat>());
		this->moduleList.push_back(std::make_shared<ChestAura>());
		this->moduleList.push_back(std::make_shared<ChestESP>());
		this->moduleList.push_back(std::make_shared<ChestStealer>());
		this->moduleList.push_back(std::make_shared<ClickGuiMod>());
		this->moduleList.push_back(std::make_shared<ClickTP>());
		this->moduleList.push_back(std::make_shared<Compass>());
		this->moduleList.push_back(std::make_shared<Crasher>());
		this->moduleList.push_back(std::make_shared<Criticals>());
		this->moduleList.push_back(std::make_shared<CrystalAura>());
		this->moduleList.push_back(std::make_shared<CustomSky>());
		this->moduleList.push_back(std::make_shared<Damage>());
		this->moduleList.push_back(std::make_shared<DeathPosition>());
		this->moduleList.push_back(std::make_shared<Derp>());
		this->moduleList.push_back(std::make_shared<DeviceIDSpoofer>());
		this->moduleList.push_back(std::make_shared<Disabler>());
		this->moduleList.push_back(std::make_shared<DVDLogo>());
		this->moduleList.push_back(std::make_shared<EditionFaker>());
		this->moduleList.push_back(std::make_shared<ElytraFly>());
		this->moduleList.push_back(std::make_shared<EntityDespawner>());
		this->moduleList.push_back(std::make_shared<EntityFly>());
		this->moduleList.push_back(std::make_shared<EntityJesus>());
		this->moduleList.push_back(std::make_shared<EntitySpeed>());
		this->moduleList.push_back(std::make_shared<ESP>());
		this->moduleList.push_back(std::make_shared<FastEat>());
		this->moduleList.push_back(std::make_shared<FastLadder>());
		this->moduleList.push_back(std::make_shared<FastStop>());
		this->moduleList.push_back(std::make_shared<FastThrow>());
		this->moduleList.push_back(std::make_shared<Swing>());
		this->moduleList.push_back(std::make_shared<Fly>());
		this->moduleList.push_back(std::make_shared<FollowPathModule>());
		this->moduleList.push_back(std::make_shared<Freecam>());
		this->moduleList.push_back(std::make_shared<Freelook>());
		this->moduleList.push_back(std::make_shared<Fucker>());
		this->moduleList.push_back(std::make_shared<FullBright>());
		this->moduleList.push_back(std::make_shared<Glide>());
		this->moduleList.push_back(std::make_shared<GhostHand>());
		this->moduleList.push_back(std::make_shared<HealthCheck>());
		this->moduleList.push_back(std::make_shared<HighJump>());
		this->moduleList.push_back(std::make_shared<Hitbox>());
		this->moduleList.push_back(std::make_shared<HudEditor>());
		this->moduleList.push_back(std::make_shared<HudModule>());
		this->moduleList.push_back(std::make_shared<InfiniteAura>());
		this->moduleList.push_back(std::make_shared<InstaBreak>());
		this->moduleList.push_back(std::make_shared<InventoryCleaner>());
		this->moduleList.push_back(std::make_shared<InventoryMove>());
		this->moduleList.push_back(std::make_shared<InventoryViewer>());
		this->moduleList.push_back(std::make_shared<JavaInv>());
		this->moduleList.push_back(std::make_shared<JavaSneak>());
		this->moduleList.push_back(std::make_shared<Jesus>());
		this->moduleList.push_back(std::make_shared<Jetpack>());
		this->moduleList.push_back(std::make_shared<Keystrokes>());
		this->moduleList.push_back(std::make_shared<Killaura>());
		this->moduleList.push_back(std::make_shared<MidClick>());
		this->moduleList.push_back(std::make_shared<MurderTool>());
		this->moduleList.push_back(std::make_shared<NameTags>());
		this->moduleList.push_back(std::make_shared<Nbt>());
		this->moduleList.push_back(std::make_shared<NoClip>());
		this->moduleList.push_back(std::make_shared<NoFall>());
		this->moduleList.push_back(std::make_shared<NoFriends>());
		this->moduleList.push_back(std::make_shared<NoHurtcam>());
		this->moduleList.push_back(std::make_shared<NoPacket>());
		this->moduleList.push_back(std::make_shared<NoPaintingCrash>());
		this->moduleList.push_back(std::make_shared<NoRender>());
		this->moduleList.push_back(std::make_shared<NoSlowDown>());
		this->moduleList.push_back(std::make_shared<NoSwing>());
		this->moduleList.push_back(std::make_shared<Offhand>());
		this->moduleList.push_back(std::make_shared<Nuker>());
		this->moduleList.push_back(std::make_shared<Phase>());
		this->moduleList.push_back(std::make_shared<Radar>());
		this->moduleList.push_back(std::make_shared<Reach>());
		this->moduleList.push_back(std::make_shared<Scaffold>());
		this->moduleList.push_back(std::make_shared<ShulkerNesting>());
		this->moduleList.push_back(std::make_shared<SkinStealer>());
		this->moduleList.push_back(std::make_shared<Spammer>());
		this->moduleList.push_back(std::make_shared<Speed>());
		this->moduleList.push_back(std::make_shared<Spider>());
		this->moduleList.push_back(std::make_shared<StackableItem>());
		this->moduleList.push_back(std::make_shared<Step>());
		this->moduleList.push_back(std::make_shared<Surround>());
		this->moduleList.push_back(std::make_shared<SwingSounds>());
		this->moduleList.push_back(std::make_shared<TabGuiMod>());
		this->moduleList.push_back(std::make_shared<Targethud>());
		this->moduleList.push_back(std::make_shared<Teams>());
		this->moduleList.push_back(std::make_shared<TimeChanger>());
		this->moduleList.push_back(std::make_shared<Timer>());
		this->moduleList.push_back(std::make_shared<ToggleSneak>());
		this->moduleList.push_back(std::make_shared<Tower>());
		this->moduleList.push_back(std::make_shared<Tracer>());
		this->moduleList.push_back(std::make_shared<TriggerBot>());
		this->moduleList.push_back(std::make_shared<TrueSight>());
		this->moduleList.push_back(std::make_shared<Velocity>());
		this->moduleList.push_back(std::make_shared<VHop>());
		this->moduleList.push_back(std::make_shared<ViewModel>());
		this->moduleList.push_back(std::make_shared<Watermark>());
		this->moduleList.push_back(std::make_shared<Waypoints>());
		this->moduleList.push_back(std::make_shared<Xray>());
		this->moduleList.push_back(std::make_shared<Zoom>());

	#ifdef HORION_DEBUG
		this->moduleList.push_back(std::make_shared<AutoPot>());
		this->moduleList.push_back(std::make_shared<BlockESP>());
		this->moduleList.push_back(std::make_shared<AutoEat>());
		this->moduleList.push_back(std::make_shared<AutoGapple>());
		this->moduleList.push_back(std::make_shared<InvLogger>());
		this->moduleList.push_back(std::make_shared<PacketLogger>());
		this->moduleList.push_back(std::make_shared<TestModule>());
		this->moduleList.push_back(std::make_shared<Minimap>());
		this->moduleList.push_back(std::make_shared<SignEdit>());
	#endif

		// Sort modules alphabetically
		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
			auto current = lhs;
			auto other = rhs;
			return std::string{*current->getModuleName()} < std::string{*other->getModuleName()};
		});

		initialized = true;
	}

	this->getModule<ClickGuiMod>("ClickGui")->setEnabled(false);
	this->getModule<AntiBot>("AntiBot")->setEnabled(true);
}

void ModuleManager::disable() {
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled())
			mod->setEnabled(false);
	}
}

void ModuleManager::onLoadConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onLoadConfig(conf);
	}

	this->getModule<ClickGuiMod>("ClickGui")->setEnabled(false);
	this->getModule<AntiBot>("AntiBot")->setEnabled(true);
}

void ModuleManager::onSaveConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onSaveConfig(conf);
	}
}

void ModuleManager::onTick(GameMode* gameMode) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onTick(gameMode);
	}
}

void ModuleManager::onAttack(Entity* attackEnt) {
	if (!isInitialized())
		return;

	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onAttack(attackEnt);
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = this->lockModuleList();

	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPreRender(renderCtx);
	}
}

void ModuleManager::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = this->lockModuleList();

	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPostRender(renderCtx);
	}
}

void ModuleManager::onSendPacket(Packet* packet) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onSendPacket(packet);
	}
}

std::vector<std::shared_ptr<IModule>>* ModuleManager::getModuleList() {
	return &this->moduleList;
}

int ModuleManager::getModuleCount() {
	return (int)moduleList.size();
}

int ModuleManager::getEnabledModuleCount() {
	int i = 0;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled()) i++;
	}
	return i;
}
void ModuleManager::onMove(MoveInputHandler* hand) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onMove(hand);
	}
}
void ModuleManager::onLevelRender() {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onLevelRender();
	}
}
void ModuleManager::onClientInputCallbacksTick(ClientInputCallbacks* CIC) {
	if (!isInitialized())
		return;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onClientInputCallbacksTick(CIC);
	}
}

ModuleManager* moduleMgr = new ModuleManager(&Game);
