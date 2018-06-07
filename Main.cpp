
#include "TravelTimes.h"
#include <fstream>
#include "Reader.h"

int main(int argc, char* argv[]) {

	if (argc < 1) {
		std::cerr << "No conduits file has been specified." << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream inputStream(argv[0]);
	Travel_Times* conduitTimes = new Travel_Times(inputStream);
	conduitTimes->dump();
	Reader reader(conduitTimes);
	reader.dumpPreLoadStructures();
	Galaxy* galaxy = reader.load();
	delete conduitTimes;
	delete galaxy;

	return 0;
}
