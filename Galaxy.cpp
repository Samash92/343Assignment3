
#include "Galaxy.h"



//Itinerary functions
void Itinerary::print(Fleet& fleet, std::ostream& out = std::cout) {
	//TODO
}

void Itinerary::buildItinerary() {
	//TODO
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
	//TODO
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
	//TODO
	return new Planet("THISPLANETNOEXISTO");
}

Itinerary* Planet::make_itinerary(Planet* destination) {
	//TODO
	return new Itinerary(destination);
}

void Planet::dump(Galaxy* galaxy) {
	//TODO
}

void Planet::relax_neighbors(PriorityQueue<Planet, int(*)(Planet*, Planet*)>& queue) {
	//TODO
}

Galaxy::~Galaxy() {
	std::cerr << "Deleting Planets..." << std::endl;
	for (auto planet : planets) {
		delete planet;
	}
	std::cerr << "...Done!" << std::endl;
	
}

void Galaxy::search() {
	//TODO
}

void Galaxy::dump() {
	//TODO
}

void Galaxy::dump_routes(Planet* origin, std::ostream& out = std::cerr) {
	//TODO
}









