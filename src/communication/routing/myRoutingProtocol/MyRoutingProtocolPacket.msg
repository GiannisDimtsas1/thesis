cplusplus {{
#include "RoutingPacket_m.h"
}}

class RoutingPacket;

enum MyRoutingProtocolPacketDef {
    	MY_ROUTING_ADV_PACKET 	= 1;		
	MY_ROUTING_JOIN_PACKET 	= 2;		
	MY_ROUTING_TDMA_PACKET 	= 3;		
	MY_ROUTING_DATA_PACKET 	= 4;

} 

packet MyRoutingProtocolPacket extends RoutingPacket {
	int MyRoutingProtocolPacketKind enum (MyRoutingProtocolPacketDef);
 	int schedule[];
}

