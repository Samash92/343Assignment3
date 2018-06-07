#include "TravelTimes.h"

Travel_Times::Travel_Times(std::ifstream& inStream) {
    fConduitCount = 0; 
	std::string nextLine;
	std::string tokens[3];
	while (!inStream.eof()) {
		std::getline(inStream, nextLine);
		if (nextLine == "")
			continue;

		//std::cerr << nextLine << std::endl;
		std::stringstream stringS(nextLine);
		for (int i = 0; i < 3; i++) {
			std::getline(stringS, tokens[i], '\t');
			//std::cerr << "Token " << i << ": " << tokens[i] << std::endl;
		}
		//insert origin->destination
		fConduitMap[tokens[0]].insert(std::pair<std::string, int>(tokens[1], atoi(tokens[2].c_str())));
		//insert destination->origin
		fConduitMap[tokens[1]].insert(std::pair<std::string, int>(tokens[0], atoi(tokens[2].c_str())));
		++fConduitCount;
	}
}

int Travel_Times::conduitTravelTime(const std::string& origin, const std::string& destination) {
	return fConduitMap[origin][destination];
}

void Travel_Times::dump() {
	for (auto conduitIterator : fConduitMap) {
		std::cerr << "New Origin: " << std::endl;
		for (auto destinationIterator : conduitIterator.second) {
			std::cerr << "--> Origin: " << conduitIterator.first << " Destination: " << destinationIterator.first << " Travel Time: " << destinationIterator.second << std::endl;
		}
	}
	std::cerr << "Total Number of Conduits: " << fConduitCount << std::endl;
}
