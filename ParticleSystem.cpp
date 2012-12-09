/**
 *  ParticleSystem.cpp
 *  
 *  
 */

#include "ParticleSystem.h"
#include <cstdlib>
#include <cmath>

using namespace std;

// Constructor, sets some default values (mostly 0's)
ParticleSystem::ParticleSystem () {
  loc = Point(0,0,0);
  radius = 1;

  springConstant = 5.0;
  repelConstant = 0.1;
  dampConstant = 0.02;
  magneticField = Vector(0.1, 0.1, 7.0);

  springForcesOn = true;
  repelForcesOn = true;
  magneticForcesOn = true;

  speedLimit = 30;

  velShader = 0;
  celShader = 0;
}

ParticleSystem::~ParticleSystem () {}

// calls update on all the particles
void ParticleSystem::update (float timePassed) {
  // Update and apply forces to each particle
  for (set<SubatomicParticle*>::iterator it = particles.begin(), end = particles.end(); it != end; ++it) {
    
    // FORCES HERE //
    // Do we want to make this code more general? //
    
    if (repelForcesOn) {
      // Repel force //
      set<SubatomicParticle*>::iterator temp = it;
      for (set<SubatomicParticle*>::iterator jt = ++temp, end = particles.end(); jt != end; ++jt) {
	Vector dist = (*it)->loc - (*jt)->loc;
	Vector force = ( repelConstant / dist.magSq() ) * dist;
	(*it)->vel += force;
	(*jt)->vel -= force;
      }
    }    

    if (springForcesOn) {
      // spring force //
      Vector ptoc = (*it)->loc - loc;
      float f = springConstant * ((*it)->data['s'] - ptoc.mag());
      //if ( fabs(f) > 1.0 ) 
      Vector force = ptoc * f;
      (*it)->vel += force;
    }

    if (magneticForcesOn) {
      Vector ptoc = (*it)->loc - loc;
      // magnetic field force //
      Vector force = cross( magneticField, ptoc );
      float t = (force.magSq()-(*it)->vel.magSq())/force.magSq();
      if (t < 0) t = 0;
      force = force * t;
      (*it)->vel += force;
    }

    // dampen the movement //
    (*it)->vel -= (*it)->vel * dampConstant; 
    if ((*it)->vel.mag() > speedLimit) {
      (*it)->vel *= (speedLimit/(*it)->vel.mag());
    }

    // finally, update the particle
    (*it)->update(timePassed);
  }
}

// draws all the particles
void ParticleSystem::draw () {
  if (velShader) glUseProgram(velShader);

  for (set<SubatomicParticle*>::iterator it = particles.begin(), end = particles.end(); it != end; ++it) {
    if (velShader) {
      GLint c = glGetUniformLocation(velShader, "center");
      GLint v = glGetUniformLocation(velShader, "velocity");
      glUniform3f((*it)->loc.x, (*it)->loc.y, (*it)->loc.z, c);
      glUniform3f((*it)->vel.x, (*it)->vel.y, (*it)->vel.z, v);
    }
    (*it)->draw();
  }  

  if (velShader) glUseProgram(0);  

}

// creates a number of new particles, places them randomly within the radius
void ParticleSystem::createParticle (int amt) {
  for (int i = 0; i < amt; ++i) {
    SubatomicParticle* part = new SubatomicParticle;
    float x = (((float)(rand()%1001)/500)-1) * (radius/4) + loc.x;
    float y = (((float)(rand()%1001)/500)-1) * (radius/4) + loc.y;
    float z = (((float)(rand()%1001)/500)-1) * (radius/4) + loc.z;
    part->loc = Point(x, y, z);

    initializeParticle(part);
    particles.insert( part );
  }
}

// Initialize a particle's values
void ParticleSystem::initializeParticle (SubatomicParticle* p) {
  float springDist = ((float)(rand()%1001)/1000) * (radius/4);
  p->data['s'] = springDist;  
}

// places a particle under this system's control
void ParticleSystem::receiveParticle (SubatomicParticle* p) {
  initializeParticle(p);
  particles.insert(p);
}

// Remove a particle from this system's control
void ParticleSystem::relinquishParticle (SubatomicParticle* p) {
  particles.erase(p);
}
