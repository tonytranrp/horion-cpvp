#include "BlockESP.h"

BlockESP::BlockESP() : IModule(0, Category::VISUAL, "Highlight blocks of choice.") {
	registerIntSetting("Update Delay", &tickDelay, tickDelay, 0, 100);
	registerIntSetting("Max Block Per Type", &maxBlockPerType, maxBlockPerType, 1, 4096);
	registerFloatSetting("Render Radius", &radius, radius, 5, 512);
}

const char* BlockESP::getModuleName() {
	return "BlockESP";
}

void BlockESP::onLevelRender() {

	std::vector<std::vector<BlockEspRenderData>>& renderList = isRenderingBlockData1 ? renderData1 : renderData2;
	Vec3 origin = DrawUtils::getOrigin();
	Tessellator* t = DrawUtils::getScreenContext()->getTessellator();
	t->setTransformOffset(origin.mul(-1.f));

	for (const std::vector<BlockEspRenderData>& batch : renderList) {
		if (batch.empty()) continue;
		t->begin(VertexFormat::LINE_LIST, (int)(batch.size() * 24 + 5));
		DrawUtils::SetColor(batch[0].color);
		for (const BlockEspRenderData& block : batch) {
			t->vertex(block.begin.x, block.end.y, block.begin.z);
			t->vertex(block.begin.x, block.end.y, block.end.z);

			t->vertex(block.begin.x, block.end.y, block.begin.z);
			t->vertex(block.end.x, block.end.y, block.begin.z);

			t->vertex(block.end.x, block.end.y, block.end.z);
			t->vertex(block.begin.x, block.end.y, block.end.z);

			t->vertex(block.end.x, block.end.y, block.end.z);
			t->vertex(block.end.x, block.end.y, block.begin.z);

			// bottom
			t->vertex(block.begin.x, block.begin.y, block.begin.z);
			t->vertex(block.begin.x, block.begin.y, block.end.z);

			t->vertex(block.begin.x, block.begin.y, block.begin.z);
			t->vertex(block.end.x, block.begin.y, block.begin.z);

			t->vertex(block.end.x, block.begin.y, block.end.z);
			t->vertex(block.begin.x, block.begin.y, block.end.z);

			t->vertex(block.end.x, block.begin.y, block.end.z);
			t->vertex(block.end.x, block.begin.y, block.begin.z);

			// sides
			t->vertex(block.begin.x, block.begin.y, block.begin.z);
			t->vertex(block.begin.x, block.end.y, block.begin.z);

			t->vertex(block.end.x, block.begin.y, block.end.z);
			t->vertex(block.end.x, block.end.y, block.end.z);

			t->vertex(block.begin.x, block.begin.y, block.end.z);
			t->vertex(block.begin.x, block.end.y, block.end.z);

			t->vertex(block.end.x, block.begin.y, block.begin.z);
			t->vertex(block.end.x, block.end.y, block.begin.z);
		}
		DrawUtils::FlushTessellator();
	}

	t->setTransformOffset(Vec3(0.f, 0.f, 0.f));
}

void BlockESP::onTick(GameMode* gm) {
	LocalPlayer* player = Game.getLocalPlayer();
	if (player != lastPlayer) {
		lastPlayer = player;
		updateRequestedBlocks();
	}

	tickDelayNow++;
	if (tickDelayNow > tickDelay) {
		tickDelayNow = 0;

		//remove blocks that are too far
		Vec3 playerpos = player->stateVector->pos;
		for (auto it = currentBlocks.begin(); it != currentBlocks.end();) {
			int x = ((int*)&it->first)[0];
			int z = ((int*)&it->first)[1];
			 
			if (sqrtf(powf((float)x - playerpos.x, 2) + powf((float)z - playerpos.z, 2)) > this->radius) 
				it = currentBlocks.erase(it);
			 else 
				it++;
		}

		std::vector<std::vector<BlockEspRenderData>>& renderList = isRenderingBlockData1 ? renderData2 : renderData1;
		renderList.clear();
		std::map<int, std::vector<BlockEspRenderData>> batches;
		for (const auto& heightmap : currentBlocks) {
			int x = ((int*)&heightmap.first)[0];
			int z = ((int*)&heightmap.first)[1];
			for (const auto& block : heightmap.second) {
				int y = block.first;
				float distance = sqrtf(powf((float)x - player->stateVector->pos.x, 2) + powf((float)y - player->stateVector->pos.y, 2) + powf((float)z - player->stateVector->pos.z, 2));
				if (distance > radius) continue;

				char fakeRequestedBlock[sizeof(RequestedBlock)];
				*(int*)fakeRequestedBlock = block.second;
				auto infoI = requestedBlocks.find(*(RequestedBlock*)fakeRequestedBlock);
				if (infoI == requestedBlocks.end()) continue;
				// BlockEspRenderData newData(Vec3((float)x, (float)y, (float)z), Vec3((float)(x+1), (float)(y+1), (float)(z+1)), infoI->color);
				batches[infoI->id].emplace_back(Vec3((float)x, (float)y, (float)z), Vec3((float)x + 1.f, (float)y + 1.f, (float)z + 1.f), infoI->color);
			}
		}

		for (auto& batch : batches) {
			std::sort(batch.second.begin(), batch.second.end(), [playerpos](const BlockEspRenderData& a, const BlockEspRenderData& b) {
				float distanceA = sqrtf(powf((float)a.begin.x - playerpos.x, 2) + powf((float)a.begin.y - playerpos.y, 2) + powf((float)a.begin.z - playerpos.z, 2));
				float distanceB = sqrtf(powf((float)b.begin.x - playerpos.x, 2) + powf((float)b.begin.y - playerpos.y, 2) + powf((float)b.begin.z - playerpos.z, 2));
				return distanceA < distanceB;
			});
			//remove elements that are not wanted
			if (batch.second.size() > maxBlockPerType)
				batch.second.erase(batch.second.begin() + maxBlockPerType, batch.second.end() - 1);
			renderList.emplace_back(batch.second);
		}

		isRenderingBlockData1 = !isRenderingBlockData1;
	}
}

