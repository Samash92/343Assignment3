
#include "TravelTimes.h"
#include <fstream>
#include "Reader.h"

int main(int argc, char* argv[]) {

	if (argc < 1) {
		std::cerr << "No conduits file has been specified." << std::endl;
		return EXIT_FAILURE;
	}
	std::string conduitsFile = argv[1];
	std::cerr << "Conduit File input: " << conduitsFile << std::endl;
	std::ifstream inputStream(conduitsFile);
	Travel_Times* conduitTimes = new Travel_Times(inputStream);
	conduitTimes->dump();
	Reader reader(conduitTimes);
	Galaxy* galaxy = reader.load();
	reader.dumpPreLoadStructures();
	galaxy->search();
	delete conduitTimes;
	delete galaxy;

	return 0;
}
