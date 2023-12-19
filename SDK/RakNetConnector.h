#pragma once

#include "../Utils/Utils.h"
#include <cstdint>
#include <string>

class RakNetConnector {
public:
	CLASS_FIELD(numericalIp, 0x410, std::string);
	CLASS_FIELD(serverIp, 0x430, std::string);
	CLASS_FIELD(serverPort, 0x450, uint32_t);

private:
	virtual ~RakNetConnector();
	virtual __int64 onAppResumed(void);
	virtual __int64 onAppSuspended(void);

public:
	virtual std::string* getLocalIps(std::string*);
	virtual std::string* getLocalIp(std::string*);

public:
	virtual unsigned int getPort(void) const;

private:
	virtual __int64 getRefinedLocalIps(void) const;
	virtual __int64 getConnectedGameInfo(void) const;

public:
	virtual void setupNatPunch(bool);

private:
	virtual __int64 getNatPunchInfo(void) const;
	virtual __int64 startNatPunchingClient(__int64);
	virtual __int64 addConnectionStateListener(__int64);
	virtual __int64 removeConnectionStateListener(__int64);

public:
	virtual bool isIPv4Supported(void) const;
	virtual bool isIPv6Supported(void) const;

private:
	virtual __int64 getIPv4Port(void) const;
	virtual __int64 getIPv6Port(void) const;
	virtual __int64 getGUID(void) const;

public:
	inline bool isOnAServer() { return !this->serverIp.empty(); }
};
