/**
 *  ParticleManager.h
 *  
 *  
 */

#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include "headers.h"

#include <vector>

#include "SubatomicParticle.h"
#include "ParticleSystem.h"

class ParticleManager {

public:

  ParticleManager();
  ~ParticleManager();

  void setShader(GLint shaderHandle);

  void update(float timePassed);
  void draw ();

  void manageSystem (ParticleSystem* system);
  
  void detachParticle (SubatomicParticle* p, ParticleSystem* ps);
  void attachParticle (SubatomicParticle* p, ParticleSystem* ps);
  
  void acquireStrayParticlesInBox (float x1, float y1, float x2, float y2, ParticleSystem* receiver);

  
private:
  std::set<SubatomicParticle*> strayParticles;
  std::set<ParticleSystem*> particleSystems;

  GLint velShader;

};

#endif // PARICLEMANAGER_H
