#include "Geometry.h"
SHAPE_TYPE Shape::getType () const  {
  return type;
}

Sphere::Sphere(Point center, double radius) {
  c = center;
  r = radius;
  type = SPHERE;
}

Box::Box(Point center, double width, double height, double depth) {
  c = center;
  w = width;
  h = height;
  d = depth;
  type = BOX;
}

double Box::dim(short i) const {
  if (i == 0) return w;
  if (i == 1) return h;
  if (i == 2) return d;
  else return 0;
}

short detectCollision (const Shape* a, const Shape* b) {
  if (a->getType() == SPHERE) {
    if (b->getType() == SPHERE) {
      return detectCollision( *((Sphere*)a), *((Sphere*)b));
    } else if (b->getType() == BOX) {
      return detectCollision( *((Sphere*)a), *((Box*)b));
    }

  } else if (a->getType() == BOX) {
    if (b->getType() == SPHERE) {
      return detectCollision( *((Sphere*)b), *((Box*)a));
    } else if (b->getType() == BOX) {
      return detectCollision( *((Box*)a), *((Box*)b));
    }
    
  }
}

short detectCollision(const Sphere a, const Sphere b) {
  double distSq = (a.c-b.c).magSq();
  if (distSq < (a.r+b.r)*(a.r+b.r) ) return 1;
  else return 0;
}

short detectCollision(const Sphere s, const Box b) {
  // for each dimension
  short collide = 1;
  for (short d = 0; d < 3; ++d) {
    float sleft  = s.c.at(d) - s.r;
    float sright = s.c.at(d) + s.r;
    float bleft = b.c.at(d) - b.dim(d)/2;
    float bright = b.c.at(d) + b.dim(d)/2;
    // Check if sphere points are on either end of a box side
    if ( (sleft -  bleft )*(sright - bleft ) < 0 ||
	 (sleft -  bright)*(sright - bright) < 0 ||
	 (bleft -  sleft )*(bright - sleft) < 0 ||
	 (bleft -  sright )*(bright - sright) < 0 ) {
      continue;
    } else {
      collide = 0;
      break; // no need to keep checking
    }
  }
  return collide;
}

short detectCollision(const Box a, const Box b) {
  // for each dimension
  short collide = 1;
  for (short d = 0; d < 3; ++d) {
    float sleft  = a.c.at(d) - a.dim(d)/2;
    float sright = a.c.at(d) + a.dim(d)/2;
    float bleft = b.c.at(d) - b.dim(d)/2;
    float bright = b.c.at(d) + b.dim(d)/2;
    // Check if sphere points are on either end of a box side
    if ( (sleft -  bleft )*(sright - bleft ) < 0 ||
	 (sleft -  bright)*(sright - bright) < 0 ||
	 (bleft -  sleft )*(bright - sleft) < 0 ||
	 (bleft -  sright )*(bright - sright) < 0 ) {
      continue;
    } else {
      collide = 0;
      break; // no need to keep checking
    }
  }
  return collide;
}
