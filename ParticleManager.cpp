/**
 *  ParticleManager.cpp
 *  
 *  
 */

#include "ParticleManager.h"

using namespace std;

ParticleManager::ParticleManager () {

  velShader = 0;

  SubatomicParticle* test = new SubatomicParticle;
  test->loc = Point(1,1,1);
  strayParticles.insert(test);
  
}

ParticleManager::~ParticleManager () {}

// Sets the shader
void ParticleManager::setShader (GLint handle) {
  velShader = handle;
  for (set<ParticleSystem*>::iterator jt = particleSystems.begin(), jend = particleSystems.end();
       jt != jend; ++jt) {
    (*jt)->velShader = handle;
  }
}

void ParticleManager::update (float timePassed) {

  // pass the update call to all particles and particle systems
  for (set<SubatomicParticle*>::iterator it = strayParticles.begin(), end = strayParticles.end();
       it != end; ++it) {
    // collide every stray particle with every attached particle
    for (set<ParticleSystem*>::iterator jt = particleSystems.begin(), jend = particleSystems.end();
	 jt != jend; ++jt) {
      //collide 
    }
    
    (*it)->update(timePassed); // moves all the stray particles
  }

  for (set<ParticleSystem*>::iterator it = particleSystems.begin(), end = particleSystems.end();
       it != end; ++it) {
    // Collide each particle system with each other
    for (set<ParticleSystem*>::iterator jt = particleSystems.begin(), jend = particleSystems.end();
	 jt != jend; ++jt) {
      //collide 
      
    }

    (*it)->update(timePassed);
  }


}

// Draws every particle system and stray particle
void ParticleManager::draw () {
  // pass the draw call to all particles and particle systems
  //if (velShader) glUseProgram(velShader);
  for (set<SubatomicParticle*>::iterator it = strayParticles.begin(), end = strayParticles.end();
       it != end; ++it) {
    (*it)->draw(); // moves all the stray particles
    if (velShader) {
      //GLint c = glGetUniformLocation(velShader, "center");
      //GLint v = glGetUniformLocation(velShader, "velocity");
      //glUniform3f((*it)->loc.x, (*it)->loc.y, (*it)->loc.z, c);
      //glUniform3f((*it)->vel.x, (*it)->vel.y, (*it)->vel.z, v);
    }    

  }
  //if (velShader) glUseProgram(0);

  for (set<ParticleSystem*>::iterator it = particleSystems.begin(), end = particleSystems.end();
       it != end; ++it) {    
    (*it)->draw();
  }

}

// attaches a particle system to this manager for managing
void ParticleManager::manageSystem (ParticleSystem* system) {
  particleSystems.insert(system);
  system->velShader = velShader;
}

// detach a particle from its owner and make it a stray 
void ParticleManager::detachParticle (SubatomicParticle* p, ParticleSystem* ps) {
  ps->particles.erase(p);
  strayParticles.insert(p);
}

// Attach a particle to a certain system
void ParticleManager::attachParticle (SubatomicParticle* p, ParticleSystem* ps) {
  ps->particles.insert(p);
  strayParticles.erase(p);
}

// Some helper functions
int pointInBox (Point pt, float x1, float y1, float x2, float y2) {
  if (!(pt.x < x1) != !(pt.x < x2)) {
    if (!(pt.y < y1) != !(pt.y < y2)) {
      return 1;
    }
  }
  return 0;
}

void ParticleManager::acquireStrayParticlesInBox (float x1, float y1, float x2, float y2, ParticleSystem* receiver) {

  GLdouble model_view[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
  GLdouble projection[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  GLdouble winX, winY, winZ;
   
  for (set<SubatomicParticle*>::iterator it = strayParticles.begin(), end = strayParticles.end();
       it != end; ) {
    gluProject((*it)->loc.x, (*it)->loc.y, (*it)->loc.z,
	       model_view, projection, viewport,
	       &winX, &winY, &winZ);
    Point winPt(winX, winY, winZ);
    if (pointInBox(winPt, x1, y1, x2, y2)) {
      receiver->receiveParticle( (*it) );
      strayParticles.erase(it++);
    } else {
      ++it;
    }
  }

  glPushMatrix(); {
    
  } glPopMatrix();

}
