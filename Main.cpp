
#include "TravelTimes.h"
#include <fstream>
#include "Reader.h"

int main(int argc, char* argv[]) {

	std::ifstream inputStream("conduits.txt");
	Travel_Times* conduitTimes = new Travel_Times(inputStream);
	conduitTimes->dump();
	inputStream.
	inputStream.close();
	delete conduitTimes;

	return 0;
}
