#!/usr/bin/bash
#Runs a Galaxy Dijkstra without debug output
echo "Processing route structure..."
./GalaxyRoutes conduits.txt <shipRouteTable.txt 2>/dev/null
echo "Done!"
#end
