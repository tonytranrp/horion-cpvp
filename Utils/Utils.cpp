#include "Utils.h"

#include "Logger.h"
#include "HMath.h"

#pragma comment(lib, "User32.lib")

unsigned int Utils::getCrcHash(const char* str, int seed) {
	static unsigned int crc32_lut[256] = {0};
	if (!crc32_lut[1]) {
		const unsigned int polynomial = 0xEDB88320;
		for (unsigned int i = 0; i < 256; i++) {
			unsigned int crc = i;
			for (unsigned int j = 0; j < 8; j++)
				crc = (crc >> 1) ^ (((unsigned int)(-((int)(crc & 1)))) & polynomial);
			crc32_lut[i] = crc;
		}
	}

	seed = ~seed;
	unsigned int crc = seed;
	const unsigned char* current = (const unsigned char*)str;
	{
		// Zero-terminated string
		while (unsigned char c = *current++) {
			// We support a syntax of "label###id" where only "###id" is included in the hash, and only "label" gets displayed.
			// Because this syntax is rarely used we are optimizing for the common case.
			// - If we reach ### in the string we discard the hash so far and reset to the seed.
			// - We don't do 'current += 2; continue;' after handling ### to keep the code smaller.
			if (c == '#' && current[0] == '#' && current[1] == '#')
				crc = seed;
			crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
		}
	}
	return ~crc;
}

// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
void Utils::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
	if (s == 0.0f) {
		// gray
		out_r = out_g = out_b = v;
		return;
	}

	h = ImFmod(h, 1.0f) / (60.0f / 360.0f);
	int i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i) {
	case 0:
		out_r = v;
		out_g = t;
		out_b = p;
		break;
	case 1:
		out_r = q;
		out_g = v;
		out_b = p;
		break;
	case 2:
		out_r = p;
		out_g = v;
		out_b = t;
		break;
	case 3:
		out_r = p;
		out_g = q;
		out_b = v;
		break;
	case 4:
		out_r = t;
		out_g = p;
		out_b = v;
		break;
	case 5:
	default:
		out_r = v;
		out_g = p;
		out_b = q;
		break;
	}
}

// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void Utils::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v) {
	float K = 0.f;
	if (g < b) {
		ImSwap(g, b);
		K = -1.f;
	}
	if (r < g) {
		ImSwap(r, g);
		K = -2.f / 6.f - K;
	}

	const float chroma = r - (g < b ? g : b);
	out_h = ImFabs(K + (g - b) / (6.f * chroma + 1e-20f));
	out_s = chroma / (r + 1e-20f);
	out_v = r;
}

void Utils::ApplyRainbow(float* rcolors, const float modifier) {
	if (rcolors[3] < 1) {
		rcolors[0] = 1;
		rcolors[1] = 0.6f;
		rcolors[2] = 0.6f;
		rcolors[3] = 1;
	}

	ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

	rcolors[0] += modifier;
	if (rcolors[0] >= 1)
		rcolors[0] = 0;

	ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);
}

