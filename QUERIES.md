## Show all nodes and relationships
```
$ START n=node(*) RETURN n;

OR

$ MATCH (n) RETURN n;
```

## Count number of nodes
```
$ MATCH (n) RETURN count(*)
```

## Count number of edges
```
$ MATCH ()-[r]->() RETURN count(*)
```

## Clear database
```
$ MATCH (n)  OPTIONAL MATCH (n)-[r]-() DETACH DELETE n,r
```

## Create a node (even if it already exists. it will create a duplicate)
```
$ CREATE (n:Node {name:"1"})
$ CREATE (n:Node {name:"2"})
```

## Create a node only if it doesn't already exist
```
$ MERGE (n:Node {name:"1"})
```

## Create an edge between nodes (assuming the nodes already exist)
```
MATCH (n1:Node {name:"1"})
MATCH (n2:Node {name:"2"})
CREATE (n1)-[:EDGE]->(n2)
```

## Create an edge between nodes and create nodes if they don't exist yet
```
MERGE (n1:Node {name:"1"})
MERGE (n2:Node {name:"2"})
CREATE (n1)-[:EDGE]->(n2)
```
## Query an edge
```
MATCH (n1:Node {name:"1"})-[:EDGE]->(n2:Node {name:"2"})
RETURN n1,n2
```
