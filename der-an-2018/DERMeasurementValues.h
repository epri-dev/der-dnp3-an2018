/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#ifndef DER_MEASUREMENT_VALUES_H
#define DER_MEASUREMENT_VALUES_H

#include <opendnp3/DNP3Manager.h>

using namespace std;
using namespace opendnp3;


namespace der{
    void ReadAnalogValues();
    void ReadBinaryValues();

    struct AnalogValues{
            uint16_t index;
            uint8_t quality;
            double val;
    };

    struct BinaryValues{
            uint16_t index;
            uint8_t quality;
            bool val;
    };
}

#endif