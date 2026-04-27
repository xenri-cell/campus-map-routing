#include "application.h"

#include <iostream>
#include <limits>
#include <map>
#include <queue> // priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "json.hpp"

#include "dist.h"
#include "graph.h"

using namespace std;

double INF = numeric_limits<double>::max();

using json = nlohmann::json;
void buildGraph(istream &input, graph<long long, double> &g,
                vector<BuildingInfo> &buildings,
                unordered_map<long long, Coordinates> &coords) {
                  json j;
                  try {
                    input >> j;

                  }
                  catch (const json::parse_error& e)  {
                    cerr << "JSON Parse Error: " 
 << e.what() << endl;
return;                  }
const double CONNECTION_DISTANCE_MI = 0.036;
unordered_set<long long> building_ids;

if (j.find("buildings") != j.end())  {
for (const auto& item : j["buildings"])  {
  long long id = item["id"].get<long long>();
  double lat = item["lat"].get<double>();
  double lon = item["lon"].get<double>();
    coords[id] = {lat, lon};

  g.addVertex(id);

    Coordinates location = coords.at(id);
  
    string shortName = "";
    string fullName = "";

    if (item.find("shortName") != item.end())  {
      shortName = item["shortName"].get<string>();
    }
    if (item.find("fullName") != item.end())  {
      fullName = item["fullName"].get<string>();
    }

  buildings.push_back({id, location, shortName, fullName});

  //store coordinates
  //coords[id] = {lat, lon};

  building_ids.insert(id);
}
}



if (j.find("waypoints") != j.end())  {
for(const auto& item : j["waypoints"])  {
  long long id = item["id"].get<long long>();

  if (coords.count(id))  {
    continue;
  }

  double lat = item["lat"].get<double>();
  double lon = item["lon"].get<double>();

  g.addVertex(id);

  //Coordinates location = {lat, lon};

  //buildings.push_back({id, location, item["shortName"], item["fullname"]});

  coords[id] = {lat, lon};

  //building_ids.insert(id);
}
}



// Load Footways
if (j.find("footways") != j.end())  {
for (const auto& way : j["footways"])  {
  if (way.find("nodes") == way.end()) continue;  
  const auto& nodes = way["nodes"];
  for (size_t i = 0; i + 1 < nodes.size(); ++i)  {
    long long from_id = nodes[i].get<long long>();
    long long to_id = nodes[i + 1].get<long long>();

    Coordinates p1 = coords.at(from_id);
    Coordinates p2 = coords.at(to_id);

    double distance = distBetween2Points(p1, p2);

    g.addEdge(from_id, to_id, distance);
    g.addEdge(to_id, from_id, distance);
  }
}
}
//Connect Buildings to Non-Buildings
for (const auto& building : buildings)  {
  long long b_id = building.id;
  Coordinates b_coord = building.location;

  for (const auto& pair : coords)  {
    long long w_id = pair.first;

    if (building_ids.count(w_id))  {
      continue;
    }
    Coordinates w_coord = pair.second;

    double distance = distBetween2Points(b_coord, w_coord);
    if (distance <= CONNECTION_DISTANCE_MI)  {
      g.addEdge(b_id, w_id, distance);
      g.addEdge(w_id, b_id, distance);
    }
  }
}


  // TODO_STUDENT
}

BuildingInfo getBuildingInfo(const vector<BuildingInfo> &buildings,
                             const string &query) {
  for (const BuildingInfo &building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo fail;
  fail.id = -1;
  return fail;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo> &buildings,
                                Coordinates c) {
  double minDestDist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo &building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < minDestDist) {
      minDestDist = dist;
      ret = building;
    }
  }
  return ret;
}

vector<long long> dijkstra(const graph<long long, double> &G, long long start,
                           long long target,
                           const set<long long> &ignoreNodes) {
  return vector<long long>{};
}

double pathLength(const graph<long long, double> &G,
                  const vector<long long> &path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long> &path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

// Honestly this function is just a holdover from an old version of the project
void application(const vector<BuildingInfo> &buildings,
                 const graph<long long, double> &G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto &building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // Look up buildings by query
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lon << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}
