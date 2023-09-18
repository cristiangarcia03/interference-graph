#include "proj6.hpp"
#include "CSVReader.hpp"
#include "InterferenceGraph.hpp"

#include <exception>
#include <string>
#include <unordered_set>

//i add
#include <map>
#include <list>
#include <iostream>
#include <vector>


using namespace std;
using namespace proj6;

// assignRegisters
//
// This is where you implement the register allocation algorithm
// as mentioned in the README. Remember, you must allocate at MOST
// d(G) + 1 registers where d(G) is the maximum degree of the graph G.
// If num_registers is not enough registers to accomodate the passed in
// graph you should return an empty map. You MUST use registers in the
// range [1, num_registers] inclusive.

vector<Variable> sorter(InterferenceGraph<Variable> graph) {

  vector<Variable> verts;
  unordered_set<Variable> vertices = graph.vertices();
  for (Variable v: vertices) {
    verts.push_back(v);
  }
  
  int i = 0;
  int j = 0;
  int indexSmallest = 0;
  Variable temp;  // Temporary variable for swap
  for (i = 0; i < verts.size() - 1; ++i) {
      // Find index of smallest remaining element
      indexSmallest = i;
      for (j = i + 1; j < verts.size(); ++j) {
         if (graph.degree(verts[j]) > graph.degree(verts[indexSmallest]) ) {
          indexSmallest = j;
         }
      }
      // Swap
      temp = verts[i];
      verts[i] = verts[indexSmallest];
      verts[indexSmallest] = temp;
  }

  return verts;
}




RegisterAssignment proj6::assignRegisters(const std::string &path_to_graph, int num_registers) noexcept {
  
  InterferenceGraph<Variable> ig = CSVReader::load(path_to_graph);

  
  RegisterAssignment res; // unordered_map<string, int>
  unordered_set<Variable> vertices = ig.vertices();


  int max_registers = 0;
  for (Variable v: vertices) {
    if (max_registers < ig.degree(v)) {
      max_registers = ig.degree(v);
    }
  }
  max_registers += 1; // d(G) + 1
  if (num_registers < max_registers) {
    max_registers = num_registers;
  }
  //cout << "registers: " << max_registers << endl;

  vector<int> c;
  int temp = 1;
  for (int i = 0; i < max_registers; i++) {
    c.push_back(temp);
    temp += 1;
  }
  vector<Variable> sorted = sorter(ig);
  int c_idx = 0;

  for (int i = 0; i < sorted.size(); i++) {
    if (c_idx == c.size()) {
      if (res.size() == sorted.size()) {
        return res;
      }
      //cout << "done" << endl;
      return {};
    }
    if (res.count(sorted[i]) == false) {
      //cout << sorted[i] << "-" << c_idx << " " << endl;
      res[sorted[i]] = c[c_idx];
      int j = i + 1;
      while (j < sorted.size()) {
        if (res.count(sorted[j]) == false && ig.interferes(sorted[i], sorted[j]) == false) {
          res[sorted[j]] = c[c_idx];
          //cout << sorted[j] << "-" <<  c_idx << endl;
        }
        j += 1;
      }
      c_idx += 1;
    }
  }

  // for (auto v: res) {
  //   cout << v.first << "-R" << v.second<< "-" << ig.degree(v.first) << " ";
  // }
  // cout << endl;
  return res;
}
