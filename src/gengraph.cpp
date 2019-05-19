#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>

#include "gengraph.h"

using namespace std;

void genNodes(uint64_t num) {
    for (uint64_t i = 0; i < num; i++) {
        Node.insert(i);
        nodeWithNoEdge.insert(i);
    }
}

void genEdges(uint64_t num) {
    // Generate random number between 0 and the number of nodes
    uint64_t n1 = 0;
    uint64_t n2 = 0;
    FILE *fd = fopen("/dev/urandom", "r");
    if (fd == NULL) {
        fprintf(stderr, "Error, couldn't open file \"/dev/urandom\"\n");
        exit(1);
    }

    // Generate random edges
    for (uint64_t i = 0; i < num; i++) {
        // Get random numbers
        fread(&n1, 8, 1, fd);
        fread(&n2, 8, 1, fd);

        uint64_t r1 = n1 % num;
        uint64_t r2 = n2 % num;

        // Add edge
        Edge.push_back(r1);
        Edge.push_back(r2);

        // Remove nodes
        nodeWithNoEdge.erase(r1);
        nodeWithNoEdge.erase(r2);
    }

    fclose(fd);
}
