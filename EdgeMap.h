#pragma once
#ifndef EDGEMAP_H
#define EDGEMAP_H

#include "EdgeNode.h"
#include "VNF.h"
#include "Link.h"
#include "Topology.h"
#include "Tenant.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <map>
#include <set>
#include <functional>
#include <cmath> 


class EdgeMap{
public:
    int length;
    int wedth;
    Topology topo;
    int population;
    int totalDelay;
    // size of randomNumber : nodeSize
    vector<int> randomNumber;
    map<VNF, int> vnfTable;
    map<string,int> delayTable;

    //void genPeople(int n, VNF vnf);
    //void printPeople();
    EdgeMap();
    EdgeMap(int l, int w, Topology topology, int population);
    EdgeMap(int l, int w, Topology topology);
    void requestGeneration(string s , int numberOfReq );
    void addVNFsIntoTable(VNF vnf, int n);
    void printVNFTable();
    VNF getVNFByID( string s );
    int calculateDelay();
    int findAvaliableMinDelayNode(EdgeNode& srcNode, string vnfType);
    static bool sortbysec(const pair<int,int> &a, const pair<int,int> &b){
        return (a.second < b.second);
    }

    static bool sortByDouble(const pair<int,double> &a, const pair<int,double> &b){
        return (a.second > b.second);
    }

    void printReqInNode();
    void printNodeCapReq();
    void printOffloadMsg(string src, string dst, string vType, int n, int delay);
    void localReqHandling();

    void greedyPlacement(VNF vnf);
    void printPlacementResult();
};

#endif