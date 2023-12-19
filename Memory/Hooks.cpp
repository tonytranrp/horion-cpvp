#include "Hooks.h"

#include "../SDK/Tag.h"
#include "../resource.h"
#include <chrono>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#pragma comment(lib, "windowsapp")

Hooks g_Hooks;
bool isTicked = false;
int frameIndex = 0;

//#define TEST_DEBUG

void Hooks::Init() {
	logF("Setting up Hooks...");
	// clang-format off

	// Signatures
	{
		void* UIScene_render = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B DA 48 8B F9 B9"));
		g_Hooks.UIScene_renderHook = std::make_unique<FuncHook>(UIScene_render, Hooks::UIScene_render);
		g_Hooks.UIScene_renderHook->enableHook();

		void* GameRenderer_renderCurrentFrame = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 0F 29 70 ?? 0F 29 78 ?? 44 0F 29 40 ?? 44 0F 29 48 ?? 44 0F 29 90 ?? ?? ?? ?? 44 0F 29 98 ?? ?? ?? ?? 44 0F 29 A0 ?? ?? ?? ?? 44 0F 29 A8 ?? ?? ?? ?? 44 0F 29 B0 ?? ?? ?? ?? 44 0F 29 B8 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 0F 28 F1"));
		g_Hooks.GameRenderer_renderCurrentFrameHook = std::make_unique<FuncHook>(GameRenderer_renderCurrentFrame, Hooks::GameRenderer_renderCurrentFrame);
		g_Hooks.GameRenderer_renderCurrentFrameHook->enableHook();

		void* ScreenView_setupAndRender = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 0F 29 70 ?? 0F 29 78 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4C 8B F2"));
		g_Hooks.ScreenView_setupAndRenderHook = std::make_unique<FuncHook>(ScreenView_setupAndRender, Hooks::ScreenView_setupAndRender);
		g_Hooks.ScreenView_setupAndRenderHook->enableHook();

		void* MinecraftUIRenderContext_drawImage = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4D 8B F1 49 8B F0 48 8B EA 48 8B F9"));
		g_Hooks.MinecraftUIRenderContext_drawImageHook = std::make_unique<FuncHook>(MinecraftUIRenderContext_drawImage, Hooks::MinecraftUIRenderContext_drawImage);
		g_Hooks.MinecraftUIRenderContext_drawImageHook->enableHook();

		void* MinecraftUIRenderContext_drawNineslice = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 7C 24 ? 55 48 8B EC 48 83 EC 60 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F8 49 8D 98 ? ? ? ? 48 89 4D E8 F3 0F 10 1B 48 8D 45 E8"));
		g_Hooks.MinecraftUIRenderContext_drawNinesliceHook = std::make_unique<FuncHook>(MinecraftUIRenderContext_drawNineslice, Hooks::MinecraftUIRenderContext_drawNineslice);
		g_Hooks.MinecraftUIRenderContext_drawNinesliceHook->enableHook();

		void* LevelRenderer_renderLevel = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B F8 48 8B DA"));
		g_Hooks.LevelRenderer_renderLevelHook = std::make_unique<FuncHook>(LevelRenderer_renderLevel, Hooks::LevelRenderer_renderLevel);
		g_Hooks.LevelRenderer_renderLevelHook->enableHook();

		void* clickHook = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9"));
		g_Hooks.ClickFuncHook = std::make_unique<FuncHook>(clickHook, Hooks::ClickFunc);
		g_Hooks.ClickFuncHook->enableHook();

		void* HIDController_keyMouse = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC ? ? ? ? 0F 29 74 24 ? 0F 29 7C 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F8"));
		g_Hooks.HIDController_keyMouseHook = std::make_unique<FuncHook>(HIDController_keyMouse, Hooks::HIDController_keyMouse);
		g_Hooks.HIDController_keyMouseHook->enableHook();

		void* keyHook = reinterpret_cast<void*>(FindSignature("48 ? ? 48 ? ? ? 4C 8D 05 ? ? ? ? 89"));
		g_Hooks.keyHookHook = std::make_unique<FuncHook>(keyHook, Hooks::keyHook);
		g_Hooks.keyHookHook->enableHook();

		void* ClientInstanceScreenModel_sendChatMessage = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B E2 4C 8B E9 48 8B 49"));
		g_Hooks.ClientInstanceScreenModel_sendChatMessageHook = std::make_unique<FuncHook>(ClientInstanceScreenModel_sendChatMessage, Hooks::ClientInstanceScreenModel_sendChatMessage);
		g_Hooks.ClientInstanceScreenModel_sendChatMessageHook->enableHook();

		void* MinecraftPackets_createPacket = reinterpret_cast<void*>(FindSignature("40 53 48 83 EC ? ? ? 48 8B D9 ? ? ? ? ? ? 0F 87 ? ? ? ? 48 63 C2 48 8D 0D ? ? ? ? 8B 94 81 ? ? ? ? 48 03 D1"));
		g_Hooks.MinecraftPackets_createPacketHook = std::make_unique<FuncHook>(MinecraftPackets_createPacket, Hooks::MinecraftPackets_createPacket);
		g_Hooks.MinecraftPackets_createPacketHook->enableHook();

		void* fogColorFunc = reinterpret_cast<void*>(FindSignature("41 0F 10 08 48 8B C2 0F"));
		g_Hooks.Dimension_getFogColorHook = std::make_unique<FuncHook>(fogColorFunc, Hooks::Dimension_getFogColor);
		g_Hooks.Dimension_getFogColorHook->enableHook();

		void* timeOfDay = reinterpret_cast<void*>(FindSignature("44 8B C2 B8 F1 19 76 05 F7 EA"));
		g_Hooks.Dimension_getTimeOfDayHook = std::make_unique<FuncHook>(timeOfDay, Hooks::Dimension_getTimeOfDay);
		g_Hooks.Dimension_getTimeOfDayHook->enableHook();

		void* chestTick = reinterpret_cast<void*>(FindSignature("40 53 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 83 79 ? ? 48 8B FA 4C 89 74 24 ? 48 8B D9"));
		g_Hooks.ChestBlockActor_tickHook = std::make_unique<FuncHook>(chestTick, Hooks::ChestBlockActor_tick);
		g_Hooks.ChestBlockActor_tickHook->enableHook();

		void* getGameEdition = reinterpret_cast<void*>(FindSignature("83 B9 ? ? ? ? ? B8 ? ? ? ? BA ? ? ? ? 0F 44 C2"));
		g_Hooks.AppPlatform_getGameEditionHook = std::make_unique<FuncHook>(getGameEdition, Hooks::AppPlatform_getGameEdition);
		g_Hooks.AppPlatform_getGameEditionHook->enableHook();

		void* autoComplete = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 41 8B D9"));
		g_Hooks.ChatAutoCompleteHook = std::make_unique<FuncHook>(autoComplete, Hooks::ChatAutoComplete);
		g_Hooks.ChatAutoCompleteHook->enableHook();

		uintptr_t** packetSenderVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)Game.getLoopbackPacketSender());
		g_Hooks.LoopbackPacketSender_sendToServerHook = std::make_unique<FuncHook>(packetSenderVtable[2], Hooks::LoopbackPacketSender_sendToServer);
		g_Hooks.LoopbackPacketSender_sendToServerHook->enableHook();

		void* getFov = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ?? ?? 48 83 EC ?? 0F 29 74 24 ?? 44"));
		g_Hooks.LevelRendererPlayer_getFovHook = std::make_unique<FuncHook>(getFov, Hooks::LevelRendererPlayer_getFov);
		g_Hooks.LevelRendererPlayer_getFovHook->enableHook();

		void* tick_entityList = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F1 0F ? ? 0F ? ? 24 ? ? 0F ? 44 24 ? 48 8B 05 ? ? ? ? 48 85 C0"));
		g_Hooks.MultiLevelPlayer_tickHook = std::make_unique<FuncHook>(tick_entityList, Hooks::MultiLevelPlayer_tick);
		g_Hooks.MultiLevelPlayer_tickHook->enableHook();

		void* MinecraftScreenController_tick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC ? ? ? 48 8B F1 44 8B F7 40 38 B9 ? ? ? ? 74 ? 40 88 B9 ? ? ? ? 44 8D 77 ? 48 8B 01"));
		g_Hooks.MinecraftScreenController_tickHook = std::make_unique<FuncHook>(MinecraftScreenController_tick, Hooks::MinecraftScreenController_tick);
		g_Hooks.MinecraftScreenController_tickHook->enableHook();

		void* fullbright = reinterpret_cast<void*>(FindSignature("48 83 EC ? 80 B9 ? ? ? ? ? 48 8D 54 24 ? 48 8B 01 48 8B 40 58 74 ? 41 B8 13 01 00 00"));
		g_Hooks.GetGammaHook = std::make_unique<FuncHook>(fullbright, Hooks::GetGamma);
		g_Hooks.GetGammaHook->enableHook();

		void* onAppSuspended = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8B EC 48 83 EC ? 48 8B F1 E8"));
		g_Hooks.MinecraftGame_onAppSuspendedHook = std::make_unique<FuncHook>(onAppSuspended, Hooks::MinecraftGame_onAppSuspended);
		g_Hooks.MinecraftGame_onAppSuspendedHook->enableHook();

		void* ConnectionRequest__create = reinterpret_cast<void*>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B D9 48 89 55"));
		g_Hooks.ConnectionRequest_createHook = std::make_unique<FuncHook>(ConnectionRequest__create, Hooks::ConnectionRequest_create);
		g_Hooks.ConnectionRequest_createHook->enableHook();

		void* PaintingRenderer__renderAddr = reinterpret_cast<void*>(FindSignature("48 89 5C 24 20 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 4D 8B E0 4C 8B FA 48 8B F9"));
		g_Hooks.PaintingRenderer__renderHook = std::make_unique<FuncHook>(PaintingRenderer__renderAddr, Hooks::PaintingRenderer__render);
		g_Hooks.PaintingRenderer__renderHook->enableHook();

		void* _getSkinPack = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B EA 48 8B F1"));
		g_Hooks.SkinRepository___loadSkinPackHook = std::make_unique<FuncHook>(_getSkinPack, Hooks::SkinRepository___loadSkinPack);
		g_Hooks.SkinRepository___loadSkinPackHook->enableHook();

		void* ShulkerBox_isItemAllowed = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B 41 ? 48 8B F9 48 85 C0 74 ? 48 8B 10"));
		g_Hooks.ShulkerBlockActor_itemAllowedHook = std::make_unique<FuncHook>(ShulkerBox_isItemAllowed, Hooks::ShulkerBlockActor_itemAllowed);
		g_Hooks.ShulkerBlockActor_itemAllowedHook->enableHook();

		void* SoulSandBlock__calcGroundFriction = reinterpret_cast<void*>(FindSignature("48 83 EC ? 48 8B 02 0F 29 74 24 ? ? 0F ? ? ? ? ? ? 48 8B CA 48 8B 80 ? ? ? ? ? ? ? ? ? ? ? ? ? ? F3 0F ? ? ? ? ? ? 0F ? ? 0F ? 74 24 ? 48 83 C4 38"));
		g_Hooks.SoulSandBlock__calcGroundFrictionHook = std::make_unique<FuncHook>(SoulSandBlock__calcGroundFriction, Hooks::SoulSandBlock__calcGroundFriction);
		g_Hooks.SoulSandBlock__calcGroundFrictionHook->enableHook();

		void* ScreenController_onHoverSlot = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B 81 ? ? ? ? 41 8B ? ? ? ? 48 8B DA 48 8B F9"));
		g_Hooks.ScreenController_onHoverSlotHook = std::make_unique<FuncHook>(ScreenController_onHoverSlot, Hooks::ScreenController_onHoverSlot);
		g_Hooks.ScreenController_onHoverSlotHook->enableHook();

		void* LevelRendererPlayer__renderOutlineSelection = reinterpret_cast<void*>(FindSignature("48 8B C4 55 53 56 57 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 44 0F 29 B0 ? ? ? ? 44 0F 29 B8 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B F1"));
		g_Hooks.LevelRendererPlayer__renderOutlineSelectionHook = std::make_unique<FuncHook>(LevelRendererPlayer__renderOutlineSelection, Hooks::LevelRendererPlayer__renderOutlineSelection);
		g_Hooks.LevelRendererPlayer__renderOutlineSelectionHook->enableHook();

		void* ClientInputCallbacks_tick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B DA 48 8B F9 0F 57 C0"));
		g_Hooks.ClientInputCallbacks_tickHook = std::make_unique<FuncHook>(ClientInputCallbacks_tick, Hooks::ClientInputCallbacks_tick);
		g_Hooks.ClientInputCallbacks_tickHook->enableHook();

		void* HitResult_Update = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 8B ? 48 8B DA 89 01 48 8B F9 8B 42 ? 89 41 ? 8B 42 ? 89 41 ? 8B 42 ? 89 41 ? 8B 42 ? 89 41 ? 8B 42 ? 89 41 ? 8B 42 ? 89 41 ?"));
		g_Hooks.HitResult_UpdateHook = std::make_unique<FuncHook>(HitResult_Update, Hooks::HitResult_Update);
		g_Hooks.HitResult_UpdateHook->enableHook();

		void* localPlayerUpdateFromCam = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 70 ? 57 48 81 EC ? ? ? ? 0F 29 70 E8 0F 29 78 D8 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 4D 8B"));
		g_Hooks.LocalPlayer__updateFromCameraHook = std::make_unique<FuncHook>(localPlayerUpdateFromCam, Hooks::LocalPlayer__updateFromCamera);
		g_Hooks.LocalPlayer__updateFromCameraHook->enableHook();

		void* renderNameTags = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 38 FE FF FF"));
		g_Hooks.LevelRendererPlayer__renderNameTagsHook = std::make_unique<FuncHook>(renderNameTags, Hooks::LevelRendererPlayer__renderNameTags);
		g_Hooks.LevelRendererPlayer__renderNameTagsHook->enableHook();

		void* ItemInHandRenderer_render = Utils::FuncFromSigOffset<void*>(FindSignature("E8 ? ? ? ? 90 48 8D 05 ? ? ? ? 48 89 45 10 48 8B 4D 50"), 1);
		g_Hooks.ItemInHandRenderer_renderHook = std::make_unique<FuncHook>(ItemInHandRenderer_render, Hooks::ItemInHandRenderer_render);
		g_Hooks.ItemInHandRenderer_renderHook->enableHook();

		void* hurtCam = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 1A 48 8B FA E8 ? ? ? ? 48 8B CB"));
		g_Hooks.HurtCameraEffectHook = std::make_unique<FuncHook>(hurtCam, Hooks::HurtCameraEffect);
		g_Hooks.HurtCameraEffectHook->enableHook();

		void* isSlowedByItemUse = reinterpret_cast<void*>(FindSignature("40 53 48 83 EC ? 48 81 C1 ? ? ? ? E8"));
		g_Hooks.isSlowedByItemUseHook = std::make_unique<FuncHook>(isSlowedByItemUse, Hooks::DirectPlayerMovementProxy_isSlowedByItemUse);
		g_Hooks.isSlowedByItemUseHook->enableHook();

		void* getStepHeight =  reinterpret_cast<void*>(FindSignature("48 83 EC ? 48 8B 51 ? 48 8B 42 ? 48 8B 08 ? ? ? 48 8D 54 24 ? 89 44 24 ? E8 ? ? ? ? 48 85 C0 74 ? ? 0F ? ? 48 83 C4 28 ? E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC ? 48 8B 41"));
		g_Hooks.GetStepHeightHook = std::make_unique<FuncHook>(getStepHeight, Hooks::GetStepHeight);
		g_Hooks.GetStepHeightHook->enableHook();

