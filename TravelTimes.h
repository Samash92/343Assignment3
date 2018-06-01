#ifndef TRAVEL_TIMES_H
#define TRAVEL_TIMES_H

#include <map>
#include <pair>
#include <string>
#include <iostream>

class Travel_Times {

private:

//map of origin planet, map of destination planet and travel time
std::map<std::string, std::map<std::string, int>>fConduitMap ;
int fConduitCount;
std::istream& fInputStream; 

public: 
Travel_Times(std::istream& inStream); 
int conduitTravelTime(const std::string& origin, const std::string& destination); 
void dump();
};


#endif