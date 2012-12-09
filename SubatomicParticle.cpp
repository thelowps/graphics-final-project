/**
 *  SubatomicParticle.cpp
 *  
 *  
 */

#include "SubatomicParticle.h"

#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

// Constructor
SubatomicParticle::SubatomicParticle () {
  // initialize some default values
  loc = Point(0,0,0);
  vel = Vector(0,0,0);
  radius = 0.1;
}

// Destructor
SubatomicParticle::~SubatomicParticle () {}

// Move the particle based on its velocity
void SubatomicParticle::update (float timePassed) {
  loc += vel * timePassed;
}

// Draw the particle to the screen
void SubatomicParticle::draw () {
  glPushMatrix(); {
    glTranslatef(loc.x, loc.y, loc.z);
    glutSolidSphere(radius, 50, 50);
  } glPopMatrix();
}
