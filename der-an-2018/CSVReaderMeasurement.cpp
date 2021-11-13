/*
Copyright © 2019 Electric Power Research Institute, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
· Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
· Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
· Neither the name of the EPRI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission
*/

#include "CSVReaderMeasurement.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace der;

void CSVReaderMeasurement::ReadBinaryValues(vector<BinaryValues>& bMeas, string fpath){

    //Clear the vector before reading
    bMeas.clear();

    ifstream ip(fpath);

    if(!ip.is_open())
    {
        std::cout<<"Error opening file"<<endl;
        return ;
    }

    //Ignore the header in the CSV file
    std::string header;
		getline(ip, header);

     while(ip.good()){
         std::string s;

         if(!getline(ip, s)) break;

         std::istringstream line(s);
         std::vector<std::string> values;

         while(line){
             std::string s;

             getline(line, s, ',');

             values.push_back(s);
         }
         
            uint16_t m_index = std::stoi(values[0]);
            uint16_t m_quality = std::stoi(values[1]);
            bool m_status = values[2] == "true\r" ? true: false; 

            //std::cout<<"Index: "<< m_index<<" Quality: " << unsigned(m_quality) << " Status: "<<std::boolalpha<<m_status<<endl;

            BinaryValues bMeausVal = {m_index, m_quality, m_status};

            bMeas.push_back(bMeausVal);
            values.clear();
     }       
     ip.close();
}

void CSVReaderMeasurement::ReadAnalogValues(vector<AnalogValues>& aMeas, string fpath){

    //Clear the vector before reading
    aMeas.clear();

    ifstream ip(fpath);

    if(!ip.is_open())
    {
        std::cout<<"Error opening file"<<endl;
        return;
    }

    //Ignore the header in the CSV file
    std::string header;
		getline(ip, header);

     while(ip.good()){
         std::string s;

         if(!getline(ip, s)) break;

         std::istringstream line(s);
         std::vector<std::string> values;

         while(line){
             std::string s;

             getline(line, s, ',');

             values.push_back(s);
         }
            uint16_t m_index = std::stoi(values[0]);
            uint8_t m_quality = std::stoi(values[1]);
            double m_val = std::stod(values[2]);

            //std::cout<<"Index: "<< m_index<<" Quality: " << unsigned(m_quality) << " Value: "<<m_val<<endl;

            AnalogValues aMeausVal = {m_index, m_quality, m_val};

            aMeas.push_back(aMeausVal);
            values.clear();
     }       
     ip.close();
}