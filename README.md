# campus-map-routing
A C++ web server that loads UIC campus map data, builds a graph, and computes shortest walking paths using Dijkstra's algorithm.

🚀 Features
Graph-based routing: Builds a weighted graph from OSM‑derived JSON data

Shortest path computation: Uses Dijkstra’s algorithm to find optimal walking routes

Interactive web interface: HTML/CSS/JS frontend served directly by the C++ server

Building search: Look up buildings by name or ID

🧠 How It Works
The server loads uic-fa25.osm.json and extracts:
- Nodes (coordinates)
- Buildings
- Edges (walkable paths)

These are stored in:
- graph<long long, double> for routing
- vector<BuildingInfo> for building metadata
- unordered_map<long long, Coordinates> for node lookup

The server exposes several endpoints:
- /pathfinder – shortest path between two buildings
- /byname – search buildings by name
- /nearest – closest building to a coordinate
- /autocomplete – name suggestions
- /nearby – buildings within a distance

The frontend (HTML/JS) calls these endpoints to draw paths and markers on the map.

🛠️ Building & Running
Requirements
- C++17 or newer
- A compiler like g++ or clang++
- httplib.h included in the project

Compile: g++ -std=gnu++17 main.cpp -o server
Run: ./server