#ifdef HORION_DEBUG
		void*  BlockSource_getBlock = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 57 48 83 EC 20 80 79 ? ? 48 8B FA 48 8B D9 74 28 E8 ? ? ? ? 8B 4B ? 39 08"));
		g_Hooks.BlockSource_getBlockHook = std::make_unique<FuncHook>(BlockSource_getBlock, Hooks::BlockSource_getBlock);
		g_Hooks.BlockSource_getBlockHook->enableHook();

		//void* ActorRenderDispatcher_render = reinterpret_cast<void*>(FindSignature("48 89 5C 24 20 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 F1 48 81 EC B0 00 00 00"));
		//g_Hooks.ActorRenderDispatcher_renderHook = std::make_unique<FuncHook>(ActorRenderDispatcher_render, Hooks::ActorRenderDispatcher_render);
		//g_Hooks.ActorRenderDispatcher_renderHook->enableHook();

		//void* addAction = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC ? ? ? ? ? 4C 8B F2 48 8B F9"));
		//g_Hooks.InventoryTransactionManager__addActionHook = std::make_unique<FuncHook>(addAction, Hooks::InventoryTransactionManager__addAction);
		//g_Hooks.InventoryTransactionManager__addActionHook->enableHook();

		/*void* Font_drawTransformed = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 ? ? 44 0F 29 58 ? 44 0F 28 DB ? 8B E8 4C 8B F2 48 8B F9"));
		g_Hooks.Font_drawTransformedHook = std::make_unique<FuncHook>(Font_drawTransformed, Hooks::Font_drawTransformed);
		g_Hooks.Font_drawTransformedHook->enableHook();

		void* FlatWorldOptions_getLayers = reinterpret_cast<void*>(FindSignature("40 53 48 83 EC 60 48 8B D9 48 89 4C 24 20 48 8D 4C 24 38 E8 98 FD FF FF 90 48 8B 48 10 48 85 C9 78 7B 48 83 78 18 10 72 03 48 8B 00 48 89 4C 24 20"));
		g_Hooks.FlatWorldOptions_getLayersHook = std::make_unique<FuncHook>(FlatWorldOptions_getLayers, Hooks::FlatWorldOptions_getLayers);
		g_Hooks.FlatWorldOptions_getLayersHook->enableHook();*/
#endif
	}

	// Vtables
	{
		// GameMode::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 8B D9 48 89 01 48 8B 89 ? ? ? ? 48 85 C9 74 ? 48 8B 01 ? ? ? ? ? 48 8B 00 ? ? ? ? ? ? 48 8B 8B ? ? ? ? 48 85 C9 74 ? 48 8B 01 ? ? ? ? ? 48 8B ? 48 83 C4 20");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** gameModeVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (gameModeVtable == 0x0 || sigOffset == 0x0)
				logF("GameMode signature not working!!!");
			else {
				g_Hooks.GameMode_startDestroyBlockHook = std::make_unique<FuncHook>(gameModeVtable[1], Hooks::GameMode_startDestroyBlock);
				g_Hooks.GameMode_startDestroyBlockHook->enableHook();

				g_Hooks.GameMode_getPickRangeHook = std::make_unique<FuncHook>(gameModeVtable[10], Hooks::GameMode_getPickRange);
				g_Hooks.GameMode_getPickRangeHook->enableHook();

				g_Hooks.GameMode_attackHook = std::make_unique<FuncHook>(gameModeVtable[14], Hooks::GameMode_attack);
				g_Hooks.GameMode_attackHook->enableHook();
			}
		}

		// BlockLegacy::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 45 33 ED 4C 89 69 ? 4C 89 69 ? 41 BE");  // BlockLegacy constructor
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
            uintptr_t** blockLegacyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
			if (blockLegacyVtable == 0x0 || sigOffset == 0x0)
				logF("BlockLegacy signature not working!!!");
			else {
				//g_Hooks.BlockLegacy_getRenderLayerHook = std::make_unique<FuncHook>(blockLegacyVtable[188], Hooks::BlockLegacy_getRenderLayer);
				g_Hooks.BlockLegacy_getRenderLayerHook = std::make_unique<FuncHook>(FindSignature("8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC F3 0F 10 81 ? ? ? ? C3 CC CC CC CC CC CC CC 41 0F B6 41 ? C3"), Hooks::BlockLegacy_getRenderLayer);
				g_Hooks.BlockLegacy_getRenderLayerHook->enableHook();
			}
		}

		// Item::vtable  (its the cooked_beef)
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 05 ?? ?? ?? ?? C7 05 ?? ?? ?? ?? 00 00 00 00 C6 05 ?? ?? ?? ?? 00 48 8D 0D");  // CookedBeef constructor
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
            uintptr_t** itemVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
			if (itemVtable == 0x0 || sigOffset == 0x0)
				logF("CookedBeef signature not working!!!");
			else {
				//g_Hooks.Item_getMaxUseDurationHook = std::make_unique<FuncHook>(itemVtable[4], Hooks::Item_getMaxUseDuration);
			}
		}

		// LocalPlayer::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 07 ? ? ? ? ? ? ? 4C 89 B7");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** localPlayerVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (localPlayerVtable == 0x0 || sigOffset == 0x0)
				logF("LocalPlayer signature not working!!!");
			else {

                g_Hooks.Actor_lerpMotionHook = std::make_unique<FuncHook>(localPlayerVtable[47], Hooks::Actor_lerpMotion);
                g_Hooks.Actor_lerpMotionHook->enableHook();

                g_Hooks.isInvisibleHook = std::make_unique<FuncHook>(localPlayerVtable[60], Hooks::isInvisible);
                g_Hooks.isInvisibleHook->enableHook();

                g_Hooks.Mob__isImmobileHook = std::make_unique<FuncHook>(localPlayerVtable[93], Hooks::Mob__isImmobile);
                g_Hooks.Mob__isImmobileHook->enableHook();

                g_Hooks.isOnFireHook = std::make_unique<FuncHook>(localPlayerVtable[105], Hooks::isOnFire);
                g_Hooks.isOnFireHook->enableHook();

                g_Hooks.Actor_startSwimmingHook = std::make_unique<FuncHook>(localPlayerVtable[198], Hooks::Actor_startSwimming);
                g_Hooks.Actor_startSwimmingHook->enableHook();

                g_Hooks.Actor_CanBeAffectedHook = std::make_unique<FuncHook>(localPlayerVtable[208], Hooks::Actor_CanBeAffected);
                g_Hooks.Actor_CanBeAffectedHook->enableHook();

                g_Hooks.Actor_swingHook = std::make_unique<FuncHook>(localPlayerVtable[216], Hooks::Actor_swing);
                g_Hooks.Actor_swingHook->enableHook();

                g_Hooks.Actor_ascendLadderHook = std::make_unique<FuncHook>(localPlayerVtable[342], Hooks::Actor_ascendLadder);
                g_Hooks.Actor_ascendLadderHook->enableHook();

                g_Hooks.JumpFromGroundHook = std::make_unique<FuncHook>(localPlayerVtable[347], Hooks::JumpFromGround); // Actor::jumpFromGround(IMobMovementProxy &)
                g_Hooks.JumpFromGroundHook->enableHook();
			}
		}

		// MoveInputHandler::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 07 48 8B C3 48 89 3B 48 8B 5C 24");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** moveInputVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
			if (moveInputVtable == 0x0 || sigOffset == 0x0)
				logF("MoveInputHandler signature not working!!!");
			else {
				g_Hooks.MoveInputHandler_tickHook = std::make_unique<FuncHook>(moveInputVtable[1], Hooks::MoveInputHandler_tick);
				g_Hooks.MoveInputHandler_tickHook->enableHook();
			}
		}

		// PackAccessStrategy vtables for isTrusted
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** directoryPackVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset +  7);

			{
				g_Hooks.DirectoryPackAccessStrategy__isTrustedHook = std::make_unique<FuncHook>(directoryPackVtable[6], Hooks::DirectoryPackAccessStrategy__isTrusted);
				g_Hooks.DirectoryPackAccessStrategy__isTrustedHook->enableHook();
			}

			g_Hooks.SkinRepository___checkSignatureFileInPack = std::make_unique<FuncHook>(FindSignature("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 79 ? 48 8B 59 ? 48 85 DB"), Hooks::ReturnTrue);
			g_Hooks.SkinRepository___checkSignatureFileInPack->enableHook();
		}
	}
	logF("Hooks initialized");

// clang-format on
}

void Hooks::Restore() {
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(10);
}

void Hooks::Enable() {
	//logF("Hooks enabled");
	//MH_EnableHook(MH_ALL_HOOKS);
}

MinecraftJson::Value* Hooks::FlatWorldOptions_getLayers(MinecraftJson::Value* a1, __int64 a2) {
	static auto oFunc = g_Hooks.FlatWorldOptions_getLayersHook->GetFastcall<MinecraftJson::Value*, MinecraftJson::Value*, __int64>();

	// Flat layers from char

	/*const char*  charInput =
		"\n"
		 "\t\t{\n"
		 "\t\t\t\"biome_id\" : 1,\n"
		 "\t\t\t\"world_version\" : \"%s\",\n"
		 "\t\t\t\"block_layers\" : [\n"
		 "\t\t\t\t{\n"
		 "\t\t\t\t\t\"block_name\" : \"minecraft:bedrock\",\n"
		 "\t\t\t\t\t\"count\" : 1\n"
		 "\t\t\t\t},\n"
		 "\t\t\t\t{\n"
		 "\t\t\t\t\t\"block_name\" : \"minecraft:dirt\",\n"
		 "\t\t\t\t\t\"count\" : 2\n"
		 "\t\t\t\t},\n"
		 "\t\t\t\t{\n"
		 "\t\t\t\t\t\"block_name\" : \"minecraft:diamond_ore\",\n"
		 "\t\t\t\t\t\"count\" : 1\n"
		 "\t\t\t\t}\n"
		 "\t\t\t],\n"
		 "\t\t\t\"encoding_version\" : 6,\n"
		 "\t\t\t\"structure_options\" : null\n"
		 "\t\t}";

	std::string input = charInput;
	MinecraftJson::Value JsonValue;
	MinecraftJson::Reader* JsonReader = &MinecraftJson::Reader::Reader();
	JsonReader->Parse(&input, &JsonValue);

	*a1 = JsonValue;
	return a1;*/



	// Flat layers from nlohmann json / file

	/*std::string path = Utils::getRoamingStatePath() + "\\Horion\\FlatWorldJson.txt";
	nlohmann::json json;
	std::ifstream jsonFile(path, std::ifstream::binary);
	jsonFile >> json;
	jsonFile.close();

	MinecraftJson::Value convertedValue = MinecraftJson::Value::fromNlohmannJson(json);

	*a1 = convertedValue;
	return a1;*/

	return oFunc(a1, a2);
}

