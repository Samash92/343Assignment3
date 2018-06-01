
#include "TravelTimes.h"
#include <fstream>

int main(int argc, char* argv[]) {

	std::ifstream& inputStream("conduits.txt");
	Travel_Times* conduitTimes = new Trave_Times(inputStream);
	conduitTimes->dump();
	inputStream.close();
	delete conduitTimes;

	return 0;
}