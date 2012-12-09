/**
 *  RigidBody.cpp
 *  
 *  Defines a body that collides with others. A simple base class
 *  that really only defines a few functions and properties.
 */

#include "RigidBody.h"

RigidBody::RigidBody (RIGID_BODY_TYPE t) {
  type = t;
  shapeType = SPHERE;
  loc = Point(0,0,0);
  vel = Vector(0,0,0);
}

// Quick initialization of a plane
RigidBody::RigidBody (Point p, double w, double h, double d, RIGID_BODY_TYPE t) {
  collisionBox = Box(p, w, h, d);
  loc = p;
  shapeType = BOX;
  type = t;
  vel = Vector(0,0,0);
}

// Quick initialization of a sphere
RigidBody::RigidBody (Point c, double r, RIGID_BODY_TYPE t) {
  collisionSphere = Sphere(c, r);
  loc = c;
  shapeType = SPHERE;
  type = t;
  vel = Vector(0,0,0);
}

RigidBody::~RigidBody () {}

const Shape* RigidBody::getShape() const {
  switch (shapeType) {
  case SPHERE: return &collisionSphere;
  case BOX: return &collisionBox;
  }
}

// Override this method if we wish exhibit behavior upon collision
void RigidBody::collide(const RigidBody* other) {}

RIGID_BODY_TYPE RigidBody::getType () const {
  return type;
}

void RigidBody::draw(Point cameraLoc) {} // no default drawing

void RigidBody::update(float elapsedTime) {
  applyVelocity(elapsedTime);
}

void RigidBody::applyForce(Vector force, float elapsedTime) {
  vel += force * elapsedTime;
}

void RigidBody::applyVelocity(float elapsedTime) {
  loc += vel * elapsedTime;
  collisionSphere.c = loc;
  collisionBox.c = loc;
}

short handleCollision(RigidBody* a, RigidBody* b) {
  if (detectCollision(a->getShape(), b->getShape())) {
    if (a->getType() == ENEMY && b->getType() == ENEMY) {
      // Vector temp = a->vel;
      // a->vel = b->vel;
      // b->vel = temp;

      // Vector N = (a-);
      // N.normalize();
      // // //nudge them apart
      // float dist = (loc-other->loc).mag();
      // float d = ((Sphere*)(a->getShape()))->r + 
      // 		 ((Sphere*)(b->getShape()))->r) - dist;
      // loc += N*d;

    }
    a->collide(b);
    b->collide(a);
  }
}

Vector multiplyVectorByMatrix(Vector v, GLfloat m[16]) {
  float temp[4] = {0};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      temp[i] += m[4*i+j]*v.at(j);
    }
  }
  return Vector(temp[0], temp[1], temp[2]);
}
