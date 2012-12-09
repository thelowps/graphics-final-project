/**
 *  Singularity.cpp
 *  
 *  
 */

#include "Singularity.h"

#include <iostream>
using namespace std;

Singularity::Singularity (Point* location) {
  manager = 0;
  loc = location;
  vel = Vector(0,0,0);
  maxVel = 0.1;

  system.radius = 1;
  system.loc = *loc;
  system.createParticle( 10 );
}

Singularity::~Singularity () {}

// Update the singularity's position
void Singularity::update (float timePassed) {  
  
  if (vel.mag() > maxVel) vel *= (maxVel/vel.mag());
  (*loc) += vel;
  
  system.loc = *loc;

  // update location
  if (!manager) { // only call update if there is no manager, which will do it for us
    system.update(timePassed);
  }
}

// Draw the singularity
void Singularity::draw () {
  if (!manager) {
    system.draw();
  }

  // draw the actual singularity
  glBegin(GL_QUADS); {
    glVertex3f(loc->x-0.1, loc->y-0.1, loc->z);
    glVertex3f(loc->x-0.1, loc->y+0.1, loc->z);
    glVertex3f(loc->x+0.1, loc->y+0.1, loc->z);
    glVertex3f(loc->x+0.1, loc->y-0.1, loc->z);
  } glEnd();
}
