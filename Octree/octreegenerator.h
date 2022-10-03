#pragma once
#ifndef OCTREE
#define OCTREE
#include <string>
#include <vector>
#include "vectorops.h"
#include "voxel.h"
#include <iostream>
#include <math.h>
#include <iterator>
#include <algorithm>
using namespace std;

struct octree {
	vector<voxel> vox;
};

class Generator {
public:
	vector<octree> octrees;
	bool see;
	int limit;

	Generator(bool s) {
		octrees.clear();
		see = s;
		limit = 5;
	}

	_inline voxel SuperVoxelGlobal() {
		voxel supervoxel;
		supervoxel.height = 0;
		supervoxel.classe = 1;
		supervoxel.pai = NULL;
		point pMin = octrees[0].vox[0].point;
		point pMax = pMin;

		for (int i = 0; i < 2; i++) {
			if (octrees[i].vox[0].point.x < pMin.x)
				pMin.x = octrees[i].vox[0].point.x;
			if (octrees[i].vox[0].point.y < pMin.y)
				pMin.y = octrees[i].vox[0].point.y;
			if (octrees[i].vox[0].point.z < pMin.z)
				pMin.z = octrees[i].vox[0].point.z;
			if (octrees[i].vox[0].point.x + octrees[i].vox[0].len > pMax.x)
				pMax.x = octrees[i].vox[0].point.x + octrees[i].vox[0].len;
			if (octrees[i].vox[0].point.y + octrees[i].vox[0].len > pMax.y)
				pMax.y = octrees[i].vox[0].point.y + octrees[i].vox[0].len;
			if (octrees[i].vox[0].point.z + octrees[i].vox[0].len > pMax.z)
				pMax.z = octrees[i].vox[0].point.z + octrees[i].vox[0].len;
		}

		supervoxel.len = pMax.x - pMin.x;
		if (pMax.y - pMin.y > supervoxel.len)
			supervoxel.len = pMax.y - pMin.y;
		if (pMax.z - pMin.z > supervoxel.len)
			supervoxel.len = pMax.z - pMin.z;

		supervoxel.point = pMin;
		return supervoxel;
	}	
	_inline octree GlobalTree(octree tree, int index) {
		 for (int i = 0; i < tree.vox.size(); i++) {
			 if (tree.vox[i].height < limit && tree.vox[i].classe == 1) {
				 vector<voxel>* newvoxes = new vector<voxel>(8);
				 newvoxes = Partioning(tree.vox[i]);
				 for (int j = 0; j < 8; j++) {
					 newvoxes->at(j).classe = 0;
					 for (int k = 1; k < octrees[index].vox.size(); k++) {
						 if (octrees[index].vox[k].classe == 2) {
							 if (Equal(newvoxes->at(j), octrees[index].vox[k]) == true ||
								 Inside(newvoxes->at(j), octrees[index].vox[k]) == true) {
								 newvoxes->at(j).classe = 2;
								 break;
							 } else {
								 if (Inside(octrees[index].vox[k], newvoxes->at(j)) == true) {
									 if (newvoxes->at(j).height == limit) {
										 newvoxes->at(j).classe = 2;
										 break;
									 } else {
										 newvoxes->at(j).classe = 1;
										 break;
									 }
								 } else {
									 vector<point> voxelVertices = Voxel2Points(octrees[index].vox[k]);
									 int nodeCount = 0;
									 for (int n = 0; n < 8; n++)
										 if (Contido(voxelVertices[n], newvoxes->at(j)) == true)
											 nodeCount++;
									 if (nodeCount > 0) {
										 if (newvoxes->at(j).height == limit) {
											 newvoxes->at(j).classe = 2;
											 break;
										 } else {
											 newvoxes->at(j).classe = 1;
											 break;
										 }
									 } else {
										 voxelVertices = Voxel2Points(newvoxes->at(j));
										 nodeCount = 0;
										 for (int n = 0; n < 8; n++)
											 if (Contido(voxelVertices[n], octrees[index].vox[k]) == true) {
												 nodeCount++;
											 }
										 if (nodeCount > 0) {
											 if (newvoxes->at(j).height == limit) { newvoxes->at(j).classe = 2; break; }
											 else {
												 newvoxes->at(j).classe = 1;
												 break;
											 }
										 }
									 }
								 }
							 }
						 }
					 }
				 }
				 tree.vox[i].newvoxes = newvoxes;
				 tree.vox.insert(tree.vox.end(), newvoxes->begin(), newvoxes->end());
			 }
		 }
		 return tree;
	 }
	 void BoolOp(char op);
	 void PrimEsfera();
	 void PrimCilin();
	 void PrimCube();
	 void PrimCone();
};

#endif 