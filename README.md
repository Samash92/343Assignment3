# 343Assignment3
Author: Torren Sampson
Date: 6/7/2018

Using Dijkstra's shortest path algorithm to evaluate ship routing structures. Algorithm is done using a priority queue.
27 Planets in total, with 10 ships, time of most of my routes was in the mid to high hundreds with a max of about 725 hours. 
The routing table was generated using a python script and Basic one line ship to planet routes. The python script really just 
did the formatting, calculation, and repeated the routes as necessary.

Attributions: 
	Professor Bernstein provided the basic structure with Reader.h Galaxy.h the conduits.txt file, and the Priority Queue 
	in Priority.h. I would like to thank a lot of people from class for discussions that were had but special thanks to Jayden (Ji Hoo) Kang, 
	Petra Power, Zach Kobayashi, Sarah Robbins, and Gohur Ali for discussing and talking through the assignment. Since this was my first time writting
	a python script from scratch, I would add these sights for their help in learning how to use the language: 
		https://docs.python.org/3/tutorial/index.html
		https://www.tutorialspoint.com/python/index.htm
		https://www.python.org/about/gettingstarted/
	Stack overflow was used to help me figure out string splitting and to track down a number of errors I was seeing. 

How to Build:
	Building is pretty basic, run the build.sh bash script and an exectuable file called GalaxyRoutes will be created. 
Longest Route: 
	I had a number of routes come out to around the same time... but the longest reported was from Alderaan to Corulag
		Stats: Total Time In Space: [76 hours] Overall Travel Time: [725 hours].

To Run:
	1. runGalaxy.sh will run the program using shipRouteTable.txt and will produce no debug output
	2. runGalaxyDebug.sh does the obvious. It will run the program using shipRouteTable.txt and will produce a debug.txt file that 
	   holds all debug output from program. It will also time the program and output that to the console. 
	3. To run manually, provide GalaxyRoutes with:
		a conduits file: A text file with format Origin Planet, a destination planet, and a travel time all separated by tabs. With each new line representing a new connection. See: Conduits.txt
		a routes file: A text file with format Ship, Origin Planet, Departure time, Destination planet, arrival time. See: shipRouteTable.txt
		Either route cerr to some sort of debug file or /dev/null

Others:
	I have included my routeGen.py file mostly as a curiosity. I was kinda proud of it even if it might not be the best thing on the planet. I also included its input file routes.txt. 
	I used git to help me manage switching between my desktop and laptop (and also to become more familiar with git), the repo can be found at: https://github.com/TSampson92/343Assignment3 or if you are 
	reading this on my github... well you are already here. 
