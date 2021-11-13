/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/


#ifndef GENERIC_CURVE_H
#define GENERIC_CURVE_H

#include <cstdint>

namespace der{

    class GenericCurve{
        public:
            void SetParameter(uint16_t index, float value);
            float *GetActiveCurveParameters();
            void DefaultVoltVarCurve();
        private:
            static const uint16_t CURVE_MODE_TYPE = 245;
            static const uint16_t NUM_POINTS = 246;
            static const uint16_t X_UNITS = 247;
            static const uint16_t Y_UNITS = 248;
            static const uint16_t MIN_CURVE_INDEX = 244;
            static const uint16_t MAX_CURVE_INDEX = 448;
            static const uint16_t NUM_PARAMS = 205;

            float m_parameters[NUM_PARAMS] = {0};
    };
}

#endif