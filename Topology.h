#pragma once
#ifndef TOPOLOGY_H

#define TOPOLOGY_H

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <climits>
#include <limits.h>
#include "EdgeNode.h"
#include "Link.h"

using namespace std;

class Topology
{
public:
	// variable
	vector<EdgeNode> nodeList;
	vector<Link> linkList;
	int **distanceMatrix;
	// funciton
	Topology();
	~Topology();
	void addEdgeNode(string name, int x, int y, int infrasCapacity);
	void addLink(EdgeNode node1, EdgeNode node2, int delay);
	void printTopo();
	void printTopoWithVNF();
	void printTopoWithReq();
	void initDistanceMatrix();
	void claenUpDistanceMatrix();
	void printDistanceMatrix();
	int findEdgeNodeIndex(EdgeNode node);
	void floydWarshallAlgo();
};



#endif // !TOPOLOGY_H
