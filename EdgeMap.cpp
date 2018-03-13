#include "EdgeMap.h"

EdgeMap::EdgeMap(){}

EdgeMap::EdgeMap(int l, int w, Topology topology, int population){
    this->length = l;
    this->wedth = w;
    this->topo = topology;
    this->population = population;
    randomNumber.assign( topo.nodeList.size(), 0 );
}

EdgeMap::EdgeMap(int l, int w, Topology topology){
    this->length = l;
    this->wedth = w;
    this->topo = topology;
    totalDelay = 0;
    randomNumber.assign( topo.nodeList.size(), 0 );
}

void EdgeMap::requestGeneration(string s , int numberOfReq ){
    VNF vnf = getVNFByID( s );
    int nodeSize = this->topo.nodeList.size();   
    vector<EdgeNode>& nodeList = this->topo.nodeList;
    int sum = numberOfReq;
    int subSum = sum;

    srand (time(NULL));

    for(int i=0;i< nodeSize-1 ;i++){
        if( subSum < sum/2 )
            randomNumber[i] = rand()%subSum;
        else
            randomNumber[i] = rand()%sum/2;
        subSum = subSum - randomNumber[i];
    }
    randomNumber[nodeSize-1] = subSum;

    /*for( int i=0;i<nodeSize;i++){
        cout << "Node " << i+1 << " Req : " << randomNumber[i] << endl;
    }*/

    for( int i=0 ; i < nodeSize ; i++){
        nodeList[i].addReqTable( s, randomNumber[i] );
    }
}

void EdgeMap::addVNFsIntoTable(VNF vnf, int n){
    this->vnfTable[ vnf ] += n;
}

void EdgeMap::printVNFTable(){
    for( map<VNF,int>::iterator it = vnfTable.begin();it!=vnfTable.end();it++){
        cout << it->first.typeID << " " << it->second << endl;
    }
}

VNF EdgeMap::getVNFByID( string s ){
    for( map<VNF,int>::iterator it = vnfTable.begin();it!=vnfTable.end();it++){
        if( it->first.typeID.compare(s) == 0 ){
            return it->first;
        }
    }
    return VNF("none",0,0);
}


int EdgeMap::calculateDelay(){
    cout << "calculateDelay : " << endl;
    vector<EdgeNode>& nodeList = this->topo.nodeList;
    // local req handling
    localReqHandling();
    // debug
    //printReqInNode();
    // for each vnfType to calculate totalDelay
    for( map<VNF,int>::iterator it = this->vnfTable.begin() ; it!=this->vnfTable.end(); it++){
        string vType = it->first.typeID;
        for( int i=0; i< nodeList.size() ; i++ ){
            EdgeNode& srcNode = nodeList[i];
            map<string, int>& remainReqFront = srcNode.remainReqTable;
            if( remainReqFront[vType] > 0 ){
                int avaNodeIdx = findAvaliableMinDelayNode( srcNode, vType );
                int m = min( remainReqFront[vType], nodeList[avaNodeIdx].remainVNFReq[vType]);
                printOffloadMsg( srcNode.nodeName, nodeList[avaNodeIdx].nodeName
                        , vType, m
                        ,this->topo.distanceMatrix[i][avaNodeIdx]
                    );
                this->totalDelay += m * this->topo.distanceMatrix[i][avaNodeIdx];
                remainReqFront[vType] -= m;
                nodeList[avaNodeIdx].remainVNFReq[vType] -= m;    
            }
            if( remainReqFront[vType] > 0 ){
                i=-1;
            }
        }
    }
    
    return totalDelay;
}

// find an shortest delay edgeNode of srcNode, and it own remainingReqs to handle 
int EdgeMap::findAvaliableMinDelayNode(EdgeNode& srcNode, string vnfType){
    //cout << "findAvaliableMinDelayNode : " << srcNode.nodeName <<endl;
    int dist[ this->topo.nodeList.size() ];
    int idx = this->topo.findEdgeNodeIndex(srcNode);

    // mapping table <nodeIdx, dist > 
    vector< pair< int, int> > nodeToDist;
    memcpy( dist, this->topo.distanceMatrix[idx], this->topo.nodeList.size() * sizeof(int));
    
    for( int i=0 ; i < this->topo.nodeList.size() ; i++){
        //nodeToDist.push_back( make_pair(this->topo.nodeList[i].nodeName, dist[i]) );
        nodeToDist.push_back( make_pair(i, dist[i]) );
    }
    sort( nodeToDist.begin(), nodeToDist.end(), this->sortbysec);

    // ascending sort : 1 3 5
    /*for( int i=0 ; i < nodeToDist.size() ; i++){
        //cout << "("<<nodeToDist[i].first << ", " << nodeToDist[i].second << ") ";
        cout << "(" << this->topo.nodeList[ nodeToDist[i].first ].nodeName ;
        cout << ", " << nodeToDist[i].second << ") ";
    }
    cout << endl;*/

    for( int i=0 ; i < nodeToDist.size() ; i++){
        // delay must be greater than 0
        int nodeIdx = nodeToDist[i].first;
        if( nodeToDist[i].second > 0 && this->topo.nodeList[nodeIdx].remainVNFReq[vnfType] > 0 ){
            return nodeIdx;
        }
        /*if( nodeToDist[i].second > 0 )
            return nodeIdx;*/
    }
    return -1;
}

