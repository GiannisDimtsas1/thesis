#ifndef _MYROUTINGPROTOCOL_H_
#define _MYROUTINGPROTOCOL_H_

#include <map>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <omnetpp.h>
#include <sstream>
#include "VirtualRouting.h"
#include "MyRoutingProtocolPacket_m.h"

using namespace std;

enum MyRoutingTimers {
	START_ROUND = 1,	
	SEND_ADV = 2,	
	JOIN_CH = 3,		
	MAKE_TDMA = 4,			
	START_SLOT = 5,	
	END_SLOT = 6,
	SEND_SINK = 7
};

struct sink {
	int id;          // the Sink's ID
	int x;       // the Sink's coordonates
	int y;
};

struct CHInfo
{
	int src;
	double rssi;
};

class myRoutingProtocol: public VirtualRouting {
	private:
		string applicationID;
		int advPacketSize;
		int tdmaPacketSize;
		int dataPacketSize;
		int joinPacketSize;
		
		double slotLength;
		int clusterLength;
		double roundLength;
		bool isCH;
		bool isSink;
		bool endFormClus;
		bool soloNode;
		string clusterHead;
		vector<RoutingPacket> bufferAggregate;
		list <CHInfo> CHcandidates;
		queue <cPacket *> tempTXBuffer;
		vector <int> clusterMembers;
		
	protected:
		void startup();
		void finishSpecific();
		void fromApplicationLayer(cPacket *, const char *);
		void fromMacLayer(cPacket *, int, double, double);
		void handleNetworkControlCommand(cMessage *);
		void sendTopologySetupPacket();
		void timerFiredCallback(int);
		void processBufferedPacket();
		void sendAggregate();
		void setStateSleep();
		
		double getResidualEnergy();
		double getHarvestingRatio();
		double getCurrentEnergyLevel();
		double getHarvestingPower();
		double getMaxHarvestingPower();
		double getRemainingEnergy();
		double getCurEnergyLevel();

};
		bool cmpRssi(CHInfo a, CHInfo b);
		
#endif
