#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <set>
#include <vector>
#include "math.h"
#include <neo4j-client.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "gengraph.h"
#include "get.h"

using namespace std;

// NOTE: Kinda messy, but it works for now
// Global variables to hold graph data
set<uint64_t> Node;
set<uint64_t> nodeWithNoEdge;
vector<uint64_t> Edge;

neo4j_session_t* session;

neo4j_connection_t *init(void) {
    neo4j_client_init();

    neo4j_connection_t *connection = neo4j_connect("neo4j://neo4j:password@localhost:7687",
                                                   NULL,
                                                   NEO4J_INSECURE);
    if (connection == NULL)
    {
        neo4j_perror(stderr, errno, "Connection failed");
        exit(EXIT_FAILURE);
    }

    return connection;
}

void uninit(neo4j_connection_t *connection) {
    neo4j_close(connection);
    neo4j_client_cleanup();
}

// Executes query to neo4j
// Returns if it was a success (0) or not (1)
bool theia_store_neo4j_execute_db(neo4j_connection_t* conn, string stmt) {
    int MAX = 3;

    // Try to send the query at most MAX times
    for (int i = 0; i < MAX; i++) {
        // Send query
        neo4j_result_stream_t* res = neo4j_send(session, stmt.c_str(), neo4j_null);

        // See if it succeeded
        int neo_err_code = neo4j_check_failure(res);

        // Query succeeded
        if (neo_err_code == 0) {
            return 0;
        }

        // Error failed because of the query
        else if (neo_err_code == NEO4J_STATEMENT_EVALUATION_FAILED) {
            string error = "Failed to execute query: " + stmt;
            neo4j_perror(stderr, neo_err_code, error.c_str());
            return 1;
        }

        // Some other error occurred. Sleep with exponential back-off
        else {
            string error = "Some other error with query: " + stmt;
            neo4j_perror(stderr, neo_err_code, error.c_str());

            double m = pow(2.0, (double) i) - 1;

            // Generate random number between 0 and 2^i - 1
            unsigned int r = 0;
            FILE *fd = fopen("/dev/urandom", "r");
            if (fd == NULL) {
                fprintf(stderr, "Error, couldn't open file \"/dev/urandom\"\n");
                fflush(stderr);
                break;
            }
            fread(&r, 4, 1, fd);
            fclose(fd);

            unsigned int t = r % (int) m;

            // Sleep
            sleep(t);
        }

    }

    // Sending query was not successful
    return 1;
}

// Clears neo4j database
// From https://gist.github.com/DaniSancas/1d5265fc159a95ff457b940fc5046887
// and https://neo4j.com/developer/kb/large-delete-transaction-best-practices-in-neo4j/
void clear_neo4j(neo4j_connection_t *conn){
    string stmt = "MATCH (n) OPTIONAL MATCH (n)-[r]-() DELETE n,r";
    theia_store_neo4j_execute_db(conn,stmt);
}


void usage(void) {
    fprintf(stderr, "usage: ./time results.csv\n");
    fflush(stderr);
    exit(2);
}

int main(int argc, char **argv) {
    // Parse arguments
    if (argc != 2) {
        usage();
    }

    // For output file
    FILE *fout = NULL;

    // Open file
    fout = fopen(argv[1],"w");

    // Initialize neo4j
    neo4j_connection_t *connection = init();
    session = neo4j_new_session(connection);

    // Number of nodes and edges
    int MAX = 1000;
    // Number of algorithms
    int algLen = 7;

    // Loop for generating different size graphs
    for (int numNodes = 10; numNodes <= MAX; numNodes *= 10) {
        for (int numEdges = 10; numEdges <= MAX; numEdges *= 10) {
            printf("Testing Graph of Size %dx%d\n", numNodes, numEdges);

            // Write CSV header
            for (int i = 0; i < algLen; i++) {
                // Get algorithm
                struct ALG *alg = getAlg(i);

                // Write algorithm name
                if (i == 0) {
                    fprintf(fout, "Graph Size");
                    fprintf(fout, "%s", alg->name);
                }
                else {
                    fprintf(fout, ",%s", alg->name);
                }
            }
            fprintf(fout, "\n");

            // Write graph size
            fprintf(fout, "%dx%d", numNodes, numEdges);

            // Generate "numNodes" nodes
            genNodes(numNodes);
            // Generate "numEdges" edges
            genEdges(numEdges);

            stringstream stats;

            // For each algorithm
            for (int i = 0; i < algLen; i++) {
                // Get algorithm
                struct ALG *alg = getAlg(i);

                // Create graph
                string rv = alg->create(connection);

                stats << "," << rv;
            }

            // Delete graph
            clear_neo4j(connection);

            // Write rest of numbers
            fprintf(fout, "%s", stats.str().c_str());
            fprintf(fout, "\n");
            fflush(fout);
        }
    }

    neo4j_end_session(session);

    // Disconnect from neo4j
    uninit(connection);

    // Close file
    fclose(fout);

    return 0;
}