__int64 Hooks::Font_drawTransformed(Font* _this, ScreenContext* a2, std::string* text, float x, float y, MC_Color color, float rotation, float scale, bool centered, float scalePercentage) {
	static auto oFunc = g_Hooks.Font_drawTransformedHook->GetFastcall<__int64, Font*, ScreenContext*, std::string*, float, float, MC_Color, float, float, bool, float>();
	std::string splash = "Horion Client";
	return oFunc(_this, a2, &splash, x, y, MC_Color(53, 194, 242), rotation, 2.f, centered, scalePercentage);
}

bool Hooks::HurtCameraEffect(void* _this, void* a2) {
	static auto oHurt = g_Hooks.HurtCameraEffectHook->GetFastcall<bool, void*, void*>();
	static auto hurtcam = moduleMgr->getModule<NoHurtcam>("NoHurtcam");
	if (hurtcam->isEnabled()) {
		return false;
	}	
	return oHurt(_this, a2);
}

void Hooks::ItemInHandRenderer_render(__int64* _this, BaseActorRenderContext* ctx, char a3) {
	static auto oFunc = g_Hooks.ItemInHandRenderer_renderHook->GetFastcall<void, __int64 *, BaseActorRenderContext*, char>();
	static auto viewModelMod = moduleMgr->getModule<ViewModel>("ViewModel");

	MatrixStack* matStackPtr = DrawUtils::getMatrixStack();
	auto& newMat = matStackPtr->push();

	if (viewModelMod->isEnabled()) {
		if (viewModelMod->doTranslate)
			*newMat = glm::translate<float>(*newMat, glm::vec3(viewModelMod->xTrans, viewModelMod->yTrans, viewModelMod->zTrans));

		if (viewModelMod->doScale)
			*newMat = glm::scale<float>(*newMat, glm::vec3(viewModelMod->xMod, viewModelMod->yMod, viewModelMod->zMod));

		if (viewModelMod->doRotate)
			*newMat = glm::rotate<float>(*newMat, viewModelMod->rotateAngle, glm::vec3(viewModelMod->xRotate, viewModelMod->yRotate, viewModelMod->zRotate));
	}

	return oFunc(_this, ctx, a3);
}

void Hooks::ClientInstanceScreenModel_sendChatMessage(void* _this, std::string* text) {
	static auto oSendMessage = g_Hooks.ClientInstanceScreenModel_sendChatMessageHook->GetFastcall<void, void*, std::string*>();

	//Game.socketMessages.push_back(text->c_str());

	if (text->size() > 0) {
		char* message = &text->at(0);

		if (*message == cmdMgr->prefix) {
			cmdMgr->execute(message);

			return;
		} else if (*message == '.') {
			// maybe the user forgot his prefix, give him some helpful advice
			static bool helpedUser = false;
			if (!helpedUser) {
				helpedUser = true;
				Game.getGuiData()->displayClientMessageF("%sYour Horion prefix is: \"%s%c%s\"", RED, YELLOW, cmdMgr->prefix, RED);
				Game.getGuiData()->displayClientMessageF("%sEnter \"%s%cprefix .%s\" to reset your prefix", RED, YELLOW, cmdMgr->prefix, RED);
			}
		}
	}
	oSendMessage(_this, text);
}

__int64 Hooks::UIScene_render(UIScene* uiscene, __int64 screencontext) {
	static auto oRender = g_Hooks.UIScene_renderHook->GetFastcall<__int64, UIScene*, __int64>();

	g_Hooks.shouldRender = false;

	std::string alloc{};
	alloc = uiscene->getScreenName();

	if (!g_Hooks.shouldRender) {
		g_Hooks.shouldRender = (strcmp(alloc.c_str(), "debug_screen") == 0);
		if (!g_Hooks.shouldRender) {
			if (alloc.size() < 100) {
				strcpy_s(g_Hooks.currentScreenName, alloc.c_str());
			}
		}
	}
	ZeroMemory(&alloc, sizeof(std::string));

	static auto hudModule = moduleMgr->getModule<HudModule>("HUD");

	if (g_Hooks.shouldRender) {
		if (hudModule && !hudModule->alwaysShow) {
			g_Hooks.shouldRender = (!strcmp(g_Hooks.currentScreenName, "hud_screen") || !strcmp(g_Hooks.currentScreenName, "start_screen"));
		}
	}

	return oRender(uiscene, screencontext);
}

