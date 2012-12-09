/**
 *  ParticleSystem.h
 *  
 *  
 */

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "headers.h"

#include <set>

#include "SubatomicParticle.h"

class ParticleSystem {

public:

  ParticleSystem();
  ~ParticleSystem();

  void update (float timePassed);
  void draw ();
  void createParticle (int amt = 1);

  void receiveParticle (SubatomicParticle* p);
  void relinquishParticle (SubatomicParticle* p);

  Point loc;
  float radius;

  float springConstant;
  float repelConstant;

  GLint velShader;
  GLint celShader;

  std::set<SubatomicParticle*> particles;

private:

  
  
};

#endif // PARTICLESYSTEM_H
