#pragma once

#include <math.h>

#include "Shape.hpp"
#include "Triangle.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include "MathFunctions.hpp"
#include "../TrollCollision/CollisionManager.hpp"
#include "ShapeList.hpp"

//! Everything in this module is in the namespace TMath;

/*

when a object is generated a function will return a normal pointer, when you call a method such as get_center()
that returns a point it will be a const pointer to the point object.
Methods that return a pointer should be checked before using with somthing like if(return_stuff()).


sub-shapes such as the rectangle, and circle are mostly just data containers

*/
