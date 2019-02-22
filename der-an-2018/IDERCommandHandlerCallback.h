/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#ifndef IDER_COMMAND_HANDLER_CALLBACK_H
#define IDER_COMMAND_HANDLER_CALLBACK_H

#include "GenericCurve.h"
#include "Schedule.h"

/* Interface that sends packaged smart function commands to the downstream device */
namespace der{

    class IDERCommandHandlerCallback {
        public:
            virtual ~IDERCommandHandlerCallback(){};

            virtual void EnableEnterService(double voltageHighLimit, double voltageLowLimit, double frequencyHighLimit, double frequencyLowLimit, double derStartDelay, double derStartTimeWindow, double derStartRampUpTime) = 0;
            virtual void DisableEnterService() = 0;

            virtual void EnableActiveLimitPower(double activePowerLimitDisCharge, double activePowerLimitCharge) = 0;
            virtual void DisableActiveLimitPower() = 0;

            virtual void EnablePowerFactor(double powerFactorDischarging, double powerFactorCharging, bool powerFactorExcitationDischarging, bool powerFactorExcitationCharging) = 0;
            virtual void DisablePowerFactor() = 0;

            virtual void EnableConstantVARs(double constReactivePower) = 0;
            virtual void DisableConstantVARs() =0;

            virtual void EnableCurve(GenericCurve* genericCurve) = 0;
            virtual void DisableCurve(GenericCurve* genericCurve) = 0;

            virtual void EnableSchedule(Schedule* schedule) = 0;
            virtual void DisableSchedule(Schedule* schedule) = 0;
    };

}

#endif