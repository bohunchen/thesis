#include "Link.h"

Link::Link()
{
}

Link::~Link()
{
}


Link::Link(EdgeNode node1, EdgeNode node2, int d) {
		nodeSet.push_back(node1);
		nodeSet.push_back(node2);
		this->delay = d;
}