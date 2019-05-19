## Purpose
Benchmarking node & edge insertion

## Requirements
  * Debian 9
  * Python 3.5

## Installing Dependencies
```
$ ./setup.sh
$ pip install -r requirements.txt
```

## Trust Neo4j
```
$ sudo service neo4j start
$ neo4j-client -u neo4j localhost
```

## Modify Code
  * Edit `src/main.cpp` with Neo4j username and password for `neo4j_connect()`

## Compile Code
```
$ mkdir bin
$ make
```

## Run Code
```
# Sudo is for storing CSV file for Neo4j to import
$ sudo ./bin/time results.csv
```

## Graph results
```
$ python graph.py results.csv graphs/
```

## Delete Neo4J graph
```
$ sudo rm -r /var/lib/neo4j/data/databases/graph.db/
$ sudo service neo4j restart
```

## Reset Neo4J password
```
$ sudo rm -r /var/lib/neo4j/data/dbms/auth
$ sudo service neo4j restart
```

## Tweaking Neo4j
```
$ neo4j-admin memrec
```

## Common Neo4j queries can be found here: [QUERIES.md](QUERIES.md)

## Helpful Documentation
  * https://neo4j.com/docs/operations-manual/current/installation/linux/debian/
  * https://neo4j.com/docs/operations-manual/current/configuration/ports/
  * https://neo4j.com/docs/operations-manual/current/configuration/file-locations/
  * https://neo4j.com/docs/operations-manual/current/security/
  * https://neo4j.com/docs/operations-manual/current/security/ssl-framework/
  * https://neo4j.com/docs/developer-manual/current/reference/status-codes/
  * https://neo4j.com/blog/cypher-write-fast-furious/
  * https://dzone.com/articles/tips-for-fast-batch-updates-of-graph-structures-wi
  * http://bigdatums.net/2017/01/01/counting-relationships-in-neo4j/
