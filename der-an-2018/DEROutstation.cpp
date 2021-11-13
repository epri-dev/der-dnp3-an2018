/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#include "DEROutstation.h"

#include <opendnp3/ConsoleLogger.h>
#include <opendnp3/master/PrintingSOEHandler.h>
#include <opendnp3/outstation/UpdateBuilder.h>
#include <opendnp3/channel/PrintingChannelListener.h>
#include <opendnp3/outstation/DefaultOutstationApplication.h>

using namespace der;

void DEROutstation::CreateOutstation()
{
	//Copyright Text
	std::cout<<"******************************************************************************"<<endl;
	std::cout<<"DER Outstation AN2018 (DOSTAN18) Version 0.1.7"<<endl;
	std::cout<<"******************************************************************************"<<endl;
	std::cout<<"Copyright © 2019 Electric Power Research Institute, Inc. All Rights Reserved."<<endl;
	std::cout<<"Permission to use, copy, modify, and distribute this software for any purpose" <<endl;
	std::cout<<"with or without fee is hereby granted, provided that the above copyright notice "<<endl;
	std::cout<<"and this permission notice appear in all copies."<<endl;
	std::cout<<"******************************************************************************"<<endl;
	std::cout<<"This software is provided by EPRI \"AS IS\" and without customer support beyond "<<endl;
	std::cout<<"such embodiments within the distribution of this software that may or may not "<<endl;
	std:cout<<"provide such support."<<endl;
	std::cout<<"******************************************************************************"<<endl;

	// Specify what log levels to use. NORMAL is warning and above
	// You can add all the comms logging by uncommenting below.
	// const opendnp3::LogLevels FILTERS = levels::ALL;
	const opendnp3::LogLevels FILTERS = levels::NORMAL;
	

	/* Find the number of cores on the machine */
	uint32_t concurentThreadsSupported = std::thread::hardware_concurrency();
	if(concurentThreadsSupported == 0)
		concurentThreadsSupported = 1;

	// This is the main point of interaction with the stack
	// Allocate a single thread to the pool since this is a single outstation
	// Log messages to the console
	DNP3Manager manager(concurentThreadsSupported, ConsoleLogger::Create());

	char in;
	std::cout << "Do you want to create a TLS channel?[Y/N]" << std::endl;
	std::cin >> in;

	std::shared_ptr<IChannel> channel;

	if(in == 'Y')
	{
		// Create a TLSserver (listener)
		std::cout << "Creating TLS Server" << std::endl;

		std::cout << "Using CA certificate: " << caCertificate << std::endl;
		std::cout << "Using certificate chain: " << certificateChain << std::endl;
		std::cout << "Using private key file: " << privateKey << std::endl;

		channel = manager.AddTLSServer("server", FILTERS, ServerAcceptMode::CloseExisting, IPEndpoint("0.0.0.0", 20000), TLSConfig(caCertificate, certificateChain, privateKey,2), PrintingChannelListener::Create());
	}
	else
	{
		// Create a TCP server (listener)
		std::cout << "Creating TCP Server" << std::endl;
		channel = manager.AddTCPServer("server", FILTERS, ServerAcceptMode::CloseExisting, IPEndpoint("0.0.0.0", 20000), PrintingChannelListener::Create());
	}

	// The main object for a outstation. The defaults are useable,
	// but understanding the options are important.
	//OutstationStackConfig config(DatabaseSizes::AllTypes(10));

	// Number of each point type per DER App Note 2018
	 // (DatabaseSizes(329, 0, 1009, 15, 15, 50, 670, 0, 0));
	const uint16_t NUM_BINARY = 329;
	const uint16_t NUM_ANALOG = 1009;
	const uint16_t NUM_COUNTER = 15;
	const uint16_t NUM_FROZEN_COUNTER = 15;
	const uint16_t NUM_BINARY_OUTPUT_STATUS = 50;
	const uint16_t NUM_ANALOG_OUTPUT_STATUS = 670;


	OutstationStackConfig config;

	// Specify the maximum size of the event buffers
	//config.outstation.eventBufferConfig = EventBufferConfig::AllTypes(10);

	// Added the Event buffer size as per DER App Note 2018
	config.outstation.eventBufferConfig.maxAnalogEvents = 1009;
	config.outstation.eventBufferConfig.maxAnalogOutputStatusEvents = 670;
	config.outstation.eventBufferConfig.maxBinaryEvents = 329;
	config.outstation.eventBufferConfig.maxBinaryOutputStatusEvents = 50;

	// you can override an default outstation parameters here
	// in this example, we've enabled the oustation to use unsolicted reporting
	// if the master enables it
	config.outstation.params.allowUnsolicited = true;

	// You can override the default link layer settings here
	// in this example we've changed the default link layer addressing
	config.link.LocalAddr = 10;
	config.link.RemoteAddr = 1;

	config.link.KeepAliveTimeout = opendnp3::TimeDuration::Max();

	// You can optionally change the default reporting variations or class assignment prior to enabling the outstation
	//ConfigureDatabase(config.dbConfig);

	//Mapping default event class, group and variation according to the DER App Note 2018 
	for (uint16_t index = 0; index < NUM_ANALOG_OUTPUT_STATUS; ++index){
			if(AOPoints[index].Supported){
				config.database.analog_output_status[index].clazz = PointClass::Class0;
				config.database.analog_output_status[index].evariation = opendnp3::EventAnalogOutputStatusVariation::Group42Var1;
				config.database.analog_output_status[index].svariation = opendnp3::StaticAnalogOutputStatusVariation::Group40Var1;
			}
	}

	for (int index = 0; index < NUM_BINARY_OUTPUT_STATUS; ++index){
		if(BOPoints[index].Supported){
			config.database.binary_output_status[index].clazz = BOPoints[index].DefEvtClass;
			config.database.binary_output_status[index].evariation = opendnp3::EventBinaryOutputStatusVariation::Group11Var2;
			config.database.binary_output_status[index].svariation = opendnp3::StaticBinaryOutputStatusVariation::Group10Var2;
		}
	}

	for (int index = 0; index < NUM_ANALOG; ++index){
		config.database.analog_input[index].clazz = AIPoints[index].DefEvtClass;
		config.database.analog_input[index].evariation = opendnp3::EventAnalogVariation::Group32Var1;
		config.database.analog_input[index].svariation = opendnp3::StaticAnalogVariation::Group30Var1;
	}

	for (int index = 0; index < NUM_BINARY; ++index){		
		config.database.binary_input[index].clazz = BIPoints[index].DefEvtClass;
		config.database.binary_input[index].evariation = opendnp3::EventBinaryVariation::Group2Var2;
		config.database.binary_input[index].svariation = opendnp3::StaticBinaryVariation::Group1Var2;
	}

	/* Creating an instance of DERCommandHandler to pass it to the Outstation */
	auto derCommand = std::make_shared<DERCommandHandler>(DERCommandHandlerCallbackDefault::getInstance());    
	
	// Create a new outstation with a log level, command handler, and
	// config info this	returns a thread-safe interface used for
	// updating the outstation's database.	
	outstation = channel->AddOutstation("DNP3 Outstation Communications Emulator", derCommand, DefaultOutstationApplication::Create(), config);

	// Enable the outstation and start communications
	outstation->Enable();

	//Set Outstation instance in DER Command Handler
	derCommand->SetOutstation(outstation);

	// variables used in example loop
	string input;
	State state;

	while (true)
	{
		std::cout << "In DER AN2018 outstation. Select an option and then press <enter>" << std::endl;
		std::cout << "a = Update Analog Inputs to Outstation, b = Update Binary Inputs to Outstation"<<endl;
		std::cout << "c = Update Analog Outputs to Outstation, d = Update Binary Outputs to Outstation 'quit' = exit" << std::endl;
		std::cin >> input;

		if (input == "quit") return ; // DNP3Manager destructor cleanups up everything automatically
		else if (input == "a"){
			filePath = "Inputs/AnalogInputs.csv";
			csv.ReadAnalogValues(aiMeas, filePath);
			UpdateAnalogInputs(aiMeas);
		}
		else if (input == "b"){
			 filePath = "Inputs/BinaryInputs.csv";
			 csv.ReadBinaryValues(biMeas, filePath);
			 UpdateBinaryInputs(biMeas);
		}
		else if (input == "c"){
			filePath = "Inputs/AnalogOutputs.csv";
			csv.ReadAnalogValues(aoMeas, filePath);
			UpdateAnalogOutputs(aoMeas);
		}
		else if(input == "d"){
			filePath = "Inputs/BinaryOutputs.csv";
			csv.ReadBinaryValues(boMeas, filePath);
			UpdateBinaryOutputs(boMeas);
		}
	}
}

