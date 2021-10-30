
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"
#include <math.h>

using namespace std;

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------



// implement

//Function that calculate the distance between 2 points
double distance(Point p1, Point p2);

// Function that chacks if point p is inside the circle c
int isInside(Circle c, Point p);

Point getCenterPoint(double x1, double y1, double x2, double y2);

// Function that returns a circle by recieving 3 points 
Circle getCircle(Point p1, Point p2, Point p3);

// Function that returns a circle by recieving 2 points 
Circle getCircle(Point A, Point B);

// Function that checks if a circle is enclosing to vector P
int isValidCircle(Circle c, vector<Point> r);
  
Circle getMinCircle(vector<Point> r);

//Function that returns the minimal enclosing circle
Circle getMEC(Point** points, vector<Point> r, int n); 


//The main function
Circle findMinCircle(Point** points, size_t size);

int new_isInside(Circle c, Point p);



#endif /* MINCIRCLE_H_ */
