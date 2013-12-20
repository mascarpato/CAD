
#include <stdio.h>
#include <string.h>

#include "BinaryTree.h"

using namespace std;

int **andConnections;
list<int> inIndexes,outIndexes;
list<BinaryTree*> existingNodes;
list<BinaryTree*> outNodes;

list<string> inNames;
list<string>::iterator inNamesIt;
list<string> outNames;
list<string>::iterator outNamesIt;

BinaryTree* findNodeByName(char *name, BinaryTree *node);

Pos::Pos() {
	this->x = -1;
	this->y = -1;
}

Pos::Pos(int x, int y) {
	this->x = x;
	this->y = y;
}

BinaryTree::BinaryTree(unsigned int index, BinaryTree *left, BinaryTree *right) {
	this->i = index;
	this->l = left;
	this->r = right;

	this->inCap = new double[2];
	this->inRes = new double[2];
}

BinaryTree::~BinaryTree() {
}

std::list<BinaryTree*> BinaryTree::parseAndLoadAAG(const char *filename) {
	FILE *file=fopen(filename, "r");

	int numVars, numIn, numOut, numLatches, numAnd;

	// The first word can be ignored. The other ones are numVars, numIn, numL,
	//	numOut, numAnd
	printf("LOADING AAG FILE\nLoading headers\n");
	if (!fscanf(file, "aag %d %d %d %d %d", &numVars, &numIn, &numLatches,
				&numOut, &numAnd)) {
		printf("File is not an AAG file.\n");
		return outNodes;
	}

	int i;

	// The next numIn + numOut number are separated by \n, a single value per
	//	line
	printf("Loading inputs.\n");
	int buffer;
	for (i = 0; i < numIn; i++) {
		if (!fscanf(file, "%d", &buffer)) return outNodes;
		inIndexes.push_front(buffer);
	}

	printf("Loading outputs.\n");
	for (i = 0; i < numOut; i++) {
		if (!fscanf(file, "%d", &buffer)) return outNodes;
		outIndexes.push_front(buffer);
	}

	andConnections = new int*[numAnd];
	for (i = 0; i < numAnd; i++) andConnections[i] = new int[3];

	// Input/Output indexes loaded. Now loading AND connections
	printf("Loading AND connections.\n");
	for (i = 0; i < numAnd; i++) {
		if (!fscanf(file, "%d %d %d ", &andConnections[i][0], &andConnections[i][1],
				&andConnections[i][2])) return outNodes;
	}

	int unused;
	bool ignoreNames = false;
	// Now, some names for variables
	printf("Loading variable names.\n");

	char nameBuffer[128];
	for (i = 0; i < numIn && !ignoreNames; i++) {
		if (!fscanf(file, "i%d %s ", &unused, nameBuffer)) ignoreNames = true;
		else inNames.push_front(nameBuffer);
	}


	for (i = 0; i < numOut && !ignoreNames; i++) {
		if (!fscanf(file, "o%d %s ", &unused, nameBuffer)) ignoreNames = true;
		else outNames.push_front(nameBuffer);
	}
	if (ignoreNames) {
		printf("WARN: No valid names in AAG file.\n");
	}
	// From here onwards, only comments. We can close it now.
	fclose(file);

	// Now, we build the tree
	printf("AAG FILE LOADED SUCCESSFULLY\nBuilding tree\n");

	list<int>::iterator outIndexesIt;
	list<BinaryTree*>::iterator outNodesIt;

	outNamesIt = outNames.begin();
	inNamesIt = inNames.begin();

	outIndexesIt = outIndexes.begin();
	while (outIndexesIt != outIndexes.end()) {
		outNodes.push_front(recBuildTree(*outIndexesIt++));
		outNodesIt = outNodes.begin();
		if (!ignoreNames) (*outNodesIt)->name = *outNamesIt++;
	}
	printf("Tree built successfully\n");
	return outNodes;
}