void DEROutstation::AddUpdates(UpdateBuilder& builder, State& state, const std::string& arguments)
{
	for (const char& c : arguments)
	{
		switch (c)
		{
		case('c'):
			{
				builder.Update(Counter(state.count), 0);
				++state.count;
				break;
			}
		case('a'):
			{
				builder.Update(Analog(state.value), 0);
				state.value += 1;
				break;
			}
		case('b'):
			{
				builder.Update(Binary(state.binary), 0);
				state.binary = !state.binary;
				break;
			}
		case('d'):
			{
				builder.Update(DoubleBitBinary(state.dbit), 0);
				state.dbit = (state.dbit == DoubleBit::DETERMINED_OFF) ? DoubleBit::DETERMINED_ON : DoubleBit::DETERMINED_OFF;
				break;
			}
		case('o'):
			{
				OctetString value(opendnp3::Buffer(&state.octetStringValue, 1));
				builder.Update(value, 0);
				state.octetStringValue += 1;
				break;
			}
		default:
			break;
		}
	}
}

/* Update Binary Inputs to the outstation */
void DEROutstation::UpdateBinaryInputs(vector<BinaryValues> values){
	int size = values.size();
	
	for (auto v : values){
		builder.Update(Binary(v.val, Flags(v.quality)), v.index);
	}
	
	outstation->Apply(builder.Build());
}