void BlockESP::onBlockRequested(Block* block, const Vec3i& pos) {
	if (!Game.getLocalPlayer()) return;
	Vec3 playerpos = Game.getLocalPlayer()->stateVector->pos;
	float distance = sqrtf(powf((float)pos.x - playerpos.x, 2) + powf((float)pos.y - playerpos.y, 2) + powf((float)pos.z - playerpos.z, 2));
	if (distance > this->radius) return;
	unsigned __int64 xzpos;
	((int*)&xzpos)[0] = pos.x;
	((int*)&xzpos)[1] = pos.z;
	char fakeRequestedBlock[sizeof(RequestedBlock)];
	*(int*)fakeRequestedBlock = block->blockLegacy->blockId;
	bool careAboutThisBlock = requestedBlocks.find(*(RequestedBlock*)fakeRequestedBlock) != requestedBlocks.end();
	if (careAboutThisBlock) {
		auto& heightmap = currentBlocks[xzpos];
		auto blockI = heightmap.find(pos.y);
		if (blockI == heightmap.end())
			heightmap[pos.y] = *(int*)fakeRequestedBlock;
		else
			blockI->second = *(int*)fakeRequestedBlock;
	} else {
		auto heightmapI = currentBlocks.find(xzpos);
		if (heightmapI == currentBlocks.end()) return;
		auto blockI = heightmapI->second.find(pos.y);
		if (blockI != heightmapI->second.end()) 
			heightmapI->second.erase(blockI);
	}
}

void BlockESP::updateRequestedBlocks() {
	if (!Game.getLocalPlayer()) return;
	std::set<RequestedBlock> result;

	for (auto& block : requestedBlocks) {
		auto item = Game.getItemRegistry()->lookUpByName(block.name);
		if (!item) continue; //we dont have that block here

		ItemStack stack(*item, 1, 0);
		Block* itemBlock = (*((Block**)&stack.item + 2));

		RequestedBlock newBlock = block;
		if (itemBlock && itemBlock->blockLegacy)
			newBlock.id = itemBlock->blockLegacy->blockId;
		else
			newBlock.id = stack.getItem()->itemId;
		result.emplace(newBlock);
	}
	this->requestedBlocks = std::move(result);
	this->currentBlocks.clear();
}

void BlockESP::onLoadConfig(void* conf) {
	IModule::onLoadConfig(conf);
	json& c = *(json*)conf;
	json& mymod = c[this->getRawModuleName()];

	json& blocks = mymod["blocks"];
	for (json& block : blocks) {
		auto rI = block.find("r");
		if (rI == block.end()) continue;
		auto gI = block.find("g");
		if (gI == block.end()) continue;
		auto bI = block.find("b");
		if (bI == block.end()) continue;
		auto aI = block.find("a");
		if (aI == block.end()) continue;
		auto nameI = block.find("name");
		if (nameI == block.end()) continue;
		RequestedBlock newBlock;
		newBlock.color.r = rI->get<float>();
		newBlock.color.g = gI->get<float>();
		newBlock.color.b = bI->get<float>();
		newBlock.color.a = aI->get<float>();
		newBlock.name = nameI->get<std::string>();
		requestedBlocks.emplace(newBlock);
	}
	updateRequestedBlocks();
}
void BlockESP::onSaveConfig(void* conf) {
	IModule::onSaveConfig(conf);
	json& c = *(json*)conf;
	json mymod = c[this->getRawModuleName()];

	json blocks;
	for (auto& block : requestedBlocks) {
		json jsonBlock;
		jsonBlock["r"] = block.color.r;
		jsonBlock["g"] = block.color.g;
		jsonBlock["b"] = block.color.b;
		jsonBlock["a"] = block.color.a;
		jsonBlock["name"] = block.name;
		blocks.push_back(jsonBlock);
	}
	mymod["blocks"] = blocks;
	c[this->getRawModuleName()] = mymod;
}
