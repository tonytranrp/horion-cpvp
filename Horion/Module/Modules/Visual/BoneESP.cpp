#include "BoneESP.h"

#include "../../../DrawUtils.h"
#include "../../../../Utils/svpng.h"
#include "../../../../Utils/gifdec.h"
#include "../../../Config/ConfigManager.h"

#include "../../../../resource.h"

BoneESP::BoneESP() : IModule(0, Category::VISUAL, "Bone ESP") {
}

void BoneESP::doInit() {
	lastRender = std::chrono::high_resolution_clock::now();
	textures.clear();

	auto boneResource = FindResourceA((HMODULE)Game.getDllModule(), MAKEINTRESOURCEA(BONE_DATA), (LPCSTR)RT_RCDATA);
	if (boneResource == 0) {
		logF("Bone resource not found");
		return;
	}

	auto data = LoadResource((HMODULE)Game.getDllModule(), boneResource);
	if (data == 0) {
		logF("Resource not loaded");
		return;
	}

	auto dataPtr = LockResource(data);
	auto dataSize = SizeofResource((HMODULE)Game.getDllModule(), boneResource);

	auto wide = ConfigManager::GetRoamingFolderPath();
	std::string path(wide.length(), 0);
	std::transform(wide.begin(), wide.end(), path.begin(), [](wchar_t c) {
		return (char)c;
	});
	path += "\\Horion\\Assets\\";
	std::filesystem::create_directory(path.c_str());
	std::string filePath = path + "mr_bone.gif";
	if (!(std::filesystem::exists(filePath))) {
		std::ofstream outfile;
		outfile.open(filePath, std::ofstream::binary);
		outfile.write((const char*)dataPtr, dataSize);
		outfile.close();
	}

	gd_GIF* gif = gd_open_gif(filePath.c_str());
	if (!gif) {
		logF("open gif is nullptr %s", filePath.c_str());
		return;
	}

	unsigned char* buffer = (unsigned char*)malloc(gif->width * gif->height * 3);
	unsigned char* rgbaBuffer = (unsigned char*)malloc(gif->width * gif->height * 4);
	int num = 0;

	while (gd_get_frame(gif)) {
		gd_render_frame(gif, (uint8_t*)buffer);
		num++;
		if (num == 1)
			continue;  // first frame is not gucci
		size_t off = 0;
		for (int y = 0; y < gif->height; y++) {
			for (int x = 0; x < gif->width; x++) {
				auto newOff = 4 * (y * gif->width + x);
				if (gd_is_bgcolor(gif, &buffer[off])) {
					rgbaBuffer[newOff] = rgbaBuffer[newOff + 1] = rgbaBuffer[newOff + 2] = rgbaBuffer[newOff + 3] = 0;
				} else {
					rgbaBuffer[newOff] = buffer[off];
					rgbaBuffer[newOff + 1] = buffer[off + 1];
					rgbaBuffer[newOff + 2] = buffer[off + 2];
					rgbaBuffer[newOff + 3] = 255;
				}

				off += 3;
			}
		}

		char name[30];
		sprintf_s(name, "bone_%d", num);
		textures.push_back(DrawUtils::rawDataToTexturePtr(rgbaBuffer, gif->width, gif->height, true, std::string(name)));
	}

	free(buffer);
	gd_close_gif(gif);
	hasInit = true;
}


void BoneESP::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (!Game.getLocalPlayer()) return;
	if (!hasInit)
		doInit();

	auto deltaDiscrete = std::chrono::high_resolution_clock::now() - lastRender;
	float delta = std::chrono::duration_cast<std::chrono::microseconds>(deltaDiscrete).count() / 1000000.f;
	delta = fmaxf(0.f, delta);
	indOverflow += delta / (1.f / fps);
	ind += (int)indOverflow;
	indOverflow -= (int)indOverflow;

	// DrawUtils::SetColor(this->color.r, this->color.g, this->color.b, 1.f);
	// DrawUtils::fillRect({this->testVec.x, this->testVec.y}, {this->testVec.x + 100, this->testVec.y + 100});
	if (ind >= textures.size())
		ind = 0;
	if (textures.empty())
		return;

	Game.forEachEntity([this](Entity* ent) {
		if (ent == Game.getLocalPlayer())
			return;

		if (ent->aabbShape->width < 0.1f || ent->aabbShape->height < 0.1f)
			return;

		if (!Target::isValidTarget(ent, false, true))
			return;

		Vec4 rect = DrawUtils::getRectForAABB(ent->aabbShape->aabb);
		if (rect.x == 0 && rect.y == 0 && rect.z == 0 && rect.w == 0)
			return;
		float wid = rect.z - rect.x;
		float hei = rect.w - rect.y;

		if (wid <= 1 || hei <= 1)
			return;

		DrawUtils::drawImageFromTexturePtr(textures[ind], {rect.x, rect.y}, {wid, hei}, {0.f, 0.f}, {1.f, 1.f});
	});

	DrawUtils::flushImages();

	lastRender = std::chrono::high_resolution_clock::now();
}


const char* BoneESP::getModuleName() {
	return "BoneESP";
}
