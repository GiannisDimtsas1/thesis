#include "MyRoutingProtocol.h"
#include "VirtualEnergyManager.h"
#include "VirtualEnergyHarvester.h"

Define_Module(myRoutingProtocol);


void myRoutingProtocol::startup(){
	//percentage = par("percentage");
	roundLength = par("roundLength");
	isSink = par("isSink");
	slotLength = par("slotLength");
	advPacketSize = par("advPacketSize");
	joinPacketSize = par("joinPacketSize");
	tdmaPacketSize = par("tdmaPacketSize");
	dataPacketSize = par("dataPacketSize");
	applicationID = par("applicationID").stringValue(); 
	
	CHcandidates.clear();
	clusterMembers.clear();
	//roundNumber=0;
	isCH = false;
	endFormClus = false;
	bool soloNode = false;
	if(!isSink) setTimer(START_ROUND, 0);
	declareOutput("Propagated_data");
	
}

void myRoutingProtocol::finishSpecific(){
	trace() << self << "Finish Specific";
}

void myRoutingProtocol::fromApplicationLayer(cPacket* pkt, const char *destination){

	if(!isSink)
	{
		string dst(destination);
		MyRoutingProtocolPacket *dataPacket = new MyRoutingProtocolPacket("My routing data packet", NETWORK_LAYER_PACKET);
		dataPacket->setMyRoutingProtocolPacketKind(MY_ROUTING_DATA_PACKET);
		dataPacket->setByteLength(dataPacketSize);
		dataPacket->setSource(SELF_NETWORK_ADDRESS);
		dataPacket->setDestination(destination);
		//encapsulatePacket(dataPacket, pkt);
		double remainingEnergy = getRemainingEnergy();
		double initialEnergy = 21168;
		//trace()<<"Packet destination is: "<<dst;
		if (!isCH)
		{
			if(remainingEnergy>=0.90*initialEnergy)
			{
			
				//trace() << "Node " << SELF_NETWORK_ADDRESS << "sents data to node: " << destination;
				dataPacket->setDestination(SINK_NETWORK_ADDRESS);
				encapsulatePacket(dataPacket, pkt);
				//processBufferedPacket();
				toMacLayer(dataPacket, BROADCAST_MAC_ADDRESS);				
			
			}
			else if(remainingEnergy<0.90*initialEnergy && remainingEnergy>=0.30*initialEnergy)
			{
				//CHInfo info = *CHcandidates.begin();
				stringstream buffer;
				buffer << clusterHead;
				string dst = buffer.str();
				dataPacket->setDestination(dst.c_str());
				encapsulatePacket(dataPacket, pkt);
				trace()<<"Node "<< self <<" sends data to " <<dst;
				//toMacLayer(dataPacket,BROADCAST_MAC_ADDRESS);
				//bufferPacket(dataPacket);
				//tempTXBuffer.push(dataPacket);
				//toMacLayer(dataPacket,BROADCAST_MAC_ADDRESS);
				processBufferedPacket();
			}
			
		}
		
		/*else if (!isCH && !endFormClus) 
		{
			tempTXBuffer.push(dataPacket);
		}*/
		else if (isCH) 
		{
			bufferAggregate.push_back(*dataPacket);
			
			encapsulatePacket(dataPacket,pkt);
			toMacLayer(dataPacket,BROADCAST_MAC_ADDRESS);
			//trace()<<"fromApplicationLayer debugging test. Checking isCH state. dataPacket->getSource: " << dataPacket->getSource();
			//trace() << "Node " << dataPacket->getSource() << " has remaining energy 2: " << getRemainingEnergy() << "\n";
		}
	}
}