void EdgeMap::printReqInNode(){
    for( int i=0 ; i< topo.nodeList.size(); i++){
        EdgeNode& tmp = topo.nodeList[i];
        cout << "the remaining reqs in the nodes : " << tmp.nodeName << endl;
        for( map<string,int >::iterator it = tmp.remainReqTable.begin();it!=tmp.remainReqTable.end();it++){
		    string vnfType = it->first;
            if( tmp.remainVNFReq[vnfType] > 0 || it->second > 0 ){
                cout << vnfType << " : " << it->second << "/" << tmp.remainVNFReq[vnfType];
                cout << endl;
            }       
        }
    }
}

void EdgeMap::printNodeCapReq(){
    printf("=======================printNodeCapReq======================\n");
    printf("%10s ","   ");
    for( int i=0; i< topo.nodeList.size() ;i++){
        printf("%10s ",topo.nodeList[i].nodeName.c_str()); 
    }
    printf("\n");
    printf("%10s ","Capacity");
    for( int i=0; i< topo.nodeList.size() ;i++){
        printf("%10d ",topo.nodeList[i].remainInfrasCapacity); 
    }
    printf("\n");
    for( map<VNF, int> ::iterator it = vnfTable.begin(); it!=vnfTable.end();it++){
        string vType = it->first.typeID;
        printf("Req %6s ",vType.c_str());
        for( int i=0; i< topo.nodeList.size() ;i++){
            printf("%10d ",topo.nodeList[i].remainReqTable[vType]); 
        }
        printf("\n");
    }
    
}

void EdgeMap::printOffloadMsg(string src, string dst, string vType, int n, int delay){
    cout << "OffloadMsg " ;
    cout <<"[" << vType <<"] ";
    cout << src << " => "<< dst << " : " << n << " * " << delay << endl ;
}

/*
/ for each VNF_Type
/      for each node
/          handling Req localy
*/
void EdgeMap::localReqHandling(){
    vector<EdgeNode> &nodeList = topo.nodeList; 
    for( map<VNF,int>::iterator it = this->vnfTable.begin() ; it!=this->vnfTable.end(); it++){
        string vType = it->first.typeID;
        for( int i=0 ; i< nodeList.size(); i++){
            EdgeNode& tmp = nodeList[i];
            for( map<string,int >::iterator it = tmp.reqTable.begin();it!=tmp.reqTable.end();it++){
                int m = min( tmp.remainReqTable[ vType ], tmp.remainVNFReq[ vType ]);
                tmp.remainReqTable[vType] -= m;
                tmp.remainVNFReq[vType] -= m;
            }
        }
    }

}

void EdgeMap::greedyPlacement( VNF vnf ){
    // map< nodeIdx, the number of VNF needed >
    vector< pair< int, double> > nodeToAmount;
    
    // map< nodeName, the number of VNF placed >
    map<int,int> pResult;
    string vType = vnf.typeID;
    int subSum = vnfTable[vnf];
    vector<EdgeNode>& nodeList = this->topo.nodeList;

    for( int i=0; i< nodeList.size() ; i++ ){
        nodeToAmount.push_back( 
            make_pair( i 
            , (double)nodeList[i].remainReqTable[ vType ]/(vnf.maxRequest)) 
            );
    }

    sort( nodeToAmount.begin(), nodeToAmount.end(), sortByDouble);

    for( int i = 0 ; i <nodeToAmount.size() ; i++){
        EdgeNode& tmp = nodeList[ nodeToAmount[i].first ];
        int m = min( (int)ceil(nodeToAmount[i].second)
                        , tmp.remainInfrasCapacity/vnf.capacity
                    );
        m = min( m, subSum );
        subSum -= m;
        pResult[ nodeToAmount[i].first ] = m;
    }

    for( map<int,int>::iterator it=pResult.begin();it!=pResult.end();it++){
        nodeList[it->first].addVNFs( vnf, it->second);
    }

    for( int i = 0 ; i < nodeList.size() ; i++){
        EdgeNode& tmp = nodeList[ i ];
        if( subSum > 0 && tmp.remainInfrasCapacity > 0 ){
            int m = tmp.remainInfrasCapacity/vnf.capacity;
            m = min( m, subSum );
            subSum -= m;
            tmp.addVNFs( vnf , m );
        }
    }


}

void EdgeMap::printPlacementResult(){
    printf("=======================PlacementResult======================\n");
    
    vector<EdgeNode> &nodeList = this->topo.nodeList;
    printf("%10s ","");
    for( int i=0; i< nodeList.size() ; i++ ){
        printf("%10s ", nodeList[i].nodeName.c_str());
    }
    cout << endl;

    for( map<VNF, int> ::iterator it = vnfTable.begin(); it!=vnfTable.end();it++){
        string vType = it->first.typeID;
        printf("%10s ",vType.c_str());
        for( int i=0; i< topo.nodeList.size() ;i++){
            printf("%10d ",topo.nodeList[i].vnfTable[it->first]); 
        }
        printf("\n");
    }
    cout << endl;
}