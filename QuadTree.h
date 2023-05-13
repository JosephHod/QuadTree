#pragma once
#include <iostream>
#include <utility>
#include <memory>

using namespace std;

class Point {
public:
	Point(double _x, double _y) : x(_x), y(_y) {};
	double x, y;
	void printPoint() { std::cout << "(" << x << "," << y << ")" << std::endl; }
};


class LifeFoam {
	int attribute;
	Point location;

public:
	LifeFoam(Point _location, int _attribute) : location(_location), attribute(_attribute) {};
	Point getLocation() { return(location); }
	int getAttribute() { return(attribute); }
};

class Node {
private:
	const Point lowerLeftCorner, upperRightCorner;
	const shared_ptr<Node> parent;
	shared_ptr<Node> upperRightChildPtr = nullptr, upperLeftChildPtr = nullptr, lowerRightChildPtr = nullptr, lowerLeftChildPtr = nullptr;
	LifeFoam* objectPtr = nullptr;

	//private helper methods
	Node* findRegion(Point point);
	bool bIsLeaf() { return(upperRightChildPtr == nullptr);}
	bool bPointInRegion(Point point) { return(point.x >= lowerLeftCorner.x && point.x <= upperRightCorner.x && point.y >= lowerLeftCorner.y && point.y <= upperRightCorner.y); }
	Node* findSubRegion(Point point);
	void seperateRegion(Node* nodeToSeperate);
	void moveCurrObjectToSubRegion();
public:
	void insertObject(LifeFoam* objPtr);
	//void removeObject(LifeFoam& obj);
	Node(const Point _lowerLeftCorner, const Point _upperRightCorner, const shared_ptr<Node> _parent) : lowerLeftCorner(_lowerLeftCorner), upperRightCorner(_upperRightCorner), parent(_parent) {};

	//public helper methods
	std::pair<const Point, const Point> getNodeVerticies() { return(make_pair(lowerLeftCorner, upperRightCorner)); };
	bool bIsEmpty() { return(objectPtr == nullptr); }
	int getObjectVal() { return(objectPtr->getAttribute()); }

	void setObject(LifeFoam* _objectPtr) { objectPtr = _objectPtr; }
	void setChildren(Node _upperRightChild, Node _upperLeftChild, Node _lowerRightChild, Node _lowerLeftChild) {
		upperRightChildPtr = make_shared<Node>(_upperRightChild);
		upperLeftChildPtr  = make_shared<Node>(_upperLeftChild);
		lowerRightChildPtr = make_shared<Node>(_lowerRightChild);
		lowerLeftChildPtr  = make_shared<Node>(_lowerLeftChild);
	}

};	

class QuadTree {
private:
	Node root;
public:
	QuadTree() : root(Point(0, 0), Point(100, 100), nullptr) {};
	QuadTree(Point upperRightEdge, Point lowerLeftEdge) : root(lowerLeftEdge, upperRightEdge, nullptr) {};
};