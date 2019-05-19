#ifndef __GENGRAPH_H__
#define __GENGRAPH_H__

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <set>
#include <vector>

using namespace std;

// Global variables of nodes and edges
extern set<uint64_t> Node;
extern set<uint64_t> nodeWithNoEdge;
extern vector<uint64_t> Edge;

// Parameter is number of nodes
void genNodes(uint64_t num);

// Parameter is the number of edges to generate
// Removes nodes connected by edges from "Node" variable above
void genEdges(uint64_t num);

#endif
