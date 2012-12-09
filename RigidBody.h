/**
 *  RigidBody.h
 *  
 *  Defines a body that collides with others. A simple base class
 *  that really only defines a few functions and properties.
 */

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Geometry.h"

// we can expand this as we see fit to check against kinds of bodies
enum RIGID_BODY_TYPE { OTHER_TYPE, ENEMY, FLOOR, ROCK, HERO, FLAME, CRATE };

class RigidBody {

public:
  RigidBody(RIGID_BODY_TYPE t = OTHER_TYPE);
  RigidBody(Point p, double w, double h, double d, RIGID_BODY_TYPE t = OTHER_TYPE);
  RigidBody(Point c, double r, RIGID_BODY_TYPE t = OTHER_TYPE); // for spheres
  ~RigidBody();

  virtual void draw(Point cameraLoc);
  virtual void collide(const RigidBody* other);
  virtual void update(float elapsedTime);
  virtual void applyForce(Vector force, float elapsedTime);

  RIGID_BODY_TYPE getType() const;
  const Shape* getShape() const;

  Point loc;
  Vector vel;  
  
protected:
  virtual void applyVelocity(float elapsedTime);

  RIGID_BODY_TYPE type;

  SHAPE_TYPE shapeType;
  Sphere collisionSphere;
  Box collisionBox;

};

short handleCollision( RigidBody* a, RigidBody* b );
Vector multiplyVectorByMatrix(Vector v, GLfloat m[16]);

#endif
