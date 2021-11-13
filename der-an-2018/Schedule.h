/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <cstdint>
#include <time.h> 
#include <iostream>
#include <chrono>
#include <unordered_map>

namespace der{
    class Schedule{
        private:
            static const uint16_t SCHEDULE_IDENTITY = 462;
			static const uint16_t SCHEDULE_PRIORITY = 463;
			static const uint16_t SCHEDULE_TYPE = 464;
			static const uint16_t SCHEDULE_START_DATE = 465;
			static const uint16_t SCHEDULE_START_TIME = 466;
			static const uint16_t SCHEDULE_REPEAT_INTERVAL = 467;
			static const uint16_t SCHEDULE_REPEAT_INTERVAL_UNITS = 468;
			static const uint16_t SCHEDULE_NUM_POINTS = 469;

            static const uint16_t MAX_SCHEDULE_INDEX = 669;
            static const uint16_t NUM_PARAMS = 209;
            float m_parameters[NUM_PARAMS] = {0};
            time_t timer;
            int daysSinceEpoch ;
            
        public:
            void SetParameter(uint16_t index, float value);
            float *GetActiveScheduleParameters();
            bool ValidateSchedule(); 
    };
}
#endif