#pragma once
#include "../Module.h"

class BlockESP : public IModule {
private:
	struct RequestedBlock {
		int id;
		std::string name;
		MC_Color color;
		bool operator>(const RequestedBlock& other) const { return id > other.id; }
		bool operator<(const RequestedBlock& other) const { return id < other.id; }
		bool operator>(int other) const { return id > other; }
		bool operator<(int other) const { return id < other; }
		bool operator==(int other) const { return id == other; }
		bool operator==(const RequestedBlock& other) const { return id == other.id; }
	};

	struct BlockEspRenderData {
		Vec3 begin;
		Vec3 end;
		MC_Color color;
		BlockEspRenderData(const Vec3& Begin, const Vec3& End, const MC_Color& Color)
			: begin(Begin), end(End), color(Color) {}
		BlockEspRenderData() {}
	};

	std::set<RequestedBlock> requestedBlocks;
	std::map<unsigned __int64, std::map<int, int>> currentBlocks;

	std::vector<std::vector<BlockEspRenderData>> renderData1;
	std::vector<std::vector<BlockEspRenderData>> renderData2;
	bool isRenderingBlockData1 = true;

	LocalPlayer* lastPlayer = 0;

	int tickDelay = 20;
	int tickDelayNow = 0;

	float radius = 64.f;
	int maxBlockPerType = 512;

public:
	BlockESP();
	~BlockESP() = default;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onLevelRender() override;
	virtual void onTick(GameMode* gm) override;

	void onBlockRequested(Block* block, const Vec3i& pos);
	void updateRequestedBlocks();

	virtual void onLoadConfig(void* conf) override;
	virtual void onSaveConfig(void* conf) override;

	friend class BlockESPCommand;
};