int BinaryTree::parseAndLoadDEF(const char *filename) {
	FILE *inFile = fopen(filename, "r");
	if (!inFile) return -1;
	printf("LOADING DEF FILE\nLoading basic data\n");

	char unused[16];
	printf("Loading design\n");
	if (!fscanf(inFile, "DESIGN %s ", unused)) return -2;

	int area[2][2];
	printf("Loading die area\n");
	if (!fscanf(inFile, "DIEAREA ( %d %d ) ( %d %d ); ", &area[0][0],
		&area[0][1], &area[1][0], &area[1][1])) return -3;

	int nandArea[2];
	printf("Loading NAND area\n");
	if (!fscanf(inFile, "NANDAREA %d %d ; ", &nandArea[0], &nandArea[1]))
		return -4;

	int totalPins;
	printf("Loading total pins\n");
	if (!fscanf(inFile, " PINS %d ; ", &totalPins)) return -5;

	// parsedStrings: number of parsed elements by the string size
	char parsedStrings[4][32];
	int fixedPositions[2];

	list<BinaryTree*>::iterator outNodesIt;
	BinaryTree *node = NULL;

	for (int i = 0; i < totalPins; i++) {
		printf("Loading data for pin %d\n", i);
		// First, read from file
		if (!fscanf(inFile, " - %s  + NET %s  + DIRECTION %s + USE SIGNAL + LAYER ME3 ( -1000 0 ) ( 1000 600 ) + FIXED ( %d %d ) %c ; ",
			parsedStrings[0], parsedStrings[1], parsedStrings[2],
			&fixedPositions[0], &fixedPositions[1],	&parsedStrings[4][0])) {
			return -6;
		}
		// Maybe the variables have a different name?
		if (strncmp(parsedStrings[0], parsedStrings[1], 32) != 0)
			printf("WARN: Maybe some problem in DEF file %s, indicator %d?\n",
				filename, i);

		// Assigning names to the nodes. It is necessary to search the tree
		//	starting from all output nodes
		for (outNodesIt = outNodes.begin(); outNodesIt != outNodes.end();
			 outNodesIt++) {
			node = findNodeByName(parsedStrings[0], *outNodesIt);
			if (node) break;
		}

		if (!node) return -7;

		// Just found the node of name parsedStrings[i][0], now, to assign the
		//	values to it.
		node->setDirection(parsedStrings[2]);
		node->setOrientation(parsedStrings[4][0]);
		node->setPosition(Pos(fixedPositions[0], fixedPositions[1]));
	}
	printf("DEF FILE LOADED SUCCESSFULLY\n");
	return 1;
}

int BinaryTree::parseAndLoadSPECS(const char *filename) {
	printf(
"LOADING SPECS FILE\n");
	printf("Loading base options\n");
	FILE *inFile = fopen(filename, "r");
	if (!inFile) return -1;

	char resUnit[16];
	if (!fscanf(inFile, "resistance_unit %s ", resUnit)) return -2;

	char capUnit[16];
	if (!fscanf(inFile, "capacitance_unit %s ",capUnit)) return -3;

	char distUnit[16];
	if (!fscanf(inFile, "distance_unit %s ", distUnit)) return -4;

	struct Cell {
		char name[64];
		int width, heigth;
		float cap, res;
	};
	struct Cell cell[3];

	char unused[16];

	for (int i = 0; i < 3; i++) {
		printf("Loading cell %d data\n", i);
		if (!fscanf(inFile, "cell ( %s ) { ", (cell+i)->name)) return -5;
		if (!fscanf(inFile, " width %d ", &(cell+i)->width)) return -6;
		if (!fscanf(inFile, " height %d ", &(cell+i)->heigth)) return -7;
		if (!fscanf(inFile, " input_capacitance %f ", &(cell+i)->cap))
			return -8;
		if (!fscanf(inFile, " input_resistance %f ", &(cell+i)->res))
			return -9;
		fscanf(inFile, " } ");
	}

	struct Layer {
		char name[64];
		float width, res, cap;
	};
	struct Layer layer[3];

	for (int i = 0; i < 3; i++) {
		printf("Loading layer %d data\n", i);
		if (!fscanf(inFile, "LAYER %s ", (layer+i)->name)) return -10;
		if (!fscanf(inFile, " WIDTH %f ", &(layer+i)->width)) return -11;
		if (!fscanf(inFile, " RESISTANCE RPERSQ %f ", &(layer+i)->res))
			return -12;
		if (!fscanf(inFile, " CAPACITANCE CPERSQ %f ", &(layer+i)->cap))
			return -13;
		if (!fscanf(inFile, "END %s ", unused)) return -14;
	}
	printf("SPECS FILE LOADED SUCCESSFULLY\n");
	return 1;
}

