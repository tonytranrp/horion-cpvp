#pragma once
#include "../Module.h"

struct DraggableItem {
	Vec4 areaPos;
	Vec2* pos;
	std::string name;

	DraggableItem(Vec4 areaPos, Vec2* pos, std::string name) {
		this->areaPos = areaPos;
		this->pos = pos;
		this->name = name;
	}

	bool operator==(DraggableItem const& input) {
		return name == input.name;
	}
};

class HudEditor : public IModule {
public:
	HudEditor();
	~HudEditor();

	std::string currentDragging;
	Vec2 targetVec;
	std::vector<DraggableItem> itemList;
	std::string mouseAlreadyOver;

	void addDraggaableItem(DraggableItem item);
	void removeDraggaableItem(DraggableItem item);
	void Render();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable();
	virtual void onDisable();
};