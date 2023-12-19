#include "GameData.h"

#include "../Utils/Logger.h"
#include "../Utils/Utils.h"
#include "Hooks.h"

GameData Game;

size_t AABBHasher::operator()(const AABB& i) const {
	return Utils::posToHash(i.lower);
}

void GameData::retrieveClientInstance() {
	static uintptr_t mainViewOffset = 0x0;

	if (!mainViewOffset) {
		uintptr_t sigOffset = FindSignature("48 3B 3D ? ? ? ? 74 ? 48 8B 07 48 8B CF");

		auto offset = *reinterpret_cast<int*>(sigOffset + 3);
		mainViewOffset = (uintptr_t)(sigOffset + offset + 7);

		while (!(*reinterpret_cast<MainView**>(mainViewOffset))) {
			debugLogF("you dumb stupid idiot why are you injecting when the game hasnt been loaded yet?");
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		debugLogF("MainView: %llX", mainViewOffset);
	}
	MainView::instance = *(MainView**)mainViewOffset;
	Game.clientInstance = MainView::instance->platform->minecraftGame->getPrimaryClientInstance();

	if (!Game.clientInstance)
		debugLogF("Client Instance is 0");
}

bool GameData::canUseMoveKeys() {
	MinecraftGame* mc = Game.clientInstance->getMinecraftGame();
	if (!mc) {
		return false;
	}

	return mc->canUseKeybinds();
}

void GameData::sendKey(uint64_t key, bool isDown) {
	using sendKey_t = void (*)(uint64_t, bool);
	static sendKey_t sendKey = Utils::FuncFromSigOffset<sendKey_t>(FindSignature("E8 ? ? ? ? EB 77 0F BE 46 16 "), 1);

	return sendKey(key, isDown);
}

bool GameData::isKeyDown(int key) {
	static uintptr_t keyMapOffset = 0x0;
	if (!keyMapOffset) {
		uintptr_t sigOffset = FindSignature("4C 8D 05 ?? ?? ?? ?? 89 54 24 20 88 4C 24 24");
		if (sigOffset) {
			int offset = *reinterpret_cast<int*>((sigOffset + 3));                                         // Get Offset from code
			keyMapOffset = sigOffset - Game.gameBase + offset + /*length of instruction*/ 7;  // Offset is relative
			debugLogF("KeyMap: %llX", keyMapOffset + Game.gameBase);
		}
	}
	// All keys are mapped as bools, though aligned as ints (4 byte)
	// key0 00 00 00 key1 00 00 00 key2 00 00 00 ...
	return *reinterpret_cast<bool*>(Utils::getBase() + keyMapOffset + ((uintptr_t)key * 0x4));
	return false;
}

bool GameData::isKeyPressed(int key) {
	if (isKeyDown(key)) {
		while (isKeyDown(key))
			Sleep(1);
		return true;
	}
	return false;
}

bool GameData::isRightClickDown() {
	if (Game.hidController == 0)
		return false;
	return Game.hidController->rightClickDown;
}

bool GameData::isLeftClickDown() {
	if (Game.hidController == 0)
		return false;
	return Game.hidController->leftClickDown;
}

bool GameData::isWheelDown() {
	if (Game.hidController == 0)
		return false;
	return Game.hidController->wheelDown;
}

bool GameData::shouldTerminate() {
	return Game.shouldTerminateB;
}

void GameData::terminate() {
	Game.shouldTerminateB = true;
}

bool GameData::shouldHide() {
	return Game.shouldHideB;
}

void GameData::hide() {
	Game.shouldHideB = !(Game.shouldHideB);
}

void GameData::updateGameData(GameMode* gameMode) {
	retrieveClientInstance();
	Game.localPlayer = Game.getLocalPlayer();

	if (Game.localPlayer && gameMode->player == Game.localPlayer) {
		Game.gameMode = gameMode;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&Game.lastUpdate));

		if (Game.localPlayer) {
			GuiData* guiData = Game.clientInstance->getGuiData();

			if (guiData) {
				{
					auto vecLock = Logger::GetTextToPrintLock();
					auto* stringPrintVector = Logger::GetTextToPrint();
#ifdef HORION_DEBUG
					int numPrinted = 0;
					std::vector<TextForPrint>::iterator it;
					for (it = stringPrintVector->begin(); it != stringPrintVector->end(); ++it) {
						numPrinted++;
						if (numPrinted > 20) {
							break;
						}

						guiData->displayClientMessageF("%s[%sHorion%s] - %s%s", GRAY, AQUA, GRAY, RESET, it->text);
					}
					stringPrintVector->erase(stringPrintVector->begin(), it);
#else
					stringPrintVector->clear();
#endif
				}
				{
					auto lock = std::lock_guard<std::mutex>(Game.textPrintLock);

					auto& stringPrintVector = Game.textPrintList;
					int numPrinted = 0;
					std::vector<std::string>::iterator it;
					for (it = stringPrintVector.begin(); it != stringPrintVector.end(); ++it) {
						numPrinted++;
						if (numPrinted > 20) {
							break;
						}

						guiData->displayClientMessageF(it->c_str());
					}
					stringPrintVector.erase(stringPrintVector.begin(), it);
				}
			}
		}
	}
}

