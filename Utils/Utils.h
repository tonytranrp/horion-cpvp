#pragma once

#include "../Memory/GameData.h"

static const char* const KeyNames[] = {
	"Unknown",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"Shift",
	"CTRL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"INSERT",
	"DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"LeftShift",
	"RightShift",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU",
	"Browser Back",
	"Browser Forward",
	"Browser Refresh",
	"Browser Stop",
	"Browser Search",
	"Browser Favourites",
	"Browser Home",
	"Volume Mute",
	"Volume Down",
	"Volume Up",
	"Media_Next",
	"Media_Prev",
	"Media_Stop",
	"Media_Pause",
	"Mail",
	"Media",
	"App1",
	"App2",
	"Unknown",
	"Unknown",
	"OEM_1",
	"PLUS",
	"COMMA",
	"MINUS",
	"DOT",
	"OEM_2",
	"OEM_3"};

#define INRANGE(x, a, b) (x >= a && x <= b)
#define GET_BYTE(x) (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? x - '0' : 0))

static inline float ImFmod(float x, float y) { return fmodf(x, y); }
static inline float ImFabs(float x) { return fabsf(x); }
template <typename T>
static inline void ImSwap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

#define FindSignature(szSignature) Utils::FindSignatureModule("Minecraft.Windows.exe", xorstr_(szSignature))

struct Vec3i;
class Block;

namespace Utils {

	template <unsigned int IIdx, typename TRet, typename... TArgs>
	inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet {
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
	}

	template <typename ret>
	inline auto FuncFromSigOffset(uintptr_t sig, int offset) -> ret {
		if (sig != 0x0) {
			sig += offset + 4 + *reinterpret_cast<int*>(sig + offset);
		}
		return reinterpret_cast<ret>(sig);
	}

	__forceinline unsigned __int64 rotBy(int in, unsigned int by) {
		auto mut = static_cast<unsigned __int64>(in);
		return ((mut & 0x7FFFFFui64) | ((static_cast<unsigned int>(in) >> 8u) & 0x800000u) /*copy sign bit*/) << by;
	}
	
	template <typename type>
	inline auto lerp(type a, type b, float t) -> type {
		return a + t * (b - a);
	}

	inline const char* getKeybindName(int keybind) {
		return KeyNames[keybind];
	}

	inline bool endsWith(std::wstring const& fullString, std::wstring const& ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		} else {
			return false;
		}
	}

	inline void nopBytes(void* dst, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memset(dst, 0x90, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	inline void copyBytes(void* src, void* dst, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, size);
		VirtualProtect(src, size, oldprotect, &oldprotect);
	}

	inline void patchBytes(void* dst, void* src, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	}

	inline bool invalidChar(char c) {
		return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
	}

	size_t posToHash(const Vec3i& pos);

	unsigned int getCrcHash(const char* str, int seed = 0);

	void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
	void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);
	void ApplyRainbow(float* rcolors, const float modifier = 0.0015f);

	std::string base64_encode(const std::string& in);
	std::string base64_decode(const std::string& in);
	std::string base64url_encode(const std::string& in);
	std::string base64url_decode(const std::string& in);

	std::string randomString(const int size);

	std::string getClipboardText();

	void setClipboardText(std::string& text);

	std::string readFileContents(std::wstring filePath);

	std::wstring wreadFileContents(std::wstring filePath);

	uintptr_t FindSignatureModule(const char* szModule, const char* szSignature);

	uintptr_t* findMultiLvlPtr(uintptr_t baseAddr, std::vector<size_t> offsets);

	void GetCurrentSystemTime(tm& timeInfo);

	void ApplySystemTime(std::stringstream* ss);

	std::string sanitize(std::string text);

	std::wstring stringToWstring(std::string txt);

	std::string getRoamingStatePath();

	Vec2 percentToWindowPos(Vec2 percent);

	bool isOnHive();

	uintptr_t getBase();
	
	uintptr_t instructionToAddress(uintptr_t addy, size_t lenOperation, size_t size);

	Block* lookupBlockByName(std::string name);

	std::string formatString(const char* fmt, ...);
	std::string generateGuid(); // the player's device id is a stringified GUID

	std::string& toLowercase(std::string& out);
	bool caseInsensitiveEquals(const std::string& rhs, const std::string& lhs);
	void* getStaticAddress(const void* location);

	std::string formatFloat(float f);

	Vec3 rotToLookVector(const Vec2& rot);
	Vec4 rotToQuaternion(const Vec2& rot);
	Vec2 quaternionToRot(const Vec4& quat);

	// we love being on c++17 :))
	bool stringStartsWith(const std::string& str, std::string_view prefix);

	inline std::mt19937_64 RNG_INSTANCE{std::random_device{}()};

	template <typename T>
	T generateRandomReal(T valMin = 0.f, T valMax = 1.f) {
		static_assert(std::is_floating_point_v<T>);
		std::uniform_real_distribution<T> genRealFunc(valMin, valMax);
		return genRealFunc(RNG_INSTANCE);
	}

	template <typename T>
	T generateRandomInt(T valMin = (std::numeric_limits<T>::min)(), T valMax = (std::numeric_limits<T>::max)()) {
		static_assert(std::is_integral_v<T>);
		std::uniform_int_distribution<T> genIntFunc(valMin, valMax);
		return genIntFunc(RNG_INSTANCE);
	}

	template <typename TreatAs, typename Pointer>
	constexpr TreatAs& directAccess(Pointer ptr, ptrdiff_t offset) {
		return *reinterpret_cast<TreatAs*>(reinterpret_cast<uintptr_t>(ptr) + offset);
	}

	} // namespace Utils

#define CLASS_FIELD(name, offset, ...)                                                               \
	__declspec(property(get = GET_FIELD_##name, put = SET_FIELD_##name)) __VA_ARGS__ name;           \
	__VA_ARGS__& GET_FIELD_##name() const { return Utils::directAccess<__VA_ARGS__>(this, offset); } \
	template <typename T>                                                                            \
	void SET_FIELD_##name(const T& value) { Utils::directAccess<__VA_ARGS__>(this, offset) = value; }










template <typename T>
struct Mc_SharedPtr {
	T* ptr;

	T* operator*() { return ptr; }
	T* operator->() { return ptr; }

	Mc_SharedPtr() { ptr = 0; }
	Mc_SharedPtr(const Mc_SharedPtr& other) { memcpy(this, &other, sizeof(void*) * 2); }
	Mc_SharedPtr(Mc_SharedPtr&& other) {
		memcpy(this, &other, sizeof(void*) * 2);
		memset(&other, 0, sizeof(void*) * 2);
	}
	~Mc_SharedPtr() = default;

private:
	struct myBloat {
		char bloat[0x18];
	};
	myBloat* bloat;
};