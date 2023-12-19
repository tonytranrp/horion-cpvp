#pragma once

#include "../Module.h"

// device id spoofed in ConnectionRequest::create hook
class DeviceIDSpoofer : public IModule {
public:
	DeviceIDSpoofer();

	virtual const char* getModuleName() override;
	virtual void onEnable() override;
};