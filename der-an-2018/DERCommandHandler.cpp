/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#include "DERCommandHandler.h"

using namespace der;

DERCommandHandler::DERCommandHandler(std::shared_ptr<IDERCommandHandlerCallback> callbacks){
    if (callbacks != nullptr) {
        m_callbacks = callbacks;
    }
}

/* Handling SELECT commands for Binary Outputs */
opendnp3::CommandStatus DERCommandHandler::Select(const opendnp3::ControlRelayOutputBlock& command, uint16_t index){
    opendnp3::CommandStatus cmdStat = opendnp3::CommandStatus::NOT_SUPPORTED;

    if(index < 0 || index >= MAX_BOPOINTS)
        return opendnp3::CommandStatus::OUT_OF_RANGE;

    //Return SUCCESS if the device supports the function, else NOT_SUPPORTED
    if(BOPoints[index].Supported)
        return opendnp3::CommandStatus::SUCCESS;

    return cmdStat;
}

const uint8_t ONLINE = 0x81; 

/* Handling OPERATE commands for Binary Outputs */
opendnp3::CommandStatus DERCommandHandler::Operate(const opendnp3::ControlRelayOutputBlock& command, uint16_t index, IUpdateHandler& handler, opendnp3::OperateType opType){
        opendnp3::CommandStatus cmdStat = opendnp3::CommandStatus::NOT_SUPPORTED;

        if(index < 0 || index >= MAX_BOPOINTS)
            return opendnp3::CommandStatus::OUT_OF_RANGE;

    //Enter if the device supports the function 
    if(BOPoints[index].Supported){

        //Update BinaryOutputStattus
        builder.Update(BinaryOutputStatus(command.opType == opendnp3::OperationType::LATCH_ON, Flags(ONLINE)) , index);
        //Updated mapped Binary Input
        builder.Update(Binary(command.opType == opendnp3::OperationType::LATCH_ON, Flags(ONLINE)) , BOPoints[index].MappedInput);

        if(index == BO_ENTER_SERVICE){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                m_callbacks->EnableEnterService(m_VoltageHighLimit, m_VoltageLowLimit, m_FrequencyHighLimit, m_FrequencyLowLimit, m_DERStartDelay, m_DERStartTimeWindow, m_DERStartRampUpTime);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }           
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                m_callbacks->DisableEnterService();
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == BO_POWER_FACTOR_DISCHARGING){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                m_powerFactorExcitationDischarging = true;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }            
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                m_powerFactorExcitationDischarging = false;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == BO_POWER_FACTOR_CHARGING){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                m_powerFactorExcitationCharging = true;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }         
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                m_powerFactorExcitationCharging = false;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == BO_DYNAMIC_REACTIVE_CURRENT_MODE){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }      
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == B0_LIMIT_ACTIVE_POWER){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                m_callbacks->EnableActiveLimitPower(m_activePowerLimitDisCharge, m_activePowerLimitCharge);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }          
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                m_callbacks->DisableActiveLimitPower();
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == B0_CHARGE_DISCHARGE_MODE){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }            
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == BO_VOLT_WATT_MODE){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                if(m_activeCurve != nullptr)
                    m_callbacks->EnableCurve(m_activeCurve);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }            
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                m_callbacks->DisableCurve(m_activeCurve);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == BO_CONST_VAR_MODE){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                m_callbacks->EnableConstantVARs(m_constReactivePower);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }           
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                m_callbacks->DisableConstantVARs();
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == BO_CONST_POWER_FACTOR){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                m_callbacks->EnablePowerFactor(m_powerFactorDischarging, m_powerFactorCharging, m_powerFactorExcitationDischarging, m_powerFactorExcitationCharging);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }          
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                m_callbacks->DisablePowerFactor();
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == BO_VOLT_VAR_MODE){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                if(m_activeCurve != nullptr)
                    m_callbacks->EnableCurve(m_activeCurve);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }        
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                m_callbacks->DisableCurve(m_activeCurve);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == BO_WATT_VAR_MODE){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                if(m_activeCurve != nullptr)
                    m_callbacks->EnableCurve(m_activeCurve);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }            
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                m_callbacks->DisableCurve(m_activeCurve);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == BO_EVENT_BASED_DYNAMIC_REACTIVE_CURRENT_MODE){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON) {
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }            
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF) {
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == B0_CHARGE_DISCHARGE_USE_RAMP_RATES){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON) {
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }            
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
        else if (index == B0_SET_SELECTED_SCHEDULE_READY){
            if(command.opType == opendnp3::OperationType::LATCH_ON || command.opType == opendnp3::OperationType::PULSE_ON){
                bool ValidSchedule = false;
                ValidSchedule = m_activeSchedule->ValidateSchedule();
                if(ValidSchedule)
                {
                    std::cout << "Selected schedule validation passed" << std::endl;
                    m_callbacks->EnableSchedule(m_activeSchedule);
                    cmdStat = opendnp3::CommandStatus::SUCCESS;
                }
                else
                {
                    std::cout << "Selected schedule validation failed" << std::endl;
                    cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
                }

                //Update BI109 Selected Schedule is Validated Bit 
                builder.Update(Binary(true, Flags(ONLINE)), BI_SELECTED_SCHEDULE_VALIDATED);
            }                        
            else if (command.opType == opendnp3::OperationType::LATCH_OFF || command.opType == opendnp3::OperationType::PULSE_OFF){
                    m_callbacks->DisableSchedule(m_activeSchedule);
                    cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else /* Raise FORMAT_ERROR incase the control code is not supported */
                cmdStat = opendnp3::CommandStatus::FORMAT_ERROR; 
        }
    }
    else /* Raise NOT_SUPPORTED incase the device doesn't support the funtion or mode */
    {
        builder.Update(BinaryOutputStatus(command.opType == opendnp3::OperationType::LATCH_ON, Flags(ONLINE)) , index);
        cmdStat = opendnp3::CommandStatus::NOT_SUPPORTED;
    }
    m_outstation->Apply(builder.Build());
    return cmdStat;
}

