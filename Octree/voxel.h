#pragma once
#ifndef VOXEL
#define VOXEL
#include <vector>
#include "vectorops.h"
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

using namespace std;

struct voxel {
	point point;
	float len;
	int height;
	int classe;
	voxel* pai;
	vector<voxel>* newvoxes;
};

_inline vector<point> Voxel2Points(const voxel& v) {
	vector<point> V8;
	V8.push_back(v.point);
	V8.push_back({ v.point.x + v.len, v.point.y, v.point.z });
	V8.push_back({ v.point.x + v.len, v.point.y, v.point.z + v.len });
	V8.push_back({ v.point.x, v.point.y, v.point.z + v.len });
	V8.push_back({ v.point.x, v.point.y + v.len, v.point.z });
	V8.push_back({ v.point.x + v.len, v.point.y + v.len, v.point.z });
	V8.push_back({ v.point.x + v.len, v.point.y + v.len, v.point.z + v.len });
	V8.push_back({ v.point.x, v.point.y + v.len, v.point.z + v.len });
	return V8;
}

_inline bool Contido(const point& p, const voxel& v) {
	if (p.x > v.point.x && p.x < v.point.x + v.len &&
		p.y > v.point.y && p.y < v.point.y + v.len &&
		p.z > v.point.z && p.z < v.point.z + v.len) {
		return true;
	}

	return false;
}

_inline bool Inside(const voxel& A, const voxel& V) {
	if (Contido(A.point, V) == true) {
		if (A.point.x + A.len <= V.point.x + V.len &&
			A.point.y + A.len <= V.point.y + V.len &&
			A.point.z + A.len <= V.point.z + V.len) {
			return true;
		}
		else {
			return false;
		}
	}
	else return false;
}

_inline bool Equal(const voxel& A, const voxel& V) {
	if (A.point == V.point && A.len == V.len) {
		return true;
	} else {
		return false;
	}
}

_inline vector<voxel>* Partioning(const voxel& origem) {
	vector<voxel>* newvoxes = new vector<voxel>(8);

	for (int i = 0; i < 8; i++) {
		voxel newvox;
		newvox.height = origem.height + 1;
		newvox.len = origem.len / 2;
		newvox.point = origem.point;

		newvox.pai = new voxel;
		newvox.pai->height = origem.height;
		newvox.pai->len = origem.len;
		newvox.pai->newvoxes = origem.newvoxes;
		newvox.pai->pai = origem.pai;
		newvox.pai->classe = origem.classe;

		switch (i) {
		case 0: 
				newvox.point.z = newvox.point.z + newvox.len;
				break;
		case 1:
				newvox.point.z = newvox.point.z + newvox.len; 
				newvox.point.x = newvox.point.x + newvox.len;
				break;
		case 2:
				newvox.point.y = newvox.point.y + newvox.len;
				newvox.point.z = newvox.point.z + newvox.len;
				newvox.point.x = newvox.point.x + newvox.len;
				break;
		case 3:
				newvox.point.y = newvox.point.y + newvox.len;
				newvox.point.z = newvox.point.z + newvox.len;
				break;
		case 5:
				newvox.point.x = newvox.point.x + newvox.len;
				break;
		case 6:
				newvox.point.y = newvox.point.y + newvox.len;
				newvox.point.x = newvox.point.x + newvox.len;
				break;
		case 7:
				newvox.point.y = newvox.point.y + newvox.len;
				break;
		}

		newvoxes->at(i) = newvox;
	}

	return newvoxes;
}
#endif 
