/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/


#ifndef DER_COMMANDHANDLER_H
#define DER_COMMANDHANDLER_H

#include <unordered_map>
#include <asiodnp3/UpdateBuilder.h>

#include "DERAppNoteDefinition.h"
#include "IDERCommandHandlerCallback.h"
#include "DERCommandHandlerCallbackDefault.h"
#include "GenericCurve.h"
#include "Schedule.h"

namespace der{

	class DERCommandHandler : public opendnp3::ICommandHandler
	{
		public:
		
			DERCommandHandler(std::shared_ptr<IDERCommandHandlerCallback> callbacks);
				/**
			* Ask if the application supports a ControlRelayOutputBlock - group 12 variation 1
			*
			* @param command command to operate
			* @param index index of the command
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Select(const opendnp3::ControlRelayOutputBlock& command, uint16_t index);

			/**
			* Operate a ControlRelayOutputBlock - group 12 variation 1
			*
			* @param command command to operate
			* @param index index of the command
			* @param opType the operation type the outstation received.
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Operate(const opendnp3::ControlRelayOutputBlock& command, uint16_t index, opendnp3::OperateType opType);


			/**
			* Ask if the application supports a 16 bit analog output - group 41 variation 2
			*
			* @param command command to operate
			* @param index index of the command
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt16& command, uint16_t index);

			/**
			* Ask if the application supports a 16 bit analog output - group 41 variation 2
			*
			* @param command command to operate
			* @param index index of the command
			* @param opType the operation type the outstation received.
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt16& command, uint16_t index, opendnp3::OperateType opType);


			/**
			* Ask if the application supports a 32 bit analog output - group 41 variation 1
			*
			* @param command command to operate
			* @param index index of the command
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt32& command, uint16_t index);

			/**
			* Operate a 32 bit analog output - group 41 variation 1
			*
			* @param command command to operate
			* @param index index of the command
			* @param opType the operation type the outstation received.
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt32& command, uint16_t index, opendnp3::OperateType opType);

			/**
			* Ask if the application supports a single precision, floating point analog output - group 41 variation 3
			*
			* @param command command to operate
			* @param index index of the command
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Select(const opendnp3::AnalogOutputFloat32& command, uint16_t index) ;

			/**
			* Operate a single precision, floating point analog output - group 41 variation 3
			*
			* @param command command to operate
			* @param index index of the command
			* @param opType the operation type the outstation received.
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputFloat32& command, uint16_t index, opendnp3::OperateType opType);

			/**
			* Ask if the application supports a double precision, floating point analog output - group 41 variation 4
			*
			* @param command command to operate
			* @param index index of the command
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Select(const opendnp3::AnalogOutputDouble64& command, uint16_t index) ;

			/**
			* Operate a double precision, floating point analog output - group 41 variation 4
			*
			* @param command command to operate
			* @param index index of the command
			* @param opType the operation type the outstation received.
			* @return result of request
			*/
			virtual opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputDouble64& command, uint16_t index, opendnp3::OperateType opType);

    		void Start();
			void End();
			void SetOutstation(std::shared_ptr<asiodnp3::IOutstation> &value);
			opendnp3::CommandStatus AOSelect(const opendnp3::AnalogOutputInt32& command, uint16_t index);
			opendnp3::CommandStatus DirectOperate(const opendnp3::AnalogOutputInt32& command, uint16_t index, opendnp3::OperateType opType);
			void UpdateSelectedCurveInOutstation();
			void UpdateSelectedCurveState(u_int8_t index);
			void UpdateSelectedScheduleInOutstation();
			void SetScheduleReady();
			
		private:

			std::shared_ptr<IDERCommandHandlerCallback> m_callbacks = DERCommandHandlerCallbackDefault::getInstance();
			std::shared_ptr<asiodnp3::IOutstation> m_outstation;
			UpdateBuilder builder;

