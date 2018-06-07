
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
Galaxy* Reader::load() {
	
	//Add each planet to the galaxy
	for (auto planet : fPlanets) {
		fGalaxy->add(planet.second);
	}

	while (!std::cin.eof()) {
		//if get_record returns false, some error with reading in the next record.
		if (!get_record()) {
			std::cerr << "Failure to read record. Exiting..." << std::endl;
			exit(EXIT_FAILURE);
		}
		
		dumpPreviousLeg();
		dumpCurrentLeg();
		fEdges[fDeparture_Planet][fDestination_Planet]->add(Leg(fShip_ID, fDeparture_Time, fArrival_Time));

		fPreviousShipName = fShipName;
		fPrevious_ship_id = fShip_ID;
		fPrevious_destination_planet = fDestination_Planet;
		previous_arrival_time = fArrival_Time;

	}


	return fGalaxy;
}	


bool Reader::get_record() {
	std::getline(std::cin, fCurrentLine);
	std::cerr << "Current Line Read In: " << fCurrentLine << std::endl;
	//Lines that are empty, begin with the comment character (#) or are just a new line
	//we skip. 
	if (fCurrentLine == "" || fCurrentLine.at(0) == '#' || fCurrentLine.at(0) == '\n')
		return true;
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

		if (!validateTravelTime()) {
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


void Reader::dumpPreLoadStructures() {
	std::cerr << "Dumping data structures used to build the galaxy..." << std::endl;

	dumpPlanets();
	dumpShips();
	dumpEdges();
}

void Reader::dumpPlanets() {
	std::cerr << "Dumping the planets..." << std::endl;
	int planetCount = 1;
	for (auto planet : fPlanets) {
		std::cerr << planetCount << ": " << planet.first << " is at [" << planet.second << "]" << std::endl;
		++planetCount;
	}
	std::cerr << "Total Number of Planets: " << planetCount << std::endl;
	std::cerr << "...Done!" << std::endl;

}

void Reader::dumpShips() {
	std::cerr << "Dumping ships..." << std::endl;
	int shipCounter = 1;
	for (auto ship : fShips) {
		std::cerr << shipCounter << ": Ship Name: " << ship.first << " Ship ID: " << ship.second << std::endl;
		++shipCounter;
	}

	std::cerr << "Total Number of Ships: " << shipCounter << std::endl;
	std::cerr << "...Done!" << std::endl;
}

void Reader::dumpEdges() {
	std::cerr << "Dumping connections between planets..." << std::endl;
	int edgeCount = 1;
	int originCounter = 1;
	for (auto origin : fEdges) {
		std::string originName = origin.first->name;
		int edgeCounter = 1;
		std::cerr << originCounter << ": " << "Origin Planet: " << originName << std::endl;
		for (auto edge : origin.second) {
			std::string destinationName = edge.first->name;
			std::cerr << std::setw(5) << "-->" << edgeCounter << ": Destination Planet: " << edge.first->name << " Travel Time: " << fConstraints->conduitTravelTime(originName, destinationName) << std::endl;
			int legCounter = 1;
			for (auto leg : edge.second->departures) {
				std::cerr << std::setw(10) << "-->" << legCounter << ": ShipID: " << leg.id << " Departing at: " << leg.departure_time << " Arriving at: " << leg.arrival_time << std::endl;
				++legCounter;
			}
			++edgeCounter;
			++edgeCount;
		}
		originCounter++;
	}

	std::cerr << "Total Number of Connections: " << edgeCount << std::endl;
	std::cerr << "...Done!" << std::endl;
}


void Reader::dumpCurrentLeg() {
	std::cerr << "Current Leg:" << std::endl;
	std::cerr << std::setw(5) << "-->" << "Ship Name: " << fShipName << " Ship ID: " << fShip_ID << " Departure Planet: " << fDeparture_Planet
		<< " Departure Time: " << fDeparture_Time << " Destination Planet: " << fDestination_Planet << " Arrival Time: " << fArrival_Time << std::endl;
}

void Reader::dumpPreviousLeg() {
	std::cerr << "Previous Leg:" << std::endl;
	std::cerr << std::setw(5) << "-->" << "Ship Name: " << fPreviousShipName << " Ship ID: " << fPrevious_ship_id
		<< " Destination Planet: " << fPrevious_destination_planet << " Arrival Time: " << previous_arrival_time << std::endl;
}




