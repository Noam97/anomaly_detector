// // 208723791 318868312

#include "minCircle.h"


//Function that calculate the distance between 2 points
double distance(Point p1, Point p2) 
{ 
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)); 
}

// Function that chacks if point p is inside the circle c
int isInside(Circle c,Point p) 
{
    if(distance(c.center, p) <= c.radius)
        return 1; //true
    return 0; // false
}

int new_isInside(Circle c, Point p) 
{
    if(distance(c.center, p) <= c.radius)
        return 1; //true
    return 0; // false
}

Point getCenterPoint(double x1, double y1, double x2, double y2) 
{ 
    
    double A = x1 * x1 + y1 * y1; 
    double B = x2 * x2 + y2 * y2; 
    double C = x1 * y2 - y1 * x2; 
    Point p((y2 * A - y1 *B) / (2 * C), (x1 * B - x2 * A) / (2 * C));
    return p; 
} 

// Function that returns a circle by recieving 3 points 
Circle getCircle(Point p1,Point p2, Point p3) {
    float A = p1.x * (p2.y - p3.y) - p1.y * (p2.x - p3.x) + p2.x * p3.y - p3.x * p2.y;
    float B = ((p1.x * p1.x) + (p1.y * p1.y)) * (p3.y - p2.y) + ((p2.x * p2.x) + (p2.y * p2.y)) * (p1.y - p3.y) + ((p3.x * p3.x) + (p3.y * p3.y)) * (p2.y - p1.y);
    float C = ((p1.x * p1.x) + (p1.y * p1.y)) * (p2.x - p3.x) + ((p2.x * p2.x) + (p2.y * p2.y)) * (p3.x - p1.x) + ((p3.x * p3.x) + (p3.y * p3.y)) * (p1.x - p2.x);
    float D = ((p1.x * p1.x) + (p1.y * p1.y)) * (p3.x * p2.y - p2.x * p3.y) + ((p2.x * p2.x) + (p2.y * p2.y)) * (p1.x * p3.y - p3.x * p1.y) + ((p3.x * p3.x) + (p3.y * p3.y)) * (p2.x * p1.y - p1.x * p2.y);
    float x = (-1 * B) / (2 * A);
    float y = (-1 * C) / (2 * A);
    float r = sqrt(((B * B) + (C * C) - 4 * A * D) / (4 * (A * A)));
    Point p(x, y);
    Circle c(p, r);
    return c;
}

// Function that returns a circle by recieving 2 points 
Circle getCircle(Point A,  Point B) 
{ 
    // Midpoint of A and B 
    Point C((A.x + B.x) / 2.0, (A.y + B.y) / 2.0); 
    // Set radios - half of the distance
    Circle cir(C, distance(A, B) / (double)2.0); 
    return cir; 
} 

// Function that checks if a circle is enclosing to vector P
int isValidCircle(Circle c, vector<Point> r) 
{ 
    //to check for each point if it is inside the circle c
    for (const Point& point: r) 
        if (!isInside(c, point)) 
            return 0; 
    return 1; 
} 
  
Circle getMinCircle(vector<Point> r) 
{ 
    if (r.size() > 3) {
        Point p(0,0);
        Circle c(p, 0);
        return c;
    } 
    //check if r is null
    if (r.size() == 0) {
        Point p(0,0);
        Circle c(p, 0); 
        return c; 
    } 
    else if (r.size() == 1) {
        Circle c(r[0], 0); 
        return c;
    } 
    else if (r.size() == 2) { 
        return getCircle(r[0], r[1]);
    }
    
    for (int i = 0; i < 3; i++) { 
        for (int j = i + 1; j < 3; j++) { 
            Circle c = getCircle(r[i], r[j]); 
            if (isValidCircle(c, r) == 1) 
                return c; 
        }
    } 
    return getCircle(r[0], r[1], r[2]);
}

//Function that returns the minimal enclosing circle
Circle getMEC(Point** points, vector<Point> r, int n) 
{ 
    if(n == 0 || r.size() == 3) {
        return getMinCircle(r);
    }

    Circle d = getMEC(points, r, n-1);

    if(isInside(d, *points[n-1]) == 1) {
        return d;
    }
    r.push_back(*points[n-1]);
    return getMEC(points, r, n-1);
}

//The main function
Circle findMinCircle(Point** points, size_t size) {
    Circle finalCircle = getMEC(points, {}, size);
    return finalCircle; 
}

