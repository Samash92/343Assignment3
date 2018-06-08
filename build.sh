#!/usr/bin/bash
#Compiles the project

echo "Compiling..."
g++ -std=c++14 -fpermissive -O3 Priority.h Galaxy.h Galaxy.cpp TravelTimes.h TravelTimes.cpp Reader.h Reader.cpp Main.cpp -o GalaxyRoutes
echo "...Done!"

#end
