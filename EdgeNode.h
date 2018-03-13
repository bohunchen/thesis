#pragma once
#include <vector>
#include <string>
#include <map>
#include "VNF.h"
#include <iostream>


#ifndef EDGENODE_H
#define EDGENODE_H

using namespace std;

class EdgeNode
{
public:
	int x, y;
	string nodeName;
	int infrasCapacity;
	int remainInfrasCapacity;
	vector<EdgeNode> adjEdgeNode;
	map<VNF, int > vnfTable;
	map<string, int > vnfToCapacity;
	map<string, int > vnfToReqs;
	map<string, int > reqTable;
	map<string, int > remainReqTable;
	map<string, int > remainVNFReq;
	

	EdgeNode();
	EdgeNode(string name, int x, int y, int infrasCapacity);
	void setLocation(int x, int y);
	void setName(string s);
	string getName();
	bool addVNFs ( VNF vnf, int cnt);
	void addReqTable( string s , int reqNum);
	void printReqTable();
	int getVNFCapacity( string s);
	~EdgeNode();
};


#endif // !EDGENODE_H

