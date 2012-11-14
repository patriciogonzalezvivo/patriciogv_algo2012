//
//  Body.h
//
//  Created by Patricio Gonzalez Vivo on 11/13/12.
//
//

#ifndef BODY
#define BODY

#include "ofMain.h"

#include "spring.h"

#include <iostream>
#include <stdlib.h> // for C qsort
#include <cmath>
#include <time.h> // for random

const int       MaxVertices = 500;
const int       MaxTriangles = 1000;
const double    EPSILON = 0.000001;

struct          Edge{   int p1, p2; };
struct          Triangle{   int p1, p2, p3; };
int             Triangulate(int nv, ofPoint pxyz[], Triangle v[], int &ntri);
int             CircumCircle(double, double, double, double, double, double, double, double, double&, double&, double&);


class Body {
public:
    Body();
    ~Body();
    
    void    init(string _file, ofPoint _initPos);
    
    void    clear();
    void    restart();
    
    void    startEditMode();
    void    stopEditMode();
    void    togleEditMode();
    
    void    addVertex(ofPoint _pos);
    int     getIndexAt(ofPoint _pos);
    bool    addSpring(unsigned int _from, unsigned int _to);
    
    void    update();
    void    draw();
    
    ofMesh  mesh;
    
private:
    void    _mousePressed(ofMouseEventArgs &e);
    void    _mouseDragged(ofMouseEventArgs &e);
    void    _mouseReleased(ofMouseEventArgs &e);
    
    void    _calculateTriangles();
    void    _updateMesh();
    void    _updateSpringsConectedTo(int _index);
    
    vector<Vertex*>     vertices;
    vector<Spring>      springs;
    vector<Triangle>    triangles;
    
    ofImage image;
    ofPoint centerImage;
    
    ofPoint origin;
    
    int     nSelectedVertex;
    int     nTriangles;
    
    bool    bRightClick;
    bool    bEdit;
};

#endif
