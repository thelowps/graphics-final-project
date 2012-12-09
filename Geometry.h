#ifndef GEOMETRY_H
#define GEOMETRY_H

/*
 * Geometry.h
 * A small library that defines collision shapes and functions to check
 * for collisions between them.
 */

#include <CSE40166/CSE40166.h>
using namespace CSE40166;

enum SHAPE_TYPE { SPHERE, BOX };

class Shape {
public:
  SHAPE_TYPE getType() const;
  Point c; // center
protected:
  SHAPE_TYPE type;
};


class Sphere : public Shape {
public:
  Sphere(Point c = Point(0,0,0), double r = 1);
  double r;
};


class Box : public Shape {
public:
  Box(Point c = Point(0,0,0), double w = 1, double h = 1, double d = 1);
  double w, h, d;
  double dim(short i) const; // get indexed dimension
};

// This function will determine what kind of shapes they are
// and call the appropriate comparison function. Ugly, I know.
short detectCollision(const Shape* a, const Shape* b);

// The different possible kinds of collisions
short detectCollision(Sphere a, Sphere b);
short detectCollision(Sphere a, Box b);
short detectCollision(Box a, Box b);

#endif
