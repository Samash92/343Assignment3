
#include "Reader.h"


//Builds Planet and edge maps from the Constraints file and sets fields.
Reader::Reader(Travel_Times* constraints) {
	fConstraints = constraints;
	fGalaxy = new Galaxy();
	fPrevious_ship_id = -1;
	fPreviousShipName = "";
	fPrevious_destination_planet = nullptr;
	previous_arrival_time = 0;
	fShip_ID = -1;
	fShipName = "";
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

		fPreviousShipName = fShipName;
		fPrevious_ship_id = fShip_ID;
		fPrevious_destination_planet = fDestination_Planet;
		previous_arrival_time = fArrival_Time;

	}


	return fGalaxy;
}	


bool Reader::get_record(std::ifstream& routeInStream) {
	std::getline(routeInStream, fCurrentLine);
	std::stringstream stringS(fCurrentLine);
	std::string routeTokens[5];
	for (int i = 0; i < 5; i++) {
		std::getline(stringS, routeTokens[i], '\t');
	}
	fShipName = routeTokens[0];
	if (fShips.find(fShipName) == fShips.end()) {
		fShip_ID = fGalaxy->fleet.add(fShipName);
		fShips[fShipName] = fShip_ID;
	}
	else {
		fShip_ID = fShips[fShipName];
	}

	fDeparture_Planet = fPlanets[routeTokens[1]];
	fDeparture_Time = atoi(routeTokens[2].c_str());
	fDestination_Planet = fPlanets[routeTokens[3]];
	fArrival_Time = atoi(routeTokens[4].c_str());
	
	
	return validate();
}


bool Reader::validate() {
	
	// only need to check if the shi
	if (fPrevious_ship_id == -1) {
		//if an edge between destination and departure does not exist there is no connection between these planets
		if (!validateEdges()) {
			return false;
		}

		if (!validateTravelTime) {
			return false;
		}

		return true;
	}

	//if we are still dealing with the same ship, our departure planet must be equal to our previous arrival planet
	if (fShip_ID == fPrevious_ship_id) {
		if (!validateDeparturePlanet()) {
			return false;
		}
	}

	if (!validateEdges()) {
		return false;
	}

	if (!validateTravelTime()) {
		return false;
	}

	return true;
}



bool Reader::validateEdges() {
	if (fEdges.at(fDeparture_Planet).find(fDestination_Planet) == fEdges.at(fDeparture_Planet).end()) {
		std::cerr << "Ship: " << fShipName << " cannot travel between " << fDeparture_Planet->getName()
			<< " and " << fDestination_Planet->getName() << " because a connection does not exist." << std::endl;
		return false;
	}

	return true;
}

bool Reader::validateTravelTime() {
	int actualTravelTime = fArrival_Time - fDeparture_Time;
	int expectedTravelTime = fConstraints->conduitTravelTime(fDeparture_Planet->getName(), fDestination_Planet->getName());
	//check travel times are correct
	if (actualTravelTime != expectedTravelTime) {
		std::cerr << "When traveling between " << fDeparture_Planet->getName() << " and " << fDestination_Planet->getName()
			<< " Ship: " << fShipName << " took [" << actualTravelTime << "] instead of [" << expectedTravelTime << "]"
			<< std::endl;
		return false;
	}

	return true;
}

bool Reader::validateDeparturePlanet() {
	if (fDeparture_Planet->getName() != fPrevious_destination_planet->getName()) {
		std::cerr << "Ship: " << fShipName << " at " << fPrevious_destination_planet->getName() << " is trying to leave from "
			<< fDeparture_Planet->getName() << "!" << std::endl;
		return false;
	}

	return true;
}



