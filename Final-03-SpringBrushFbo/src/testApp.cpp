#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    gui.setup("Panel");
    gui.add(brushWidth.setup("brush_width",25,0,50));
    gui.add(brushNumber.setup("brush_number",50,1,50));
    gui.add(brushDamp.setup("brush_damp",0.245,0.0,0.5));
    gui.add(brushSoftness.setup("brush_softness",0.0825,0.0,0.5));
    gui.add(brushHeight.setup("brush_height",0.1,0.0,2.0));
    gui.add(noiseZoom.setup("noise_zoom", 10., 0.0, 100.0));
    gui.add(noiseStrengh.setup("noise_strenght", 200, 0, 255));
    gui.add(blurRadius.setup("blur_radius", 5, 1, 10));
    gui.loadFromFile("settings.xml");
    
    brushMask.loadImage("brush.png");
    brushMask.setAnchorPercent(0.5, 0.5);
    
    width = ofGetScreenWidth();
    height = ofGetScreenHeight();
    canvas.allocate(width, height);
    canvas.begin();
    ofClear(255);
    canvas.end();
    
    grayscale.allocate(width, height);
    blur.allocate(width, height);
    normals.allocate(width, height);
    mask.allocate(width, height);
    
    noise.allocate(width, height);
    noise.setZoom(noiseZoom);
    noise.update();
    
    for(int i = 0; i < 2; i++){
        pingpong[i].allocate(width,height);
        pingpong[i].begin();
        ofClear(0);
        pingpong[i].end();
    }
    
    string fragShader = STRINGIFY(uniform sampler2DRect backbuffer;
                                  uniform sampler2DRect normals;
                                  uniform sampler2DRect maskMap;
                                  uniform sampler2DRect canvas;
                                  
                                  void main(){
                                      vec2 st = gl_TexCoord[0].st;
                                      
                                      vec4 newFrame = texture2DRect(backbuffer, st);
                                      vec4 color = texture2DRect(canvas,st);
                                      vec2 norm	= ( texture2DRect(normals, st).rg - 0.5 ) * 2.0;
                                      float mask = texture2DRect( maskMap, st ).r;
                                      
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
                                              color += texture2DRect(backbuffer, st + offset[i]);
                                          }
                                      }
                                      color = color / sources;
                                      color = color*(mask) + newFrame*(1.0-mask);
                                      
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
    timer++;
    
    brush.update();
    
    canvas.begin();
    brush.draw();
    canvas.end();
    
    blur.setRadius(blurRadius);
    
    grayscale.begin();
    ofSetColor(noiseStrengh);
    noise.draw(0,0);
    ofSetColor(255);
    pingpong[(timer+1)%2].draw(0, 0);
    grayscale.end();
    grayscale.update();
    
    mask.setTexture(grayscale.getTextureReference(), 1);
    mask.update();
    
    blur << grayscale;
    blur.update();
    normals << blur;
    normals.update();
    
    pingpong[ timer%2 ].begin();
    shader.begin();
    shader.setUniformTexture("normals", normals.getTextureReference(), 1);
    shader.setUniformTexture("maskMap", mask.getTextureReference(), 2);
    shader.setUniformTexture("canvas", canvas.getTextureReference(), 3);
    
    if (brush.bDown){
        canvas.draw(0, 0);
    }
    pingpong[ (timer+1)%2 ].draw(0, 0);
    
    shader.end();
    pingpong[ timer%2 ].end();
    
    mask.begin();
    ofClear(0);
    brush.drawPathFollower(brushMask);
    mask.end();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    pingpong[(timer+1)%2].draw(0, 0);
    
    if (bDebug) {
        ofSetColor(255, 100);
        canvas.draw(0, 0);
        ofSetColor(255);
        brush.drawDebug();
        
        ofSetColor(255);
        normals.draw(ofGetWidth()-width*0.25-15,ofGetHeight()-height*0.25-15,width*0.25,height*0.25);
        mask.draw(ofGetWidth()-width*0.25-15,ofGetHeight()-height*0.25-15,width*0.25,height*0.25);
        
        ofSetColor(100,100);
        brush.drawPathFollower(brushMask);
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
    canvas.begin();
    ofClear(255);//0, 0);
//    ofSetColor(255);
//    pingpong[(timer)%2].draw(0, 0);
//    pingpong[(timer+1)%2].draw(0, 0);
    canvas.end();
    
    noise.setZoom(noiseZoom);
    noise.update();
    
    brush.setNum(brushNumber);
    brush.setWidth(brushWidth);
    brush.damp = brushDamp;
    brush.height = brushHeight;
    brush.softness = brushSoftness;
    
    ofColor color(255,0,0);
    color.setHue(ofRandom(240));
    brush.setColor(color);
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