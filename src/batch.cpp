#include <neo4j-client.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include "get.h"
#include "gengraph.h"
#include "batch.h"

using namespace std;

string createBatch(neo4j_connection_t* conn) {
    stringstream rv;

    struct timeval start, end;
    long seconds, useconds;
    double s,total=0;

    set<uint64_t>::iterator nit;

    long BATCH_SIZE = 100;
    long stmt_count = 0;

    // Used for creating variables for neo4j
    long count = 0;

    // Create constraint on name
    stringstream query;
    query << "CREATE CONSTRAINT ON (n:Node) ASSERT n.name IS UNIQUE";
    theia_store_neo4j_execute_db(conn,query.str());
    query.str("");

    // Create graph nodes/edges
    for (uint64_t i = 0; i < Edge.size(); i+=2) {
        query << "MERGE (n" << count << ":Node {name:\"" << Edge[i] << "\"}) ";
        count++;
        query << "MERGE (n" << count << ":Node {name:\"" << Edge[i+1] << "\"}) ";
        count++;
        query << "CREATE (n" << count-2 << ")-[:EDGE]->(n" << count-1 << ") ";
        stmt_count+=3;

        // Send query
        if (stmt_count >= BATCH_SIZE) {
            gettimeofday(&start, NULL);
            theia_store_neo4j_execute_db(conn,query.str());
            gettimeofday(&end, NULL);

            seconds = end.tv_sec-start.tv_sec;
            useconds = end.tv_usec-start.tv_usec;
            s = seconds + useconds / 1000000.0;
            total += s;

            stmt_count = 0;
            count = 0;
            query.str("");
        }
    }

    // Insert remaining nodes
    for (uint64_t i = 0; i < nodeWithNoEdge.size(); i++) {
        nit = nodeWithNoEdge.find(i);

        query << "MERGE (n" << count << ":Node {name:\"" << *nit << "\"}) ";
        count++;
        stmt_count++;

        // Send query
        if (stmt_count == BATCH_SIZE) {
            gettimeofday(&start, NULL);
            theia_store_neo4j_execute_db(conn,query.str());
            gettimeofday(&end, NULL);

            seconds = end.tv_sec-start.tv_sec;
            useconds = end.tv_usec-start.tv_usec;
            s = seconds + useconds / 1000000.0;
            total += s;

            stmt_count = 0;
            count = 0;
            query.str("");
        }
    }

    // Send any remaining queries
    if (stmt_count > 0) {
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

struct ALG getBatch =
{
    "Batch",
    &createBatch
};
