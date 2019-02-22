/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#include "DERCommandHandlerCallbackDefault.h"

using namespace der;

DERCommandHandlerCallbackDefault DERCommandHandlerCallbackDefault::m_instance;

std::shared_ptr<IDERCommandHandlerCallback> DERCommandHandlerCallbackDefault::getInstance(){
    auto commandPtr = std::make_shared<DERCommandHandlerCallbackDefault>(m_instance);
    return std::dynamic_pointer_cast<IDERCommandHandlerCallback>(commandPtr);
}

void DERCommandHandlerCallbackDefault::EnableEnterService(double voltageHighLimit, double voltageLowLimit, double frequencyHighLimit, double frequencyLowLimit, double derStartDelay, double derStartTimeWindow, double derStartRampUpTime){
        std::cout << "Enable Enter Service function \n";
}

void DERCommandHandlerCallbackDefault::DisableEnterService(){
        std::cout << "Disable AEnter Service function \n";
}

void DERCommandHandlerCallbackDefault::EnableActiveLimitPower(double activePowerLimitDisCharge, double activePowerLimitCharge){
        std::cout << "Enable Active Power Limit function \n";
}

void DERCommandHandlerCallbackDefault::DisableActiveLimitPower(){
        std::cout << "Disable Active Power Limit function \n";
}

void DERCommandHandlerCallbackDefault::EnablePowerFactor(double powerFactorDischarging, double powerFactorCharging, bool powerFactorExcitationDischarging, bool powerFactorExcitationCharging){
        std::cout << "Enable Power Factor \n";
}

void DERCommandHandlerCallbackDefault::DisablePowerFactor() {
        std::cout << "Disable Power Factor \n";
}

void DERCommandHandlerCallbackDefault::EnableConstantVARs(double constReactivePower){
        std::cout << "Enable Constant VARs \n";
}

void DERCommandHandlerCallbackDefault::DisableConstantVARs(){
        std::cout << "Disable Constant VARs \n";
}

void DERCommandHandlerCallbackDefault::EnableCurve(GenericCurve* genericCurve){
        std::cout << "Enable Generic Curve \n";
}

void DERCommandHandlerCallbackDefault::DisableCurve(GenericCurve* genericCurve){
        std::cout << "Disable Generic Curve \n";
}

void DERCommandHandlerCallbackDefault::EnableSchedule(Schedule* schedule){
        std::cout << "Enable schedule \n";
} 

void DERCommandHandlerCallbackDefault::DisableSchedule(Schedule* schedule){
        std::cout << "Disable schedule \n";
} 