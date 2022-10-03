#include "octreegenerator.h"

void Generator::BoolOp(char op) {
	// Supervoxel (dessa vez global)
	voxel supervoxel = SuperVoxelGlobal();
	octree tree1, tree2, Tree;
	tree1.vox.push_back(supervoxel); 
	tree2.vox.push_back(supervoxel); 
	Tree.vox.push_back(supervoxel); // Nova tree

	// Nova voxeliza��o das 2 �rvores (dessa vez global)
	tree1 = GlobalTree(tree1, 0);
    tree2 = GlobalTree(tree2, 1);

	// Inicializa nova �rvore
	for (int i = 0; i < Tree.vox.size(); i++) {
		if (Tree.vox[i].height < limit && Tree.vox[i].classe == 1) {
			vector<voxel>* newvoxes = new vector<voxel>(8);
			newvoxes = Partioning(Tree.vox[i]);

			for (int j = 0; j < 8; j++) {
				if (newvoxes->at(j).height == limit) {
					newvoxes->at(j).classe = 0;
				}
				else {
					newvoxes->at(j).classe = 1;
				}
			}
			Tree.vox[i].newvoxes = newvoxes;
			Tree.vox.insert(Tree.vox.end(), newvoxes->begin(), newvoxes->end());
		}
	}

	// Opera��es
	for (int i = 1; i < Tree.vox.size(); i++) {
		voxel A, B;
		A.classe = 0; B.classe = 0;
		for (int j = 1; j < tree1.vox.size(); j++) {
			if (tree1.vox[j].height == Tree.vox[i].height && tree1.vox[j].point == Tree.vox[i].point) {
				A = tree1.vox[j];
			}
		}
		for (int j = 1; j < tree2.vox.size(); j++) {
			if (tree2.vox[j].height == Tree.vox[i].height && tree2.vox[j].point == Tree.vox[i].point) {
				B = tree2.vox[j];
			}
		}

		switch (op) {
		// Intersse��o
		case 'i':
			if (A.classe == 2 && B.classe == 2) {
				Tree.vox[i].classe = 2;
			}
			if (A.classe == 0 || B.classe == 0) {
				Tree.vox[i].classe = 0;
			}
			break;
         // Uni�o
		case 'u':
			if (A.classe == 2 || B.classe == 2) {
				Tree.vox[i].classe = 2;
			}
			break;
		// Subtra��o
		case 's':
			if (A.classe == 2 && B.classe == 2) {
				Tree.vox[i].classe = 0;
			}
			if (A.classe == 2 && B.classe == 0) {
				Tree.vox[i].classe = 2;
			}
			if (A.classe == 0 && B.classe == 2) {
				Tree.vox[i].classe = 0;
			}
			if (A.classe == 2 && B.classe == 1) {
				Tree.vox[i].classe = 1;
			}
			if (A.classe == 1 && B.classe == 2) {
				Tree.vox[i].classe = 0;
			}
			if (A.classe == 1 && B.classe == 1) {
				Tree.vox[i].classe = 1;
			}
			break;
		}
	 }

	octrees.clear();
	octrees.push_back(Tree);
}