void myRoutingProtocol::fromMacLayer(cPacket *pkt, int macAddress, double rssi, double lqi){
	MyRoutingProtocolPacket *netPacket = dynamic_cast <MyRoutingProtocolPacket*>(pkt);
	if (!netPacket)
		return;
		
	switch (netPacket->getMyRoutingProtocolPacketKind()) {
		case MY_ROUTING_DATA_PACKET:{
		collectOutput("Propagated_data");
			string dst(netPacket->getDestination());

			if (isCH && dst.compare(SELF_NETWORK_ADDRESS) == 0){
				//trace() << "Node " << self << " Aggregate Data Frame from node "<<netPacket->getSource();
				netPacket->setDestination(SINK_NETWORK_ADDRESS);
				bufferAggregate.push_back(*netPacket);
				sendAggregate();
				processBufferedPacket();
				//trace()<<"Cluster head" << self << " received data packet from " << netPacket->getSource() << " and sends it to " << pkt->getDestination();
				toApplicationLayer(decapsulatePacket(pkt));	
			}
			else if(!isCH && dst.compare(SINK_NETWORK_ADDRESS)==0 && !isSink) 
			{
				//trace() << "Node " << self << " sent data to " << dst;
				//bufferPacket(netPacket);
				netPacket->setSource(netPacket->getSource());
				netPacket->setDestination(SINK_NETWORK_ADDRESS);
				//trace()<<"Node " << self << " received data packet from " << netPacket->getSource() << " and sends it to " << dst;
				toApplicationLayer(decapsulatePacket(pkt));
			
			}
			else if (dst.compare(SINK_NETWORK_ADDRESS) == 0 && isSink) 
			{
				//trace() << "Node " << self << " Processing Data Packet from " << netPacket->getSource();
				//netPacket->setDestination(SINK_NETWORK_ADDRESS);
				//trace()<<"fromMacLayer debugging test. netPacket->getSource(): "<<netPacket->getSource();
				toApplicationLayer(decapsulatePacket(pkt));
			}
			break;	
		}
		case MY_ROUTING_ADV_PACKET:{
			if(!isCH && !isSink)
			{	
				trace() << "Node " << self << " Received a Advertisement Message of node " << netPacket->getSource();	
				CHInfo rec;
				rec.src = atoi(netPacket->getSource());
				rec.rssi = rssi;
				CHcandidates.push_front(rec);
				//trace()<<"CH "<<CHcandidates.front().src<< " added to candidates list.\n";

			}
			break;
		}
		case MY_ROUTING_JOIN_PACKET:{
			string dst(netPacket->getDestination());
			if(isCH && dst.compare(SELF_NETWORK_ADDRESS) == 0) {
				trace() << "Node " << self << " Received a Join Request from node " << netPacket->getSource() << ". Adding to clusterMembers\n";
				clusterMembers.push_back(atoi(netPacket->getSource()));

			}
	 		break;
		}
		case MY_ROUTING_TDMA_PACKET:{
			if(!isCH && !isSink)
			{
				clusterLength = netPacket->getScheduleArraySize();
				for(int i=0; i<netPacket->getScheduleArraySize(); i++) {
					if (netPacket->getSchedule(i)==atoi(SELF_NETWORK_ADDRESS)) {	
						setStateSleep();
						//setTimer(START_SLOT, i*slotLength);
						trace() << "Node " << self << " Received TDMA pkt from node: "<<netPacket->getSource()<< " I am: "<< i << "th \n";
						break;
					}			
				}	
			}
			break;
		}
	
	}
}

void myRoutingProtocol::handleNetworkControlCommand(cMessage*){

}