			//Supported BO Points from IEEE 1547 
			static const u_int16_t BO_ENTER_SERVICE = 3;
			static const u_int16_t BO_POWER_FACTOR_DISCHARGING = 10;
			static const u_int16_t BO_POWER_FACTOR_CHARGING = 11;
			static const u_int16_t BO_DYNAMIC_REACTIVE_CURRENT_MODE= 14;
			static const u_int16_t B0_LIMIT_ACTIVE_POWER = 17;
			static const u_int16_t B0_CHARGE_DISCHARGE_MODE = 18;
			static const u_int16_t BO_VOLT_WATT_MODE = 25;
			static const u_int16_t BO_CONST_VAR_MODE = 27;
			static const u_int16_t BO_CONST_POWER_FACTOR = 28;
			static const u_int16_t BO_VOLT_VAR_MODE = 29;
			static const u_int16_t BO_WATT_VAR_MODE = 30;
			static const u_int16_t BO_EVENT_BASED_DYNAMIC_REACTIVE_CURRENT_MODE= 33;
			static const u_int16_t B0_CHARGE_DISCHARGE_USE_RAMP_RATES = 38;
			static const u_int16_t B0_SET_SELECTED_SCHEDULE_READY = 42;

			//Supported AO points from IEEE 1547 
			static const u_int16_t AO_REF_VOLTS = 0;
			static const u_int16_t AO_REF_VOLTS_OFFSET = 1;
			static const u_int16_t AO_ES_VOLTS_HIGH = 6;
			static const u_int16_t AO_ES_VOLTS_LOW = 7;

			static const u_int16_t AO_ES_FREQ_HIGH = 8;
			static const u_int16_t AO_ES_FREQ_LOW = 9;
			static const u_int16_t AO_ES_DELAY= 10;
			static const u_int16_t AO_ES_RAND_DELAY = 11;
			static const u_int16_t AO_ES_RAMP_RATE= 12;

			static const u_int16_t LH_VOLTS_RIDE_THROUGH_HIGH_TRIP_CURVE_INDEX = 23;
			static const u_int16_t LH_VOLTS_RIDE_THROUGH_LOW_TRIP_CURVE_INDEX = 24;
			static const u_int16_t LH_VOLTS_RIDE_THROUGH_MOMENTARY_CESSATION_TRIP_CURVE_INDEX = 25;
			static const u_int16_t LH_VOLTS_RIDE_THROUGH_LOW_TRIP_DISCHARGE_CURVE_INDEX = 26;
			static const u_int16_t LH_FREQ_RIDE_THROUGH_HIGH_TRIP_DISCHARGE_CURVE_INDEX = 28;
			static const u_int16_t LH_FREQ_RIDE_THROUGH_LOW_TRIP_DISCHARGE_CURVE_INDEX = 29;

			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_MODE_PRIORITY = 32;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_TIME_WINDOW = 33;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_RAMP_TIME= 34;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_REVERSION_TIMEOUT = 35;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_SIGNAL_METER_ID = 36;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_GRADIENT_MODE = 37;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_DEADBAND_MIN_VOLTS= 38;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_DEADBAND_MAX_VOLTS= 39;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_GRADIENT_SAGS = 40;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_GRADIENT_SWELLS = 41;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_FILTER_TIME_MOVE_AVG_VOLTS= 42;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_BLOCK_ZONE_VOLTS = 43;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_BLOCK_ZONE_TIME = 44;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_START_HOLD_TIME = 45;
			static const u_int16_t DYNAMIC_REACTIVE_CURRENT_END_HOLD_TIME = 46;

			static const u_int16_t FREQ_WATT_HIGH_START_FREQUENCY= 62;
			static const u_int16_t FREQ_WATT_HIGH_STOP_FREQUENCY= 63;
			static const u_int16_t FREQ_WATT_HIGH_DISCHARGE_GRADIENT= 64;
			static const u_int16_t FREQ_WATT_HIGH_CHARGE_GRADIENT= 65;
			static const u_int16_t FREQ_WATT_LOW_START_FREQUENCY= 66;
			static const u_int16_t FREQ_WATT_LOW_STOP_FREQUENCY= 67;
			static const u_int16_t FREQ_WATT_LOW_DISCHARGE_GRADIENT= 68;
			static const u_int16_t FREQ_WATT_LOW_CHARGE_GRADIENT= 69;

			static const u_int16_t FREQ_WATT_TIME_CONST_RAMP_UP_TIME= 72;
			static const u_int16_t FREQ_WATT_TIME_CONST_RAMP_DOWN_TIME= 73;