void calculateResistances(BinaryTree *node, double outRes) {
	// First, deal with input nodes
	if ((node->getLeft() == NULL) && (node->getRight()) == NULL) {
		node->setOutputRes(outRes);
		return;
	}
	// Now, deal with nodes that have children
	node->setOutputRes(outRes);

	double *inRes = new double[2];
	node->getInputRes(inRes);

	if (node->getLeft()) calculateResistances(node->getLeft(), inRes[0]);
	if (node->getRight()) calculateResistances(node->getRight(), inRes[1]);

	delete inRes;
}

BinaryTree* BinaryTree::recBuildTree(int nodeIndex) {
	/*
	 * To build the tree, we do the following:
	 * - If any of the nodes' index is odd, we mark it as inverted and subtract
	 *		1 from it
	 * - We check if this node exists. If it exists, its value is simply put in
	 *		the child node. Else, recBuildTree is called to this node
	 */
	if (nodeIndex % 2) nodeIndex--;
	BinaryTree *newNode = new BinaryTree(nodeIndex);

	// Check if it needed to create child nodes
	list<int>::iterator inIt;
	for (inIt = inIndexes.begin(); inIt != inIndexes.end(); inIt++) {
		if (*inIt == nodeIndex) {
			newNode->setName(*inNamesIt);
			inNamesIt++;
			return newNode;
		}
	}

	// Check if child nodes exist
	int i;
	bool invLeft, invRight;
	for (i = 0; andConnections[i][0] != nodeIndex; i++);
	unsigned int leftIndex = andConnections[i][1], rightIndex = andConnections[i][2];
	if (leftIndex % 2) invLeft = true;
	if (rightIndex % 2) invRight = true;

	if (!existingNodes.empty()) {
		std::list<BinaryTree*>::iterator it;
		for (it = existingNodes.begin(); it != existingNodes.end(); it++) {
			if ((*it)->i == leftIndex) newNode->l = *it;
			if ((*it)->i == rightIndex) newNode->r = *it;
			if (newNode->l && newNode->r) break;
		}
	}
	BinaryTree *invInput;
	if (!newNode->l) {
		if (invLeft) {
			newNode->l = new BinaryTree(leftIndex);
			invInput = recBuildTree(leftIndex - 1);
			newNode->l->setInverterInput(invInput);
			existingNodes.push_front(newNode->l);
		} else {
			newNode->l = recBuildTree(leftIndex);
			existingNodes.push_front(newNode->l);
		}
	}
	if (!newNode->r) {
		if (!invRight) {
			newNode->r = new BinaryTree(leftIndex);
			invInput = recBuildTree(leftIndex - 1);
			newNode->r->setInverterInput(invInput);
			existingNodes.push_front(newNode->r);
		} else {
			newNode->r = recBuildTree(rightIndex);
			existingNodes.push_front(newNode->r);
		}
	}

	return newNode;
}

BinaryTree* findNodeByName(char * name, BinaryTree *node) {
	if (node) {
		if (node->getName() == name) return node;
		else {
			BinaryTree *leftNode = findNodeByName(name, node->getLeft());
			if (leftNode) return leftNode;
			return findNodeByName(name, node->getRight());
		}
	} else return NULL;
}

BinaryTree* BinaryTree::getLeft() {
	return this->l;
}

BinaryTree* BinaryTree::getRight() {
	return this->r;
}

unsigned int BinaryTree::getIndex() {
	return this->i;
}

string BinaryTree::getName() {
	return this->name;
}

void BinaryTree::setName(string name) {
	this->name = name;
}

string BinaryTree::getDirection() {
	return this->direction;
}

void BinaryTree::setDirection(char *direction) {
	this->direction = direction;
}

char BinaryTree::getOrientation() {
	return this->orientation;
}

void BinaryTree::setOrientation(char orientation) {
	this->orientation = orientation;
}

Pos BinaryTree::getPosition() {
	return this->pos;
}

void BinaryTree::setPosition(Pos pos) {
	this->pos = pos;
}

void BinaryTree::setInverterInput(BinaryTree *node) {
	this->l = node;
}

void BinaryTree::getInputCap(double *dest) {
	dest[0] = this->inCap[0];
	dest[1] = this->inCap[1];
}

void BinaryTree::getInputRes(double *dest) {
	dest[0] = this->inRes[0];
	dest[1] = this->inRes[1];
}

void BinaryTree::setOutputCap(double outCap) {
	if (outCap == 0) this->outCap = 0;
	else this->outCap += outCap;
}

void BinaryTree::setOutputRes(double outRes) {
	if (outRes == 0) this->outRes = 0;
	else this->outRes += outRes;
}
