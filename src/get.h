#ifndef __GET_H__
#define __GET_H__

#include <stdint.h>
#include <string>
#include <neo4j-client.h>

using namespace std;

// To send statements to neo4j
bool theia_store_neo4j_execute_db(neo4j_connection_t* conn, string stmt);

// Structure for generalizing graph DB interactions
struct ALG {
    // Algorithm's name
    const char *name;

    // Creates a graph
    // Returns timing data
    string(*create)(neo4j_connection_t* conn);
};

// Function for retrieving an algorithm
struct ALG *getAlg(int c);

#endif
