/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#ifndef DER_COMMAND_HANDLER_CALLBACK_DEFAULT_H
#define DER_COMMAND_HANDLER_CALLBACK_DEFAULT_H

#include "IDERCommandHandlerCallback.h"
#include "GenericCurve.h"
#include "Schedule.h"

#include <iostream>
#include <memory>

namespace der {
    class DERCommandHandlerCallbackDefault : public virtual IDERCommandHandlerCallback{
        public:
            static std::shared_ptr<IDERCommandHandlerCallback> getInstance();

            void EnableEnterService(double voltageHighLimit, double voltageLowLimit, double frequencyHighLimit, double frequencyLowLimit, double derStartDelay, double derStartTimeWindow, double derStartRampUpTime);
            void DisableEnterService();

            void EnableActiveLimitPower(double activePowerLimitDisCharge, double activePowerLimitCharge);
            void DisableActiveLimitPower();

            void EnablePowerFactor(double powerFactorDischarging, double powerFactorCharging, bool powerFactorExcitationDischarging, bool powerFactorExcitationCharging);
            void DisablePowerFactor();

            void EnableConstantVARs(double constReactivePower);
            void DisableConstantVARs();

            void EnableCurve(GenericCurve* genericCurve);
            void DisableCurve(GenericCurve* genericCurve);

            void EnableSchedule(Schedule* schedule);
            void DisableSchedule(Schedule* schedule);

            DERCommandHandlerCallbackDefault() = default;
            ~DERCommandHandlerCallbackDefault() = default;
        private:
            static DERCommandHandlerCallbackDefault m_instance;
    };
}

#endif