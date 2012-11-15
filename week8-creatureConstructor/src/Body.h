//
//  Body.h
//
//  Created by Patricio Gonzalez Vivo on 11/13/12.
//
//
#ifndef BODY
#define BODY

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "Spring.h"
#include "VectorField.h"

//  Triangulation
//
#include <iostream>
#include <stdlib.h> // for C qsort
#include <time.h>   // for random
#include <cmath>
//
const int       MaxVertices = 500;
const int       MaxTriangles = 1000;
const double    EPSILON = 0.000001;
//
struct          Edge{ int p1, p2; };
struct          Triangle{ int p1, p2, p3; };
int             Triangulate( int nv, ofPoint pxyz[], Triangle v[], int &ntri );
int             CircumCircle( double, double, double, double, double, double, double, double, double&, double&, double& );

class Body {
public:
    Body();
    ~Body();
    
    bool    init( string _imgfile, ofPoint _initPos);
    bool    load( string _file, ofPoint _initPos );
    bool    save( string _file );
    
    void    clear();
    void    restart();
    
    void    startEditMode();
    void    stopEditMode();
    void    togleEditMode();
    
    int     getIndexAt(ofPoint _pos);
    
    void    addVertex(ofPoint _pos);
    bool    addSpring(unsigned int _from, unsigned int _to, float _k = 0.2f);
    
    void    update(VectorField *_VF = NULL);
    void    draw();
    
private:
    void    _mousePressed(ofMouseEventArgs &e);
    void    _mouseDragged(ofMouseEventArgs &e);
    void    _mouseReleased(ofMouseEventArgs &e);
    
    void    _calculateTriangles();
    void    _updateMesh();
    void    _updateSpringsConectedTo(int _index);
    int     _getIndexForId(int _nId);
    
    vector<Vertex*>     vertices;
    vector<Spring>      springs;
    vector<Triangle>    triangles;
    
    ofMesh  mesh;
    
    ofImage image;
    string  imageFile;
    ofPoint imageCenter;
    
    ofPoint origin;
    
    int     nVertexSelected;
    int     nVertexHover;
    int     nTriangles;
    
    bool    bRightClick;
    bool    bEdit;
};

#endif