void myRoutingProtocol::timerFiredCallback(int index){

	switch (index) {
	
		case START_ROUND:{
			endFormClus = false;
			CHcandidates.clear();
			clusterMembers.clear();
			double timer = uniform(0,1);
			double currentEnergyLevel = getCurrentEnergyLevel();
			double harvestingPower = getHarvestingPower();
			double initialEnergy = 21168;
			double remainingEnergy = getRemainingEnergy();
			double maxHarvestingPower = getMaxHarvestingPower(); //It doesn't work. Returns always 0.
			double chJoin = uniform(0,1);
			string clusterHead;


			if(!isSink){
					
				
				//check if energy level and harvesting power requirments permits the node to be a CH
				if (remainingEnergy >= 0.90*initialEnergy && harvestingPower >= 0.75*harvestingPower && chJoin >= 0.5) 
				{
					trace() << "Node ID:" << self << "-> Cluster Head. Remaining energy: "<<getRemainingEnergy();
					setTimer(SEND_ADV, 0);	
					setTimer(MAKE_TDMA, 60.0);
					isCH=true;	
					
				}
				
				else if(remainingEnergy >= 0.90*initialEnergy && !isCH) //It allows the node to send data to sink without joining a cluster
				{
					trace() << "Node ID:" << self << "->Non-Cluster Head. Remaining energy: " << getRemainingEnergy();
					soloNode = true;
					isCH=false;
					//setTimer(MAKE_TDMA, 0);
					
				}
				else if(remainingEnergy<0.90*initialEnergy && remainingEnergy>=0.30*initialEnergy)//The node has low energy remaining, so it has to join a Cluster.
				{
					trace() << "Node ID:" << self << "-> Non-Cluster Head. Remaining energy: " << getRemainingEnergy();
					isCH=false;
					setTimer(JOIN_CH, 16.0);
					//setTimer(MAKE_TDMA,20);
				}
				else
				{
					setTimer(END_SLOT, 1.0+timer);
					isCH=false;
				
				}

			}
			
			
			break;
			
		}	
		case SEND_ADV:{
			MyRoutingProtocolPacket *crtlPkt = new MyRoutingProtocolPacket("ClusterHead Announcement Packet", NETWORK_LAYER_PACKET);
			crtlPkt->setByteLength(advPacketSize);
			crtlPkt->setMyRoutingProtocolPacketKind(MY_ROUTING_ADV_PACKET);
			crtlPkt->setSource(SELF_NETWORK_ADDRESS);
			crtlPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
			toMacLayer(crtlPkt, BROADCAST_MAC_ADDRESS);
			trace() << "Node " << self << " Sent Beacon.";
			break;
		}
		case JOIN_CH:{
			if(CHcandidates.size()!=0){	
				CHcandidates.sort(cmpRssi);
				MyRoutingProtocolPacket *crtlPkt = new MyRoutingProtocolPacket("ClusterMember Join Packet", NETWORK_LAYER_PACKET);
				crtlPkt->setMyRoutingProtocolPacketKind(MY_ROUTING_JOIN_PACKET);
				crtlPkt->setByteLength(joinPacketSize);
				crtlPkt->setSource(SELF_NETWORK_ADDRESS);
				CHInfo info = *CHcandidates.begin();
				stringstream buffer;
				buffer << info.src;
				string dst = buffer.str();
				crtlPkt->setDestination(dst.c_str());			
				toMacLayer(crtlPkt, BROADCAST_MAC_ADDRESS);
				endFormClus = true;
				trace() << "Node " << self << " Sent Join Request to " << dst;
				clusterHead = dst;
				
			}
			break;
		}
		case MAKE_TDMA:{

			if (clusterMembers.size()!=0){
				trace()<<"Cluster Head's "<< self<< " size cluster is: " << clusterMembers.size();
				MyRoutingProtocolPacket *crtlPkt = new MyRoutingProtocolPacket("ClusterHead TDMA Packet", NETWORK_LAYER_PACKET);
				crtlPkt->setByteLength(tdmaPacketSize);
				crtlPkt->setMyRoutingProtocolPacketKind(MY_ROUTING_TDMA_PACKET);
				crtlPkt->setSource(SELF_NETWORK_ADDRESS);
				crtlPkt->setDestination(BROADCAST_NETWORK_ADDRESS);
				clusterLength = clusterMembers.size();
				trace()<<"Cluster length of node " << self << " is "<<clusterLength;	
				crtlPkt->setScheduleArraySize(clusterMembers.size());	
				for(int i=0; i<clusterLength; i++) crtlPkt->setSchedule(i,clusterMembers[i]);
				toMacLayer(crtlPkt, BROADCAST_MAC_ADDRESS);
				//trace() << "Node " << self << " Sent TDMA pkt";
				//setTimer(START_SLOT, clusterLength*slotLength);
			}
			//else setTimer(START_SLOT, slotLength);						
			break;
		}
		case START_SLOT:{
			//if(isCH && clusterMembers.size()==0) setTimer(START_SLOT, slotLength);	
			//else setTimer(START_SLOT, clusterLength*slotLength);
			
			if (isCH){
				sendAggregate(); 
				processBufferedPacket();
				//trace() << "Node " << self << " Sent Pkt Aggr"  << "\n";
			}
			if (!isCH && !soloNode){

				//CHInfo info = *CHcandidates.begin();
				
				//trace() << "Node " << self << " Sent Data Packet to node " << clusterHead << "\n";
				//processBufferedPacket();
				//setTimer(END_SLOT, slotLength);
			}
			/*if(!isCH && soloNode)
			{
				MyRoutingProtocolPacket *newPacket = new MyRoutingProtocolPacket("Node send data packet",NETWORK_LAYER_PACKET);
				newPacket->setByteLength(dataPacketSize);
				newPacket->setMyRoutingProtocolPacketKind(MY_ROUTING_DATA_PACKET);
				newPacket->setSource(SELF_NETWORK_ADDRESS);
				newPacket->setDestination(SINK_NETWORK_ADDRESS);
				toMacLayer(newPacket, BROADCAST_MAC_ADDRESS);
				trace() << "Sink received data packet from node " << self;
			
			}*/
			break;
		}
		case END_SLOT:{
			
			//trace() << "Node " << self << " Sleept"  << "\n";
			if (!isSink && !isCH) send(createRadioCommand(SET_STATE, SLEEP), "toMacModule");
			break;
			
		}
		case SEND_SINK:{
			MyRoutingProtocolPacket *newPacket = new MyRoutingProtocolPacket("Node send data packet",NETWORK_LAYER_PACKET);
			newPacket->setByteLength(dataPacketSize);
			newPacket->setMyRoutingProtocolPacketKind(MY_ROUTING_DATA_PACKET);
			newPacket->setSource(SELF_NETWORK_ADDRESS);
			newPacket->setDestination(SINK_NETWORK_ADDRESS);
			//toMacLayer(newPacket, BROADCAST_MAC_ADDRESS);
			//trace() << "Sink received data packet from node " << self;
			break;
		}
	}
}