__int64 Hooks::ScreenView_setupAndRender(__int64 a1, MinecraftUIRenderContext* renderCtx) {
	static auto oText = g_Hooks.ScreenView_setupAndRenderHook->GetFastcall<__int64, __int64, MinecraftUIRenderContext*>();

	GuiData* dat = Game.getGuiData();

	DrawUtils::setCtx(renderCtx, dat);

	if (!moduleMgr->isInitialized())
		return oText(a1, renderCtx);
	if (NotifMgr::sInstance == NULL) {
		NotifMgr::sInstance = new NotifMgr();

		Notif* notif = NotifMgr::makeNotifStatic(4 * 60, MC_Color(0.0f, 0.0f, 0.0f, 0.65f));
		notif->setName(MC_Color(1.0f, 1.0f, 1.0f), "Notification");
		notif->setText(MC_Color(1.0f, 1.0f, 1.0f), "Successfully Injected!");
		notif->setDefTexture(Notif::DefTexture::SLogo);
	}

	static auto hudModule = moduleMgr->getModule<HudModule>("HUD");
	static auto ArrayListMod = moduleMgr->getModule<Arraylist>("Arraylist");
	static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>("ClickGui");
	static auto tabGuiModule = moduleMgr->getModule<TabGuiMod>("TabGui");
	static auto hudEditorModule = moduleMgr->getModule<HudEditor>("HudEditor");

	if (GameData::shouldHide() || !g_Hooks.shouldRender)
		return oText(a1, renderCtx);

	auto wid = Game.getGuiData()->windowSize;

	// Call PreRender() functions
	moduleMgr->onPreRender(renderCtx);
	DrawUtils::Flush();

	bool shouldPostRender = true;
	bool shouldRenderArrayList = true;
	bool shouldRenderTabGui = true;
	;

	static float rcolors[4];          // Rainbow color array RGBA
	static float disabledRcolors[4];  // Rainbow Colors, but for disabled modules
	static float currColor[4];        // ArrayList colors

	// Rainbow color updates
	{
		Utils::ApplyRainbow(rcolors);  // Increase Hue of rainbow color array
		disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
		disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
		disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
		disabledRcolors[3] = 1;
	}

	{
		if (strcmp(g_Hooks.currentScreenName, "start_screen") == 0) {
			//Changelog
			{
				GuiData* dat = Game.getGuiData();
				Vec2 windowSize = dat->windowSize;

#ifdef HORION_DEBUG
				std::string version = "Horion: 1.19.60 - Developer build";
#else
				std::string version = "Horion: 1.19.60 - Release build";
#endif
				if (rcolors[3] < 1) {
					rcolors[0] = 1;
					rcolors[1] = 0.2f;
					rcolors[2] = 0.2f;
					rcolors[3] = 1;
				}

				std::string devs = "Floppy, brampedgex, Expando, JayRSky, Haxxie, ambient,";
				std::string devs2 = "OldGreggo5081, Onix86, Slap, Turakan, weedelbhoy1, Void.";
				std::string ogDevs = "Survivor, Gamer and Richard + contributors.";
				std::string credits = "Credits (Hold to view)";
				std::string HorionHome = "Discord (Click to open)";
				std::string LinksStr = "https://horion.download/discord";

				float scale = 1.f;
				std::string plusBox = "[+] ";
				std::string minusBox = "[-] ";
				std::string updateBox = "[*] ";
				std::array ChangeLogArray = {
					plusBox + "1.19.60 support\n",
				};

				std::string combinedString{"Changelog: \n"};
				for (int32_t i = 0; i < ChangeLogArray.size(); i++) {
					combinedString += ChangeLogArray[i];
				}

				float x = 5.f;
				static float rcolors[4];

				Utils::ApplyRainbow(rcolors, 0.0015f);  //0.0015f
				MC_Color HoveredColor = MC_Color(rcolors);
				Vec2 mousePos = *Game.getClientInstance()->getMousePos();

				// Convert mousePos to visual Pos

				Vec2 windowSizeReal = Game.getGuiData()->windowSizeReal;
				float imageHeight = 35.f;
				float imageWidth = 86.f;

				mousePos = mousePos.div(windowSizeReal);
				mousePos = mousePos.mul(windowSize);
				bool hoveredLink = mousePos.x >= x && mousePos.y >= 25.f && mousePos.x < x + DrawUtils::getTextWidth(&HorionHome, scale, DesiredUi::HUD) && mousePos.y < 25.f + DrawUtils::getFontHeight(scale, DesiredUi::HUD);
				bool hoveredCredits = mousePos.x >= x && mousePos.y >= 15.f && mousePos.x < x + DrawUtils::getTextWidth(&credits, scale, DesiredUi::HUD) && mousePos.y < 15.f + DrawUtils::getFontHeight(scale, DesiredUi::HUD);
				bool shouldRenderCredits = false;

				MC_Color normalcolor = MC_Color(1.f, 1.f, 1.f);
				DrawUtils::flushImages();
				DrawUtils::drawText(Vec2(x, 5.f), &version, normalcolor, scale, 1.f, DesiredUi::HUD);
				DrawUtils::drawText(Vec2(x, 15.f), &credits, hoveredCredits ? HoveredColor : normalcolor, scale, 1.f, DesiredUi::HUD);
				DrawUtils::drawText(Vec2(x, 25.f), &HorionHome, hoveredLink ? HoveredColor : normalcolor, scale, 1.f, DesiredUi::HUD);
				//updated
				DrawUtils::drawText(Vec2(x, 35.f), &combinedString, MC_Color(1.f, 1.f, 1.f, 1.f), scale, 1.f, DesiredUi::HUD);
				if (hoveredLink) {
					static bool hasClicked = false;
					if (Game.isLeftClickDown() && !hasClicked) {
						//Utils::setClipboardText(LinksStr);
						hasClicked = true;
						winrt::Windows::Foundation::Uri uri(L"https://horion.download/discord");
						winrt::Windows::System::Launcher::LaunchUriAsync(uri);
					}
					if (!Game.isLeftClickDown() && hasClicked) {
						hasClicked = false;
					}
				}
				if (hoveredCredits) {
					if (Game.isLeftClickDown()) {
						shouldRenderCredits = true;
					}
				}
				if (shouldRenderCredits) {
					std::string title1 = "Original Horion developers";
					std::string title2 = "Current Horion developers";
					float title1Width = DrawUtils::getTextWidth(&title1, 2.5f, DesiredUi::HUD);
					float title2Width = DrawUtils::getTextWidth(&title2, 2.5f, DesiredUi::HUD);
					float ogDevsWidth = DrawUtils::getTextWidth(&ogDevs, 1.5f, DesiredUi::HUD);
					float devsWidth = DrawUtils::getTextWidth(&devs, 1.5f, DesiredUi::HUD);
					float devs2Width = DrawUtils::getTextWidth(&devs2, 1.5f, DesiredUi::HUD);
					Vec2 pos1 = {(windowSize.x / 2) - (devsWidth / 2 + 15), (windowSize.y / 2) - 75};
					Vec2 pos2 = {(windowSize.x / 2) + (devsWidth / 2 + 15), (windowSize.y / 2) + 90};
					DrawUtils::SetColor(1.f, 1.f, 1.f, 1.f);
					DrawUtils::drawRect(pos1, pos2, 6.f);
					DrawUtils::drawText({(windowSize.x / 2) - (title1Width / 2), (windowSize.y / 2) - 65}, &title1, normalcolor, 2.5, 1.f, DesiredUi::HUD);
					DrawUtils::drawText({(windowSize.x / 2) - (ogDevsWidth / 2), (windowSize.y / 2) - 35}, &ogDevs, normalcolor, 1.5, 1.f, DesiredUi::HUD);
					DrawUtils::drawText({(windowSize.x / 2) - (title2Width / 2), (windowSize.y / 2) + 10}, &title2, normalcolor, 2.5, 1.f, DesiredUi::HUD);
					DrawUtils::drawText({(windowSize.x / 2) - (devsWidth / 2), (windowSize.y / 2) + 40}, &devs, normalcolor, 1.5, 1.f, DesiredUi::HUD);
					DrawUtils::drawText({(windowSize.x / 2) - (devs2Width / 2), (windowSize.y / 2) + 60}, &devs2, normalcolor, 1.5, 1.f, DesiredUi::HUD);
					DrawUtils::SetColor(0.f, 0.f, 0.f, 2.f);
					DrawUtils::fillRect(pos1, {pos2.x - 0.3f, pos2.y - 0.3f});
				}
			}
		}

		else {
			shouldRenderTabGui = tabGuiModule->isEnabled();
			shouldRenderArrayList = ArrayListMod->isEnabled();

			if (clickGuiModule->isEnabled()) {
				ClickGui::render();
				shouldPostRender = false;
				shouldRenderArrayList = false;
				shouldRenderTabGui = false;
			}

			if (hudEditorModule->isEnabled()) {
				shouldRenderArrayList = false;
				shouldRenderTabGui = false;
			}

			if (shouldRenderTabGui) TabGui::render();

			{
				// Display ArrayList on the Right?
				static constexpr bool isOnRightSide = true;

				float yOffset = 0;  // Offset of next Text
				Vec2 windowSize = Game.getGuiData()->windowSize;
				Vec2 windowSizeReal = Game.getGuiData()->windowSizeReal;

				Vec2 mousePos = *Game.getClientInstance()->getMousePos();
				mousePos = mousePos.div(windowSizeReal);
				mousePos = mousePos.mul(windowSize);

				// Draw ArrayList
				if (moduleMgr->isInitialized() && shouldRenderArrayList) {
					// Parameters
					float textSize = ArrayListMod->scale;
					float textPadding = 1.0f * textSize;
					float textHeight = 10.0f * textSize;
					float smoothness = 0.2f;

					if (ArrayListMod->bottom)
						yOffset = windowSize.y - textHeight;

					struct IModuleContainer {
						// Struct used to Sort IModules in a std::set
						std::shared_ptr<IModule> backingModule;
						std::string moduleName;
						std::string modeText;
						bool enabled;
						int keybind;
						float textWidth;
						float* pos;
						float* nextPos;
						bool shouldRender = true;
						MC_Color ModColor;
						IModuleContainer(std::shared_ptr<IModule> mod) {
							const char* moduleNameChr = mod->getModuleName();
							this->enabled = mod->isEnabled();
							this->keybind = mod->getKeybind();
							this->backingModule = mod;
							this->pos = mod->getPos();
							this->nextPos = mod->getNextPos();
							switch (mod->getCategory()) {
							default:
								this->ModColor = MC_Color(1.f, 1.f, 1.f);
								break;
							case Category::COMBAT:
								this->ModColor = MC_Color(0.9f, 0.2f, 0.2f);
								break;
							case Category::VISUAL:
								this->ModColor = MC_Color(0.5f, 0.9f, 0.4f);
								break;
							case Category::MOVEMENT:
								this->ModColor = MC_Color(1.f, 0.6f, 0.3f);
								break;
							case Category::PLAYER:
								this->ModColor = MC_Color(0.1f, 0.6f, 0.9f);
								break;
							case Category::WORLD:
								this->ModColor = MC_Color(0.9f, 0.5f, 0.9f);
								break;
							case Category::MISC:
								this->ModColor = MC_Color(1.f, 0.9f, 0.3f);
								break;
							case Category::GUI:
								this->ModColor = MC_Color(1.f, 0.9f, 0.f);
								break;
							}
							std::string pad = mod->getModeText() == "" ? "" : " ";
							float textPadding = ArrayListMod->font.selected != 1 ? 1.0f * ArrayListMod->scale : 0.f;
							if (keybind == 0x0) {
								char text[50];
								sprintf_s(text, 50, "%s%s%s", moduleNameChr, std::string(GRAY).c_str(), ArrayListMod->modes ? std::string(pad + mod->getModeText()).c_str() : "");
								moduleName = text;
							}
							else {
								char text[50];
								sprintf_s(text, 50, "%s%s%s%s", moduleNameChr, ArrayListMod->keybinds ? std::string(" [" + std::string(Utils::getKeybindName(keybind)) + "]").c_str() : "", std::string(GRAY).c_str(), ArrayListMod->modes ? std::string(pad + mod->getModeText()).c_str() : "");
								moduleName = text;
							}
							this->textWidth = DrawUtils::getTextWidth(&moduleName, ArrayListMod->scale, DesiredUi::ARRAYLIST);
							if (!this->enabled && (*this->pos) <= -textWidth * 0.5f)
								this->shouldRender = false;
						}

						bool operator<(const IModuleContainer& other) const {
							/*if (enabled) {
						if (!other.enabled)  // We are enabled
							return true;
					} else if (other.enabled)  // They are enabled
						return false;*/

							if (this->textWidth == other.textWidth)
								return moduleName < other.moduleName;
							return this->textWidth > other.textWidth;
						}
					};

					// Mouse click detector
					static bool wasLeftMouseDown = GameData::isLeftClickDown();  // Last isDown value
					bool leftMouseDown = GameData::isLeftClickDown();            // current isDown value

					bool executeClick = leftMouseDown && leftMouseDown != wasLeftMouseDown;  // isDown == true AND (current state IS NOT last state)
					wasLeftMouseDown = leftMouseDown;                                        // Set last isDown value

					std::set<IModuleContainer> modContainerList;
					// Fill modContainerList with Modules
					{
						auto lock = moduleMgr->lockModuleList();
						std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();
						for (auto it : *moduleList) {
							if (it->getCategory() != Category::GUI) {
								modContainerList.emplace(IModuleContainer(it));
							}
						}
					}

					int a = 0;
					int b = 0;
					int c = 0;

					float lastStartPos = 0;
					Vec4 underline;
					MC_Color arrayColor;
					// Loop through mods to display Labels
					float* prevPos = NULL;
					for (std::set<IModuleContainer>::iterator it = modContainerList.begin(); it != modContainerList.end(); ++it) {
						if (!it->shouldRender)
							continue;
						if (prevPos != NULL) {
							(*prevPos) = (*it->pos);
						}
						prevPos = it->nextPos;
					}
					if (prevPos != NULL) {
						*prevPos = 0.0f;
					}
					for (std::set<IModuleContainer>::iterator it = modContainerList.begin(); it != modContainerList.end(); ++it) {
						if (!it->shouldRender || !it->backingModule->isVisibleInArrayList())
							continue;

						std::string textStr = it->moduleName;
						float textWidth = it->textWidth;

						float xOffsetOri = windowSize.x - textWidth - (textPadding * 2);
						float xOri = windowSize.x + textPadding * 2 + textWidth * 0.5f;

						float xOffset = windowSize.x - (*it->pos);
						float roundedOff = ((*it->pos) - (*it->nextPos));
						float roundRad = roundedOff;
						if (roundRad < 0.0f) {
							roundRad = 0.0f;
						}
						if (roundRad > textHeight)
							roundRad = textHeight;
						if (it->enabled) {
							*it->pos += smoothness * (xOffset - xOffsetOri);
						} else {
							*it->pos += smoothness * (xOffset - xOri);
						}

						if (xOffset < xOffsetOri) {
							xOffset = xOffsetOri;
						}
						if (xOffset >= xOri - textPadding * 2 && !it->enabled) {
							*it->pos = -(textPadding * 2 + textWidth * 0.5f);
						}

						Vec2 textPos = Vec2(
							xOffset + textPadding,
							yOffset + textPadding);
						Vec4 rectPos = Vec4(
							xOffset - 2 - float(textPadding * (ArrayListMod->mode.selected == 3)),
							yOffset,
							isOnRightSide ? windowSize.x : textWidth + (textPadding * 2),
							yOffset + textPadding * 2 + textHeight);
						Vec4 leftRect = Vec4(
							xOffset - 2,
							yOffset,
							xOffset - 1,
							yOffset + textPadding * 2 + textHeight);
						if (ArrayListMod->bottom)
							underline = Vec4(lastStartPos, leftRect.w, leftRect.z, leftRect.w + 1.f);
						else
							underline = Vec4(lastStartPos, leftRect.y, leftRect.x, leftRect.y + 1.f);

						c++;
						b++;

						if (b < 20)
							a = moduleMgr->getEnabledModuleCount() * 2;
						else
							b = 0;

						if (ArrayListMod->rainbowMode.selected == 0) {
							currColor[3] = rcolors[3];
							Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[1], currColor[2]);
							currColor[0] += 1.f / a * c;
							Utils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);
						} else if (ArrayListMod->rainbowMode.selected == 1) {
							currColor[3] = rcolors[3];
							Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[1], currColor[2]);
							currColor[0] += 1.f / a * c;
							Utils::ColorConvertHSVtoRGB(currColor[0], currColor[2], currColor[2], currColor[0], currColor[1], currColor[2]);
						}

						if (ArrayListMod->colorMode.selected == 1) {
							arrayColor = it->ModColor;
						} else {
							arrayColor = MC_Color(ArrayListMod->color.r, ArrayListMod->color.g, ArrayListMod->color.b);
						}

						if (ArrayListMod->mode.selected != 3) {
							DrawUtils::fillRect(rectPos, MC_Color(12, 12, 12), ArrayListMod->opacity);

							if (ArrayListMod->mode.selected == 1) {
								DrawUtils::fillRect(leftRect, ArrayListMod->colorMode.selected == 0 ? MC_Color(currColor) : arrayColor, 1.f);
							}

							if (ArrayListMod->mode.selected == 0) {
								DrawUtils::fillRect(leftRect, ArrayListMod->colorMode.selected == 0 ? MC_Color(currColor) : arrayColor, 1.f);
								if (lastStartPos )
									DrawUtils::fillRect(underline, ArrayListMod->colorMode.selected == 0 ? MC_Color(currColor) : arrayColor, 1.f);
								if (ArrayListMod->bottom)
									lastStartPos = leftRect.z;
								else
									lastStartPos = leftRect.x;
							}
						} else {
							if (ArrayListMod->bottom) {
								rectPos.y -= 1.0f;
								rectPos.w -= 1.0f;
								rectPos.x -= (float(isOnRightSide) * 2.0f - 1.0f);
								DrawUtils::drawRoundRect(rectPos, CornerRadius(roundRad * float(isOnRightSide), roundRad* float(!isOnRightSide), 0.0f, 0.0f), ArrayListMod->colorMode.selected == 0 ? MC_Color(currColor) : arrayColor, 1.f);
								rectPos.x += (float(isOnRightSide) * 2.0f - 1.0f);
								rectPos.y += 1.0f;
								rectPos.w += 1.0f;
								DrawUtils::drawRoundRect(rectPos, CornerRadius(roundRad * float(isOnRightSide), roundRad* float(!isOnRightSide), 0.0f, 0.0f), MC_Color(12, 12, 12), 1.f);
							} else {
								rectPos.y += 1.0f;
								rectPos.w += 1.0f;
								rectPos.x -= (float(isOnRightSide) * 2.0f - 1.0f);
								DrawUtils::drawRoundRect(rectPos, CornerRadius(0.0f, 0.0f, roundRad * float(isOnRightSide), roundRad* float(!isOnRightSide)), ArrayListMod->colorMode.selected == 0 ? MC_Color(currColor) : arrayColor, 1.f);
								rectPos.x += (float(isOnRightSide) * 2.0f - 1.0f);
								rectPos.y -= 1.0f;
								rectPos.w -= 1.0f;
								DrawUtils::drawRoundRect(rectPos, CornerRadius(0.0f, 0.0f, roundRad * float(isOnRightSide), roundRad* float(!isOnRightSide)), MC_Color(12, 12, 12), 1.f);
							}
						}

						if (ArrayListMod->bottom)
							underline = Vec4(leftRect.z, leftRect.y, windowSize.x, leftRect.y + 1.f);
						else
							underline = Vec4(leftRect.x, leftRect.w, windowSize.x, leftRect.w + 1.f);

						if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos) && ArrayListMod->clickToggle) {
							Vec4 selectedRect = rectPos;
							selectedRect.x = leftRect.z;
							if (leftMouseDown) {
								DrawUtils::fillRect(selectedRect, MC_Color(0.8f, 0.8f, 0.8f), 0.8f);
								if (executeClick)
									it->backingModule->toggle();
							} else
								DrawUtils::fillRect(selectedRect, MC_Color(0.8f, 0.8f, 0.8f, 0.8f), 0.3f);
						}
						//textStr = std::to_string(((*it->pos) - (*it->nextPos)));
						DrawUtils::drawText(textPos, &textStr, ArrayListMod->colorMode.selected == 0 ? MC_Color(currColor) : arrayColor, textSize, 1.f, DesiredUi::ARRAYLIST);
						float yAnim = (xOri - xOffset) / (xOri - windowSize.x);
						if (yAnim > 1.0f)
							yAnim = 1.0f;
						if (ArrayListMod->bottom)
							yOffset -= (textHeight + (textPadding * 2)) * yAnim;
						else
							yOffset += (textHeight + (textPadding * 2)) * yAnim;
					}
					if (ArrayListMod->mode.selected == 0)
						DrawUtils::fillRect(underline, ArrayListMod->colorMode.selected == 0 ? MC_Color(currColor) : arrayColor, 1.f);
					c = 0;
					modContainerList.clear();
				}
			}
		}
	}

	// Zoom calc
	{
		static auto zoomModule = moduleMgr->getModule<Zoom>("Zoom");
		if (zoomModule->isEnabled()) zoomModule->target = zoomModule->strength;
		zoomModule->modifier = zoomModule->target - ((zoomModule->target - zoomModule->modifier) * 0.8f);
		if (abs(zoomModule->modifier - zoomModule->target) < 0.1f && !zoomModule->isEnabled())
			zoomModule->zooming = false;
	}

	if (shouldPostRender) moduleMgr->onPostRender(renderCtx);
	DrawUtils::Flush();
	if (hudEditorModule->isEnabled()) hudEditorModule->Render();
	DrawUtils::Flush();

	NotifMgr::sInstance->onCalc();
	NotifMgr::sInstance->onCalcDraw();

	return oText(a1, renderCtx);
}

