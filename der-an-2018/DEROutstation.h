/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#ifndef DER_OUTSTATION_H
#define DER_OUTSTATION_H

#include <opendnp3/DNP3Manager.h>

#include <opendnp3/logging/LogLevels.h>

#include <string>
#include <thread>
#include <iostream>

#include "DERAppNoteDefinition.h"
#include "DERCommandHandler.h"
#include "DERMeasurementValues.h"
#include "CSVReaderMeasurement.h"
#include "DERCommandHandlerCallbackDefault.h"


using namespace std;
using namespace opendnp3;

namespace der{
    class DEROutstation{
        private:
            struct State
            {
                uint32_t count = 0;
                double value = 0;
                bool binary = false;
                DoubleBit dbit = DoubleBit::DETERMINED_OFF;
                uint8_t octetStringValue = 1;
            };
            std::shared_ptr<IOutstation> outstation;
            UpdateBuilder builder;
            CSVReaderMeasurement csv;
            vector<AnalogValues> aiMeas; 
            vector<AnalogValues> aoMeas; 
            vector<BinaryValues> biMeas;  
            vector<BinaryValues> boMeas;  

            std::string caCertificate = "TLS/ca.crt";
		    std::string certificateChain = "TLS/ia_master_chain.pem";
		    std::string privateKey = "TLS/ia.key";

            std::string filePath = "";

        public:
            void CreateOutstation();
            void AddUpdates(UpdateBuilder& builder, State& state, const std::string& arguments);    
            void UpdateBinaryInputs(vector<BinaryValues>);
            void UpdateAnalogInputs(vector<AnalogValues>);
            void UpdateBinaryOutputs(vector<BinaryValues>);
            void UpdateAnalogOutputs(vector<AnalogValues>);
    };
}

#endif