void myRoutingProtocol::sendAggregate()
{
	if(bufferAggregate.size()!=0)
	{
		MyRoutingProtocolPacket *aggrPacket = new MyRoutingProtocolPacket("ClusterHead Aggredated Packet", NETWORK_LAYER_PACKET);
		aggrPacket->setByteLength(dataPacketSize+4);
		aggrPacket->setMyRoutingProtocolPacketKind(MY_ROUTING_DATA_PACKET);
		aggrPacket->setSource(SELF_NETWORK_ADDRESS);
		aggrPacket->setDestination(SINK_NETWORK_ADDRESS);
		
		ApplicationPacket *newPacket = new ApplicationPacket("App generic packet", APPLICATION_PACKET);
		newPacket->setData(0);
		newPacket->getAppNetInfoExchange().destination = string("0");
		newPacket->getAppNetInfoExchange().source = selfAddress;
		newPacket->getAppNetInfoExchange().timestamp = simTime();
		newPacket->setApplicationID(applicationID.c_str());
		encapsulatePacket(aggrPacket, newPacket);

		bufferPacket(aggrPacket);
		bufferAggregate.clear();	
	}


}

void myRoutingProtocol::processBufferedPacket()
{
	

	while (!TXBuffer.empty()) {
		//edw prepei na dw oti o ch pairnei to data kai to stelnei sto sink
		//trace()<<"Cluster head receives from node "<<TXBuffer.front().src<< " and sents the data to "<<TXBuffer.front().dst;
		//trace()<<" TXBuffer.front() from cluster head "<< self << " is " <<TXBuffer.front();

		//toApplicationLayer(decapsulatePacket(TXBuffer.front()));
		
		toMacLayer(TXBuffer.front(),BROADCAST_MAC_ADDRESS);
		TXBuffer.pop();
	}	
}

double myRoutingProtocol::getCurrentEnergyLevel(){
	VirtualEnergyManager* energyStrg = check_and_cast<VirtualEnergyManager*>(getParentModule()->getParentModule()->getSubmodule("ResourceManager")->getSubmodule("EnergySubsystem")->getSubmodule("EnergyManager"));
	
	double currentEnergyRatio = energyStrg->getCurrentEnergyRatio();
	return currentEnergyRatio;
}

double myRoutingProtocol::getHarvestingPower(){
	VirtualEnergyManager* harvestingPwr = check_and_cast<VirtualEnergyManager*>(getParentModule()->getParentModule()->getSubmodule("ResourceManager")->getSubmodule("EnergySubsystem")->getSubmodule("EnergyManager"));
	
	double harvestingPower = harvestingPwr->getCurrentHarvestingPower();
	return harvestingPower;
}

double myRoutingProtocol::getRemainingEnergy(){

	VirtualEnergyManager* remEnergy = check_and_cast<VirtualEnergyManager*>(getParentModule()->getParentModule()->getSubmodule("ResourceManager")->getSubmodule("EnergySubsystem")->getSubmodule("EnergyManager"));
	double remainingEnergy = remEnergy->getRemainingEnergy();
	return remainingEnergy;
}

bool cmpRssi(CHInfo a, CHInfo b){
	return (a.rssi > b.rssi);
}

double myRoutingProtocol::getMaxHarvestingPower(){
	VirtualEnergyManager* maxHarvestingPwr = check_and_cast<VirtualEnergyManager*>(getParentModule()->getParentModule()->getSubmodule("ResourceManager")->getSubmodule("EnergySubsystem")->getSubmodule("EnergyManager"));
	
	double maxHarvestingPower = maxHarvestingPwr->getMaxHarvestingPower();
	return maxHarvestingPower;
}

void myRoutingProtocol::setStateSleep()
{
	send(createRadioCommand(SET_STATE, SLEEP), "toMacModule");
}