/* Handling SELECT command for 16-bit AOs */
opendnp3::CommandStatus DERCommandHandler::Select(const opendnp3::AnalogOutputInt16& command, uint16_t index){
    opendnp3::AnalogOutputInt32 t(command.value);
    return AOSelect (t, index);
}

/* Handling  OPERATE command for 16-bit AOs */
opendnp3::CommandStatus DERCommandHandler::Operate(const opendnp3::AnalogOutputInt16& command, uint16_t index, IUpdateHandler& handler, opendnp3::OperateType opType){
    opendnp3::AnalogOutputInt32 t(command.value);
    return DirectOperate (t, index, opType);
}

/* Handling SELECT command for 32-bit AOs */
opendnp3::CommandStatus DERCommandHandler::Select(const opendnp3::AnalogOutputInt32& command, uint16_t index){
    return AOSelect(command, index);
}

/* Handling OPERATE command for 32-bit AOs */
opendnp3::CommandStatus DERCommandHandler::Operate(const opendnp3::AnalogOutputInt32& command, uint16_t index, IUpdateHandler& handler, opendnp3::OperateType opType){
    opendnp3::AnalogOutputInt32 t(command.value);
    return DirectOperate (t, index, opType);
}

opendnp3::CommandStatus DERCommandHandler::Select(const opendnp3::AnalogOutputFloat32& command, uint16_t index) {
    return CommandStatus::NOT_SUPPORTED;
}

opendnp3::CommandStatus DERCommandHandler::Operate(const opendnp3::AnalogOutputFloat32& command, uint16_t index, IUpdateHandler& handler, opendnp3::OperateType opType) {
    return CommandStatus::NOT_SUPPORTED;
}

/* Handling SELECT command for 64-bit AOs */
opendnp3::CommandStatus DERCommandHandler::Select(const opendnp3::AnalogOutputDouble64& command, uint16_t index){
    return CommandStatus::NOT_SUPPORTED;
}

/* Handling OPERATE command for 64-bit AOs */
opendnp3::CommandStatus DERCommandHandler::Operate(const opendnp3::AnalogOutputDouble64& command, uint16_t index, IUpdateHandler& handler, opendnp3::OperateType opType){
    return CommandStatus::NOT_SUPPORTED;
}

void DERCommandHandler::SetOutstation(std::shared_ptr<IOutstation> &value)
{
    m_outstation = value;
}

