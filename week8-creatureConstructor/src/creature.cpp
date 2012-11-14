//
//  creature.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/13/12.
//
//

#include "creature.h"

creature::creature(){
    bEdit = false;
    bRightClick = false;
    nSelectedParticle = -1;
    origin.set(0,0);
}

void creature::init(string _file, ofPoint _initPos){
    image.loadImage(_file);
    centerImage.set(image.getWidth()*0.5, image.getHeight()*0.5);
    
    origin = _initPos;
    
    clear();
}

void creature::startEditMode(){
    if (!bEdit){
        ofAddListener(ofEvents().mousePressed, this, &creature::_mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &creature::_mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &creature::_mouseReleased);
        bEdit = true;
    }
}

void creature::stopEditMode(){
    if (bEdit){
        ofRemoveListener(ofEvents().mousePressed, this, &creature::_mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &creature::_mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &creature::_mouseReleased);
        bEdit = false;
    }
}

void creature::togleEditMode(){
    if(!bEdit){
        startEditMode();
    } else {
        stopEditMode();
    }
}

void creature::clear(){
    for(int i = 0; i < particles.size(); i++){
        delete particles[i];
    }
    particles.clear();
    springs.clear();
}

void creature::addParticle(ofPoint _pos){
    particle *newParticle = new particle();
    
    newParticle->org = _pos;
    
    ofPoint toCenter = _pos - centerImage;
    float angle = atan2(toCenter.y,toCenter.x);
    float radio = toCenter.length();
    
    newParticle->pos.x = origin.x + cos(angle)*radio;
    newParticle->pos.y = origin.y + sin(angle)*radio;
    
    newParticle->vel.set(0,0);
    
    particles.push_back(newParticle);
    
    _calculateTriangles();
}

int creature::getIndexAt(ofPoint _pos){
    int rta = -1;
    
    for (int i = 0; i < particles.size(); i++) {
        if (_pos.distance( particles[i]->org ) < 10){
            rta = i;
        }
    }
    
    return rta;
}

bool creature::addSpring(unsigned int _from, unsigned int _to){
    bool rta = false;
    
    if ( (_from < particles.size()) && (_to < particles.size()) && (_to != _from) ){
        spring newSpring;
        
		newSpring.springiness	= 0.2f;
		newSpring.particleA     = particles[ _from];
		newSpring.particleB     = particles[ _to];
        newSpring.distance		= particles[ _from]->org.distance( particles[ _to]->org );
        
        springs.push_back(newSpring);
        rta = true;
    }
    
    return rta;
}

void creature::update(){
    for (int i = 0; i < particles.size(); i++){
		particles[i]->resetForce();
	}
	
	for (int i = 0; i < springs.size(); i++){
		springs[i].update();
	}
	
    for (int i = 0; i < particles.size(); i++){
//		particles[i]->addForce(0,0.1f);
		particles[i]->addRepulsionForce(ofGetMouseX(), ofGetMouseY(), 300, 0.7f);
	}
	
	for (int i = 0; i < particles.size(); i++){
		particles[i]->bounceOffWalls();
		particles[i]->addDampingForce();
		particles[i]->update();
	}
}

