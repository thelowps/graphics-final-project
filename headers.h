#ifndef HEADERS_H
#define HEADERS_H

#include <GL/glew.h>

#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <SOIL/soil.h>

#include <CSE40166/CSE40166.h>
using namespace CSE40166;

#endif
