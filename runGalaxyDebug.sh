#!/usr/bin/bash
#Runs a GalaxyRoutes with timing and debug output to debug.txt
echo "Processing routes, timing processing, debug in debug.txt"
time ./GalaxyRoutes conduits.txt <shipRouteTable.txt 2>debug.txt
echo "Done!"
#end