void creature::draw(){
    if (bEdit){
        ofPushStyle();
        ofSetColor(255);
        image.draw(0, 0);
        
        ofSetColor(35, 211, 255);
        ofPushMatrix();
        ofTranslate(centerImage);
        ofLine(5,0,-5,0);
        ofLine(0,5,0,-5);
        ofPopMatrix();
        
        ofSetColor(35, 211, 255);
        ofPushMatrix();
        ofTranslate(origin);
        ofLine(5,0,-5,0);
        ofLine(0,5,0,-5);
        ofPopMatrix();
        
        ofSetColor(35, 211, 255);
        for (int i = 0; i < springs.size(); i++){
            ofLine(springs[i].particleA->org, springs[i].particleB->org);
        }
        
        ofSetColor(21, 48, 144);
        ofFill();
        for (int i = 0; i < particles.size(); i++){
            ofCircle(particles[i]->org,3);
        }
        
        if(nSelectedParticle != -1){
            ofPoint mouse = ofPoint(ofGetMouseX(),ofGetMouseY());
            ofNoFill();
            ofSetColor(112, 31, 215);
            ofCircle( particles[nSelectedParticle]->org, 5);
            ofLine( particles[nSelectedParticle]->org, mouse);
        }
        
        ofPopStyle();
    }
    
//    for (int i = 0; i < particles.size(); i++){
//		particles[i]->draw();
//	}
//	
//	for (int i = 0; i < springs.size(); i++){
//		springs[i].draw();
//	}
    
    _updateMesh();
    
    ofSetColor(255);
    image.bind();
    mesh.draw();
    image.unbind();
    
    if (bEdit){
        ofSetColor(35, 211, 255,100);
        mesh.drawWireframe();
    }
}

void creature::_mousePressed(ofMouseEventArgs &e){
	if(bEdit){
        ofPoint mouse = ofPoint(e.x,e.y);
        int index = getIndexAt(mouse);
        
        if (index == -1){
            addParticle(mouse);
        } else {
            nSelectedParticle = index;
        }
        
        if (e.button != 0) {
            bRightClick = true;
        }
    }
}

void creature::_mouseDragged(ofMouseEventArgs &e){
    ofPoint mouse = ofPoint(e.x,e.y);
    
    if (bRightClick){
        if (nSelectedParticle != -1){
            particles[nSelectedParticle]->org = mouse;
        }
    }
}

void creature::_mouseReleased(ofMouseEventArgs &e){
    if (bEdit){
        ofPoint mouse = ofPoint(e.x,e.y);
        int index = getIndexAt(mouse);
        
        if ( nSelectedParticle != -1){
            
            if ( index == -1 ){
                addParticle(mouse);
                index = getIndexAt(mouse);
            }
            
            addSpring(nSelectedParticle, index);
        }
        nSelectedParticle = -1;
        bRightClick = false;
        
        
        _calculateTriangles();
    }
}

// comparison routine for sort...
//
bool comparisonFunction(  particle * a, particle * b ) {
	return a->org.x < b->org.x;
}

void creature::_calculateTriangles(){
    //  Prepare the particles
    //
    nSelectedParticle = -1;
    int nv = particles.size();
    
    if (nv >= 3){
        sort( particles.begin(), particles.end(), comparisonFunction );
        
        //  Extract the origin postions
        //
        vector<ofPoint> vertices;
        for (int i = 0 ; i < particles.size(); i++) {
            ofPoint newPoint = particles[i]->org;
            vertices.push_back( newPoint );
        }
        
        //  Add 3 more slots ( required by the Triangulate call)
        //
        vertices.push_back( ofPoint(0,0) );
        vertices.push_back( ofPoint(0,0) );
        vertices.push_back( ofPoint(0,0) );
        
        //  Allocate space for triangles indices
        //
        triangles.resize(3*nv);
        
        //  Make the triangulation
        //
        nTriangles = 0;
        Triangulate( nv, &vertices[0], &triangles[0], nTriangles );
    }
}

void creature::_updateMesh(){
	mesh.clear();
    
    //  Pass the position of the particles
    //
	for (int i = 0; i < particles.size(); i++){
        mesh.addVertex( particles[i]->pos );
        mesh.addTexCoord( ofVec2f(particles[i]->org.x,particles[i]->org.y ) );
    }

    //  Pass the triagles indices of how to arrange them
    //
	for(int i = 0; i < nTriangles; i++){
		mesh.addIndex(triangles[ i ].p1);
		mesh.addIndex(triangles[ i ].p2);
		mesh.addIndex(triangles[ i ].p3);
	}
}

