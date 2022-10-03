#pragma once

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <Windows.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "vectorops.h"
#include "octreegenerator.h"

struct Camera {
	point eye, lookat, up;

	Camera(const point& _position, const point& _view, const point& _up) : eye(_position), lookat(_view), up(_up) {
		up.norm();
	}
};

Camera cam = Camera({ 2.0f, 5.0f, 10.0f }, { 0.0f, 3.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
GLfloat light[4] = { 10.0f, 10.0f, 10.0f, 1.0f };

Generator Gen(false);

void loaderoctree(Generator& _Gen) {
	if (_Gen.see == false)
		return;

	//  Desenha cada voxel preto (2) de cada octree 
	for (int i = 0; i < _Gen.octrees.size(); i++) {
		for (int j = 0; j < _Gen.octrees[i].vox.size(); j++) {
			voxel vox = _Gen.octrees[i].vox[j];

			if (vox.classe == 2) {
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				glBegin(GL_POLYGON);
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glVertex3f(vox.point.x, vox.point.y, vox.point.z);
				glVertex3f(vox.point.x, vox.point.y, vox.point.z + vox.len);
				glVertex3f(vox.point.x, vox.point.y + vox.len, vox.point.z + vox.len);
				glVertex3f(vox.point.x, vox.point.y + vox.len, vox.point.z);
				glEnd();
				glBegin(GL_POLYGON);
				glNormal3f(1.0f, 0.0f, 0.0f);
				glVertex3f(vox.point.x + vox.len, vox.point.y + vox.len, vox.point.z);
				glVertex3f(vox.point.x + vox.len, vox.point.y + vox.len, vox.point.z + vox.len);
				glVertex3f(vox.point.x + vox.len, vox.point.y, vox.point.z + vox.len);
				glVertex3f(vox.point.x + vox.len, vox.point.y, vox.point.z);
				glEnd();
				glBegin(GL_POLYGON);
				glNormal3f(0.0f, 0.0f, 1.0f);
				glVertex3f(vox.point.x, vox.point.y, vox.point.z + vox.len);
				glVertex3f(vox.point.x + vox.len, vox.point.y, vox.point.z + vox.len);
				glVertex3f(vox.point.x + vox.len, vox.point.y + vox.len, vox.point.z + vox.len);
				glVertex3f(vox.point.x, vox.point.y + vox.len, vox.point.z + vox.len);
				glEnd();
				glBegin(GL_POLYGON);
				glNormal3f(0.0f, 0.0f, -1.0f);
				glVertex3f(vox.point.x, vox.point.y, vox.point.z);
				glVertex3f(vox.point.x, vox.point.y + vox.len, vox.point.z);
				glVertex3f(vox.point.x + vox.len, vox.point.y + vox.len, vox.point.z);
				glVertex3f(vox.point.x + vox.len, vox.point.y, vox.point.z);
				glEnd();
				glBegin(GL_POLYGON);
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(vox.point.x, vox.point.y + vox.len, vox.point.z);
				glVertex3f(vox.point.x, vox.point.y + vox.len, vox.point.z + vox.len);
				glVertex3f(vox.point.x + vox.len, vox.point.y + vox.len, vox.point.z + vox.len);
				glVertex3f(vox.point.x + vox.len, vox.point.y + vox.len, vox.point.z);
				glEnd();
				glBegin(GL_POLYGON);
				glNormal3f(0.0f, -1.0f, 0.0f);
				glVertex3f(vox.point.x, vox.point.y, vox.point.z);
				glVertex3f(vox.point.x + vox.len, vox.point.y, vox.point.z);
				glVertex3f(vox.point.x + vox.len, vox.point.y, vox.point.z + vox.len);
				glVertex3f(vox.point.x, vox.point.y, vox.point.z + vox.len);
				glEnd();
			}
		}
	}
}

void disp(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z, cam.lookat.x, cam.lookat.y, cam.lookat.z, cam.up.x, cam.up.y, cam.up.z);

	loaderoctree(Gen);

	glPushMatrix();
	glTranslatef(light[0], light[1], light[2]);
	glutWireSphere(0.2, 20, 16); 
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 4.0f, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'E':
	case 'e':
		Gen.PrimEsfera();
		Gen.see = true;
		break;
	case 'C':
	case 'c':
		Gen.PrimCilin();
		Gen.see = true;
		break;
	case 'B':
	case 'b':
		Gen.PrimCube(); 
		Gen.see = true;
		break;
	case 'F':
	case 'f':
		Gen.PrimCone();
		Gen.see = true;
		break;
	case 'U':
	case 'u':
		if (Gen.see == true && Gen.octrees.size() >= 2 && Gen.octrees.size() < 3)
			Gen.BoolOp('u');
		break;
	case 'I':
	case 'i':
		if (Gen.see == true && Gen.octrees.size() >= 2 && Gen.octrees.size() < 3)
			Gen.BoolOp('i');
		break;
	case 'S':
	case 's':
		if (Gen.see == true && Gen.octrees.size() >= 2 && Gen.octrees.size() < 3)
			Gen.BoolOp('s');
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glLightfv(GL_LIGHT0, GL_POSITION, light);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}