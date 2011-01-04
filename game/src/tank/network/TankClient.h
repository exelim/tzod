// TankClient.h

#pragma once


#include "Socket.h"
#include "ControlPacket.h"


/////////////////////////////////////////////////////////

struct NetworkStats
{
	size_t  bytesSent;
	size_t  bytesRecv;
	size_t  bytesPending;
};

struct ILevelController;
struct ControlPacket;
struct PlayerDesc;
struct BotDesc;

class Peer;
class Variant;


class TankClient
{
public:
	SafePtr<Peer> _peer;
	int _frame;
	ControlPacketVector _ctrl;
	float _boost;
	bool _hasCtrl;
	bool _gameStarted;

	void OnDisconnect(Peer *, int err);

public:
	int _latency;

private:
	NetworkStats _stats;
	ILevelController *_levelController;
	bool _isLocal;

public:
	TankClient(bool isLocal, ILevelController *levelController);
	~TankClient();

	bool IsLocal() const { return true; } // TODO: FIXME
	float GetBoost() const { return _boost; }

	void Connect(const string_t &hostaddr);

	void SendPlayerReady(bool ready);
	void SendAddBot(const BotDesc &bot);
	void SendPlayerInfo(const PlayerDesc &pd);
	void SendTextMessage(const std::string &msg);
	void SendControl(const ControlPacket &cp); // this function terminates current frame and starts next
	bool RecvControl(ControlPacketVector &result);

	void GetStatistics(NetworkStats *pStats);



	Delegate<void()> eventPlayersUpdate;
	Delegate<void()> eventStartGame;
	Delegate<void(size_t, bool)> eventPlayerReady;
	Delegate<void(const std::string&)> eventTextMessage;
	Delegate<void(const std::string&)> eventErrorMessage;
	Delegate<void()> eventConnected;


private:
	// remote functions
	void ClTextMessage(Peer *from, int task, const Variant &arg);
	void ClErrorMessage(Peer *from, int task, const Variant &arg);
	void ClGameInfo(Peer *from, int task, const Variant &arg);
	void ClPlayerQuit(Peer *from, int task, const Variant &arg);
	void ClControl(Peer *from, int task, const Variant &arg);
	void ClPlayerReady(Peer *from, int task, const Variant &arg);
	void ClStartGame(Peer *from, int task, const Variant &arg);
	void ClAddHuman(Peer *from, int task, const Variant &arg);
	void ClAddBot(Peer *from, int task, const Variant &arg);
	void ClSetPlayerInfo(Peer *from, int task, const Variant &arg);
	void ClSetBoost(Peer *from, int task, const Variant &arg);
};

///////////////////////////////////////////////////////////////////////////////
// end of file
