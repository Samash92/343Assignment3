
#include "Reader.h"


//Builds Planet and edge maps from the Constraints file.
Reader::Reader(Travel_Times* constraints) {
	fConstraints = constraints;
	fGalaxy = new Galaxy();
	std::map<std::string, std::map<std::string, int>> conduits = constraints->getConduitMap();

	//build planet map and edge map
	for (auto originIterator : conduits) {
		std::string originName = originIterator.first;
		Planet* originPlanet = fPlanets[originName];
		//add origin planet to map if it does not exist in planets map
		if (originPlanet == nullptr) {
			originPlanet = new Planet(originName);
			fPlanets[originName] = originPlanet;
		}
		//add destination planet to map if it does not exist in map 
		for (auto destinationIterators : originIterator.second) {
			std::string destinationName = destinationIterators.first; 
			Planet* destinationPlanet = fPlanets[destinationName];
			if (destinationPlanet == nullptr) {
				destinationPlanet = new Planet(destinationName);
				fPlanets[destinationName] = destinationPlanet;
			}
			if (fEdges[originPlanet][destinationPlanet] == nullptr) {
				fEdges[originPlanet][destinationPlanet] = new Edge(destinationPlanet);
				//add the reverse edge at the same time
				fEdges[destinationPlanet][originPlanet] = new Edge(originPlanet);
			}
		}


	}
	
}

Reader::~Reader() {

}

//Loads in the routestructure and verifies it against constraints file and planets/edges maps
Galaxy* Reader::load(std::ifstream& routeInputStream) {
		return fGalaxy;
}	 

bool Reader::validate() {
	return false;
}

bool get_record(std::ifstream& routeInStream) {
	return false;
}

