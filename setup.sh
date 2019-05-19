#!/bin/bash

sudo apt update
sudo apt install -y apt-transport-https

# Get Neo4j source
wget -O - https://debian.neo4j.org/neotechnology.gpg.key | sudo apt-key add -
echo 'deb https://debian.neo4j.org/repo stable/' | sudo tee /etc/apt/sources.list.d/neo4j.list
sudo apt update

# Install Neo4j
sudo apt install -y neo4j

# Install Neo4j Library
sudo apt install -y neo4j-client libneo4j-client-dev

# Install Make
sudo apt install -y make

# Install g++
sudo apt install -y g++