void GameData::setHIDController(HIDController* Hid) {
	Game.hidController = Hid;
}

void GameData::setClientInputCallbacks(ClientInputCallbacks* callbacks) {
	Game.clientInputCallbacks = callbacks;
}

void GameData::forEachEntity(std::function<void(Entity*)> callback) {
	auto lvl = Game.getLevel();
	if (lvl) {
		auto list = lvl->getRuntimeActorList();
		std::function<bool __cdecl(Player&)> functor = [&list](Player& p) -> bool {
			list.push_back(&p);
			return true;
		};
		lvl->forEachPlayer(functor);

		for (auto actor : list) {
			callback(actor);
		}
	}
}

void GameData::addChestToList(ChestBlockActor* chest) {
	if (!chest || !chest->isMainSubchest())
		return;
	AABB chestAabb = chest->getFullAABB();
	std::lock_guard<std::mutex> listGuard(Game.chestListMutex);
	if (Game.chestList.count(chestAabb) > 0)
		return;

	Game.chestList.insert(chestAabb);
}

void GameData::initGameData(void* hDllInst) {
	Game.gameBase = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");
	Game.hDllInst = hDllInst;
	retrieveClientInstance();
	debugLogF("======================================");
	debugLogF("Base: %llX", Game.gameBase);
	debugLogF("ClientInstance %llX", Game.clientInstance);
	if (Game.clientInstance) {
		debugLogF("Minecraft: %llX", Game.getMinecraft());
		debugLogF("MinecraftGame: %llX", Game.getMinecraftGame());
		debugLogF("GuiData: %llX", Game.getGuiData());
		debugLogF("MoveInputHandler: %llX", Game.getMoveInputHandler());
		debugLogF("LoopbackPacketSender: %llX", Game.getLoopbackPacketSender());
		debugLogF("RakNetConnector: %llX", Game.getRakNetConnector());
		debugLogF("NetworkHandler: %llX", Game.getNetworkHandler());
		debugLogF("GameSettings: %llX", Game.getGameSettingsInput());
		
		if (Game.getLocalPlayer()) {
			debugLogF("LocalPlayer %llX", Game.getLocalPlayer());
			debugLogF("GameMode: %llX", Game.getLocalPlayer()->getGameMode());
			debugLogF("Level: %llX", Game.getLevel());
			debugLogF("Dimension: %llX", Game.getDimension());
			debugLogF("LevelRenderer: %llX", Game.getLevelRenderer());
			debugLogF("LevelRendererPlayer: %llX", Game.getLevelRendererPlayer());
			debugLogF("Region: %llX", Game.getRegion());
			debugLogF("Supplies: %llX", Game.getSupplies());
			debugLogF("Inventory: %llX", Game.getInventory());
			debugLogF("TransactionManager: %llX", Game.getTransactionManager());
		}
		debugLogF("======================================");
	}
}

void GameData::log(const char* fmt, ...) {
	std::scoped_lock PRINT_MUTEX(Game.textPrintLock);
	va_list arg;
	va_start(arg, fmt);
	static char msgBuf[300]{};
	vsprintf_s(msgBuf, sizeof(msgBuf), fmt, arg);
	Game.textPrintList.emplace_back(msgBuf);
	va_end(arg);
}