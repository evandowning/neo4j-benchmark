#include <neo4j-client.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include "get.h"
#include "gengraph.h"
#include "unwind.h"

using namespace std;

/*
Creating nodes:
   WITH [{name: '1'}, {name: '2'}] as batches
   UNWIND batches as batch
   MERGE (n:Node {name: batch.name})

Creating edges:
   WITH [{name: '1',to: '2'}, {name: '2',to: '1'}, {name: '3',to: '1'}] as batches
   UNWIND batches as batch
   MERGE (n1:Node {name: batch.name})
   MERGE (n2:Node {name: batch.to})
   CREATE (n1)-[:EDGE]->(n2)
*/

string createUnwind(neo4j_connection_t* conn) {
    stringstream rv;

    struct timeval start, end;
    long seconds, useconds;
    double s,total=0;

    set<uint64_t>::iterator nit;

    long BATCH_SIZE = 100;
    long count = 0;

    // When to incrementally print out timing data
    long PRINT = 1000000;

    // Create constraint on name
    stringstream query;
    query << "CREATE CONSTRAINT ON (n:Node) ASSERT n.name IS UNIQUE";
    theia_store_neo4j_execute_db(conn,query.str());
    query.str("");

    // Create graph nodes/edges
    query << "WITH [";
    for (uint64_t i = 0; i < Edge.size(); i+=2) {
        if (count % BATCH_SIZE == 0) {
            query << "{name: '" << Edge[i] << "',to: '" << Edge[i+1] << "'}";
        }
        else {
            query << ", {name: '" << Edge[i] << "',to: '" << Edge[i+1] << "'}";
        }
        count+=1;

        // Send query
        if ((count % BATCH_SIZE == 0) || (i == Edge.size()-2)) {
            // Finalize query
            query << "] as batches ";
            query << "UNWIND batches as batch ";
            query << "MERGE (n1:Node {name: batch.name}) ";
            query << "MERGE (n2:Node {name: batch.to}) ";
            query << "CREATE (n1)-[:EDGE]->(n2)";

            gettimeofday(&start, NULL);
            theia_store_neo4j_execute_db(conn,query.str());
            gettimeofday(&end, NULL);

            seconds = end.tv_sec-start.tv_sec;
            useconds = end.tv_usec-start.tv_usec;
            s = seconds + useconds / 1000000.0;
            total += s;

            query.str("");
            query << "WITH [";
        }

        // Print out incremental timing data
        if (count % PRINT == 0) {
            rv << "," << total;
        }
    }

    // Send any remaining queries
    if (count % BATCH_SIZE != 0) {
        // Finish query
        query << "] as batches ";
        query << "UNWIND batches as batch ";
        query << "MERGE (n1:Node {name: batch.name}) ";
        query << "MERGE (n2:Node {name: batch.to}) ";
        query << "CREATE (n1)-[:EDGE]->(n2)";

        gettimeofday(&start, NULL);
        theia_store_neo4j_execute_db(conn,query.str());
        gettimeofday(&end, NULL);

        seconds = end.tv_sec-start.tv_sec;
        useconds = end.tv_usec-start.tv_usec;
        s = seconds + useconds / 1000000.0;
        total += s;
    }

    count = 0;

    // Insert remaining nodes
    query.str("");
    query << "WITH [";
    for (uint64_t i = 0; i < nodeWithNoEdge.size(); i++) {
        nit = nodeWithNoEdge.find(i);

        if (count % BATCH_SIZE == 0) {
            query << "{name: '" << *nit << "'}";
        }
        else {
            query << ", {name: '" << *nit << "'}";
        }
        count+=1;

        // Send query
        if (count % BATCH_SIZE == 0) {
            // Finish query
            query << "] as batches ";
            query << "UNWIND batches as batch ";
            query << "MERGE (n:Node {name: batch.name})";

            gettimeofday(&start, NULL);
            theia_store_neo4j_execute_db(conn,query.str());
            gettimeofday(&end, NULL);

            seconds = end.tv_sec-start.tv_sec;
            useconds = end.tv_usec-start.tv_usec;
            s = seconds + useconds / 1000000.0;
            total += s;

            query.str("");
            query << "WITH [";
        }

        // Print out incremental timing data
        if (count % PRINT == 0) {
            rv << "," << total;
        }
    }

    // Send any remaining queries
    if (count % BATCH_SIZE != 0) {
        // Finish query
        query << "] as batches ";
        query << "UNWIND batches as batch ";
        query << "MERGE (n:Node {name: batch.name})";

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

struct ALG getUnwind = 
{
    "Unwind",
    &createUnwind
};