float* Hooks::Dimension_getFogColor(__int64* _this, float* color, __int64 a3, float a4) {
	static auto oGetFogColor = g_Hooks.Dimension_getFogColorHook->GetFastcall<float*, __int64*, float*, __int64, float>();

	static float rcolors[4];

	static float currColor[4];

	static auto rainbowSkyMod = moduleMgr->getModule<CustomSky>("CustomSky");

	if (rainbowSkyMod->isEnabled() && rainbowSkyMod->rainbowSky) {
		if (rcolors[3] < 1) {
			rcolors[0] = 1;
			rcolors[1] = 0.2f;
			rcolors[2] = 0.2f;
			rcolors[3] = 1;
		}

		Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);  // perfect code, dont question this

		rcolors[0] += 0.00015f;
		if (rcolors[0] >= 1)
			rcolors[0] = 0;

		Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

		return rcolors;
	}

	else if (rainbowSkyMod->isEnabled() && !rainbowSkyMod->rainbowSky) {
		if (currColor[3] < 1) {
			currColor[0] = 1;
			currColor[1] = 0.2f;
			currColor[2] = 0.2f;
			currColor[3] = 1;
		}

		Utils::ColorConvertRGBtoHSV(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);  // perfect code, dont question this

		currColor[0] += 0.00015f;
		if (currColor[0] >= 1) {
			currColor[0] = 0;
		}

		Utils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);

		currColor[0] = CustomSky::color.r;
		currColor[1] = CustomSky::color.g;
		currColor[2] = CustomSky::color.b;

		return currColor;
	}

	return oGetFogColor(_this, color, a3, a4);
}

float Hooks::Dimension_getTimeOfDay(__int64 _this, int a2, float a3) {
	static auto oGetTimeOfDay = g_Hooks.Dimension_getTimeOfDayHook->GetFastcall<float, __int64, int, float>();

	static auto getFuckingTimed = moduleMgr->getModule<HudModule>("HUD");
	if (getFuckingTimed->isEnabled() && getFuckingTimed->MinecraftTime) {
		getFuckingTimed->timeOfDay = oGetTimeOfDay(_this, a2, a3);
	}

	static auto timeChange = moduleMgr->getModule<TimeChanger>("TimeChanger");
	if (timeChange->isEnabled()) {
		return timeChange->modifier;
	}

	return oGetTimeOfDay(_this, a2, a3);
}

void Hooks::ChestBlockActor_tick(ChestBlockActor* _this, void* a) {
	static auto oTick = g_Hooks.ChestBlockActor_tickHook->GetFastcall<void, ChestBlockActor*, void*>();
	oTick(_this, a);
	static auto* chestEspMod = moduleMgr->getModule<ChestESP>("ChestVisuals");
	if (_this && chestEspMod->isEnabled())
		GameData::addChestToList(_this);
}

void Hooks::Actor_lerpMotion(Entity* _this, Vec3 motVec) {
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetFastcall<void, Entity*, Vec3>();

	if (Game.getLocalPlayer() != _this)
		return oLerp(_this, motVec);

	static auto noKnockbackmod = moduleMgr->getModule<Velocity>("Velocity");
	if (noKnockbackmod->isEnabled()) {
		static void* networkSender = nullptr;
		if (!networkSender)
			networkSender = reinterpret_cast<void*>(16 + FindSignature("48 8B CB 48 8B 80 ? ? ? ? ? ? ? ? ? ? C6 ? ? 01 48 8B 5C 24 ? 48 83 C4"));

		if (networkSender == _ReturnAddress()) {
			motVec = _this->stateVector->velocity.lerp(motVec, noKnockbackmod->xModifier, noKnockbackmod->yModifier, noKnockbackmod->xModifier);
		}
	}

	oLerp(_this, motVec);
}

int Hooks::AppPlatform_getGameEdition(__int64 _this) {
	static auto oGetEditon = g_Hooks.AppPlatform_getGameEditionHook->GetFastcall<signed int, __int64>();

	static auto mod = moduleMgr->getModule<EditionFaker>("EditionFaker");
	if (mod->isEnabled()) {
		return mod->getFakedEditon();
	}

	return oGetEditon(_this);
}

void Hooks::ChatAutoComplete(__int64 a1, __int64 a2, std::string* text, int a4) {
	static auto oAutoComplete = g_Hooks.ChatAutoCompleteHook->GetFastcall<void, __int64, __int64, std::string*, int>();

	if (text->size() >= 1 && text->at(0) == '.') {
		std::string search = text->c_str() + 1;                                   // Dont include the '.'
		std::transform(search.begin(), search.end(), search.begin(), ::tolower);  // make the search text lowercase

		struct LilPlump {
			std::string cmdAlias;
			IMCCommand* command = 0;
			bool shouldReplace = true;  // Should replace the current text in the box (autocomplete)

			bool operator<(const LilPlump& o) const {
				return cmdAlias < o.cmdAlias;
			}
		};  // This is needed so the std::set sorts it alphabetically

		std::set<LilPlump> searchResults;

		std::vector<IMCCommand*>* commandList = cmdMgr->getCommandList();
		for (auto it = commandList->begin(); it != commandList->end(); ++it) {  // Loop through commands
			IMCCommand* c = *it;
			auto* aliasList = c->getAliasList();
			for (auto it = aliasList->begin(); it != aliasList->end(); ++it) {  // Loop through aliases
				std::string cmd = *it;
				LilPlump plump;

				for (size_t i = 0; i < search.size(); i++) {  // Loop through search string
					char car = search.at(i);
					if (car == ' ' && i == cmd.size()) {
						plump.shouldReplace = false;
						break;
					} else if (i >= cmd.size())
						goto outerContinue;

					if (car != cmd.at(i))  // and compare
						goto outerContinue;
				}
				// Not at outerContinue? Then we got a good result!
				{
					cmd.insert(0, 1, '.');  // Prepend the '.'

					plump.cmdAlias = cmd;
					plump.command = c;
					searchResults.emplace(plump);
				}

			outerContinue:
				continue;
			}
		}

		if (!searchResults.empty()) {
			LilPlump firstResult = *searchResults.begin();

			size_t maxReplaceLength = firstResult.cmdAlias.size();
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin()++; it != searchResults.end(); it++) {
					auto alias = it->cmdAlias;
					maxReplaceLength = std::min(maxReplaceLength, alias.size());

					for (int i = 0; i < maxReplaceLength; i++) {
						if (alias[i] != firstResult.cmdAlias[i]) {
							maxReplaceLength = i;
							break;
						}
					}
				}
			} else
				maxReplaceLength = firstResult.cmdAlias.size();

			Game.getGuiData()->displayClientMessageF("==========");
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin(); it != searchResults.end(); ++it) {
					LilPlump plump = *it;
					Game.getGuiData()->displayClientMessageF("%s%s - %s%s", plump.cmdAlias.c_str(), GRAY, ITALIC, plump.command->getDescription());
				}
			}
			if (firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1)[0] == 0)
				Game.getGuiData()->displayClientMessageF("%s%s %s- %s", WHITE, firstResult.cmdAlias.c_str(), GRAY, firstResult.command->getDescription());
			else
				Game.getGuiData()->displayClientMessageF("%s%s %s %s- %s", WHITE, firstResult.cmdAlias.c_str(), firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1 /*exclude prefix*/), GRAY, firstResult.command->getDescription());

			if (firstResult.shouldReplace) {
				if (search.size() == firstResult.cmdAlias.size() - 1 && searchResults.size() == 1) {
					maxReplaceLength++;
					firstResult.cmdAlias.append(" ");
				}

				*text = std::string(firstResult.cmdAlias.substr(0, maxReplaceLength));  // Set text
				// now sync with the UI thread
				using syncShit = void(__fastcall*)(std::string*, std::string*);
				static syncShit sync =  Utils::FuncFromSigOffset<syncShit>(FindSignature("E8 ? ? ? ? 48 8B 7D F7 48 85 FF"), 1);

				sync(text, text);
			}
		}

		return;
	}
	oAutoComplete(a1, a2, text, a4);
}

void Hooks::PacketHandlerDispatcherInstance_handle(PacketHandlerDispatcherInstance* dispatcher, void* networkIdentifier, __int64* netEventCallback, Packet** packet) {
	auto oFunc = g_Hooks.packetHooks[(int)(*packet)->getId() - 1]->GetFastcall<void, PacketHandlerDispatcherInstance*, void*, __int64*, Packet**>();

	static auto autoFishMod = moduleMgr->getModule<AutoFish>("AutoFish");
	if (autoFishMod->isEnabled() && autoFishMod->shouldFish == false && (*packet)->getId() == PacketID::ActorEvent) {
		ActorEventPacket* p = reinterpret_cast<ActorEventPacket*>(*packet);
		if (p->eventId == ActorEventPacket::ActorEvent::FishHookHooktime) {
			autoFishMod->reelRod = true;
			autoFishMod->shouldFish = true;
		}
	}

	static auto autoDisconnectMod = moduleMgr->getModule<AutoDisconnect>("AutoDisconnect");
	if (autoDisconnectMod->isEnabled() && (Game.getLocalPlayer()->getHealth() <= autoDisconnectMod->HealthSlider) && (*packet)->getId() == PacketID::Disconnect) {
		DisconnectPacket* p = reinterpret_cast<DisconnectPacket*>(*packet);
		p->skipMessage = false;
		p->disconnectMsg = std::string("Â§lÂ§bYou have been automatically disconnected! Current Health: ") + std::to_string((int)Game.getLocalPlayer()->getHealth()) + "/20";
	}

#ifdef HORION_DEBUG
	static auto packetLogger = moduleMgr->getModule<PacketLogger>("PacketLogger");
	if ((packetLogger->PacketType.selected > 0) && packetLogger->isEnabled())
		logF("%s", (*packet)->getName().c_str());
#endif

	return oFunc(dispatcher, networkIdentifier, netEventCallback, packet);
}

void Hooks::MinecraftPackets_createPacket(Packet** packet, int packetId) {
	static auto oFunc = g_Hooks.MinecraftPackets_createPacketHook->GetFastcall<void, Packet**, int>();
	oFunc(packet, packetId);
	
	static auto AutoMod = moduleMgr->getModule<AutoDisconnect>("AutoDisconnect");
	if (!Game.getLocalPlayer()) {
		AutoMod->setEnabled(false);
	}
	if (AutoMod->isEnabled() && (Game.getLocalPlayer()->getHealth() <= AutoMod->HealthSlider)) {
		oFunc(packet, 5);
	}

	if (!*packet)
		return;

	const auto index = packetId - 1;
	auto& hook = g_Hooks.packetHooks.at(index);
	if (!hook) {
		g_Hooks.packetHooks.at(index) = std::make_unique<FuncHook>((*packet)->dispatcher->vTable[1], Hooks::PacketHandlerDispatcherInstance_handle);
		g_Hooks.packetHooks.at(index)->enableHook();
	}
	return;
}

