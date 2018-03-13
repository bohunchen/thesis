#include "Topology.h"

Topology::Topology(){}

Topology::~Topology(){}

void Topology::addEdgeNode(string name, int x, int y, int infrasCapacity) {
	EdgeNode n(name, x, y, infrasCapacity);
	this->nodeList.push_back(n);
}

void Topology::addLink(EdgeNode n1, EdgeNode n2, int delay) {
	Link link(n1, n2, delay);
	this->linkList.push_back(link);
}

void Topology::printTopo(){

	cout << "Nodes : " << endl;
	for( vector<EdgeNode>::iterator it = nodeList.begin(); it!=nodeList.end(); it++ ){
		cout << it->nodeName << " " ;
		cout << "(" << it->x << ", " << it->y << ") ";
		cout << it->infrasCapacity << endl;
	}

	cout << "Links :" << endl;
	for( vector<Link>::iterator it = linkList.begin(); it!=linkList.end(); it++ ){
		cout << it->nodeSet[0].nodeName << " " ;
		cout << it->nodeSet[1].nodeName << " " ;
		cout << it->delay << endl;
	}
	
}

void Topology::printTopoWithVNF(){
	cout << "Nodes with VNF : " << endl;
	for( vector<EdgeNode>::iterator nodeIt = nodeList.begin(); nodeIt!=nodeList.end(); nodeIt++ ){
		cout << nodeIt->nodeName << " " ;
		cout << "(" << nodeIt->x << ", " << nodeIt->y << ") ";
		cout << nodeIt->remainInfrasCapacity << "/"<< nodeIt->infrasCapacity << " ";
		for( map<VNF, int >::iterator vnfIt = nodeIt->vnfTable.begin() ; vnfIt!=nodeIt->vnfTable.end(); vnfIt++){
			cout << "(";
			cout << vnfIt->first.typeID << ", " << vnfIt->second << ") ";
		}
		//cout << nodeIt->vnfTable.size() << endl;
		cout << endl;
	}
}

void Topology::printTopoWithReq(){
	cout << "Nodes with Req : " << endl;
	for( vector<EdgeNode>::iterator nodeIt = nodeList.begin(); nodeIt!=nodeList.end(); nodeIt++ ){
		cout << nodeIt->nodeName << ":\n" ;
		nodeIt->printReqTable();
	}
}

void Topology::initDistanceMatrix(){
	int len = this->nodeList.size();
	this->distanceMatrix = new int* [ len ];
	for( int i=0 ; i< len ;i++){
		this->distanceMatrix[i] = new int[ len ];
	}
	// init dist[][] 
	// INT_MAX/2 is used to avoid overflow
	// i==j with 0 , otherwise with INT_MAX/2
	for(int i=0;i<nodeList.size();i++){
		for( int j=0;j<nodeList.size();j++){
			if( i==j )
				this->distanceMatrix[i][j] = 0;
			else	
				this->distanceMatrix[i][j] = 32767; // INT_MAX
		}
			
	}

	for( int i=0 ; i<linkList.size(); i++){
		EdgeNode node1 = linkList[i].nodeSet[0];
		EdgeNode node2 = linkList[i].nodeSet[1];
		int n1 = findEdgeNodeIndex( node1 );
		int n2 = findEdgeNodeIndex( node2 );
		// find index of node1 and node2 
		if( n1 != -1 && n2 != -1){
			distanceMatrix[n1][n2] = linkList[i].delay;
			distanceMatrix[n2][n1] = linkList[i].delay; 
		}
	}
}

void Topology::claenUpDistanceMatrix(){
	for( int i=0 ; i< this->nodeList.size() ; i++){
		delete this->distanceMatrix[i];
	}
	delete [] this->distanceMatrix;
}

void Topology::printDistanceMatrix(){
	cout << "=======================Latency Matrix=======================" << endl;
	
	printf("           ");
	for(int i=0;i<nodeList.size();i++){
		printf("%10s ", nodeList[i].nodeName.c_str());
	}
	cout << endl;
	for(int i=0;i<nodeList.size();i++){
		for( int j=0;j<nodeList.size();j++){
			if( j == 0 ){
				printf("%10s ", nodeList[i].nodeName.c_str());
			}
			printf("%10d ", distanceMatrix[i][j]);
		}
			
		cout << endl;
	}
}

int Topology::findEdgeNodeIndex(EdgeNode node){
	for( int i=0;i<this->nodeList.size();i++){
		if( nodeList[i].nodeName.compare( node.nodeName) == 0 )
			return i;
	}
	return -1;
}

// find all pair shortest path
void Topology::floydWarshallAlgo(){
	int len = this->nodeList.size();
	for( int k=0; k < len; k++ ){
		for( int i = 0; i < len ; i++ ){
			for( int j=0 ; j < len ; j++ ){
				if( distanceMatrix[i][k]+ distanceMatrix[k][j] < distanceMatrix[i][j] )
					distanceMatrix[i][j] = distanceMatrix[i][k]+ distanceMatrix[k][j];
			}
		}
	}
}