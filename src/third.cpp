#include <neo4j-client.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include "get.h"
#include "gengraph.h"
#include "third.h"

using namespace std;

string createThird(neo4j_connection_t* conn) {
    stringstream rv;

    struct timeval start, end;
    long seconds, useconds;
    double s,total=0;

    set<uint64_t>::iterator nit;

    // Create constraint on name
    stringstream query;
    query << "CREATE CONSTRAINT ON (n:Node) ASSERT n.name IS UNIQUE";
    theia_store_neo4j_execute_db(conn,query.str());

    // Create graph nodes/edges
    for (uint64_t i = 0; i < Edge.size(); i+=2) {
        stringstream query;
        query << "MERGE (n1:Node {name:\"" << Edge[i] << "\"}) ";
        query << "MERGE (n2:Node {name:\"" << Edge[i+1] << "\"}) ";
        query << "CREATE (n1)-[:EDGE]->(n2)";

        // Send query
        gettimeofday(&start, NULL);
        theia_store_neo4j_execute_db(conn,query.str());
        gettimeofday(&end, NULL);

        seconds = end.tv_sec-start.tv_sec;
        useconds = end.tv_usec-start.tv_usec;
        s = seconds + useconds / 1000000.0;
        total += s;
    }

    // Create remaining nodes
    for (uint64_t i = 0; i < nodeWithNoEdge.size(); i++) {
        nit = nodeWithNoEdge.find(i);

        stringstream query;
        query << "MERGE (n:Node {name:\"" << *nit << "\"})";

        // Send query
        gettimeofday(&start, NULL);
        theia_store_neo4j_execute_db(conn,query.str());
        gettimeofday(&end, NULL);

        seconds = end.tv_sec-start.tv_sec;
        useconds = end.tv_usec-start.tv_usec;
        s = seconds + useconds / 1000000.0;
        total += s;
    }

    rv << total;

    // Drop constraint on name
    query.str("");
    query << "DROP CONSTRAINT ON (n:Node) ASSERT n.name IS UNIQUE";
    theia_store_neo4j_execute_db(conn,query.str());

    return rv.str();
}

struct ALG getThird =
{
    "Third",
    &createThird
};