void Hooks::LoopbackPacketSender_sendToServer(LoopbackPacketSender* a, Packet* packet) {
	static auto oFunc = g_Hooks.LoopbackPacketSender_sendToServerHook->GetFastcall<void, LoopbackPacketSender*, Packet*>();

	static auto autoSneakMod = moduleMgr->getModule<AutoSneak>("AutoSneak");
	static auto blinkMod = moduleMgr->getModule<Blink>("Blink");
	static auto noPacketMod = moduleMgr->getModule<NoPacket>("NoPacket");
	static auto DisablerMod = moduleMgr->getModule<Disabler>("Disabler");
#ifdef HORION_DEBUG
	static auto packetLogger = moduleMgr->getModule<PacketLogger>("PacketLogger");
#endif
	if (noPacketMod->isEnabled() && Game.isInGame())
		return;

	if (DisablerMod->isEnabled() && DisablerMod->Mode.selected == 1 && packet->getId() == PacketID::NetworkStackLatency)
		return;
	if (packet == nullptr) {
		// Perform a nullptr check to avoid crashes
		return;
	}
	if (blinkMod->isEnabled()) {
		if (packet->getId() == PacketID::MovePlayer || packet->getId() == PacketID::PlayerAuthInput) {
			if (blinkMod->isEnabled()) {
				if (packet->getId() == PacketID::MovePlayer) {
					MovePlayerPacket* meme = reinterpret_cast<MovePlayerPacket*>(packet);
					meme->onGround = true;                                                          //Don't take Fall Damages when turned off
					blinkMod->getMovePlayerPacketHolder()->push_back(new MovePlayerPacket(*meme));  // Saving the packets
				} else {
					blinkMod->getPlayerAuthInputPacketHolder()->push_back(new PlayerAuthInputPacket(*reinterpret_cast<PlayerAuthInputPacket*>(packet)));
				}
			}
			return;  // Dont call LoopbackPacketSender_sendToServer
		}
	} else if (!blinkMod->isEnabled()) {
		if (blinkMod->getMovePlayerPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getMovePlayerPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getMovePlayerPacketHolder()->clear();
			return;
		}
		if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0) {
			for (PlayerAuthInputPacket* it : *blinkMod->getPlayerAuthInputPacketHolder()) {
				memset((int*)&it->headYaw + 2, 0, 0x5C);
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getPlayerAuthInputPacketHolder()->clear();
			return;
		}
	}

	if (autoSneakMod->isEnabled() && Game.getLocalPlayer() && autoSneakMod->doSilent && packet->getId() == PacketID::PlayerAction) {
		auto* pp = reinterpret_cast<PlayerActionPacket*>(packet);

		if (pp->action == PlayerActionPacket::Action::StopSneak && pp->actorRuntimeId == Game.getLocalPlayer()->getRuntimeId())
			return;
	}

#ifdef HORION_DEBUG
	if (packetLogger->isEnabled() && Game.getLocalPlayer() && packetLogger->authInput != true && (packetLogger->PacketType.selected == 0 || packetLogger->PacketType.selected == 2)) {
		if (packet->getId() != PacketID::PlayerAuthInput) {
			auto addr = *reinterpret_cast<uintptr_t*>(packet) - ((uintptr_t)GetModuleHandleA(NULL)) + 0x140000000;
			logF("%s, %llX", packet->getName().c_str(), addr);
		}
	} else if (packetLogger->isEnabled() && Game.getLocalPlayer() && (packetLogger->PacketType.selected == 0 || packetLogger->PacketType.selected == 2)) {
		auto addr = *reinterpret_cast<uintptr_t*>(packet) - ((uintptr_t)GetModuleHandleA(NULL)) + 0x140000000;
		logF("%s, %llX", packet->getName().c_str(), addr);
	}
#endif

	moduleMgr->onSendPacket(packet);

	/*if (packet->getId() == PacketID::Login) {
		if (_PrivKeyManager) {
			auto& loginPkt = *reinterpret_cast<LoginPacket*>(packet);
			auto token = loginPkt.req->rawToken.get();
			auto payload = nlohmann::json::parse(Utils::base64url_decode(token->payload.c_str()), nullptr, false);

			// modify jsonPayload here and assign it back to token->payload in string format

			// now we have to resign the webtoken for it to be valid
			token->payload = Utils::base64url_encode(payload.dump());
			std::string signiture = _PrivKeyManager->sign(token->header + "." + token->payload);
			token->signature = Utils::base64url_encode(WebToken::_DERToBinary(&signiture));
		}
	}*/

	oFunc(a, packet);
}

float Hooks::LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3) {
	static auto oGetFov = g_Hooks.LevelRendererPlayer_getFovHook->GetFastcall<float, __int64, float, bool>();
	static void* setupCamera = reinterpret_cast<void*>(FindSignature("F3 0F 59 05 ?? ?? ?? ?? 48 8B C7 48 8B 5C 24"));
	
	static auto zoomModule = moduleMgr->getModule<Zoom>("Zoom");

	if (_ReturnAddress() == setupCamera) {
		Game.fov = -oGetFov(_this, a2, a3) + 110.f;
		if (moduleMgr->isInitialized()) {
			if (!zoomModule->smooth && zoomModule->isEnabled()) return -zoomModule->target + 110.f;
			if (zoomModule->smooth && zoomModule->zooming) return -zoomModule->modifier + 110.f;
		}
		return oGetFov(_this, a2, a3);
	}

	return oGetFov(_this, a2, a3);
}

void Hooks::MultiLevelPlayer_tick(EntityList* _this) {
	static auto oTick = g_Hooks.MultiLevelPlayer_tickHook->GetFastcall<void, EntityList*>();
	oTick(_this);

	static void* serverSide = reinterpret_cast<void*>(FindSignature("48 8B 8B ? ? ? ? 48 85 C9 74 ? 48 8B 01 48 8B 40 ? ? ? ? ? ? ? 48 8B 8B ? ? ? ? 48 85 C9 74 0D 48 8B 01 48 8B 40 18"));

	if (_ReturnAddress() == serverSide) {
		return;
	}

	auto plr = Game.getLocalPlayer();
	if (plr) {
		GameMode* gm = plr->getGameMode();
		GameData::updateGameData(gm);
		moduleMgr->onTick(gm);
		//g_Hooks.entityList.clear();
	}
}

void Hooks::GameMode_startDestroyBlock(GameMode* _this, Vec3i* a2, uint8_t face, void* a4, void* a5) {
	static auto oFunc = g_Hooks.GameMode_startDestroyBlockHook->GetFastcall<void, GameMode*, Vec3i*, uint8_t, void*, void*>();

	static auto nukerModule = moduleMgr->getModule<Nuker>("Nuker");
	static auto instaBreakModule = moduleMgr->getModule<InstaBreak>("InstaBreak");

	if (nukerModule->isEnabled()) {
		Vec3i tempPos;

		int range = nukerModule->getNukerRadius();
		const bool isVeinMiner = nukerModule->isVeinMiner();
		const bool isAutoMode = nukerModule->isAutoMode();

		BlockSource* region = Game.getRegion();
		auto selectedBlockId = ((region->getBlock(*a2)->blockLegacy))->blockId;
		uint16_t selectedBlockData = region->getBlock(*a2)->data;

		if (!isAutoMode) {
			for (int x = -range; x < range; x++) {
				for (int y = -range; y < range; y++) {
					for (int z = -range; z < range; z++) {
						tempPos.x = a2->x + x;
						tempPos.y = a2->y + y;
						tempPos.z = a2->z + z;
						if (tempPos.y > -64) {
							Block* blok = region->getBlock(tempPos);
							uint16_t data = blok->data;
							auto id = blok->blockLegacy->blockId;
							if (blok->blockLegacy->material->isSolid == true && (!isVeinMiner || (id == selectedBlockId && data == selectedBlockData)))
								_this->destroyBlock(tempPos, face);
						}
					}
				}
			}
		}
		return;
	}
	if (instaBreakModule->isEnabled()) {
		_this->destroyBlock(*a2, face);
		return;
	}

	oFunc(_this, a2, face, a4, a5);
}

void Hooks::HIDController_keyMouse(HIDController* _this, void* a2, void* a3) {
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetFastcall<void, HIDController*, void*, void*>();
	GameData::setHIDController(_this);
	isTicked = true;
	oFunc(_this, a2, a3);
	return;
}

int Hooks::BlockLegacy_getRenderLayer(BlockLegacy* a1) {
	static auto oFunc = g_Hooks.BlockLegacy_getRenderLayerHook->GetFastcall<int, BlockLegacy*>();
	static auto xrayMod = moduleMgr->getModule<Xray>("Xray");
	
	if (xrayMod->isEnabled()) {
		const char* BlockIDs[] = {
			"ore",
			"lava",
			"deny",
			"water",
			"allow",
			"portal",
			"border",
			"bedrock",
			"amethyst",
			"ancient_debris",
			"structure_block",
			"command_block",
			"gilded_blackstone",
			"reinforced_deepslate",
		};
		bool isInList = false;
		for (auto name : BlockIDs) {
			if (a1->getRawNameStr().find(name) != std::string::npos)
				isInList = true;
		}
		if (!isInList) return 10;
	}
	return oFunc(a1);
}

__int64 Hooks::LevelRenderer_renderLevel(__int64 _this, ScreenContext* a2, __int64 a3) {
	static auto oFunc = g_Hooks.LevelRenderer_renderLevelHook->GetFastcall<__int64, __int64, ScreenContext*, __int64>();

	DrawUtils::setCtx3D(a2);
	moduleMgr->onLevelRender();
	DrawUtils::setCtx3D(0);
	return oFunc(_this, a2, a3);
}

