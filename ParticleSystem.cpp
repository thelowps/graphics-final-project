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

  springConstant = 1;

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

    // Repel force //
    //for (set<SubatomicParticle*>::iterator jt = particles.begin(), end = particles.end(); jt != end; ++jt) {
      
    //}
    
    // spring force //
    float x_dist = (*it)->loc.x - loc.x;
    float y_dist = (*it)->loc.y - loc.y;
    //float z_dist = loc.z - (*it)->loc.z;
    float ptoc = (x_dist*x_dist + y_dist*y_dist);
    //if (ptoc < 1) ptoc = 1;
    float force = springConstant * ((*it)->data['s'] - ptoc);
    float phi = atan2f(y_dist, x_dist);
    float x_force = force*cos(phi);
    float y_force = force*sin(phi);
    Vector F (x_force, y_force, 0.0);
    (*it)->vel += F;

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
    float x = (((float)(rand()%1001)/500)-1) * radius + loc.x;
    float y = (((float)(rand()%1001)/500)-1) * radius + loc.y;
    float z = (((float)(rand()%1001)/500)-1) * radius + loc.z;
    part->loc = Point(x, y, z);
    float springDist = ((float)(rand()%1001)/1000) * radius;
    part->data['s'] = springDist;
    particles.insert( part );
  }
}

// places a particle under this system's control
void ParticleSystem::receiveParticle (SubatomicParticle* p) {
  particles.insert(p);
}

// Remove a particle from this system's control
void ParticleSystem::relinquishParticle (SubatomicParticle* p) {
  particles.erase(p);
}