// https://stackoverflow.com/a/34571089
std::string Utils::base64_encode(const std::string& in) {
	std::string out;

	int val = 0, valb = -6;
	for (unsigned char c : in) {
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
	while (out.size() % 4) out.push_back('=');
	return out;
}

std::string Utils::base64_decode(const std::string& in) {
	std::string out;

	std::vector<int> T(256, -1);
	for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

	int val = 0, valb = -8;
	for (unsigned char c : in) {
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}
const char base64_url_alphabet[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'};

std::string Utils::base64url_encode(const std::string& in) {
	std::string out;
	int val = 0, valb = -6;
	size_t len = in.length();
	unsigned int i = 0;
	for (i = 0; i < len; i++) {
		unsigned char c = in[i];
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back(base64_url_alphabet[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) {
		out.push_back(base64_url_alphabet[((val << 8) >> (valb + 8)) & 0x3F]);
	}
	return out;
}

std::string Utils::base64url_decode(const std::string& in) {
	std::string out;
	std::vector<int> T(256, -1);
	unsigned int i;
	for (i = 0; i < 64; i++) T[base64_url_alphabet[i]] = i;

	int val = 0, valb = -8;
	for (i = 0; i < in.length(); i++) {
		unsigned char c = in[i];
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}

std::string Utils::randomString(const int size) {
	std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	static std::random_device rd;
	static std::mt19937 generator(rd());
	std::shuffle(str.begin(), str.end(), generator);
	return str.substr(0, size);
}

void Utils::ApplySystemTime(std::stringstream* ss) {
	using namespace std::chrono;
#ifdef HORION_DEBUG
	const std::chrono::system_clock::time_point systemNow = system_clock::now();
	auto ms = duration_cast<milliseconds>(systemNow.time_since_epoch()) % 10000;
	time_t now_c = system_clock::to_time_t(systemNow);
	tm timeInfo{};
	localtime_s(&timeInfo, &now_c);  // using localtime_s as std::localtime is not thread-safe.
	*ss << "[" << std::put_time(&timeInfo, "%T") << "." << std::setfill('0') << std::setw(4) << ms.count() << "] ";
#endif
}

void Utils::GetCurrentSystemTime(tm& timeInfo) {
	const std::chrono::system_clock::time_point systemNow = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(systemNow);
	localtime_s(&timeInfo, &now_c);  // using localtime_s as std::localtime is not thread-safe.
}

std::string Utils::sanitize(std::string text) {
	std::string out;
	bool wasValid = true;
	for (char c : text) {
		bool isValid = !Utils::invalidChar(c);
		if (wasValid) {
			if (!isValid) {
				wasValid = false;
			} else {
				out += c;
			}
		} else {
			wasValid = isValid;
		}
	}
	return out;
}

std::wstring Utils::stringToWstring(std::string txt) {
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, txt.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, txt.c_str(), -1, wstr, wchars_num);
	// do whatever with wstr
	std::wstring gamer(wstr);
	delete[] wstr;
	return gamer;
}

size_t Utils::posToHash(const Vec3i& pos) {
	return rotBy(pos.x, 0) | rotBy(pos.z, 24) | (static_cast<unsigned __int64>(pos.y) << 48u);
}
std::string Utils::getClipboardText() {
	if (!OpenClipboard(nullptr)) {
		return "";
	} else {
		HANDLE hData = GetClipboardData(CF_TEXT);
		char* pszText = static_cast<char*>(GlobalLock(hData));
		if (!pszText) {
			CloseClipboard();
			return "";
		}

		std::string text(pszText);

		GlobalUnlock(hData);

		CloseClipboard();
		return text;
	}
}
void Utils::setClipboardText(std::string& text) {
	if (!OpenClipboard(nullptr))
		return;
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), text.c_str(), text.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

std::string Utils::readFileContents(std::wstring filePath) {
	std::ifstream fileStr(filePath, std::ios::in | std::ios::binary);
	if (fileStr) {
		std::string contents;
		fileStr.seekg(0, std::ios::end);
		contents.resize(fileStr.tellg());
		fileStr.seekg(0, std::ios::beg);
		fileStr.read(&contents[0], contents.size());
		fileStr.close();
		return contents;
	}
	return "";
}

std::wstring Utils::wreadFileContents(std::wstring filePath) {
	std::wifstream fileStr(filePath, std::ios::in | std::ios::binary);
	if (fileStr) {
		std::wstring contents;
		fileStr.seekg(0, std::ios::end);
		contents.resize(fileStr.tellg());
		fileStr.seekg(0, std::ios::beg);
		fileStr.read(&contents[0], contents.size());
		fileStr.close();
		return contents;
	}
	return L"";
}

uintptr_t Utils::FindSignatureModule(const char* szModule, const char* szSignature) {
	const char* pattern = szSignature;
	uintptr_t firstMatch = 0;
	static const auto rangeStart = (uintptr_t)GetModuleHandleA(szModule);
	static MODULEINFO miModInfo;
	static bool init = false;
	if (!init) {
		init = true;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	}
	static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

	BYTE patByte = GET_BYTE(pattern);
	const char* oldPat = pattern;

	for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++) {
		if (!*pattern)
			return firstMatch;

		while (*(PBYTE)pattern == ' ')
			pattern++;

		if (!*pattern)
			return firstMatch;

		if (oldPat != pattern) {
			oldPat = pattern;
			if (*(PBYTE)pattern != '\?')
				patByte = GET_BYTE(pattern);
		}

		if (*(PBYTE)pattern == '\?' || *(BYTE*)pCur == patByte) {
			if (!firstMatch)
				firstMatch = pCur;

			if (!pattern[2] || !pattern[1])
				return firstMatch;

			//if (*(PWORD)pattern == '\?\?' || *(PBYTE)pattern != '\?')
			//pattern += 3;

			//else
			pattern += 2;
		} else {
			pattern = szSignature;
			firstMatch = 0;
		}
	}
#ifdef HORION_DEBUG
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
	const char* sig = szSignature;  // Put sig in here to access it in debugger
	// This will not get optimized away because we are in debug
	// Leave this in here to quickly find bad signatures in case of updates
	logF("Signature dead: %s", szSignature);
	if (false) {
		const char* msgToTheOverwhelmedDebugger = "SIGNATURE NOT FOUND";
		__debugbreak();
	}

#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif
	return 0u;
}

uintptr_t* Utils::findMultiLvlPtr(uintptr_t baseAddr, std::vector<size_t> offsets) {
	auto ptr =  baseAddr;
	auto i = 0;

	do {
		if (*(uintptr_t*)ptr + offsets[i] == offsets[i] || *(uintptr_t*)ptr + offsets[i] > 0xFFFFFFFFFFFF)
			break;

		ptr = *(uintptr_t*)ptr + offsets[i];

		if (ptr == NULL)
			break;

		i++;

	} while (i < offsets.size());

	return (i == offsets.size() ? (uintptr_t*)ptr : nullptr);
};

std::string Utils::getRoamingStatePath() {
	Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appDataStatics;
	auto hr = RoGetActivationFactory(Microsoft::WRL::Wrappers::HStringReference::HStringReference(L"Windows.Storage.ApplicationData").Get(), __uuidof(appDataStatics), &appDataStatics);
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve application data statics");

	Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appData;
	hr = appDataStatics->get_Current(&appData);
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve current application data");

	Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> roamingFolder;
	hr = appData->get_RoamingFolder(&roamingFolder);
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve roaming folder");

	Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> folderItem;
	hr = roamingFolder.As(&folderItem);
	if (FAILED(hr)) throw std::runtime_error("Failed to cast roaming folder to IStorageItem");

	Microsoft::WRL::Wrappers::HString roamingPathHString;
	hr = folderItem->get_Path(roamingPathHString.GetAddressOf());
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve roaming folder path");

	uint32_t pathLength;
	auto roamingPathCStr = roamingPathHString.GetRawBuffer(&pathLength);
	std::wstring wStringPath(roamingPathCStr, pathLength);
	std::string path(wStringPath.length(), 0);
	std::transform(wStringPath.begin(), wStringPath.end(), path.begin(), [](wchar_t c) {
		1;
		return (char)c;
	});

	return path;
}

Vec2 Utils::percentToWindowPos(Vec2 percent) {
	Vec2 windowSize = Game.getGuiData()->windowSize;
	return Vec2(percent.x * windowSize.x, percent.y * windowSize.y);
}

bool Utils::isOnHive() {
	if (!Game.getRakNetConnector()) return false;
	if (std::string(Game.getRakNetConnector()->serverIp.c_str()).find("hivebedrock") != std::string::npos) return true;
	if (strcmp(Game.getRakNetConnector()->serverIp.c_str(), "51.222.228.185") == 0) return true;
	if (strcmp(Game.getRakNetConnector()->serverIp.c_str(), "144.217.196.205") == 0) return true;
	if (strcmp(Game.getRakNetConnector()->serverIp.c_str(), "51.79.121.246") == 0) return true;
	if (strcmp(Game.getRakNetConnector()->serverIp.c_str(), "51.222.228.180") == 0) return true;
	if (strcmp(Game.getRakNetConnector()->serverIp.c_str(), "146.59.254.237") == 0) return true;
	if (strcmp(Game.getRakNetConnector()->serverIp.c_str(), "51.178.179.61") == 0) return true;
	if (strcmp(Game.getRakNetConnector()->serverIp.c_str(), "139.99.17.123") == 0) return true;
	if (strcmp(Game.getRakNetConnector()->serverIp.c_str(), "51.79.229.174") == 0) return true;
	return false;
}

uintptr_t Utils::getBase() {
	return Game.getModuleBase();
}


uintptr_t Utils::instructionToAddress(uintptr_t addy, size_t lenOperation, size_t size) { // todo: actual utils function
	if (addy == 0) return NULL;
	uintptr_t out = addy;
	int offset = *reinterpret_cast<int*>(out + lenOperation);
	out += offset + (static_cast<unsigned long long>(lenOperation) + size);
	return out;
}

Block* Utils::lookupBlockByName(std::string name) {
	// VanillaBlocks::unassignBlocks signature is too big so we get the function from something that calls it
	// this instruction calls VanillaBlocks::unassignBlocks (function that sets everything in the VanillaBlocks namespace to 0)
	uintptr_t coolSig = FindSignature("48 8B 1D ?? ?? ?? ?? 33 FF 48 89 3D") + 55;
	static uintptr_t regFunction = instructionToAddress(coolSig, 1, 4);
	if (regFunction != NULL) {
		// VanillaBlocks::unassignBlocks + 2 (the first 2 bytes are "xor eax,eax" and we want to ignore whatever thats doing so we add 2)
		static unsigned char* blocksSig = reinterpret_cast<unsigned char*>(regFunction + 2);
		for (unsigned char* i = blocksSig; *i == 0x48; i += 7 /*length of instruction*/) {
			// mov [-> we want this <-], rax
			// then dereference that and we get the Block*

			Block* block = *reinterpret_cast<Block**>(instructionToAddress(reinterpret_cast<uintptr_t>(i), 3, 4));

			if (!block) return nullptr;

			std::string name_ = (std::string)(block->blockLegacy->tileName.c_str());
			if (name_.substr(5) == name) {
				return block;
			}
		}
	} else {
		//[VanillaBlocks] RegFunction is null!
		throw std::exception("send help");
	}
	return nullptr;
}

std::string Utils::formatString(const char* fmt, ...) {
	char output[500];
	va_list arg;
	va_start(arg, fmt);
	vsprintf_s(output, 300, const_cast<const char*>(fmt), arg);
	va_end(arg);
	return output;
}

std::string Utils::generateGuid() {
	GUID guid;
	HRESULT hCreateGuid = CoCreateGuid(&guid);

	std::string strGuid = Utils::formatString(
		"%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	std::transform(strGuid.begin(), strGuid.end(), strGuid.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return strGuid;
}

std::string& Utils::toLowercase(std::string& out) {
	std::transform(out.begin(), out.end(), out.begin(), [](char c) { return std::tolower(c); });
	return out;
}

bool Utils::caseInsensitiveEquals(const std::string& rhs, const std::string& lhs) {
	return std::equal(rhs.begin(), rhs.end(), lhs.begin(), lhs.end(), [](char a, char b) {
		return (std::tolower(a) == std::tolower(b));
	});
}

void* Utils::getStaticAddress(const void* location) {
	return (void*)((((uintptr_t)location) - Utils::getBase()) + 0x140000000);
}

bool Utils::stringStartsWith(const std::string& str, std::string_view prefix) {
	if (prefix.size() > str.size()) return false;
	return std::equal(prefix.begin(), prefix.end(), str.begin());
}

std::string Utils::formatFloat(float f) {
	std::string str = std::to_string(f);
	size_t idx = str.find_last_not_of('0') + 1;
	if (idx != std::string::npos) {
		str.erase(idx);
		if (str.back() == '.') {
			str += '0';
		}
	}
	return str;
}

Vec3 Utils::rotToLookVector(const Vec2& rot) {
	constexpr float invRadDegrees = PI / 180.f;

	float tmpRotX = rot.x * -invRadDegrees;
	float tmpRotY = (rot.y + 90.f) * invRadDegrees;
	float verticalMovement = std::cos(tmpRotX);
	return Vec3{
		std::cos(tmpRotY) * verticalMovement,
		std::sin(tmpRotX),
		std::sin(tmpRotY) * verticalMovement
	};
}

Vec4 Utils::rotToQuaternion(const Vec2& rot) {
	constexpr float invRadDegrees = PI / 180.f;

	float pitch = rot.x * invRadDegrees;
	float yaw = rot.y * invRadDegrees;
	float roll = 0.f;

	float cx = std::cos(pitch * 0.5f);
	float sx = std::sin(pitch * 0.5f);
	float cy = std::cos(yaw * 0.5f);
	float sy = std::sin(yaw * 0.5f);
	float cz = std::cos(roll * 0.5f);
	float sz = std::sin(roll * 0.5f);

	// should be correct, but minecraft doesnt like this?
	/*return Vec4{
		(cx * cy * sz) - (sx * sy * cz), // quat.x
		(cx * cy * cz) + (sx * sy * sz), // quat.y
		(cx * sy * cz) - (sx * cy * sz), // quat.z
		(sx * cy * cz) + (cx * sy * sz)  // quat.w
	};*/

	// dunno why but this works even though convering back and forth isnt the same
	return Vec4{
		(cx * cy * sz) - (sx * sy * cz), // quat.x
		(cx * cy * cz) + (sx * sy * sz), // quat.y
		(sx * cy * cz) - (cx * sy * sz), // quat.z
		(cx * sy * cz) + (sx * cy * sz)  // quat.w
	};
}

// so annoying how the thing members in horions vec4 are ordered wrong
// horion -> normal: quat.x, quat.y, quat.z, quat.w -> quat.w, quat.x, quat.y, quat.z
Vec2 Utils::quaternionToRot(const Vec4& quat) {
	constexpr float radDegrees = 180.f / PI;

	float pitch = 0.f;
	float yaw = 0.f;
	[[maybe_unused]] float roll = 0.f;

	// roll (x-axis rotation)
	float sinr_cosp = 2.f * (quat.x * quat.y + quat.z * quat.w);
	float cosr_cosp = 1.f - (2.f * (quat.y * quat.y + quat.z * quat.z));
	roll = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = 2.f * (quat.y * quat.w - quat.x * quat.z);
	if (std::abs(sinp) >= 1.f) {
		pitch = std::copysign(PI / 2.f, sinp);  // use 90 degrees if out of range
	} else {
		pitch = std::asin(sinp);
	}

	// yaw (z-axis rotation)
	float siny_cosp = 2.f * (quat.x * quat.w + quat.y * quat.z);
	float cosy_cosp = 1.f - (2.f * (quat.z * quat.z + quat.w * quat.w));
	yaw = std::atan2(siny_cosp, cosy_cosp);

	return Vec2{
		pitch * radDegrees,
		yaw * radDegrees
	};
}