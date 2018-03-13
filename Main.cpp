#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include "Topology.h"
#include "EdgeNode.h"
#include "VNF.h"
#include "EdgeMap.h"

// function delcaration
Topology simpleTopo();
Topology eastAsiaTopo();
map<VNF,int> simpleTable();

void simplePlacement(EdgeMap& edgeMap);
int countDelay(EdgeMap edgeMap);
VNF* getVNFwithID(map<VNF, int > table, string s );
EdgeNode findAvaliableNode(Tenant p, vector<EdgeNode> nodeList);
void requestGeneration(vector<EdgeNode> nodeList, int numberOfReq );

// global variable
map<VNF,int> vnfTable;

using namespace std;

int main(){

	Topology t = eastAsiaTopo();

	t.initDistanceMatrix();
	t.floydWarshallAlgo();
	t.printDistanceMatrix();
	
	
	EdgeMap simpleMap = EdgeMap( 1100, 800, t);
	//simpleMap.addVNFsIntoTable( VNF("vnf1", 25, 5), 5 );
	simpleMap.addVNFsIntoTable( VNF("vnf1", 10, 5), 20 );
	simpleMap.printVNFTable();
	simpleMap.requestGeneration("vnf1", 100);
	//simpleMap.requestGeneration("vnf2",  50);
	simpleMap.printNodeCapReq();
	//cout << simpleMap.getVNFByID("vnf1").capacity << endl;
	simpleMap.greedyPlacement( simpleMap.getVNFByID("vnf1") );
	simpleMap.printPlacementResult();

	simpleMap.calculateDelay();
	cout << "Avg delay : " ;
	cout << (double)simpleMap.totalDelay/25 << endl;

	t.claenUpDistanceMatrix();
    return 0;
}

Topology simpleTopo(){
	Topology t;
	vector<EdgeNode>& nodes = t.nodeList;
	vector<Link>& links = t.linkList;

	nodes.push_back( EdgeNode("n1", 200, 300, 100) );
	nodes.push_back( EdgeNode("n2", 400, 150, 100) );
	nodes.push_back( EdgeNode("n3", 400, 450, 100) );
	nodes.push_back( EdgeNode("n4", 600, 300, 100) );
	nodes.push_back( EdgeNode("n5", 700, 300, 100) );

	links.push_back( Link(nodes[0], nodes[1], 50) );
	links.push_back( Link(nodes[0], nodes[2], 30) );
	links.push_back( Link(nodes[0], nodes[3], 40) );
	links.push_back( Link(nodes[1], nodes[3], 10) );
	links.push_back( Link(nodes[2], nodes[3], 15) );
	links.push_back( Link(nodes[1], nodes[4], 15) );
	links.push_back( Link(nodes[3], nodes[4], 10) );
	links.push_back( Link(nodes[2], nodes[4], 10) );
	

	//t.printTopo();
	//t.printTopoWithVNF();

	return t;
}

Topology eastAsiaTopo(){
	Topology t;
	vector<EdgeNode>& nodes = t.nodeList;
	vector<Link>& links = t.linkList;

	nodes.push_back( EdgeNode("Beijing", 200, 200, 50) );
	nodes.push_back( EdgeNode("Singapore", 250, 600, 50) );
	nodes.push_back( EdgeNode("HongKong", 300, 400, 50) );
	nodes.push_back( EdgeNode("Shangai", 350, 250, 50) );
	nodes.push_back( EdgeNode("Manila", 400, 500, 50) );
	nodes.push_back( EdgeNode("Tokyo", 500, 200, 50) );
	nodes.push_back( EdgeNode("Sydney", 600, 600, 50) );
	nodes.push_back( EdgeNode("Hawii", 800, 300, 50) );

	// BEJ => SHA 10
	links.push_back( Link(nodes[0], nodes[3], 10) );
	// BEJ => HKG 10
	links.push_back( Link(nodes[0], nodes[2], 10) );
	// SIN => HKG 10
	links.push_back( Link(nodes[1], nodes[2], 10) );
	// SIN => MAN 10
	links.push_back( Link(nodes[1], nodes[4], 10) );
	// SIN => TOK 10
	links.push_back( Link(nodes[1], nodes[5], 10) );
	// SIN => SYD 50
	links.push_back( Link(nodes[1], nodes[6], 50) );
	// HKG => SHA 10
	links.push_back( Link(nodes[2], nodes[3], 10) );
	// HKG => MAN 10
	links.push_back( Link(nodes[2], nodes[4], 10) );
	// MAN => HAW 75
	links.push_back( Link(nodes[4], nodes[7], 75) );
	// TOK => SYD 30
	links.push_back( Link(nodes[5], nodes[6], 30) );
	// TOK => HAW 75
	links.push_back( Link(nodes[5], nodes[7], 75) );
	// SYD => HAW 75
	links.push_back( Link(nodes[6], nodes[7], 75) );

	return t;
}

map<VNF, int> simpleTable(){
	map<VNF, int > vTable;

	VNF vnf1 = VNF("vnf1", 50, 5);
	// number of vnf1
	vTable[ vnf1 ] = 5;
	
	return vTable;
}

void simplePlacement(EdgeMap& edgeMap){
	vector<EdgeNode>& nodeList = edgeMap.topo.nodeList;

	nodeList[0].addVNFs( VNF("vnf1", 50, 5), 2);
	nodeList[1].addVNFs( VNF("vnf1", 50, 5), 1);
	nodeList[2].addVNFs( VNF("vnf1", 50, 5), 1);
	nodeList[3].addVNFs( VNF("vnf1", 50, 5), 1);
	nodeList[4].addVNFs( VNF("vnf1", 50, 5), 0);
	
	edgeMap.topo.printTopoWithVNF();
}

