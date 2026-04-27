#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

/// @brief Simple directed graph using an adjacency list.
/// @tparam VertexT vertex type
/// @tparam WeightT edge weight type
template <typename VertexT, typename WeightT>
class graph {
 private:
 unordered_map<VertexT, unordered_map<VertexT, WeightT>> adjacency_list;
  // TODO_STUDENT
  unordered_map<VertexT, bool> all_vertices;
  size_t edge_count = 0;

 public:
  /// Default constructor
  graph() {
    // TODO_STUDENT
  }

  /// @brief Add the vertex `v` to the graph, must typically be O(1).
  /// @param v
  /// @return true if successfully added; false if it existed already
  bool addVertex(VertexT v) {
    if (all_vertices.count(v))  {
      return false;
    }
    all_vertices[v] = true;
    // TODO_STUDENT
    return true;
  }

  /// @brief Add or overwrite directed edge in the graph, must typically be
  /// O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight edge weight / label
  /// @return true if successfully added or overwritten;
  ///         false if either vertices isn't in graph
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    // TODO_STUDENT
    if (!all_vertices.count(from) || !all_vertices.count(to))  {
      return false;
    }

    bool edge_exists = adjacency_list[from].count(to);

    adjacency_list[from][to] = weight;

    if (!edge_exists)  {
      edge_count++;
    }
    return true;
  }

  /// @brief Maybe get the weight associated with a given edge, must typically
  /// be O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight output parameter
  /// @return true if the edge exists, and `weight` is set;
  ///         false if the edge does not exist
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    auto it = adjacency_list.find(from);

    if (it == adjacency_list.end())  {
      return false;
    }

    auto to_it = it->second.find(to);

    if (to_it == it->second.end())  {
      return false;
    }

    weight = to_it->second;
    // TODO_STUDENT
    return true;
  }

  /// @brief Get the out-neighbors of `v`. Must run in at most O(|V|).
  /// @param v
  /// @return vertices that v has an edge to
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT> S;
    auto it = adjacency_list.find(v);

    if (it == adjacency_list.end())  {
      return S;
    }  
    for (const auto& edge_pair : it->second)  {
      S.insert(edge_pair.first);
    }
    // TODO_STUDENT
    return S;
  }

  /// @brief Return a vector containing all vertices in the graph
  vector<VertexT> getVertices() const {
    vector<VertexT> vertices;
    for (const auto& pair : all_vertices)  {
      vertices.push_back(pair.first);
    }
    // TODO_STUDENT
    return vertices;
  }

  /// @brief Get the number of vertices in the graph. Runs in O(1).
  size_t numVertices() const {
    // TODO_STUDENT
    return all_vertices.size();
  }

  /// @brief Get the number of directed edges in the graph. Runs in at most
  /// O(|V|), but should be O(1).
  size_t numEdges() const {
    // TODO_STUDENT
    return edge_count;
  }
};