////////////////////////////////////////////////////////////////////////
// CircumCircle() :
//   Return true if a point (xp,yp) is inside the circumcircle made up
//   of the points (x1,y1), (x2,y2), (x3,y3)
//   The circumcircle centre is returned in (xc,yc) and the radius r
//   Note : A point on the edge is inside the circumcircle
////////////////////////////////////////////////////////////////////////

int CircumCircle(double xp, double yp, double x1, double y1, double x2,
                 double y2, double x3, double y3, double &xc, double &yc, double &r){
    double m1, m2, mx1, mx2, my1, my2;
    double dx, dy, rsqr, drsqr;
    
    /* Check for coincident points */
    if(abs(y1 - y2) < EPSILON && abs(y2 - y3) < EPSILON)
        return(false);
    if(abs(y2-y1) < EPSILON){
        m2 = - (x3 - x2) / (y3 - y2);
        mx2 = (x2 + x3) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (x2 + x1) / 2.0;
        yc = m2 * (xc - mx2) + my2;
    }else if(abs(y3 - y2) < EPSILON){
        m1 = - (x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        xc = (x3 + x2) / 2.0;
        yc = m1 * (xc - mx1) + my1;
    }else{
        m1 = - (x2 - x1) / (y2 - y1);
        m2 = - (x3 - x2) / (y3 - y2);
        mx1 = (x1 + x2) / 2.0;
        mx2 = (x2 + x3) / 2.0;
        my1 = (y1 + y2) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        yc = m1 * (xc - mx1) + my1;
    }
    dx = x2 - xc;
    dy = y2 - yc;
    rsqr = dx * dx + dy * dy;
    r = sqrt(rsqr);
    dx = xp - xc;
    dy = yp - yc;
    drsqr = dx * dx + dy * dy;
    return((drsqr <= rsqr) ? true : false);
}
///////////////////////////////////////////////////////////////////////////////
// Triangulate() :
//   Triangulation subroutine
//   Takes as input NV vertices in array pxyz
//   Returned is a list of ntri triangular faces in the array v
//   These triangles are arranged in a consistent clockwise order.
//   The triangle array 'v' should be malloced to 3 * nv
//   The vertex array pxyz must be big enough to hold 3 more points
//   The vertex array must be sorted in increasing x values say
//
//   qsort(p,nv,sizeof(XYZ),XYZCompare);
///////////////////////////////////////////////////////////////////////////////

int Triangulate(int nv, ofPoint pxyz[], ITRIANGLE v[], int &ntri){
    int *complete = NULL;
    IEDGE *edges = NULL;
    IEDGE *p_EdgeTemp;
    int nedge = 0;
    int trimax, emax = 200;
    int status = 0;
    int inside;
    int i, j, k;
    double xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r;
    double xmin, xmax, ymin, ymax, xmid, ymid;
    double dx, dy, dmax;
    
    /* Allocate memory for the completeness list, flag for each triangle */
    trimax = 4 * nv;
    complete = new int[trimax];
    /* Allocate memory for the edge list */
    edges = new IEDGE[emax];
    /*
     Find the maximum and minimum vertex bounds.
     This is to allow calculation of the bounding triangle
     */
    xmin = pxyz[0].x;
    ymin = pxyz[0].y;
    xmax = xmin;
    ymax = ymin;
    for(i = 1; i < nv; i++){
        if (pxyz[i].x < xmin) xmin = pxyz[i].x;
        if (pxyz[i].x > xmax) xmax = pxyz[i].x;
        if (pxyz[i].y < ymin) ymin = pxyz[i].y;
        if (pxyz[i].y > ymax) ymax = pxyz[i].y;
    }
    dx = xmax - xmin;
    dy = ymax - ymin;
    dmax = (dx > dy) ? dx : dy;
    xmid = (xmax + xmin) / 2.0;
    ymid = (ymax + ymin) / 2.0;
    /*
     Set up the supertriangle
     his is a triangle which encompasses all the sample points.
     The supertriangle coordinates are added to the end of the
     vertex list. The supertriangle is the first triangle in
     the triangle list.
     */
    pxyz[nv+0].x = xmid - 20 * dmax;
    pxyz[nv+0].y = ymid - dmax;
    pxyz[nv+1].x = xmid;
    pxyz[nv+1].y = ymid + 20 * dmax;
    pxyz[nv+2].x = xmid + 20 * dmax;
    pxyz[nv+2].y = ymid - dmax;
    v[0].p1 = nv;
    v[0].p2 = nv+1;
    v[0].p3 = nv+2;
    complete[0] = false;
    ntri = 1;
    /*
     Include each point one at a time into the existing mesh
     */
    for(i = 0; i < nv; i++){
        xp = pxyz[i].x;
        yp = pxyz[i].y;
        nedge = 0;
        /*
         Set up the edge buffer.
         If the point (xp,yp) lies inside the circumcircle then the
         three edges of that triangle are added to the edge buffer
         and that triangle is removed.
         */
        for(j = 0; j < ntri; j++){
            if(complete[j])
                continue;
            x1 = pxyz[v[j].p1].x;
            y1 = pxyz[v[j].p1].y;
            x2 = pxyz[v[j].p2].x;
            y2 = pxyz[v[j].p2].y;
            x3 = pxyz[v[j].p3].x;
            y3 = pxyz[v[j].p3].y;
            inside = CircumCircle(xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r);
            if (xc + r < xp)
                // Suggested
                // if (xc + r + EPSILON < xp)
                complete[j] = true;
            if(inside){
                /* Check that we haven't exceeded the edge list size */
                if(nedge + 3 >= emax){
                    emax += 100;
                    p_EdgeTemp = new IEDGE[emax];
                    for (int i = 0; i < nedge; i++) { // Fix by John Bowman
                        p_EdgeTemp[i] = edges[i];
                    }
                    delete []edges;
                    edges = p_EdgeTemp;
                }
                edges[nedge+0].p1 = v[j].p1;
                edges[nedge+0].p2 = v[j].p2;
                edges[nedge+1].p1 = v[j].p2;
                edges[nedge+1].p2 = v[j].p3;
                edges[nedge+2].p1 = v[j].p3;
                edges[nedge+2].p2 = v[j].p1;
                nedge += 3;
                v[j] = v[ntri-1];
                complete[j] = complete[ntri-1];
                ntri--;
                j--;
            }
        }
        /*
         Tag multiple edges
         Note: if all triangles are specified anticlockwise then all
         interior edges are opposite pointing in direction.
         */
        for(j = 0; j < nedge - 1; j++){
            for(k = j + 1; k < nedge; k++){
                if((edges[j].p1 == edges[k].p2) && (edges[j].p2 == edges[k].p1)){
                    edges[j].p1 = -1;
                    edges[j].p2 = -1;
                    edges[k].p1 = -1;
                    edges[k].p2 = -1;
                }
                /* Shouldn't need the following, see note above */
                if((edges[j].p1 == edges[k].p1) && (edges[j].p2 == edges[k].p2)){
                    edges[j].p1 = -1;
                    edges[j].p2 = -1;
                    edges[k].p1 = -1;
                    edges[k].p2 = -1;
                }
            }
        }
        /*
         Form new triangles for the current point
         Skipping over any tagged edges.
         All edges are arranged in clockwise order.
         */
        for(j = 0; j < nedge; j++) {
            if(edges[j].p1 < 0 || edges[j].p2 < 0)
                continue;
            v[ntri].p1 = edges[j].p1;
            v[ntri].p2 = edges[j].p2;
            v[ntri].p3 = i;
            complete[ntri] = false;
            ntri++;
        }
    }
    /*
     Remove triangles with supertriangle vertices
     These are triangles which have a vertex number greater than nv
     */
    for(i = 0; i < ntri; i++) {
        if(v[i].p1 >= nv || v[i].p2 >= nv || v[i].p3 >= nv) {
            v[i] = v[ntri-1];
            ntri--;
            i--;
        }
    }
    delete[] edges;
    delete[] complete;
    return 0;
} 