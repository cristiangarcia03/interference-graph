#ifndef __INTERFERENCE_GRAPH__HPP
#define __INTERFERENCE_GRAPH__HPP

#include <exception>
#include <string>
#include <unordered_set>

//i add
#include <map>
#include <list>
#include <iostream>

using namespace std;

class UnknownVertexException : public std::runtime_error {
public:
  UnknownVertexException(const std::string &v)
      : std::runtime_error("Unknown vertex " + v) {}
};

class UnknownEdgeException : public std::runtime_error {
public:
  UnknownEdgeException(const std::string &v, const std::string &w)
      : std::runtime_error("Unknown edge " + v + " - " + w) {}
};

// InterferenceGraph
//
// This is a class representing an interference graph
// as described in "Part 1: Interference Graph" of the README.md
// file. Though this class is templated, because of the usage of exceptions
// UnknownVertexException and UnknownEdgeException it will
// ONLY be tested with strings.
template <typename T> 
class InterferenceGraph {
public:
  // Custom type used to represent edges. This is mainly
  // used in the utility function for reading and writing
  // the graph structure to/from files. You don't need to use it.
  using EdgeTy = std::pair<T, T>;

  InterferenceGraph();
  ~InterferenceGraph();
  void addEdge(const T &v, const T &w);
  void addVertex(const T &vertex) noexcept;
  void removeEdge(const T &v, const T &w);
  void removeVertex(const T &vertex);
  std::unordered_set<T> vertices() const noexcept;
  std::unordered_set<T> neighbors(const T &vertex) const;
  unsigned numVertices() const noexcept;
  unsigned numEdges() const noexcept;
  bool interferes(const T &v, const T &w) const;
  unsigned degree(const T &v) const;

  void view ();

private:
  // Private member variables here.
  map<T, list<T>> graph;
  int node_count;
  int edge_count;
};

template <typename T> 
InterferenceGraph<T>::InterferenceGraph() {
    node_count = 0;
    edge_count = 0;
}

template <typename T> InterferenceGraph<T>::~InterferenceGraph() {}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::neighbors(const T &vertex) const {
  if (graph.count(vertex) == false) {
    throw UnknownVertexException("Given unknown node");
  }
  unordered_set<T> s;
  for (auto value : graph.at(vertex)) {
    s.insert(value);
  }
  /*
  for (auto const &node : graph) {
    if (node.first == vertex) {
      continue;
    }
    for (auto const &v : node.second) {
      if (v == vertex) {
        s.insert(node.frist);
        continue;
      }
    }
    std::cout << std::endl;
  }
  */
  return s;
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::vertices() const noexcept {
  unordered_set<T> s;
  for (auto node : graph) {
    s.insert(node.first);
  }
  return s;
}

template <typename T>
unsigned InterferenceGraph<T>::numVertices() const noexcept {
  return node_count;
}

template <typename T> unsigned InterferenceGraph<T>::numEdges() const noexcept {
  return edge_count;
}

template <typename T>
void InterferenceGraph<T>::addEdge(const T &v, const T &w) {
  if (graph.count(v) == false || graph.count(w) == false) {
    throw UnknownVertexException("Given unknown node");
  }
  graph.at(w).push_back(v);
  graph.at(v).push_back(w);
  edge_count += 1;
}

template <typename T>
void InterferenceGraph<T>::removeEdge(const T &v, const T &w) {
  if (graph.count(v) == false || graph.count(w) == false) {
    throw UnknownVertexException("Given unknown node");
  }
  if (interferes(v, w) == false) {
    throw UnknownVertexException("Given unknown node");
  }
  graph.at(v).remove(w);
  graph.at(w).remove(v);
  edge_count -= 1;
}

template <typename T>
void InterferenceGraph<T>::addVertex(const T &vertex) noexcept {
  if (graph.count(vertex) == false) {
    graph[vertex] = {};
    node_count += 1;
  }
}

template <typename T>
void InterferenceGraph<T>::removeVertex(const T &vertex) {
  if (graph.count(vertex) == false) {
    throw UnknownVertexException("Given unknown node");
  }
  node_count -= 1;
  for (auto &value : graph.at(vertex)) {
    graph.at(value).remove(vertex);
  }
  graph.erase(vertex);
}

template <typename T>
bool InterferenceGraph<T>::interferes(const T &v, const T &w) const {

  if (graph.count(v) == false || graph.count(w) == false) {
    throw UnknownVertexException("Given unknown node");
  }
  for (auto const &value : graph.at(v)) {
    if (value == w) {
      return true;
    }
  }
  return false;
}

template <typename T> unsigned InterferenceGraph<T>::degree(const T &v) const {
  if (graph.count(v) == false) {
    throw UnknownVertexException("Given unknown node");
  }
  return graph.at(v).size();
}

template <typename T>
void InterferenceGraph<T>::view () {
   for (auto kv : graph) {
        std::cout << kv.first << " =>";
        for (auto i : kv.second) {
          std::cout << " " << i;
        }
        std::cout << std::endl;
    }
}

#endif
