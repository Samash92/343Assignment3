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
		fConduitMap[tokens[0]].insert(std::pair<std::string, int>(tokens[1], atoi(tokens[2].c_str())));
		++fConduitCount;
	}
}

int Travel_Times::conduitTravelTime(const std::string& origin, const std::string& destination) {
	return fConduitMap[origin][destination];
}

void Travel_Times::dump() {
	std::map<std::string, std::map<std::string, int>>::iterator conduitIterator;
	std::map<std::string, int>::iterator destinationIterator;
	for (conduitIterator : fConduitMap) {
		std::cerr << "New Origin: " << std::endl;
		for (destinationIterator : conduitIterator.second) {
			std::cerr << "--> Origin: " << conduitIterator.first << " Destination: " << destinationIterator.first << " Travel Time: " << destinationIterator.second << std::endl;
		}
	}
	std::cerr << "Total Number of Conduits: " << fConduitCount << std::endl;
}
