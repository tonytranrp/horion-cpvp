#pragma once

#include "../../Memory/GameData.h"
#include "Horion\Module\Modules\Combat\Aimbot.h" 
#include "Horion\Module\Modules\Combat\AntiCrystal.h" 
#include "Horion\Module\Modules\Combat\AutoClicker.h" 
#include "Horion\Module\Modules\Combat\BowAimbot.h"
#include "Horion\Module\Modules\Combat\BowSpam.h"
#include "Horion\Module\Modules\Combat\Criticals.h" 
#include "Horion\Module\Modules\Combat\CrystalAura.h"
#include "Horion\Module\Modules\Combat\GhostHand.h" 
#include "Horion\Module\Modules\Combat\Hitbox.h" 
#include "Horion\Module\Modules\Combat\InfiniteAura.h" 
#include "Horion\Module\Modules\Combat\Killaura.h" 
#include "Horion\Module\Modules\Combat\Reach.h" 
#include "Horion\Module\Modules\Combat\Targethud.h" 
#include "Horion\Module\Modules\Combat\Teams.h" 
#include "Horion\Module\Modules\Combat\TriggerBot.h" 
#include "Horion\Module\Modules\Gui\ArmorHud.h" 
#include "Horion\Module\Modules\Gui\Arraylist.h" 
#include "Horion\Module\Modules\Gui\ClickGuiMod.h" 
#include "Horion\Module\Modules\Gui\Compass.h" 
#include "Horion\Module\Modules\Gui\HudEditor.h" 
#include "Horion\Module\Modules\Gui\HudModule.h" 
#include "Horion\Module\Modules\Gui\InventoryViewer.h" 
#include "Horion\Module\Modules\Gui\Keystrokes.h" 
#include "Horion\Module\Modules\Gui\Radar.h" 
#include "Horion\Module\Modules\Gui\TabGuiMod.h" 
#include "Horion\Module\Modules\Gui\Watermark.h" 
#include "Horion\Module\Modules\Misc\Chat.h" 
#include "Horion\Module\Modules\Misc\Crasher.h" 
#include "Horion\Module\Modules\Misc\Derp.h" 
#include "Horion\Module\Modules\Misc\DeviceIDSpoofer.h" 
#include "Horion\Module\Modules\Misc\Disabler.h" 
#include "Horion\Module\Modules\Misc\EditionFaker.h" 
#include "Horion\Module\Modules\Misc\Freecam.h"  
#include "Horion\Module\Modules\Misc\MurderTool.h" 
#include "Horion\Module\Modules\Misc\NoPacket.h" 
#include "Horion\Module\Modules\Misc\NoPaintingCrash.h"  
#include "Horion\Module\Modules\Misc\ShulkerNesting.h" 
#include "Horion\Module\Modules\Misc\SkinStealer.h" 
#include "Horion\Module\Modules\Misc\Spammer.h" 
#include "Horion\Module\Modules\Misc\Timer.h" 
#include "Horion\Module\Modules\Module.h" 
#include "Horion\Module\Modules\Movement\AirJump.h" 
#include "Horion\Module\Modules\Movement\AntiVoid.h" 
#include "Horion\Module\Modules\Movement\AutoSneak.h" 
#include "Horion\Module\Modules\Movement\AutoSprint.h" 
#include "Horion\Module\Modules\Movement\Bhop.h" 
#include "Horion\Module\Modules\Movement\ElytraFly.h" 
#include "Horion\Module\Modules\Movement\FastLadder.h" 
#include "Horion\Module\Modules\Movement\FastStop.h" 
#include "Horion\Module\Modules\Movement\Fly.h" 
#include "Horion\Module\Modules\Movement\FollowPathModule.h" 
#include "Horion\Module\Modules\Movement\Glide.h" 
#include "Horion\Module\Modules\Movement\HighJump.h" 
#include "Horion\Module\Modules\Movement\InventoryMove.h" 
#include "Horion\Module\Modules\Movement\JavaSneak.h" 
#include "Horion\Module\Modules\Movement\Jesus.h" 
#include "Horion\Module\Modules\Movement\Jetpack.h" 
#include "Horion\Module\Modules\Movement\NoClip.h" 
#include "Horion\Module\Modules\Movement\NoFall.h" 
#include "Horion\Module\Modules\Movement\NoSlowDown.h" 
#include "Horion\Module\Modules\Movement\Phase.h" 
#include "Horion\Module\Modules\Movement\Speed.h" 
#include "Horion\Module\Modules\Movement\Spider.h" 
#include "Horion\Module\Modules\Movement\Step.h" 
#include "Horion\Module\Modules\Movement\ToggleSneak.h" 
#include "Horion\Module\Modules\Movement\Velocity.h" 
#include "Horion\Module\Modules\Movement\VHop.h" 
#include "Horion\Module\Modules\Player\AntiBot.h" 
#include "Horion\Module\Modules\Player\AntiEffect.h" 
#include "Horion\Module\Modules\Player\AntiImmobile.h" 
#include "Horion\Module\Modules\Player\AutoArmor.h" 
#include "Horion\Module\Modules\Player\AutoDisconnect.h" 
#include "Horion\Module\Modules\Player\AutoMine.h" 
#include "Horion\Module\Modules\Player\AutoRespawn.h" 
#include "Horion\Module\Modules\Player\AutoTool.h"  
#include "Horion\Module\Modules\Player\Blink.h"
#include "Horion\Module\Modules\Player\BlockReach.h"
#include "Horion\Module\Modules\Player\ClickTP.h" 
#include "Horion\Module\Modules\Player\DeathPosition.h" 
#include "Horion\Module\Modules\Player\FastEat.h" 
#include "Horion\Module\Modules\Player\FastThrow.h" 
#include "Horion\Module\Modules\Player\HealthCheck.h" 
#include "Horion\Module\Modules\Player\InventoryCleaner.h" 
#include "Horion\Module\Modules\Player\JavaInv.h" 
#include "Horion\Module\Modules\Player\MidClick.h" 
#include "Horion\Module\Modules\Player\Nbt.h" 
#include "Horion\Module\Modules\Player\NoFriends.h" 
#include "Horion\Module\Modules\Player\NoSwing.h"
#include "Horion\Module\Modules\Player\Offhand.h"
#include "Horion\Module\Modules\Player\StackableItem.h"
#include "Horion\Module\Modules\Visual\BlockOutline.h" 
#include "Horion\Module\Modules\Visual\BoneESP.h" 
#include "Horion\Module\Modules\Visual\Breadcrumbs.h" 
#include "Horion\Module\Modules\Visual\CameraNoClip.h" 
#include "Horion\Module\Modules\Visual\ChestESP.h" 
#include "Horion\Module\Modules\Visual\CustomSky.h" 
#include "Horion\Module\Modules\Visual\DVDLogo.h" 
#include "Horion\Module\Modules\Visual\EntityDespawner.h" 
#include "Horion\Module\Modules\Visual\ESP.h" 
#include "Horion\Module\Modules\Visual\Freelook.h" 
#include "Horion\Module\Modules\Visual\FullBright.h"  
#include "Horion\Module\Modules\Visual\NameTags.h"  
#include "Horion\Module\Modules\Visual\NoHurtcam.h"
#include "Horion\Module\Modules\Visual\NoRender.h"
#include "Horion\Module\Modules\Visual\Swing.h" 
#include "Horion\Module\Modules\Visual\TimeChanger.h" 
#include "Horion\Module\Modules\Visual\Tracer.h" 
#include "Horion\Module\Modules\Visual\TrueSight.h" 
#include "Horion\Module\Modules\Visual\ViewModel.h" 
#include "Horion\Module\Modules\Visual\Waypoints.h" 
#include "Horion\Module\Modules\Visual\Zoom.h" 
#include "Horion\Module\Modules\World\AutoEat.h" 
#include "Horion\Module\Modules\World\AutoFish.h" 
#include "Horion\Module\Modules\World\AutoGapple.h" 
#include "Horion\Module\Modules\World\AutoMove.h" 
#include "Horion\Module\Modules\World\ChestAura.h" 
#include "Horion\Module\Modules\World\ChestStealer.h" 
#include "Horion\Module\Modules\World\EntityFly.h" 
#include "Horion\Module\Modules\World\EntityJesus.h" 
#include "Horion\Module\Modules\World\EntitySpeed.h" 
#include "Horion\Module\Modules\World\Fucker.h" 
#include "Horion\Module\Modules\World\InstaBreak.h" 
#include "Horion\Module\Modules\World\Nuker.h" 
#include "Horion\Module\Modules\World\Scaffold.h" 
#include "Horion\Module\Modules\World\SignEdit.h" 
#include "Horion\Module\Modules\World\Surround.h" 
#include "Horion\Module\Modules\World\SwingSounds.h" 
#include "Horion\Module\Modules\World\Tower.h" 
#include "Horion\Module\Modules\World\Xray.h" 

