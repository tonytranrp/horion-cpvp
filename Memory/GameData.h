#pragma once

#include "../SDK/ChestBlockActor.h"
#include "../SDK/ClientInputCallbacks.h"
#include "../SDK/ClientInstance.h"
#include "../SDK/GameMode.h"
#include "../SDK/HIDController.h"
#include "../SDK/MoveInputHandler.h"
#include "../SDK/RakNetConnector.h"
#include "../SDK/Level.h"
#include "../SDK/Item.h"
#include "../Utils/TextFormat.h"
#include <optional>

struct AABBHasher {
	size_t operator()(const AABB& i) const;
};

class GameData {
private:
	ClientInstance* clientInstance = nullptr;
	LocalPlayer* localPlayer = nullptr;
	GameMode* gameMode = nullptr;
	HIDController* hidController = nullptr;
	ClientInputCallbacks* clientInputCallbacks = nullptr;
	uintptr_t gameBase;
	void* hDllInst = 0;
	std::unordered_set<AABB, AABBHasher> chestList;
	std::vector<std::string> textPrintList;
	std::mutex textPrintLock;
	std::mutex chestListMutex;
	bool shouldTerminateB = false;
	bool shouldHideB = false;
	__int64 lastUpdate;
	std::optional<std::string> fakeName;

	static void retrieveClientInstance();

public:
	float fov = 0.f;
	std::vector<__int64> frames{};
	std::vector<__int64> cpsLeft{};
	std::vector<__int64> cpsRight{};
	//std::vector<const char*> socketMessages;

	static bool canUseMoveKeys();
	static void sendKey(uint64_t key, bool isDown);
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);
	static bool isRightClickDown();
	static bool isLeftClickDown();
	static bool isWheelDown();
	static bool shouldTerminate();
	static bool shouldHide();
	static void hide();
	static void terminate();
	static void updateGameData(GameMode* gameMode);
	static void initGameData(void* hDllInst);
	static void addChestToList(ChestBlockActor* ChestBlock2);
	static void setHIDController(HIDController* Hid);
	static void setClientInputCallbacks(ClientInputCallbacks* callbacks);
	static void log(const char* fmt, ...);
	
	inline __int64 getLastUpdateTime() { return lastUpdate; };

	void clearChestList() {
		std::lock_guard<std::mutex> listGuard(chestListMutex);
		this->chestList.clear();
	}
	std::unordered_set<AABB, AABBHasher>& getChestList() { return chestList; };
	auto lockChestList() { return std::lock_guard<std::mutex>(this->chestListMutex); }

	inline uintptr_t getModuleBase() { return gameBase; };
	
	inline void* getDllModule() { return hDllInst; };
	
	bool isInGame() {
		return localPlayer;
	}

	inline void setFakeName(std::string&& name) { this->fakeName = {std::move(name)}; };
	inline void setFakeName(const std::string& name) { this->fakeName = {name}; };
	inline void resetFakeName() { this->fakeName.reset(); };
	inline std::optional<std::string>& getFakeName() { return this->fakeName; };

	void forEachEntity(std::function<void(Entity*)> callback);
	size_t getFPS() { return frames.size(); };
	size_t getLeftCPS() { return cpsLeft.size(); };
	size_t getRightCPS() { return cpsRight.size(); };

	inline ClientInstance* getClientInstance() { return clientInstance; }
	inline Minecraft* getMinecraft() { return clientInstance->getMinecraft(); }
	inline MinecraftGame* getMinecraftGame() { return clientInstance->getMinecraftGame(); }
	inline Level* getLevel() { return clientInstance->getMinecraft()->getLevel(); }
	inline LevelRenderer* getLevelRenderer() { return clientInstance->getLevelRenderer(); }
	inline LevelRendererPlayer* getLevelRendererPlayer() { return clientInstance->getLevelRenderer()->getLevelRendererPlayer(); }
	inline GuiData* getGuiData() { return clientInstance->getGuiData(); }
	inline LoopbackPacketSender* getLoopbackPacketSender() { return clientInstance->getLoopbackPacketSender(); }
	inline GameSettingsInput* getGameSettingsInput() { return clientInstance->getGameSettingsInput(); }
	inline MoveInputHandler* getMoveInputHandler() { return clientInstance->getMoveTurnInput(); }
	inline GameMode* getGameMode() { return gameMode; }
	inline ClientInputCallbacks* getClientInputCallbacks() { return clientInputCallbacks; }
	inline HIDController** getHIDController() { return &hidController; }
	inline NetworkHandler* getNetworkHandler() { return clientInstance->getLoopbackPacketSender()->getNetworkHandler(); }
	inline RakNetConnector* getRakNetConnector() { return clientInstance->getLoopbackPacketSender()->getNetworkHandler()->getRakNetConnector(); }
	inline LocalPlayer* getLocalPlayer() {
		localPlayer = clientInstance->getLocalPlayer();
		if (!localPlayer)
			gameMode = nullptr;
		return localPlayer;
	}
	inline LocalPlayer** getPtrLocalPlayer() {
		return &localPlayer;
	}
	inline Dimension* getDimension() {
		if (!localPlayer) return nullptr;
		return localPlayer->getDimension();
	}
	inline Weather* getWeather() {
		if (!localPlayer) return nullptr;
		auto dim = localPlayer->getDimension();
		if (!dim) return nullptr;
		return dim->weather.get();
	}
	inline BlockSource* getRegion() {
		return this->getClientInstance()->getRegion();
	}
	inline Inventory* getInventory() {
		if (!localPlayer)
			return 0;
		return localPlayer->getSupplies()->inventory;
	}
	inline InventoryTransactionManager* getTransactionManager() {
		if (!localPlayer)
			return 0;
		return localPlayer->getTransactionManager();
	}
	inline PlayerInventoryProxy* getSupplies() {
		if (!localPlayer)
			return 0;
		return localPlayer->getSupplies();
	}
	inline ItemRegistry* getItemRegistry() {
		return ItemRegistryManager::getItemRegistry();
	}
};

extern GameData Game;