void Hooks::ClickFunc(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	static auto oFunc = g_Hooks.ClickFuncHook->GetFastcall<void, __int64, char, char, __int16, __int16, __int16, __int16, char>();
	static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>("ClickGui");
	static auto hudeditorModule = moduleMgr->getModule<HudEditor>("HudEditor");

	//MouseButtons
	//0 = mouse move
	//1 = left click
	//2 = right click
	//3 = middle click
	//4 = scroll   (isDown: 120 (SCROLL UP) and -120 (SCROLL DOWN))

	ClickGui::onMouseClickUpdate((int)mouseButton, isDown);

	if (isDown)
		if (mouseButton == 1)
			Game.cpsLeft.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
		else if (mouseButton == 2)
			Game.cpsRight.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

	if (clickGuiModule->isEnabled()) {
		if (mouseButton == 4) {
			ClickGui::onWheelScroll(isDown > 0);
		}
		if (mouseButton )  // Mouse click event
			return;
	}
	if (hudeditorModule->isEnabled()) {
		if (mouseButton )  // Mouse click event
			return;
	}
	return oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

__int64 Hooks::MoveInputHandler_tick(MoveInputHandler* a1, Entity* a2) {
	static auto oTick = g_Hooks.MoveInputHandler_tickHook->GetFastcall<__int64, MoveInputHandler*, Entity*>();

	auto ret = oTick(a1, a2);
	moduleMgr->onMove(a1);

	return 0;
}

__int64 Hooks::keyHook(uint64_t key, bool isDown) {
	static auto oFunc = g_Hooks.keyHookHook->GetFastcall<__int64, uint64_t, bool>();

	return oFunc(key, isDown);
}

__int64 Hooks::MinecraftScreenController_tick(ScreenController* a1) {
	static auto oFunc = g_Hooks.MinecraftScreenController_tickHook->GetFastcall<__int64, ScreenController*>();

	static auto OffhandMod = moduleMgr->getModule<Offhand>("Offhand");
	static auto javaInvMod = moduleMgr->getModule<JavaInv>("JavaInv");
	static auto autoArmorMod = moduleMgr->getModule<AutoArmor>("AutoArmor");
	static auto chestStealerMod = moduleMgr->getModule<ChestStealer>("ChestStealer");
	if (OffhandMod->isEnabled()) OffhandMod->ScreenController_tick(a1);
	if (javaInvMod->isEnabled()) javaInvMod->ScreenController_tick(a1);
	if (autoArmorMod->isEnabled()) autoArmorMod->ScreenController_tick(a1);
	if (chestStealerMod->isEnabled()) chestStealerMod->ScreenController_tick(a1);

	return oFunc(a1);
}

float Hooks::GetGamma(Options** optionList) {
	static auto fullbright = moduleMgr->getModule<FullBright>("Fullbright");
	static auto xrayMod = moduleMgr->getModule<Xray>("Xray");

	if (xrayMod->isEnabled())
		return 25.f;

	if (fullbright->isEnabled())
		return fullbright->intensity;

	static auto ofunc = g_Hooks.GetGammaHook->GetFastcall<float, Options**>();
	return ofunc(optionList);
}

bool Hooks::isOnFire(Entity* _this, void* a1) {
	static auto oFunc = g_Hooks.isOnFireHook->GetFastcall<bool, Entity*, void*>();

	static auto noFireMod = moduleMgr->getModule<NoRender>("NoRender");

	if (noFireMod->isEnabled() && noFireMod->noFire && _this == Game.getLocalPlayer())
		return false;

	return oFunc(_this, a1);
}

void Hooks::JumpFromGround(Entity* a1, void* a2) {
	static auto oFunc = g_Hooks.JumpFromGroundHook->GetFastcall<void, Entity*, void*>();
	static auto sprintMod = moduleMgr->getModule<AutoSprint>("AutoSprint");
	auto input = Game.getMoveInputHandler();
	static auto highJumpMod = moduleMgr->getModule<HighJump>("HighJump");
	if (sprintMod->isEnabled() && sprintMod->selectedSprintMode.selected == static_cast<int32_t>(AutoSprint::SprintMode::ALL_DIRECTIONS) && (input->forwardMovement || input->sideMovement)) {
		float rr = .017453292f * a1->viewAnglesPtr->viewAngles.y;
		a1->stateVector->velocity.x -= std::sinf(rr) * .2f;
		a1->stateVector->velocity.z += std::cosf(rr) * .2f; 
	}
	if (highJumpMod->isEnabled() && Game.getLocalPlayer() == a1) {
		a1->stateVector->velocity.y = highJumpMod->jumpPower;
		return;
	}
	oFunc(a1, a2);
}

__int64 Hooks::MinecraftGame_onAppSuspended(__int64 _this) {
	static auto oFunc = g_Hooks.MinecraftGame_onAppSuspendedHook->GetFastcall<__int64, __int64>();
	configMgr->saveConfig();
	return oFunc(_this);
}

void Hooks::Actor_ascendLadder(Entity* _this) {
	static auto oFunc = g_Hooks.Actor_ascendLadderHook->GetFastcall<void, Entity*>();

	static auto fastLadderModule = moduleMgr->getModule<FastLadder>("FastLadder");
	if (fastLadderModule->isEnabled() && Game.getLocalPlayer() == _this) {
		_this->stateVector->velocity.y = fastLadderModule->speed;
		return;
	}
	return oFunc(_this);
}

void Hooks::Actor_swing(Entity* _this) {
	static auto oFunc = g_Hooks.Actor_swingHook->GetFastcall<void, Entity*>();
	static auto noSwingMod = moduleMgr->getModule<NoSwing>("NoSwing");
	if (!noSwingMod->isEnabled()) return oFunc(_this);
}

void Hooks::Actor_startSwimming(Entity* _this) {
	static auto oFunc = g_Hooks.Actor_startSwimmingHook->GetFastcall<void, Entity*>();

	static auto jesusModule = moduleMgr->getModule<Jesus>("Jesus");
	if (jesusModule->isEnabled() && Game.getLocalPlayer() == _this)
		return;
	oFunc(_this);
}

float Hooks::GameMode_getPickRange(GameMode* _this, __int64 a2, char a3) {
	static auto oFunc = g_Hooks.GameMode_getPickRangeHook->GetFastcall<float, GameMode*, __int64, char>();

	if (Game.getLocalPlayer()) {
		static auto BlockReachModule = moduleMgr->getModule<BlockReach>("BlockReach");
		if (BlockReachModule->isEnabled())
			return BlockReachModule->getBlockReach();

		static auto clickTPmod = moduleMgr->getModule<ClickTP>("ClickTP");
		if (clickTPmod->isEnabled())
			return 255;
	}

	return oFunc(_this, a2, a3);
}

ConnectionRequest* Hooks::ConnectionRequest_create(ConnectionRequest* result, PrivateKeyManager* privKeyManager, Certificate* cert,
	std::string* SelfSignedID, std::string* ServerAddress, uint64_t ClientRandomID, std::string* SkinID, __int64 SkinData, __int64 CapeData,
	SerializedSkin* Skin, std::string* DeviceID, int InputMode, int UIProfile, int GuiScale, std::string* LanguageCode, bool isEditorMode,
	bool IsEduMode, std::string* TenantId, int8_t ADRole, std::string* PlatformUserId, std::string* ThirdPartyName, bool ThirdPartyNameOnly,
	std::string* PlatformOnlineID, std::string* PlatformOfflineID, std::string* CapeID) {

	static auto oFunc = g_Hooks.ConnectionRequest_createHook->GetFastcall<
		ConnectionRequest*, ConnectionRequest*, PrivateKeyManager*, Certificate*, std::string*, std::string*,
		uint64_t, std::string*, __int64, __int64, SerializedSkin*, std::string*, int, int, int, std::string*,
		bool, bool, std::string*, int8_t, std::string*, std::string*, bool, std::string*, std::string*, std::string*>();

	static auto deviceIdModule = moduleMgr->getModule<DeviceIDSpoofer>("DeviceIDSpoofer");
	if (deviceIdModule->isEnabled()) {
		
		/*
		logF("before: SelfSignedID: %s", SelfSignedID->c_str());
		logF("before: ClientRandomID: %lld", ClientRandomID);
		logF("before: DeviceID: %s", DeviceID->c_str());
		logF("before: SkinID: %s", SkinID->c_str());
		*/

		*SelfSignedID = Utils::generateGuid();
		ClientRandomID = Utils::generateRandomInt<uint64_t>();
		auto spoofedDeviceId = Utils::generateGuid();
		*DeviceID = spoofedDeviceId;

		// custom skins are this prefix + your device id
		constexpr std::string_view customSkinPrefix{ "c18e65aa-7b21-4637-9b63-8ad63622ef01.Custom" };
		if (Utils::stringStartsWith(*SkinID, customSkinPrefix)) {
			*SkinID = std::string{customSkinPrefix} + spoofedDeviceId;
		}
	}

	auto& fakeName = Game.getFakeName();
	if (fakeName) {
		*ThirdPartyName = *fakeName;
		ThirdPartyNameOnly = true;
	}

	auto res = oFunc(
		result, privKeyManager, cert, SelfSignedID, ServerAddress, ClientRandomID, SkinID, SkinData, CapeData,
		Skin, DeviceID, InputMode, UIProfile, GuiScale, LanguageCode, isEditorMode, IsEduMode, TenantId, ADRole,
		PlatformUserId, ThirdPartyName, ThirdPartyNameOnly, PlatformOnlineID, PlatformOfflineID, CapeID);

	/*
	logF("after: SelfSignedID: %s", SelfSignedID->c_str());
	logF("after: ClientRandomID: %lld", ClientRandomID);
	logF("after: DeviceID: %s", DeviceID->c_str());
	logF("after: SkinID: %s", SkinID->c_str());
	*/

	return res;
}

__int64 Hooks::PaintingRenderer__render(__int64 _this, __int64 a2, __int64 a3) {
	static auto Func = g_Hooks.PaintingRenderer__renderHook->GetFastcall<__int64, __int64, __int64, __int64>();

	static auto NoPaintingCrashMod = moduleMgr->getModule<NoPaintingCrash>("NoPaintingCrash");
	if (NoPaintingCrashMod->isEnabled())
		return 0;

	return Func(_this, a2, a3);
}

bool Hooks::DirectoryPackAccessStrategy__isTrusted(__int64 _this) {
	static auto func = g_Hooks.DirectoryPackAccessStrategy__isTrustedHook->GetFastcall<bool, __int64>();

	static uintptr_t** directoryPackAccessStrategyVtable = 0;

	if (!directoryPackAccessStrategyVtable) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		directoryPackAccessStrategyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
	}

	if (*reinterpret_cast<uintptr_t*>(_this) == (uintptr_t)directoryPackAccessStrategyVtable)
		return true;

	return func(_this);
}

bool Hooks::ReturnTrue(__int64 _this) {
	return true;
}

__int64 Hooks::SkinRepository___loadSkinPack(__int64 _this, Pack* pack, bool a3) {
	static auto func = g_Hooks.SkinRepository___loadSkinPackHook->GetFastcall<__int64, __int64, Pack*, bool>();

	pack->manifest->origin = PackOrigin::Package;

	return func(_this, pack, a3);
}

__int64 Hooks::GameMode_attack(GameMode* _this, Entity* ent) {
	auto func = g_Hooks.GameMode_attackHook->GetFastcall<__int64, GameMode*, Entity*>();
	moduleMgr->onAttack(ent);
	return func(_this, ent);
}

void Hooks::LocalPlayer__updateFromCamera(__int64 a1, Camera* camera, __int64* a3, Entity* a4) {
	auto oFunc = g_Hooks.LocalPlayer__updateFromCameraHook->GetFastcall<__int64, __int64, Camera*, __int64*, Entity*>();

	static auto javaSneakMod = moduleMgr->getModule<JavaSneak>("JavaSneak");
	static auto noRenderMod = moduleMgr->getModule<NoRender>("NoRender");
	static auto freecamMod = moduleMgr->getModule<Freecam>("Freecam");
	static auto freelookMod = moduleMgr->getModule<Freelook>("Freelook");
	static auto killAuraMod = moduleMgr->getModule<Killaura>("Killaura");

#ifdef HORION_DEBUG
	// COMPLETE GARBAGE UGH!!!!!!!!!!!
	static auto autoPotMod = moduleMgr->getModule<AutoPot>("AutoPot");

	if (autoPotMod->isEnabled() && autoPotMod->rotate) {
		const auto newEulerAngle = Utils::quaternionToRot(camera->lookingAt);
		auto oldQuat = camera->lookingAt;
		auto newQuat = Utils::rotToQuaternion({-static_cast<float>(autoPotMod->pitch), newEulerAngle.y});

		camera->lookingAt = std::move(newQuat);
		oFunc(a1, camera, a3, a4);
		camera->lookingAt = std::move(oldQuat);

		if (autoPotMod->potTimer > 0) {
			autoPotMod->potTimer--;
			return;
		}
		autoPotMod->shouldThrow = false;
		autoPotMod->throwPot();
		return;
	}
#endif

	freecamMod->cam = camera;

	if (noRenderMod->isEnabled() && noRenderMod->noNausea) {
		camera->nauseaModifier = glm::mat4(1.f);
	}

	if (javaSneakMod->isEnabled() && Game.getMoveInputHandler()->isSneakDown) {
		camera->cameraPos.y -= 0.1f;
	}

	if (!freecamMod->isEnabled()) {
		if (killAuraMod->isEnabled() && killAuraMod->hasTarget && (killAuraMod->RotMode.selected == 3)) {
			Vec2 killAuraRot{killAuraMod->angle.x, killAuraMod->angle.y};
			camera->lookingAt = Utils::rotToQuaternion(killAuraRot);
		}
		else if (freelookMod->isEnabled()) {
			const auto& initialViewAngles = freelookMod->initialViewAngles;
			auto oldLookVector = camera->lookingAt;
			camera->lookingAt = Utils::rotToQuaternion(initialViewAngles);
			oFunc(a1, camera, a3, a4);
			camera->lookingAt = std::move(oldLookVector);
			return;
		}
	}

	oFunc(a1, camera, a3, a4);
}
bool Hooks::Mob__isImmobile(Entity* ent) {
	auto func = g_Hooks.Mob__isImmobileHook->GetFastcall<bool, Entity*>();
	static auto freecamMod = moduleMgr->getModule<Freecam>("Freecam");
	static auto antiImmobileMod = moduleMgr->getModule<AntiImmobile>("AntiImmobile");
	if (freecamMod->isEnabled())
		return true;
	if (antiImmobileMod->isEnabled() && ent == Game.getLocalPlayer())
		return false;

	return func(ent);
}

void Hooks::InventoryTransactionManager__addAction(InventoryTransactionManager* _this, InventoryAction& action, bool balanced) {
	auto func = g_Hooks.InventoryTransactionManager__addActionHook->GetFastcall<void, InventoryTransactionManager*, InventoryAction*, bool>();
	func(_this, &action, balanced);
#ifdef HORION_DEBUG
	static auto invLoggerMod = moduleMgr->getModule<InvLogger>("InvLogger");
	if (invLoggerMod->isEnabled()) {
		const char* srcName = "none";
		if (action.sourceItem.item && *action.sourceItem.item)
			srcName = (*action.sourceItem.item)->name.c_str();
		const char* targetName = "none";
		if (action.targetItem.item && *action.targetItem.item)
			targetName = (*action.targetItem.item)->name.c_str();
		logF("%s====================", RED);
		logF("%s[%sSlot%s] %i", GRAY, GOLD, GRAY, action.slot);
		logF("%s[%sSourceItem%s] %s", GRAY, GOLD, GRAY, srcName);
		logF("%s[%sTargetItem%s] %s", GRAY, GOLD, GRAY, targetName);
		logF("%s[%sSourceType%s] %i", GRAY, GOLD, GRAY, action.source.type);
		logF("%s[%sContainerID%s] %i", GRAY, GOLD, GRAY, action.source.container);
		logF("%s[%sFlags%s] %i", GRAY, GOLD, GRAY, action.source.flags);
		logF("%s====================", RED);
	}
#endif
}

