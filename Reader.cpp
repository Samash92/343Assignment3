
#include "Reader.h"


//Builds Planet and edge maps from the Constraints file and sets fields.
Reader::Reader(Travel_Times* constraints) {
	fConstraints = constraints;
	fGalaxy = new Galaxy();
	fPrevious_ship_id = -1;
	fPrevious_destination_planet = nullptr;
	previous_arrival_time = 0;
	fShip_ID = -1;
	fDeparture_Planet = nullptr;
	fDeparture_Time = -1;
	fArrival_Time = 0;
	fCurrentLine = "";

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
				Edge* originEdge = new Edge(destinationPlanet);
				//Also add the destination edge at this point
				Edge* destinationEdge = new Edge(originPlanet);
				//add edges to respective planets
				originPlanet->add(originEdge);
				destinationPlanet->add(destinationEdge);
				fEdges[originPlanet][destinationPlanet] = originEdge;
				fEdges[destinationPlanet][originPlanet] = destinationEdge;
			}
		}


	}
	
}

Reader::~Reader() {

}

//Loads in the routestructure and verifies it against constraints file and planets/edges maps
Galaxy* Reader::load(std::ifstream& routeInputStream) {
	
	//Add each planet to the galaxy
	for (auto planet : fPlanets) {
		fGalaxy->add(planet.second);
	}
	
	if (!routeInputStream.is_open()) {
		std::cerr << "Error: Route Tables file is not open." << std::endl;
		exit(EXIT_FAILURE);
	}

	while (!routeInputStream.eof()) {
		//if get_record returns false, some error with reading in the next record.
		if (!get_record(routeInputStream)) {
			std::cerr << "Failure to read record. Exiting..." << std::endl;
			exit(EXIT_FAILURE);
		}
		dumpCurrentLeg();
		dumpPreviousLeg();
		fEdges[fDeparture_Planet][fDestination_Planet]->add(Leg(fShip_ID, fDeparture_Time, fArrival_Time));
	}


	return fGalaxy;
}	




bool Reader::validate() {
	return false;
}

bool get_record(std::ifstream& routeInStream) {
	return false;
}