opendnp3::CommandStatus DERCommandHandler::AOSelect(const opendnp3::AnalogOutputInt32& command, uint16_t index){

    if(index < 0 || index >= MAX_AOPOINTS)
        return opendnp3::CommandStatus::OUT_OF_RANGE;
    opendnp3::CommandStatus cmdStat = opendnp3::CommandStatus::NOT_SUPPORTED;

    if(AOPoints[index].Supported)
        cmdStat = opendnp3::CommandStatus::SUCCESS;

    return cmdStat;
}

opendnp3::CommandStatus DERCommandHandler::DirectOperate(const opendnp3::AnalogOutputInt32& command, uint16_t index, opendnp3::OperateType opType){
    
    if(index < 0 || index >= MAX_AOPOINTS)
        return opendnp3::CommandStatus::OUT_OF_RANGE;

    opendnp3::CommandStatus cmdStat = opendnp3::CommandStatus::NOT_SUPPORTED;

    if(AOPoints[index].Supported){

        //Update AnalogOutputStatus
        builder.Update(AnalogOutputStatus((double) command.value, Flags(ONLINE)) , index);
        //Update Mapped Analog Input
        builder.Update(Analog((double) command.value, Flags(ONLINE)),AOPoints[index].MappedInput);

        if(index == AO_REF_VOLTS){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            } 
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == AO_REF_VOLTS_OFFSET){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == AO_ES_VOLTS_HIGH){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_VoltageHighLimit = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == AO_ES_VOLTS_LOW){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_VoltageLowLimit = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == AO_ES_FREQ_HIGH){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_FrequencyHighLimit = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == AO_ES_FREQ_LOW){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_FrequencyLowLimit = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == AO_ES_DELAY){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_DERStartDelay = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == AO_ES_RAND_DELAY){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_DERStartTimeWindow = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == AO_ES_RAMP_RATE){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_DERStartRampUpTime = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == LH_VOLTS_RIDE_THROUGH_HIGH_TRIP_CURVE_INDEX){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == LH_VOLTS_RIDE_THROUGH_LOW_TRIP_CURVE_INDEX){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == LH_VOLTS_RIDE_THROUGH_MOMENTARY_CESSATION_TRIP_CURVE_INDEX){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == LH_VOLTS_RIDE_THROUGH_LOW_TRIP_DISCHARGE_CURVE_INDEX){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == LH_FREQ_RIDE_THROUGH_HIGH_TRIP_DISCHARGE_CURVE_INDEX){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == LH_FREQ_RIDE_THROUGH_LOW_TRIP_DISCHARGE_CURVE_INDEX){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_MODE_PRIORITY){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_TIME_WINDOW){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_RAMP_TIME){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_REVERSION_TIMEOUT){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_SIGNAL_METER_ID){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_GRADIENT_MODE){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_DEADBAND_MIN_VOLTS){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_DEADBAND_MAX_VOLTS){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_GRADIENT_SAGS){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_GRADIENT_SWELLS){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_FILTER_TIME_MOVE_AVG_VOLTS){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_BLOCK_ZONE_VOLTS){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_BLOCK_ZONE_TIME){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_START_HOLD_TIME){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == DYNAMIC_REACTIVE_CURRENT_END_HOLD_TIME){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_HIGH_START_FREQUENCY){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_HIGH_STOP_FREQUENCY){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_HIGH_DISCHARGE_GRADIENT){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_HIGH_CHARGE_GRADIENT){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_LOW_START_FREQUENCY){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_LOW_STOP_FREQUENCY){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_LOW_DISCHARGE_GRADIENT){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_LOW_CHARGE_GRADIENT){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_TIME_CONST_RAMP_UP_TIME){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FREQ_WATT_TIME_CONST_RAMP_DOWN_TIME){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == ACTIVE_POWER_LIMIT_CHARGE_SETPOINT){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_activePowerLimitCharge = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == ACTIVE_POWER_LIMIT_DISCHARGE_SETPOINT){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_activePowerLimitDisCharge = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_DISCHARGE_MODE_PRIORITY){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_DISCHARGE_ENABLE_TIME_WINDOW){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_DISCHARGE_ENABLE_RAMP_TIME){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_DISCHARGE_REVERSION_TIMEOUT){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_DISCHARGE_ACTIVE_POWER_TARGET){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_DISCHARGE_TIME_CONSTANT_RAMP_UP_TIME){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_DISCHARGE_TIME_CONSTANT_RAMP_DOWN_TIME){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == DISCHARGE_RAMP_UP_RATE){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == DISCHARGE_RAMP_DOWN_RATE){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_RAMP_UP_RATE){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_RAMP_DOWN_RATE){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_DISCHARGE_MIN_RESERVE_STORAGE){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == CHARGE_DISCHARGE_MAX_RESERVE_STORAGE){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
         else if (index == VOLT_WATT_CURVE_INDEX){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
                //Add the curve Index
                if(m_curveModeIndex.find(index) != m_curveModeIndex.end()) 
                    m_curveModeIndex[index]= (unsigned) command.value;
                else
                    m_curveModeIndex.insert({index, (unsigned) command.value});
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == CONST_REACTIVE_POWER){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_constReactivePower = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FIXED_POWER_FACTOR_DISCHARGING){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_powerFactorDischarging = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == FIXED_POWER_FACTOR_CHARGING){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                m_powerFactorCharging = command.value;
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == VOLT_VAR_CURVE_INDEX){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
                //Add the curve Index
                if(m_curveModeIndex.find(index) != m_curveModeIndex.end()) 
                    m_curveModeIndex[index]= (unsigned) command.value;
                else
                    m_curveModeIndex.insert({index, (unsigned) command.value});
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == VOLT_VAR_RAMP_UP_TIME_CONST){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == VOLT_VAR_RAMP_DOWN_TIME_CONST){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == VOLT_VAR_VOLTS_REF_ADJ_TIME_CONST){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == WATT_VAR_CURVE_INDEX){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){
                cmdStat = opendnp3::CommandStatus::SUCCESS;
                //Add the curve Index
                if(m_curveModeIndex.find(index) != m_curveModeIndex.end()) 
                    m_curveModeIndex[index]= (unsigned) command.value;
                else
                    m_curveModeIndex.insert({index, (unsigned) command.value});
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == CURVE_INDEX_SELECTOR){
            //Return OUT_OF_RANGE if the curve index is not between 1 and 100
            //Number of Curves supported in outstation is 100
            if(command.value < 1 || command.value > 100){
                std::cout << "Curve index out of range" << std::endl;
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
            }
            else{
                m_selectedCurveIndex = command.value;
                
                if (m_curves.find(m_selectedCurveIndex) == m_curves.end()) {
                    std::cout << "Curve does not exist, creating a new one" << std::endl;
                    //the curve does not exist; add a new one
                    GenericCurve genericCurve;
                    m_curves.insert({m_selectedCurveIndex, genericCurve});
                }
                else{//Curve exists already
                    std::cout << "Curve already exists" << std::endl;
                }
                //Update the Outstation with the selected curve
                m_activeCurve = &m_curves[m_selectedCurveIndex];
                UpdateSelectedCurveInOutstation();
                UpdateSelectedCurveState(m_selectedCurveIndex);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
        }
        /* Curve points */
        else if(index >= 245 && index <=448){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){

                if (m_activeCurve != nullptr)
                    m_activeCurve->SetParameter(index, command.value);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
        else if (index == SCHEDULE_EDIT_SELECTOR){
            //Return OUT_OF_RANGE if the schedule index is not between 1 and 10
            //Number of schedules supported in outstation is 10
            if(command.value < 1 || command.value > 10){
                std::cout << "Schedule index out of range" << std::endl;
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
            }
            else{
                m_selectedScheduleIndex = command.value;
                
                if (m_schedules.find(m_selectedScheduleIndex) == m_schedules.end()) {
                    std::cout << "Schedule does not exist, creating a new one" << std::endl;
                    //the Schedule does not exist; add a new one
                    Schedule schedule;
                    m_schedules.insert({m_selectedScheduleIndex, schedule});
                }
                else{//Schedule exists already
                    std::cout << "Schedule already exists" << std::endl;
                }
                //Update the Outstation with the selected schedule
                m_activeSchedule = &m_schedules[m_selectedScheduleIndex];
                UpdateSelectedScheduleInOutstation();
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
        }
        /* Schedule points */
        else if(index >= 462 && index <= 669){
            //Data Range validation
            if(command.value >= AOPoints[index].MinValue && command.value <= AOPoints[index].MaxValue){

                if (m_activeSchedule != nullptr)
                    m_activeSchedule->SetParameter(index, command.value);
                cmdStat = opendnp3::CommandStatus::SUCCESS;
            }
            else
                cmdStat = opendnp3::CommandStatus::OUT_OF_RANGE;
        }
    }
    else
    {
        builder.Update(AnalogOutputStatus((double) command.value, Flags(ONLINE)) , index);
        cmdStat = opendnp3::CommandStatus::NOT_SUPPORTED;
    }
    m_outstation->Apply(builder.Build());

    return cmdStat;
}

    /* Updates the Outstation with the selected curve set parameters */
    void DERCommandHandler::UpdateSelectedCurveInOutstation(){
        std::cout << "Updating the Outstation with the selected curve set parameters" << std::endl;

        float* m_activeCurveParams = m_activeCurve->GetActiveCurveParameters();

        for (int index = 245; index <= 448; index++){
            int i = NUM_PARAMS_CURVES - (MAX_CURVE_INDEX - index) - 1;
            //Update AnalogOutputStatus
            builder.Update(AnalogOutputStatus((double) m_activeCurveParams[i], Flags(ONLINE)) , index);
            //Update Mapped Analog Input
            builder.Update(Analog((double) m_activeCurveParams[i], Flags(ONLINE)),AOPoints[index].MappedInput);
        }      
        m_outstation->Apply(builder.Build());
    }

    /* Updates the “Selected Curve is Referenced by a Mode” ( BI107) based on the curve state */
    void DERCommandHandler::UpdateSelectedCurveState(uint8_t curveIndex){
        std::cout << "Updating the “Selected Curve is Referenced by a Mode” ( BI107) based on the curve state" << std::endl;
        float *m_activeCurveParams = m_activeCurve->GetActiveCurveParameters();
 
        //Get the selected curve's no. of points
        int index = NUM_PARAMS_CURVES - (MAX_CURVE_INDEX - NUM_POINTS_CURVES) - 1;
        unsigned numPoints = (unsigned) m_activeCurveParams[index];

        // Check if any of the DER modes has the value of the selected curve index 
        bool derRef = false;
        std::unordered_map<unsigned, unsigned>::iterator iter;
        for(iter = m_curveModeIndex.begin(); iter != m_curveModeIndex.end(); iter++)
        {
            if(iter->second == curveIndex){
                derRef = true;
                break;
            }
        }

        //Check if any of the DER modes reference the curve index
        if (derRef) {
            //Update “Selected Curve is Referenced by a Mode” ( BI107)
            builder.Update(Binary(true, Flags(ONLINE)) , BI_SELECTED_CURVE_IS_REFERENCED_BY_A_MODE);
        }
        else{
            builder.Update(Binary(false, Flags(ONLINE)) , BI_SELECTED_CURVE_IS_REFERENCED_BY_A_MODE);
        }
    }


    /* Updates the Outstation with the selected schedule parameters */
    void DERCommandHandler::UpdateSelectedScheduleInOutstation(){
        std::cout << "Updating the Outstation with the selected schedule" << std::endl;
        
        float* m_activeScheduleParams = m_activeSchedule->GetActiveScheduleParameters();

        for (int index = 462; index <= 669; index++){
            int i =  NUM_PARAMS_SCHEDULE - (MAX_SCHEDULE_INDEX - index) - 1;
            //Update AnalogOutputStatus
            builder.Update(AnalogOutputStatus((double) m_activeScheduleParams[i], Flags(ONLINE)) , index);
            //Update Mapped Analog Input
            builder.Update(Analog((double) m_activeScheduleParams[i], Flags(ONLINE)),AOPoints[index].MappedInput);
        }      

        //Update BI109 Selected Schedule is Validated Bit 
        builder.Update(Binary(false, Flags(ONLINE)), BI_SELECTED_SCHEDULE_VALIDATED );
        m_outstation->Apply(builder.Build());
    }