#include "Loader.h"

bool isRunning = true;

//#include <stdio.h>
//#include <stdlib.h>
//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//
//// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment(lib, "Ws2_32.lib")
//#pragma comment(lib, "Mswsock.lib")
//#pragma comment(lib, "AdvApi32.lib")
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"

//DWORD WINAPI socketThread(LPVOID lpParam) {
//	WSADATA wsaData;
//	SOCKET ConnectSocket = INVALID_SOCKET;
//	struct addrinfo *result = NULL,
//					*ptr = NULL,
//					hints;
//	const char* sendbuf = "this is a test";
//	int iResult;
//	char recvbuf[DEFAULT_BUFLEN];
//	int recvbuflen = DEFAULT_BUFLEN;
//
//	// Initialize Winsock
//	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (iResult != 0) {
//		logF("WSAStartup failed with error: %d\n", iResult);
//		return 1;
//	}
//
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_UNSPEC;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//
//	// Resolve the server address and port
//	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//	if (iResult != 0) {
//		logF("getaddrinfo failed with error: %d\n", iResult);
//		WSACleanup();
//		return 1;
//	}
//
//	// Attempt to connect to an address until one succeeds
//	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
//		// Create a SOCKET for connecting to server
//		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
//							   ptr->ai_protocol);
//		if (ConnectSocket == INVALID_SOCKET) {
//			logF("socket failed with error: %ld\n", WSAGetLastError());
//			WSACleanup();
//			return 1;
//		}
//
//		// Connect to server.
//		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
//		if (iResult == SOCKET_ERROR) {
//			closesocket(ConnectSocket);
//			ConnectSocket = INVALID_SOCKET;
//			continue;
//		}
//		break;
//	}
//
//	freeaddrinfo(result);
//
//	if (ConnectSocket == INVALID_SOCKET) {
//		logF("Unable to connect to server!\n");
//		WSACleanup();
//		return 1;
//	}
//
//	bool open = true;
//	std::thread inputThread([&] {
//		while (open && isRunning) {
//			if (!Game.socketMessages.empty()) {
//				for (auto msg : Game.socketMessages) {
//					if (strcmp(msg, "-stop") == 0)
//						open = false;
//					else {
//						const char* sendBuffer = msg;
//						iResult = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
//						if (iResult == SOCKET_ERROR) {
//							logF("send failed with error: %d\n", WSAGetLastError());
//							closesocket(ConnectSocket);
//							WSACleanup();
//							return 1;
//						}
//					}
//				}
//				Game.socketMessages.clear();
//			}
//		}
//	});
//	inputThread.detach();
//
//	
//	while (open && isRunning) {
//		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
//		if (iResult > 0) {
//			auto msg = std::string(recvbuf, iResult);
//			if (strcmp(msg.c_str(), "-stop") == 0)
//				open = false;
//			else {
//				if (msg.size() > 0) {
//					char* message = &msg.at(0);
//
//					if (*message == cmdMgr->prefix)
//						cmdMgr->execute(message);
//					else
//						Game.getGuiData()->displayClientMessageF("[Server] %s\n", msg.c_str());
//				}
//			}	
//		}
//	}
//
//	// cleanup
//	closesocket(ConnectSocket);
//	WSACleanup();
//
//	ExitThread(0);
//}

DWORD WINAPI keyThread(LPVOID lpParam) {
	logF("Key thread started");

	bool* keyMap = static_cast<bool*>(malloc(0xFF * 4 + 0x4));
	if (!keyMap)
		throw std::exception("Keymap not allocated");

	bool* keyMapAddr = nullptr;
	uintptr_t sigOffset = FindSignature("4C 8D 05 ?? ?? ?? ?? 89 54 24 20 88 4C 24 24");
	if (sigOffset ) {
		int offset = *reinterpret_cast<int*>((sigOffset + 3));                                   // Get Offset from code
		keyMapAddr = reinterpret_cast<bool*>(sigOffset + offset + /*length of instruction*/ 7);  // Offset is relative
	} else {
		logF("!!!KeyMap not located!!!");
		throw std::exception("Keymap not located");
	}

	HIDController** hidController = Game.getHIDController();

	while (isRunning) {
		if ((GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL)) || GameData::shouldTerminate()) {  // Press L to uninject
			isRunning = false;
			break;
		}

		for (uintptr_t i = 0; i < 0xFF; i++) {
			bool* newKey = keyMapAddr + (4 * i);
			bool newKeyPressed = (*newKey) && GameData::canUseMoveKeys();  // Disable Keybinds when in chat or inventory
			bool* oldKey = keyMap + (4 * i);
			if (newKeyPressed != *oldKey) {
				moduleMgr->onKeyUpdate((int)i, newKeyPressed);
			}
			if (*newKey != *oldKey) {  // Skip Chat or inventory checks
				TabGui::onKeyUpdate((int)i, *newKey);
				ClickGui::onKeyUpdate((int)i, *newKey);
			}
		}

		memcpy_s(keyMap, 0xFF * 4, keyMapAddr, 0xFF * 4);

		Sleep(2);
	}

	logF("Stopping Threads...");
	GameData::terminate();
	Sleep(200);  // Give the threads a bit of time to exit

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 1);  // Uninject
}

#ifndef _MSC_VER
#define _MSC_VER "unk"
#endif

DWORD WINAPI start(LPVOID lpParam) {
	logF("Starting up...");
	logF("MSC v%i at %s", _MSC_VER, __TIMESTAMP__);
	init();

	MH_Initialize();
	GameData::initGameData((HMODULE)lpParam);
	Target::init(Game.getPtrLocalPlayer());

	DWORD keyThreadId;
	CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)keyThread, lpParam, NULL, &keyThreadId);  // Checking Keypresses
	debugLogF("KeyThread ID: %i", keyThreadId);

	/*DWORD socketThreadId;
	CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)socketThread, lpParam, NULL, &socketThreadId);
	debugLogF("SocketThread ID: %i", socketThreadId);*/

	cmdMgr->initCommands();
	logF("Initialized command manager (1/3)");
	moduleMgr->initModules();
	logF("Initialized module manager (2/3)");
	configMgr->init();
	logF("Initialized config manager (3/3)");

	std::thread hookThread([] {
		Hooks::Init();
	});
	TabGui::init();
	ClickGui::init();

	ExitThread(0);
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
	} break;
	case DLL_PROCESS_DETACH:
		isRunning = false;

		configMgr->saveConfig();
		moduleMgr->disable();
		cmdMgr->disable();
		Hooks::Restore();

		logF("Removing logger");
		Logger::Disable();

		MH_Uninitialize();
		delete moduleMgr;
		delete cmdMgr;
		delete configMgr;
		if (Game.getLocalPlayer()) {
			GuiData* guiData = Game.getGuiData();
			if (guiData && !GameData::shouldHide())
				guiData->displayClientMessageF("%sEjected!", RED);
		}
		break;
	}
	return TRUE;
}
