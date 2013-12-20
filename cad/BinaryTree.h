
#include <cstdio>
#include <list>
#include <string>

class Pos {
public:
	int x, y;
	Pos (int x, int y);
	Pos ();
};

class BinaryTree {
public:
	BinaryTree(unsigned int index = 0, BinaryTree *left = NULL,
			   BinaryTree *right = NULL);
	~BinaryTree();

	static std::list<BinaryTree*> parseAndLoadAAG(const char *filename);
	int parseAndLoadDEF(const char *filename);
	int parseAndLoadSPECS(const char *filename);

	void calculateResistances(BinaryTree *node, double outRes);

	// Access functions
	BinaryTree* getLeft();
	BinaryTree* getRight();
	unsigned int getIndex();

	std::string getName();
	void setName(std::string name);
	std::string getDirection();
	void setDirection(char *direction);
	char getOrientation();
	void setOrientation(char orientation);
	Pos getPosition();
	void setPosition(Pos pos);

	void setInverterInput(BinaryTree *node);

	void getInputRes(double *dest);
	void getInputCap(double *dest);
	void setOutputCap(double outCap);
	void setOutputRes(double outRes);

    void placement(const char *filename);

    int maxHeight(BinaryTree *p);

private:
	unsigned int i;
	BinaryTree *l; // left node
	BinaryTree *r; // right node

	// Interpreted values
	// These values are interpreted from SPECS file
	double *inRes;
	double *inCap;
	double outRes;
	double outCap;

	// This is the name imported from AAG file
	std::string name;

	// These are the positions imported from DEF file
	std::string direction;
	char orientation;
	Pos pos;

	// Private functions
	static BinaryTree *recBuildTree(int nodeIndex);


};
