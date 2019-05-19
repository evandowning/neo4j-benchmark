#define __STDC_FORMAT_MACROS
#include <inttypes.h>

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

string createLoadcsv(neo4j_connection_t* conn) {
    stringstream rv;

    struct timeval start, end;
    long seconds, useconds;
    double s,total=0;

    set<uint64_t>::iterator nit;

    FILE *fout = NULL;

    string foutName;
    stringstream out;

    // Construct data in CSV files
    foutName = "/var/lib/neo4j/import/loadcsv-test-edges.csv";
    fout = fopen(foutName.c_str(),"w");
    out.str("");
    for (uint64_t i = 0; i < Edge.size(); i+=2) {
        out << i << "," << Edge[i] << "," << Edge[i+1] << "\n";
    }
    fprintf(fout,"%s",out.str().c_str());
    fclose(fout);

    // Insert remaining nodes
    foutName = "/var/lib/neo4j/import/loadcsv-test-nodes.csv";
    fout = fopen(foutName.c_str(),"w");
    out.str("");
    for (uint64_t i = 0; i < nodeWithNoEdge.size(); i++) {
        nit = nodeWithNoEdge.find(i);
        out << i << "," << *nit << "\n";
    }
    fprintf(fout,"%s",out.str().c_str());
    fclose(fout);

    // Create constraint on name
    stringstream query;
    query << "CREATE CONSTRAINT ON (n:Node) ASSERT n.name IS UNIQUE";
    theia_store_neo4j_execute_db(conn,query.str());

    // Construct queries to Neo4j
    query.str("");
    query << "USING PERIODIC COMMIT 100 ";
    query << "LOAD CSV FROM 'file:///loadcsv-test-edges.csv' AS line ";
    query << "MERGE (n1:Node { name: line[1]}) ";
    query << "MERGE (n2:Node { name: line[2]}) ";
    query << "CREATE (n1)-[:EDGE]->(n2) ";

    gettimeofday(&start, NULL);
    theia_store_neo4j_execute_db(conn,query.str());
    gettimeofday(&end, NULL);
    seconds = end.tv_sec-start.tv_sec;
    useconds = end.tv_usec-start.tv_usec;
    s = seconds + useconds / 1000000.0;
    total += s;

    query.str("");
    query << "USING PERIODIC COMMIT 100 ";
    query << "LOAD CSV FROM 'file:///loadcsv-test-nodes.csv' AS line ";
    query << "MERGE (:Node { name: line[1]})";

    gettimeofday(&start, NULL);
    theia_store_neo4j_execute_db(conn,query.str());
    gettimeofday(&end, NULL);
    seconds = end.tv_sec-start.tv_sec;
    useconds = end.tv_usec-start.tv_usec;
    s = seconds + useconds / 1000000.0;
    total += s;

    rv << total;

    // Drop constraint on name
    query.str("");
    query << "DROP CONSTRAINT ON (n:Node) ASSERT n.name IS UNIQUE";
    theia_store_neo4j_execute_db(conn,query.str());

    return rv.str();
}

struct ALG getLoadcsv =
{
    "Loadcsv",
    &createLoadcsv
};
