	package node.communication.routing.myRoutingProtocol;

	simple myRoutingProtocol like node.communication.routing.iRouting
	{
	 parameters:
		string applicationID = default ("ThroughputTest");
		bool collectTraceInfo;
		int maxNetFrameSize = default (0);
		int netDataFrameOverhead = default (14);
		int netBufferSize = default (32);		
		bool isSink = default (false);
		
		double percentage;	
		double roundLength @unit(s);	
		double slotLength;

		//routing layer packet sizes
		int advPacketSize = default (9); // Type + Source + Destination = 9 bytes
		int joinPacketSize = default (9);
		int tdmaPacketSize = default (150); // Type + Source + Destination + tdma = 150 bytes
		int dataPacketSize = default (9);

	 gates:
		output toCommunicationModule;
		output toMacModule;
		input fromCommunicationModule;
		input fromMacModule;
		input fromCommModuleResourceMgr;
	}
