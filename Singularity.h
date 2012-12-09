/**
 *  Singularity.h
 *  
 *  
 */

#ifndef SINGULARITY_H
#define SINGULARITY_H

#include "headers.h"

#include "ParticleSystem.h"
#include "ParticleManager.h"

class Singularity {

public:

  Singularity( Point* location );
  ~Singularity();

  void update (float timePassed);
  void draw ();

  Point* loc;
  Vector vel;
  float maxVel;

  ParticleSystem system;
  ParticleManager* manager;
  
private:

};

#endif