#ifdef HORION_DEBUG
#include "Horion\Module\Modules\Combat\AutoPot.h" 
#include "Horion\Module\Modules\Misc\InvLogger.h"
#include "Horion\Module\Modules\Misc\PacketLogger.h"
#include "Horion\Module\Modules\Misc\TestModule.h"
#include "Horion\Module\Modules\Visual\BlockESP.h"
#include "Horion\Module\Modules\Visual\Minimap.h"
#endif

class ModuleManager {
private:
	GameData* gameData;
	std::vector<std::shared_ptr<IModule>> moduleList;
	bool initialized = false;
	std::shared_mutex moduleListMutex;

public:
	~ModuleManager();
	ModuleManager(GameData* gameData);
	void initModules();
	void disable();
	void onLoadConfig(void* conf);
	void onSaveConfig(void* conf);
	void onTick(GameMode* gameMode);
	void onAttack(Entity* attackedEnt);
	void onKeyUpdate(int key, bool isDown);
	void onPreRender(MinecraftUIRenderContext* renderCtx);
	void onPostRender(MinecraftUIRenderContext* renderCtx);
	void onLevelRender();
	void onClientInputCallbacksTick(ClientInputCallbacks* CIC);
	void onMove(MoveInputHandler* handler);
	void onSendPacket(Packet*);

