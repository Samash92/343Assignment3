
#include "Galaxy.h"



//Itinerary functions
void Itinerary::print(Fleet& fleet, std::ostream& out = std::cout) {
	out << fleet.name(legs[legs.size() - 1].id) << "\t" << origin->name << "\t" << legs[legs.size() - 1].departure_time << "\t" << destinations[destinations.size() - 1]->name << "\t" << legs[legs.size() - 1].arrival_time << std::endl;
	for (int i = legs.size() - 2; i >= 0; i--) {
		out << fleet.name(legs[i].id) << "\t" << destinations[i + 1]->name << "\t" << legs[i].departure_time << "\t" << destinations[i]->name << "\t" << legs[i].arrival_time << std::endl;
	}

	out << "Route Stats->" << " Total Time In Space: [" << fTimeInSpace << " hours] Overall Travel Time: [" << fTotalTravelTime << " hours]" << std::endl;
}

void Itinerary::buildItinerary() {
	Planet* nextPlanet = destinations[destinations.size() - 1]->getPredecessor();
	while (nextPlanet != origin) {
		std::cerr << "Next Planet In Itinerary: " << nextPlanet->name << std::endl;
		destinations.push_back(nextPlanet);
		legs.push_back(nextPlanet->getBestLeg());
		fTimeInSpace += (nextPlanet->getBestLeg().arrival_time - nextPlanet->getBestLeg().departure_time);
		nextPlanet = nextPlanet->getPredecessor();
	}
}

//Edge Functions 
Edge::~Edge() {
	destination = nullptr;
	departures.clear();
}
void Edge::sort() {
	std::sort(departures.begin(), departures.end(), Leg::less_than);
}

void Edge::dump(Galaxy* galaxy) {
		std::cerr << std::setw(10) << "-->" << ": Destination Planet: " << destination->name << std::endl;
		int legCounter = 0;
		for (auto leg : departures) {
			std::cerr << std::setw(15) << "-->" << legCounter << ": ShipID: " << leg.id << " Departing at: " << leg.departure_time << " Arriving at: " << leg.arrival_time << std::endl;
			++legCounter;
		}
}

//Planet functions
Planet::~Planet() {
	std::cerr << "Deleting edges from " << name << "..." << std::endl;
	for (auto edge : edges) {
		delete edge;
	}
	std::cerr << "...Done!" << std::endl;
}
Planet* Planet::search(PriorityQueue<Planet, int(*)(Planet*, Planet*)>& queue) {
	Planet* currentPlanet;
	while (!queue.empty()) {
		currentPlanet = queue.pop();
		std::cerr << "Relaxing Neighbors for " << currentPlanet->name << std::endl;
		currentPlanet->relax_neighbors(queue);
	}
	
	return currentPlanet;
}

Itinerary* Planet::make_itinerary(Planet* destination) {
	Itinerary* furthestItin = new Itinerary(this);
	furthestItin->destinations.push_back(destination);
	furthestItin->legs.push_back(destination->best_leg);
	furthestItin->fTotalTravelTime = destination->best_leg.arrival_time;
	furthestItin->fTimeInSpace = destination->best_leg.arrival_time - destination->best_leg.departure_time;
	furthestItin->buildItinerary();
	return furthestItin;
}

void Planet::dump(Galaxy* galaxy) {
	std::cerr << "Dumping planet info..." << std::endl;
	if (predecessor == nullptr) {
		std::cerr << name << " is not connected!" << std::endl;
	}
	else {
		std::cerr << std::setw(5) << "Best Leg: Predecessor:" << predecessor->name << "Ship: " << galaxy->fleet.name(best_leg.id) <<" Departure: " << best_leg.departure_time << " Arrival: " << best_leg.arrival_time << std::endl;
	}

	for (auto edge : edges) {
		edge->dump(galaxy);
	}
}

void Planet::relax_neighbors(PriorityQueue<Planet, int(*)(Planet*, Planet*)>& queue) {
	int layoverEnd = 0;
	if (!best_leg.arrival_time == 0)
		layoverEnd = best_leg.arrival_time + 4;
	for (auto edge : edges) {
		for (int i = 0; i < edge->departures.size(); i++) {
			if (layoverEnd > edge->departures[i].departure_time)
				continue;

			if (Leg::less_than(edge->departures[i], edge->destination->best_leg)) {
				std::cerr << std::setw(10) << "Relaxing neighbor for: " << name << " to " << edge->destination->name << std::endl;
				edge->destination->best_leg = edge->departures[i];
				edge->destination->predecessor = this;
				queue.reduce(edge->destination);
			}
			
		}
	}
}

Galaxy::~Galaxy() {
	std::cerr << "Deleting Planets..." << std::endl;
	for (auto planet : planets) {
		delete planet;
	}
	std::cerr << "...Done!" << std::endl;
	
}

void Galaxy::search() {
	//Run Dijkstra's shortest path on ever planet in the galaxy...
	PriorityQueue<Planet, int(*)(Planet*, Planet*)> queue(Planet::compare);
	for (auto planet : planets) {
		std::cerr << "Running dijkstra's on: " << planet->getName() << std::endl;
		planet->setOriginBestLeg();
		for (auto planetIn : planets) {
			queue.push_back(planetIn);
		}
		Planet* furthestPlanet = planet->search(queue);
		std::cerr << "Done!" << "furthest planet is: " << furthestPlanet->name << std::endl;
		if(!furthestPlanet->validateRouteSuccessful()) {
				std::cerr << "Route structure not valid. " << furthestPlanet->name << " is not visited from " << planet->name << "!" << std::endl;
				std::cerr << "Dumping Planets to check for other non visited planets..." << std::endl;
				dump();
				exit(EXIT_FAILURE);
		}
		Itinerary* destinationItin = planet->make_itinerary(furthestPlanet);
		destinationItin->print(fleet);
		std::cerr << "Checking Itineraries" << std::endl;
		if (fFurthestPlanetItin == nullptr || (destinationItin->fTotalTravelTime > fFurthestPlanetItin->fTotalTravelTime)) {
			std::cerr << "New Greatest Itinierary" << std::endl;
			delete fFurthestPlanetItin;
			fFurthestPlanetItin = destinationItin;
			destinationItin = nullptr;
		}
		if (destinationItin != nullptr)
			delete destinationItin;
		

		for (auto rPlanet : planets) {
			std::cerr << "Resetting: " << rPlanet->name << std::endl;
			rPlanet->reset();
		}
	}
	std::ofstream outStream("LongestRoute.txt");
	fFurthestPlanetItin->print(fleet, outStream);
	outStream.close();
	delete fFurthestPlanetItin;

}

void Galaxy::dump() {
	std::cerr << "Dumping the Planets..." << std::endl;
	for (auto planet : planets) {
		dump_routes(planet);
	}
	std::cerr << "...Done!" << std::endl;
}

void Galaxy::dump_routes(Planet* origin, std::ostream& out = std::cerr) {
	origin->dump(this);
}









