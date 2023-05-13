#include <iostream>
#include <cassert>
#include "QuadTree.h"
using namespace std;

#define assertm(msg, exp)

Node* Node::findSubRegion(Point point) {
	assertm("out of bounds", bPointInRegion(x, y));

	Point refPoint = Point((lowerLeftCorner.x + upperRightCorner.x) / 2, (lowerLeftCorner.y + upperRightCorner.y) / 2);
	Node* subRegionPtr;
	if (point.x >= refPoint.x) {
		subRegionPtr = (point.y >= refPoint.y ? upperRightChildPtr.get() : lowerRightChildPtr.get());
	} else {
		subRegionPtr = (point.y >= refPoint.y ? upperLeftChildPtr.get() : lowerLeftChildPtr.get());
	}
	return(subRegionPtr);
}
Node* Node::findRegion(Point point) {
	assertm("out of bounds", bPointInRegion(point));
	if (bIsLeaf()) return(this);

	Node* subRegionPtr = findSubRegion(point);
	return(subRegionPtr->findRegion(point));
}

void Node::seperateRegion(Node* nodeToSeperate) {
	const std::pair<const Point, const Point> nodeToSeperateVerticies= nodeToSeperate->getNodeVerticies();
	const Point midVertex = Point((nodeToSeperateVerticies.first.x+ nodeToSeperateVerticies.second.x) / 2,
							      (nodeToSeperateVerticies.first.y + nodeToSeperateVerticies.second.y) / 2);

	shared_ptr<Node> pp(nodeToSeperate);
	Node upperRightSubRegion = Node(midVertex, nodeToSeperateVerticies.second, pp);
	Node upperLeftSubRegion = Node(Point(nodeToSeperateVerticies.first.x, midVertex.y), Point(midVertex.x, nodeToSeperateVerticies.second.y), pp);
	Node lowerRightSubRegion = Node(Point(midVertex.x, nodeToSeperateVerticies.first.y), Point(nodeToSeperateVerticies.second.x, midVertex.y), pp);
	Node lowerLeftSubRegion = Node(nodeToSeperateVerticies.first, midVertex, pp);
	
	nodeToSeperate->setChildren(upperRightSubRegion, upperLeftSubRegion, lowerRightSubRegion, lowerLeftSubRegion);

}

void Node::moveCurrObjectToSubRegion() {

	Node* subRegionPtrForCurrentObject = findSubRegion(objectPtr->getLocation());
	subRegionPtrForCurrentObject->setObject(objectPtr);
	objectPtr = nullptr;
}

void Node::insertObject(LifeFoam* objPtr) {
	Point point = objPtr->getLocation();
	Node* currRegionForPoint = findRegion(point);
	if (currRegionForPoint->bIsEmpty()) {
		currRegionForPoint->setObject(objPtr);
	}
	else {
		seperateRegion(currRegionForPoint);
		currRegionForPoint->moveCurrObjectToSubRegion();

		Node* subRegionPtr = findSubRegion(point);
		subRegionPtr->insertObject(objPtr);
	}

}