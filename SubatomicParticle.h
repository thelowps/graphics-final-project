/**
 *  SubatomicParticle.h
 *  
 *  
 */

#ifndef SUBATOMICPARTICLE_H
#define SUBATOMICPARTICLE_H

#include <CSE40166/CSE40166.h>
using namespace CSE40166;

class SubatomicParticle {

public:
  SubatomicParticle();
  ~SubatomicParticle();

  void update (float timePassed);
  void draw ();
  
  Point loc;
  Vector vel;

  float radius;

  std::map<char, double> data; // for values we want to attach on the fly

private:
  
  
};

#endif
