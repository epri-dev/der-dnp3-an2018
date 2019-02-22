/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#include "Schedule.h"

namespace der{

    void Schedule::SetParameter(u_int16_t index, float value){

        u_int16_t i = NUM_PARAMS - (MAX_SCHEDULE_INDEX - index) - 1;

        if (i < 0 || i >= NUM_PARAMS){
            throw "Invalid Schedule Index";
        }

        m_parameters[i] = value;
    }

    float *Schedule::GetActiveScheduleParameters(){
        return m_parameters;
    }

    bool Schedule::ValidateSchedule(){

        /* Verify all required schedule points */

        /* Verify Schedule Type */
        if(m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_TYPE) - 1] < 1 || m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_TYPE) - 1] > 30){
            std::cout << "Invalid Schedule Type" << std::endl;
            return false;
        }

        /* Verify Schedule Date */
        if(m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_START_DATE) - 1] < 0 || m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_START_DATE) - 1] > 2147483647){
            std::cout << "Invalid Schedule Date" << std::endl;
            return false;
        }

        /* Verify if the date is in the future */
        timer = time (NULL);
        daysSinceEpoch = timer/(60*60*24);
        if(m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_START_DATE) - 1] < daysSinceEpoch){
            std::cout << "Schedule Date cannot be in the past" << std::endl;
            return false;
        }

        /* Verify Schedule Time */
        if(m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_START_TIME) - 1] < 0 || m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_START_TIME) - 1] > 86400000){
            std::cout << "Invalid Schedule Time" << std::endl;
            return false;
        }

        /* Verify Schedule Repeat Interval */
        if(m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_REPEAT_INTERVAL) - 1] < 0 || m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_REPEAT_INTERVAL) - 1] > 2147483647){
            std::cout << "Invalid Schedule Repeat Interval" << std::endl;
            return false;
        }

        /* Verify Schedule Repeat Interval Units*/
        if(m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_REPEAT_INTERVAL_UNITS) - 1] < 0 || m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_REPEAT_INTERVAL_UNITS) - 1] > 8){
            std::cout << "Invalid Schedule Repeat Interval Units" << std::endl;
            return false;
        }

        /* Verify Schedule Number of Points*/
        if(m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_NUM_POINTS) - 1] < 0 || m_parameters[NUM_PARAMS - (MAX_SCHEDULE_INDEX - SCHEDULE_NUM_POINTS) - 1] > 100){
            std::cout << "Invalid Schedule Number of Points" << std::endl;
            return false;
        }

        return true;
    }
}