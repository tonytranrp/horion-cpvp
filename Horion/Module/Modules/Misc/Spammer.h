#pragma once
#include "../Module.h"

class Spammer : public IModule {
private:
	bool bypass = true;
	int delay = 2;
	std::string message = "Horion - the best minecraft bedrock utility mod - horion.download";

public:
	Spammer();
	~Spammer();

	inline std::string& getMessage() { return message; };
	inline int& getDelay() { return delay; };
	inline bool& getBypass() { return bypass; };

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
};
