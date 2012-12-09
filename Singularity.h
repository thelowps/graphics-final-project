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

  Singularity( Point location = Point(0,0,0) );
  ~Singularity();

  void update (float timePassed);
  void draw ();

  Point loc;
  Vector vel;

  ParticleSystem system;
  ParticleManager* manager;
  
private:

};

#endif