			static const u_int16_t ACTIVE_POWER_LIMIT_CHARGE_SETPOINT= 87;
			static const u_int16_t ACTIVE_POWER_LIMIT_DISCHARGE_SETPOINT= 88;
			static const u_int16_t CHARGE_DISCHARGE_MODE_PRIORITY = 89;
			static const u_int16_t CHARGE_DISCHARGE_ENABLE_TIME_WINDOW = 90;
			static const u_int16_t CHARGE_DISCHARGE_ENABLE_RAMP_TIME = 91;
			static const u_int16_t CHARGE_DISCHARGE_REVERSION_TIMEOUT =92;
			static const u_int16_t CHARGE_DISCHARGE_ACTIVE_POWER_TARGET = 93;
			static const u_int16_t CHARGE_DISCHARGE_TIME_CONSTANT_RAMP_UP_TIME = 94;
			static const u_int16_t CHARGE_DISCHARGE_TIME_CONSTANT_RAMP_DOWN_TIME = 95;
			static const u_int16_t DISCHARGE_RAMP_UP_RATE = 96;
			static const u_int16_t DISCHARGE_RAMP_DOWN_RATE = 97;
			static const u_int16_t CHARGE_RAMP_UP_RATE = 98;
			static const u_int16_t CHARGE_RAMP_DOWN_RATE = 99;

			static const u_int16_t CHARGE_DISCHARGE_MIN_RESERVE_STORAGE = 100;
			static const u_int16_t CHARGE_DISCHARGE_MAX_RESERVE_STORAGE = 101;

			static const u_int16_t VOLT_WATT_CURVE_INDEX= 173;

			static const u_int16_t CONST_REACTIVE_POWER = 203;

			static const u_int16_t FIXED_POWER_FACTOR_DISCHARGING= 210;
			static const u_int16_t FIXED_POWER_FACTOR_CHARGING= 211;

			static const u_int16_t VOLT_VAR_CURVE_INDEX = 217;

			static const u_int16_t VOLT_VAR_RAMP_UP_TIME_CONST = 218;
			static const u_int16_t VOLT_VAR_RAMP_DOWN_TIME_CONST = 219;

			static const u_int16_t VOLT_VAR_VOLTS_REF_ADJ_TIME_CONST = 220;

			static const u_int16_t WATT_VAR_CURVE_INDEX = 226;

			static const u_int16_t CURVE_INDEX_SELECTOR = 244;

			static const u_int16_t BI_SELECTED_CURVE_IS_REFERENCED_BY_A_MODE = 107;
			static const u_int16_t BI_SELECTED_SCHEDULE_VALIDATED = 109;

			static const u_int16_t MAX_CURVE_INDEX = 448;
            static const u_int16_t NUM_PARAMS_CURVES = 205;
			static const u_int16_t CURVE_MODE_TYPE = 245;
			static const u_int16_t NUM_POINTS_CURVES = 246;

			static const u_int16_t SCHEDULE_EDIT_SELECTOR = 461;
			static const u_int16_t SCHEDULE_NUM_POINTS = 469;
            static const u_int16_t MAX_SCHEDULE_INDEX = 669;
            static const u_int16_t NUM_PARAMS_SCHEDULE = 209;

			u_int8_t m_selectedCurveIndex = 1;

			std::unordered_map<u_int8_t, GenericCurve> m_curves {};
			//Used to map curve index to curve index value for curve management
			std::unordered_map<unsigned, unsigned> m_curveModeIndex{};

			GenericCurve* m_activeCurve;

			std::unordered_map<u_int8_t, Schedule> m_schedules{};

			u_int8_t m_selectedScheduleIndex = 1;

			Schedule* m_activeSchedule;

			double m_activePowerLimitCharge = 1000.00;
			double m_activePowerLimitDisCharge = 1000.00;
			double m_VoltageHighLimit = 20000;
			double m_VoltageLowLimit = 10000;
			double m_FrequencyHighLimit = 70000;
			double m_FrequencyLowLimit = 60000;
			double m_DERStartDelay = 30;
			double m_DERStartTimeWindow = 30;
			double m_DERStartRampUpTime = 30;

			double m_powerFactorDischarging = 100.0f;
			double m_powerFactorCharging = 100.00f;

			double m_constReactivePower = 1000;

			bool m_powerFactorExcitationDischarging = false;
			bool m_powerFactorExcitationCharging = false;
	};
}

#endif