	std::shared_lock<std::shared_mutex> lockModuleList() { return std::shared_lock(this->moduleListMutex); }
	std::unique_lock<std::shared_mutex> lockModuleListExclusive() { return std::unique_lock(this->moduleListMutex); }

	std::shared_mutex* getModuleListLock() { return &this->moduleListMutex; }

	bool isInitialized() { return initialized; };
	std::vector<std::shared_ptr<IModule>>* getModuleList();

	int getModuleCount();
	int getEnabledModuleCount();

	/*
	 *	Use as follows:
	 *		IModule* mod = moduleMgr.getModule<NoKnockBack>();
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !
	 */
	template <typename TRet>
	[[deprecated]] TRet* getModule() {
#if 0
		if (!isInitialized())
			return nullptr;
		auto lock = this->lockModuleList();
		for (auto pMod : moduleList) {
			if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod.get())) {
				return pRet;
			}
		}
#endif
		return nullptr;
	};

	template <typename TRet>
	// TODO: Module id system
	TRet* getModule(const char* modName) {
		if (!isInitialized())
			return nullptr;
		auto lock = this->lockModuleList();
		for (auto pMod : moduleList) {
			if (!strcmp(pMod->getRawModuleName(), modName)) {
				return reinterpret_cast<TRet*>(pMod.get());
			}
		}
		return nullptr;
	}

	// Dont Use this functions unless you absolutely need to. The function above this one works in 99% of cases
	std::optional<std::shared_ptr<IModule>> getModuleByName(const std::string name) {
		if (!isInitialized())
			return nullptr;
		std::string nameCopy = name;
		std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);

		auto lock = this->lockModuleList();
		for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
			std::shared_ptr<IModule> mod = *it;
			std::string modNameCopy = mod->getRawModuleName();
			std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
			if (modNameCopy == nameCopy)
				return std::optional<std::shared_ptr<IModule>>(mod);
		}
		return std::optional<std::shared_ptr<IModule>>();
	}
};

extern ModuleManager* moduleMgr;
