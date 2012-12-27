#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    gui.setup("brush");
    gui.add(brushWidth.setup("width",25,0,50));
    gui.add(brushNumber.setup("number",10,1,50));
    gui.add(brushDamp.setup("damp",0.245,0.0,0.5));
    gui.add(brushK.setup("k",0.0825,0.0,0.5));
    
    gui.add(brushRepRad.setup("repulsion_rad", 5, 0, 20));
    gui.add(brushRepPct.setup("repulsion_pct", 0.5, 0.0, 1.0));
    
    gui.add(blurRadius.setup("blur_radius", 5, 1, 10));
    
    gui.loadFromFile("settings.xml");
    
    width = ofGetWidth();//ofGetScreenWidth();
    height = ofGetHeight();//ofGetScreenHeight();
    canvas.allocate(width, height);
    canvas.begin();
    ofClear(0,0);
    canvas.end();
    
    grayscale.allocate(width, height);
    blur.allocate(width, height);
    normals.allocate(width, height);
    
    noise.allocate(width, height);
    noise.setZoom(noiseZoom);
    noise.update();
    
    for(int i = 0; i < 2; i++){
        pingpong[i].allocate(width,height);
        pingpong[i].begin();
        ofClear(0);
        pingpong[i].end();
    }
    
    string fragShader = STRINGIFY(uniform sampler2DRect normals;
                                  uniform sampler2DRect canvas;
                                  
                                  void main(){
                                      vec2 st = gl_TexCoord[0].st;
                                      
                                      vec4 color = texture2DRect(canvas,st);
                                      vec2 norm	= ( texture2DRect(normals, st).rg - 0.5 ) * 2.0;
                                      
                                      vec2 offset[36];
                                      int iTotal = 36;
                                      float fTotal = 36.0;
                                      
                                      float pi = 3.14159265358979323846;
                                      float step = (pi*2.0)/fTotal;
                                      float angle = 0.0;
                                      for (int i = 0; i < iTotal; i++) {
                                          offset[i].x = cos(angle);
                                          offset[i].y = sin(angle);
                                          angle += step;
                                      }
                                      
                                      float sources = 1.0;
                                      for (int i = 0; i < iTotal; i++){
                                          vec4 goingTo = ( texture2DRect( normals, st + offset[i] ) - 0.5 ) * 2.0;
                                          
                                          if ( dot(goingTo.rg,offset[i]) < -1.0/fTotal ){
                                              sources += 1.0;
                                              color += texture2DRect(canvas, st + offset[i]);
                                          }
                                      }
                                      color = color / sources;
                                      
                                      gl_FragColor = color;
                                  }
                                  );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    timer   = 0;
    bDebug  = false;
}

//--------------------------------------------------------------
void testApp::update(){
    brush.update();
    
    if (brush.bDown){
        canvas.begin();
        ofEnableAlphaBlending();
        brush.draw();
        canvas.end();
        
        blur.setRadius(blurRadius);
        
        grayscale.begin();
        ofSetColor(255);
        pingpong[(timer+1)%2].draw(0, 0);
        grayscale.end();
        grayscale.update();
        
        blur << grayscale;
        blur.update();
        normals << blur;
        normals.update();
        
        pingpong[ timer%2 ].begin();
        shader.begin();
        shader.setUniformTexture("normals", normals.getTextureReference(), 0);
        shader.setUniformTexture("canvas", canvas.getTextureReference(), 1);
        
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
        glTexCoord2f(width, height); glVertex3f(width, height, 0);
        glTexCoord2f(0,height);  glVertex3f(0,height, 0);
        glEnd();
        
        shader.end();
        pingpong[ timer%2 ].end();
        
        timer++;
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
//    ofBackground(255);
    
    ofSetColor(255);
    pingpong[(timer+1)%2].draw(0, 0);
    
    if (bDebug) {
        ofSetColor(255, 100);
        canvas.draw(0, 0);
        ofSetColor(255);
        brush.drawDebug();
        
        ofSetColor(255);
        normals.draw(ofGetWidth()-width*0.25-15,ofGetHeight()-height*0.25-15,width*0.25,height*0.25);
        
        gui.draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'd'){
        bDebug = !bDebug;
    } else if (key == ' '){
        brush.clear();
        canvas.begin();
        ofClear(0,0);
        canvas.end();
        for(int i = 0; i < 2; i++){
            pingpong[i].allocate(width,height);
            pingpong[i].begin();
            ofClear(0);
            pingpong[i].end();
        }
    } else if (key == 'f'){
        ofToggleFullscreen();
    } else if (key == 'n'){
        noise.setZoom(noiseZoom);
        noise.update();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    brush.set(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    brush.setNum(brushNumber);
    brush.setWidth(brushWidth);
    brush.damp = brushDamp;
    brush.k = brushK;
    brush.repPct = brushRepPct;
    brush.repRad = brushRepRad;
    
    brush.begin();
    brush.set(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    brush.set(x, y);
    brush.end();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}