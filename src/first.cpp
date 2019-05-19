#include <neo4j-client.h>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include "get.h"
#include "gengraph.h"
#include "first.h"

using namespace std;

string createFirst(neo4j_connection_t* conn) {
    stringstream rv;

    struct timeval start, end;
    long seconds, useconds;
    double s,total=0;

    set<uint64_t>::iterator nit;

    // Create graph nodes
    for (uint64_t i = 0; i < Node.size(); i++) {
        nit = Node.find(i);

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

    // Create graph edges
    for (uint64_t i = 0; i < Edge.size(); i+=2) {
        stringstream query;
        query << "MATCH (n1:Node),(n2:Node) WHERE n1.name='" << Edge[i] << "' AND n2.name='" << Edge[i+1] << "' CREATE (n1)-[:EDGE]->(n2)";

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

    return rv.str();
}

struct ALG getFirst =
{
    "First",
    &createFirst
};
