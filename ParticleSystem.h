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

  bool springForcesOn;
  bool repelForcesOn;
  bool magneticForcesOn;

  float springConstant;
  float repelConstant;
  float dampConstant;
  Vector magneticField;

  float speedLimit;

  GLint velShader;
  GLint celShader;

  std::set<SubatomicParticle*> particles;

private:
  
  void initializeParticle (SubatomicParticle* p);
};

#endif // PARTICLESYSTEM_H
