#ifndef TRAVEL_TIMES_H
#define TRAVEL_TIMES_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Travel_Times {

private:

//map of origin planet, map of destination planet and travel time
std::map<std::string, std::map<std::string, int>>fConduitMap ;
int fConduitCount; 

public: 
Travel_Times(std::ifstream& inStream); 
int conduitTravelTime(const std::string& origin, const std::string& destination); 
void dump();
};


#endif
