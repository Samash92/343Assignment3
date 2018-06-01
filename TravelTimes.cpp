#include "TravelTimes.h"

Travel_Times::Travel_Times(std::ifstream& inStream) {
    fConduitCount = 0;
    fInputStream = inStream; 
	std::string nextLine;
	std::string tokens[3];
	while (fInputStream.getline(nextLine) {
		nextLine = fInputStream.getline(); 
		char* tokenizer;
		tokenizer = strok(nextLine, "\t");
		int i = 0;
		while (tokenizer != nullptr) {
			tokens[i] = tokenizer;
			tokenizer = strok(NULL, "\t");
			i++;
		}

		fConduitMap[tokens[0]].insert(std::pair<std::string, int>(tokens[1], atoi(tokens[2]);
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
		std::cout << "New Origin: " << std::endl;
		for (destinationIterator : conduitIterator->second) {
			std::cout << "--> Origin: " << conduitIterator->first << " Destination: " << destinationIterator->first << " Travel Time: " << destinationIterator->second << std::endl;
		}
	}
	std::cout << "Total Number of Conduits: " << fConduitCount << std::endl;
}