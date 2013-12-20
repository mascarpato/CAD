
#include "BinaryTree.h"

using namespace std;

int main(int argc, char **argv) {
	list<BinaryTree*> rootNodes;
	if (argc != 2) {
		printf("Usage: ./parser <common name for AAG, DEF and SPECS>\nExiting...\n");
		return 0;
	}

	string filename = argv[1];
	filename += ".aag";
	rootNodes = BinaryTree::parseAndLoadAAG(filename.c_str());
	if (rootNodes.empty()) {
		printf("Problems during AAG file parsing\n");
		return 0;
	}

	list<BinaryTree*>::iterator nodesIt;
	int defRetCode = 0;
	for (nodesIt = rootNodes.begin(); nodesIt != rootNodes.end(); nodesIt++) {
		filename = argv[1];
		filename += ".def";
		if ((defRetCode = (*nodesIt)->parseAndLoadDEF(filename.c_str())) != 1) {
			printf("Problems during DEF file parsing, error %d\n", defRetCode);
			return 0;
		}
		filename = argv[1];
		filename += ".specs";
		int specsRetCode;
		if ((specsRetCode = (*nodesIt)->parseAndLoadSPECS(filename.c_str())) != 1) {
			printf("Problems during SPECS file parsing, error %d\n", specsRetCode);
			return 0;
		}
	}

	return 0;
}
