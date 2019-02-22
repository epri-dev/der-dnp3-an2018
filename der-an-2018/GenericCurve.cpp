/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#include "GenericCurve.h"

namespace der{

    void GenericCurve::SetParameter(u_int16_t index, float value){

        u_int16_t i = NUM_PARAMS - (MAX_CURVE_INDEX - index) - 1;

        if (i < 0 || i >= NUM_PARAMS){
            throw "Invalid Curve Index";
        }

        m_parameters[i] = value;
    }

    float *GenericCurve::GetActiveCurveParameters(){
        return m_parameters;
    }

    /* Default Volt-VAR curve as described in IEEE 1547 for DER Cat B */
    void GenericCurve::DefaultVoltVarCurve(){
        int index = CURVE_MODE_TYPE - MIN_CURVE_INDEX;
        m_parameters[index++] = 2 ; //Curve Mode type
        m_parameters[index++] = 4; //No. of points
        m_parameters[index++] = 129; //X-Axis Units - Percent Voltage
        m_parameters[index++] = 2; //Y-Axis Units - Percent of max VARs

        //Curve Index
        m_parameters[index++] = 92; // X1 :: V1
        m_parameters[index++] = 100; // Y1 :: Q1

        m_parameters[index++] = 98; // V2
        m_parameters[index++] = 0; // Q2

        m_parameters[index++] = 102; //V3
        m_parameters[index++] = 0; //Q3

        m_parameters[index++] = 108; // V4
        m_parameters[index++] = -100; // Q4
    }    
}