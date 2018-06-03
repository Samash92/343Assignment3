
#include "TravelTimes.h"
#include <fstream>
#include "Reader.h"

int main(int argc, char* argv[]) {

	std::ifstream inputStream("conduits.txt");
	Travel_Times* conduitTimes = new Travel_Times(inputStream);
	conduitTimes->dump();
	Reader reader(conduitTimes);
	inputStream.close();
	inputStream.open("RouteTable.txt");
	Galaxy* galaxy = reader.load(inputStream);
	delete conduitTimes;

	return 0;
}