bool Hooks::Actor_CanBeAffected(Entity* _this, int a1) {
	static auto oFunc = g_Hooks.Actor_CanBeAffectedHook->GetFastcall<bool, Entity*, int>();
	static auto AntiEffectMod = moduleMgr->getModule<AntiEffect>("AntiEffect");

	if (AntiEffectMod->isEnabled() && Game.getLocalPlayer() == _this) {
		switch (a1) {
		case 8:
			return !AntiEffectMod->JumpBoost;
			break;
		case 9:
			return !AntiEffectMod->Nausea;
			break;
		case 15:
			return !AntiEffectMod->Blindness;
		case 16:
			return !AntiEffectMod->NightVision;
			break;
		case 24:
			return !AntiEffectMod->Levitation;
			break;
		case 27:
			return !AntiEffectMod->SlowFalling;
			break;
		case 30:
			return !AntiEffectMod->Darkness;
			break;
		default:
			break;
		}
	}
	return oFunc(_this, a1);
}

void Hooks::LevelRendererPlayer__renderNameTags(__int64 a1, __int64 a2, std::string* a3, __int64 a4, __int64 a5) {
	static auto func = g_Hooks.LevelRendererPlayer__renderNameTagsHook->GetFastcall<void, __int64, __int64, std::string*, __int64, __int64>();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>("NameTags");

	if (nameTagsMod->isEnabled() && nameTagsMod->nameTags.size() > 0) {
		std::string text = Utils::sanitize(*a3);

		if (nameTagsMod->nameTags.find(text) != nameTagsMod->nameTags.end())
			return;
	}

	return func(a1, a2, a3, a4, a5);
}

void Hooks::GameRenderer_renderCurrentFrame(void* renderer, float f) {
	static auto oFrame = g_Hooks.GameRenderer_renderCurrentFrameHook->GetFastcall<void, void*, float>();
	frameIndex = 0;
	oFrame(renderer, f);
	auto timeNow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	Game.frames.push_back(timeNow);

	// do cps/fps
#define PRO_UPDATE(list) \
	for (size_t i = 0; i < list.size(); i++) { \
		auto start = list[i]; \
		auto it = std::next(list.begin(), i); \
		__int64 time_ = (timeNow - start); \
		if (time_ > 1000) {\
			list.erase(it);\
		}\
	}

	PRO_UPDATE(Game.frames);
	PRO_UPDATE(Game.cpsLeft);
	PRO_UPDATE(Game.cpsRight);


#undef PRO_UPDATE
}

bool Hooks::ShulkerBlockActor_itemAllowed(ItemStack* a1) {
	static auto nestMod = moduleMgr->getModule<ShulkerNesting>("ShulkerNesting");
	if (nestMod->isEnabled())
		return true;

	auto oFunc = g_Hooks.ShulkerBlockActor_itemAllowedHook->GetFastcall<bool, void*>();
	return oFunc(a1);
}

float Hooks::SoulSandBlock__calcGroundFriction(BlockLegacy* a1, __int64 mobMovementProxy) {
	static auto oFunc = g_Hooks.SoulSandBlock__calcGroundFrictionHook->GetFastcall<float, BlockLegacy*, __int64>();
	static auto noSlowMod = moduleMgr->getModule<NoSlowDown>("NoSlowDown");
	if (noSlowMod->isEnabled())
		return 0.60000002f;
	else
		return oFunc(a1, mobMovementProxy);
}

__int64 Hooks::ScreenController_onHoverSlot(ScreenController* _this, std::string* name, int slot) {
	static auto oFunc = g_Hooks.ScreenController_onHoverSlotHook->GetFastcall<__int64, ScreenController*, std::string*, int>();
	static auto javaInvMod = moduleMgr->getModule<JavaInv>("JavaInv");

	if (javaInvMod->isEnabled()) {
		javaInvMod->screenName = *name;
		javaInvMod->hoverSlot = slot;
	}

	return oFunc(_this, name, slot);
}

__int64 Hooks::LevelRendererPlayer__renderOutlineSelection(__int64* _this, ScreenContext* a2, Block* a3, BlockSource* a4, Vec3i& a5) {
	static auto oFunc = g_Hooks.LevelRendererPlayer__renderOutlineSelectionHook->GetFastcall<__int64, __int64*, ScreenContext*, Block*, BlockSource*, Vec3i&>();
	static auto blockOutlineMod = moduleMgr->getModule<BlockOutline>("BlockOutline");

	if (blockOutlineMod->isEnabled())
		return 0;

	return oFunc(_this, a2, a3, a4, a5);
}

__int64 Hooks::ClientInputCallbacks_tick(ClientInputCallbacks* _this, __int64 a2) {
	static auto oFunc = g_Hooks.ClientInputCallbacks_tickHook->GetFastcall<__int64, ClientInputCallbacks*, __int64>();

	//static auto AutoMineMod = moduleMgr->getModule<AutoMine>("AutoMine");
	//AutoMineMod->InputCallback = _this;

	moduleMgr->onClientInputCallbacksTick(_this);
	Game.setClientInputCallbacks(_this);

	return oFunc(_this, a2);
}

HitResult* Hooks::HitResult_Update(HitResult* currentData, HitResult* newData) {
	static auto oFunc = g_Hooks.HitResult_UpdateHook->GetFastcall<HitResult*, HitResult*, HitResult*>();
	static auto ghostHandMod = moduleMgr->getModule<GhostHand>("GhostHand");

	if (ghostHandMod->isEnabled() == false)
		oFunc(currentData, newData);

	if (currentData->weakEntity.id.value != -1 && ghostHandMod->isEnabled())
		currentData->type = HitResultType::Miss;
	else
		currentData->type = newData->type;
	currentData->blockFace = newData->blockFace;
	currentData->block = newData->block;
	currentData->hitpos = newData->hitpos;
	currentData->weakEntity.id.value = newData->weakEntity.id.value;
	return currentData;
}

__int64 Hooks::MinecraftUIRenderContext_drawImage(MinecraftUIRenderContext* _this, TexturePtr* a2, Vec2& a3, Vec2& a4, Vec2& a5, Vec2& a6) {
	static auto oFunc = g_Hooks.MinecraftUIRenderContext_drawImageHook->GetFastcall<__int64, MinecraftUIRenderContext*, TexturePtr*, Vec2&, Vec2&, Vec2&, Vec2&>();

	if (strcmp(a2->location->filePath.c_str(), "textures/ui/title") == 0) {
		/*if (strcmp(g_Hooks.currentScreenName, "start_screen") == 0) {
			DrawUtils::drawImageFromTexturePtr(DrawUtils::resourceToTexturePtr(BACKGROUND_DATA, "Background"), Vec2(0.f, 0.f), Game.getGuiData()->windowSize, Vec2(0.f, 0.f), Vec2(1.f, 1.f));
			DrawUtils::flushImages();
		}*/
		auto ptr = DrawUtils::resourceToTexturePtr(HORION_BANNER_DATA, "HorionBanner"); // Needs to be its own variable or else DrawImageFromTexturePtr will delete it before it's flushed
		DrawUtils::drawImageFromTexturePtr(ptr, Vec2((Game.getGuiData()->windowSize.x / 2.f) - (217.f / 2.f), 15.f), Vec2(217.f, 93.f), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
		DrawUtils::flushImages();
		return 0;
	}

	return oFunc(_this, a2, a3, a4, a5, a6);
}

__int64 Hooks::MinecraftUIRenderContext_drawNineslice(MinecraftUIRenderContext* _this, TexturePtr* a2, __int64* a3) {
	static auto oFunc = g_Hooks.MinecraftUIRenderContext_drawNinesliceHook->GetFastcall<__int64, MinecraftUIRenderContext*, TexturePtr*, __int64*>();

	if (strcmp(g_Hooks.currentScreenName, "start_screen") == 0) {
		if (strcmp(a2->location->filePath.c_str(), "textures/ui/focus_border_white") == 0) {
			return 0;
		}

		//a3 is nineslice info
		float* x = reinterpret_cast<float*>((__int64)a3);
		float* y = reinterpret_cast<float*>((__int64)a3 + 0x4);
		float* z = reinterpret_cast<float*>((__int64)a3 + 0x60);
		float* w = reinterpret_cast<float*>((__int64)a3 + 0x64);
		Vec4 pos(*x, *y, *z, *w);
		Vec4 pos2(pos.x - 2.f, pos.y - 2.f, pos.z + 2.f, pos.w + 2.f);

		if (strcmp(a2->location->filePath.c_str(), "textures/ui/button_borderless_light") == 0) {
			DrawUtils::drawRoundRect(pos2, CornerRadius(5.f), MC_Color(0.6f, 0.6f, 0.6f, 0.5f));
			DrawUtils::drawRoundRect(pos, CornerRadius(5.f), MC_Color(0.f, 0.f, 0.f, 0.65f));
			return 0;
		}

		if (strcmp(a2->location->filePath.c_str(), "textures/ui/button_borderless_lighthover") == 0) {
			DrawUtils::drawRoundRect(pos, CornerRadius(5.f), MC_Color(0.f, 0.f, 0.f, 0.65f));
			DrawUtils::drawRoundRectOutline(pos, CornerRadius(5.f), MC_Color(1.f, 1.f, 1.f, 1.f));
			return 0;
		}
	}

	return oFunc(_this, a2, a3);
}

//void Hooks::ContainerManagerController__updatePreviewItem(__int64 _this, ItemStack* item, ItemStack* itemInstance, __int64 unknown) {
//	static auto oFunc = g_Hooks.ContainerManagerController__updatePreviewItemHook->GetFastcall<void, __int64, ItemStack*, ItemStack*, __int64>();
//	oFunc(_this, item, itemInstance, unknown);
//}
//

Block* Hooks::BlockSource_getBlock(BlockSource* _this, Vec3i blockPos) {
	static auto oFunc = g_Hooks.BlockSource_getBlockHook->GetFastcall<Block*, BlockSource*, Vec3i>();
	auto block = oFunc(_this, blockPos);

#ifdef HORION_DEBUG
	static auto mod = moduleMgr->getModule<BlockESP>("BlockESP");

	// if (plr && mod->isEnabled() && _this == plr->region) {  // is our region
	if (Game.getLocalPlayer() && mod->isEnabled())
		mod->onBlockRequested(block, blockPos);
	//}
#endif

	return block;
}

bool Hooks::isInvisible(void* a1) {
	static auto oFunc = g_Hooks.isInvisibleHook->GetFastcall<bool, void*>();

	static auto TrueSightMod = moduleMgr->getModule<TrueSight>("TrueSight");

	if (TrueSightMod->isEnabled())
		return false;

	return oFunc(a1);
}

bool Hooks::DirectPlayerMovementProxy_isSlowedByItemUse(DirectPlayerMovementProxy* _this) {
	static auto oFunc = g_Hooks.isSlowedByItemUseHook->GetFastcall<bool, class DirectPlayerMovementProxy*>();
	
	if (moduleMgr->getModule<NoSlowDown>("NoSlowDown")->isEnabled()) {
		return false; 
	}

	return oFunc(_this);
}

float Hooks::GetStepHeight(void *ent) {
	// TODO Figure out what the fuck relation ent has to an actual entity pointer
	// I dug around for a little printing the addrs and punching them into reclass and couldn't figure it out
	static auto oFunc = g_Hooks.GetStepHeightHook->GetFastcall<float, void*>();
	static auto mod = moduleMgr->getModule<Step>("Step");
	if (mod->isEnabled() && mod->mode.selected == 0)
		return std::fmin(mod->height, 2.f);
	return oFunc(ent);
}

//:trollface:
//__int64 Hooks::ActorRenderDispatcher_render(ActorRenderDispatcher* _this, BaseActorRenderContext* baseActorCtx, Entity* actor, Vec3 a4, Vec3 a5, Vec2 a6, char a7) {
//	static auto oFunc = g_Hooks.ActorRenderDispatcher_renderHook->GetFastcall<__int64, ActorRenderDispatcher*, BaseActorRenderContext*, Entity*, Vec3, Vec3, Vec2, char>();
//
//	logF("%llX", (uintptr_t)_ReturnAddress() - Game.getModuleBase() + 0x140000000);
//
//	return oFunc(_this, baseActorCtx, actor, a4, a5, a6, a7);
// }
//
//__int64 Hooks::MaterialPtr_contructor(__int64 _this, __int64* a2, HashedString* a3) {
//	static auto oFunc = g_Hooks.MaterialPtr_contructorHook->GetFastcall<__int64, __int64, __int64*, HashedString*>();
//
//	//logF("%s", a3->getText().getText());
//
//	return oFunc(_this, a2, a3);
//}
