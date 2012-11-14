#ifndef SPRING
#define SPRING

#include "ofMain.h"
#include "Vertex.h"

class Spring {
public:

    Spring();
		
    void update();
    void draw();
    
    Vertex * vertexA;
    Vertex * vertexB;
		
    float distance;
    float springiness;	 // this is the k, Springiness constant
};

#endif