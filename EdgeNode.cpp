#include "EdgeNode.h"

using namespace std;

EdgeNode::EdgeNode()
{
	this->x = this->y = -1;
	this->nodeName = "null";
}

EdgeNode::EdgeNode(string name, int x, int y, int infrasCapacity)
{
	this->nodeName = name;
	this->x = x;
	this->y = y;
	this->infrasCapacity = infrasCapacity;
	this->remainInfrasCapacity = infrasCapacity;
}

EdgeNode::~EdgeNode(){}

void EdgeNode::setLocation(int x, int y) {
	this->x = x;
	this->y = y;
}
void EdgeNode::setName(string s) {
	this->nodeName = s;
}
string EdgeNode::getName() {
	return this->nodeName;
}

bool EdgeNode::addVNFs(VNF vnf, int cnt){
	
	if( this->infrasCapacity >= vnf.capacity * cnt ){
		// insert vnf element
		if( vnfTable.count(vnf) == 0 )
			vnfTable[vnf] = cnt;
		else
			vnfTable[vnf] += cnt;
		// update vnf capacity and reqs that node can handle
		vnfToCapacity[ vnf.typeID ] += vnf.capacity * cnt ;
		vnfToReqs[ vnf.typeID] += vnf.maxRequest * cnt;
		// update the remainInfras Capacity
		remainInfrasCapacity -= vnf.capacity * cnt;
		// update the remain reqs that node can handle
		remainVNFReq[ vnf.typeID ] += vnf.maxRequest * cnt;
		return true;
	}
	else{
		return false;
	}
}

int EdgeNode::getVNFCapacity( string s ){
	return vnfToCapacity[s];
}

// update the reqTable and remainReqTable
void EdgeNode::addReqTable( string s, int reqNum){
	reqTable[s] += reqNum ;
	remainReqTable[s] += reqNum;
}

// print the "vnfType reqTable vnfToReqs
void EdgeNode::printReqTable(){
	//cout << "printReqTable : " << endl;
	for( map<string,int >::iterator it = reqTable.begin();it!=reqTable.end();it++){
		cout << it->first << " " << it->second;
		cout << "/" << vnfToReqs[ it->first ] << endl;
	}
}