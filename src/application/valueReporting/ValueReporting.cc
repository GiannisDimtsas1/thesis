/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Dimosthenis Pediaditakis, Yuriy Tselishchev                  *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "ValueReporting.h"

Define_Module(ValueReporting);

void ValueReporting::startup()
{
	maxSampleInterval = ((double)par("maxSampleInterval")) / 1000.0;
	minSampleInterval = ((double)par("minSampleInterval")) / 1000.0;
	currSentSampleSN = 0;
	randomBackoffIntervalFraction = genk_dblrand(0);
	
	numNodes = getParentModule()->getParentModule()->par("numNodes");

	recipientAddress = par("nextRecipient").stringValue();
	recipientId = atoi(recipientAddress.c_str());
	packetsSent.clear();
	packetsReceived.clear();
	packetsAggregated.clear();
	
	packetsSentSum = 0;
	packetsReceivedSum =0;
	dataSN = 0;
	sentOnce = false;
	setTimer(REQUEST_SAMPLE, maxSampleInterval * randomBackoffIntervalFraction);
	
	declareOutput("Packets received per node");
	declareOutput("Packets generated");
	declareOutput("Packets aggregated");
}

void ValueReporting::timerFiredCallback(int index)
{
	switch (index) {

		case REQUEST_SAMPLE:{
			requestSensorReading();
			trace() << "Sending packet #" << dataSN << " to node " << recipientAddress;
			//Appinfos()<<"S "<<SELF_NETWORK_ADDRESS<<" "<<dataSN;
			//toNetworkLayer(createGenericDataPacket(0, dataSN), recipientAddress.c_str());
			collectOutput("Packets generated");
			packetsSent[recipientId]++;
			dataSN++;
			setTimer(REQUEST_SAMPLE, maxSampleInterval);
			break;
		}
	}
}

void ValueReporting::fromNetworkLayer(ApplicationPacket * genericPacket,
		 const char *source, double rssi, double lqi)
{
	ValueReportingDataPacket *rcvPacket = check_and_cast<ValueReportingDataPacket*>(genericPacket);
	ValueReportData theData = rcvPacket->getExtraData();
	
	int sourceId = atoi(source);
	
	
	
	
	if (recipientAddress.compare(SELF_NETWORK_ADDRESS) == 0) {
		collectOutput("Packets aggregated",sourceId,"",(int)rcvPacket->getData());
		trace()<<"Node "<<self<<" received data packet from node "<<sourceId;
		collectOutput("Packets received per node", sourceId);
		packetsReceived[sourceId]++;
	
	}/*else {
		ApplicationPacket* fwdPacket = rcvPacket->dup();
		// Reset the size of the packet, otherwise the app overhead will keep adding on
		fwdPacket->setByteLength(0);
		toNetworkLayer(fwdPacket, recipientAddress.c_str());
	}*/
	
	if (isSink){
		trace() << "Sink received from: " << theData.nodeID << " \tvalue=" << rcvPacket->getData();
		//Appinfos()<<"R "<<SELF_NETWORK_ADDRESS<<" "<<source<<" "<<"Sensed_"<<rcvPacket->getData()<<"_from_node_"<<theData.nodeID;	
	}
}

void ValueReporting::handleSensorReading(SensorReadingMessage * rcvReading)
{
	// int sensIndex =  rcvReading->getSensorIndex();
	// string sensType(rcvReading->getSensorType());
	
	
	double sensValue = rcvReading->getSensedValue();

	// schedule the TX of the value
	trace() << "Sensed = " << sensValue;

	ValueReportData tmpData;
	tmpData.nodeID = (unsigned short)self;
	//tmpData.locX = mobilityModule->getLocation().x;
	//tmpData.locY = mobilityModule->getLocation().y;

	ValueReportingDataPacket *packet2Net =
	    new ValueReportingDataPacket("Value reporting pck", APPLICATION_PACKET);
	packet2Net->setExtraData(tmpData);
	packet2Net->setData(sensValue);
	packet2Net->setSequenceNumber(currSentSampleSN);
	currSentSampleSN++;

	toNetworkLayer(packet2Net, SINK_NETWORK_ADDRESS);
	//Appinfos()<<"S "<<SELF_NETWORK_ADDRESS<<" to "<<SINK_NETWORK_ADDRESS<<" Sensed_value_"<<sensValue;
	sentOnce = true;
}

void ValueReporting::finishSpecific() {

	declareOutput("Packets reception rate");
	declareOutput("Packets loss rate");

	cTopology *topo;	// temp variable to access packets received by other nodes
	topo = new cTopology("topo");
	topo->extractByNedTypeName(cStringTokenizer("node.Node").asVector());

	for (int i = 0; i < numNodes; i++) {
		ValueReporting *appModule = dynamic_cast<ValueReporting*>
			(topo->getNode(i)->getModule()->getSubmodule("Application"));
		if (appModule) {
			int packetsSent = appModule->getPacketsSent(self);
			if (packetsSent > 0) { // this node sent us some packets
				float rate = (float)packetsReceived[i]/packetsSent;
				collectOutput("Packets reception rate", i, "total", rate);
				collectOutput("Packets loss rate", i, "total", 1-rate);
			}


		}
	}
	delete(topo);

}