/* Update Analog Inputs to the outstation */
void DEROutstation::UpdateAnalogInputs(vector<AnalogValues> values){

	for (auto v : values)
	{
		if(v.val >= AIPoints[v.index].MinValue && v.val <= AIPoints[v.index].MaxValue)
			builder.Update(Analog(v.val, Flags(v.quality)), v.index);
	}

	outstation->Apply(builder.Build());
}

/* Update Binary Outputs to the outstation */
void DEROutstation::UpdateBinaryOutputs(vector<BinaryValues> values){
		
	for (auto v : values)
	{
		if(BOPoints[v.index].Supported){
			builder.Update(BinaryOutputStatus(v.val, Flags(v.quality)), v.index);
			//Update associated BinaryInput
			builder.Update(Binary(v.val, Flags(v.quality)) , BOPoints[v.index].MappedInput);
		}
	}
	outstation->Apply(builder.Build());
}

/* Update Analog Outputs to the outstation */
void DEROutstation::UpdateAnalogOutputs(vector<AnalogValues> values){

	for (auto v : values)
	{
		if(AOPoints[v.index].Supported){
			if(v.val >= AOPoints[v.index].MinValue && v.val <= AOPoints[v.index].MaxValue){
				builder.Update(AnalogOutputStatus(v.val, Flags(v.quality)), v.index);
				//Update associated AnalogInput
				builder.Update(Analog(v.val, Flags(v.quality)),AOPoints[v.index].MappedInput);
			}
		}
	}
	outstation->Apply(builder.Build());
}