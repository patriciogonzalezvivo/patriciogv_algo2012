#include "testApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    width   = 1024;
    height  = 768;
    bImage  = false;
    
    grayscale.allocate(width, height);
    inverse.allocate(width, height);
    blur.allocate(width, height);
    blur.setRadius(1);
    blur.setPasses(1);
    normals.allocate(width, height);
    mask.allocate(width, height);

    brush.loadImage("brush.png");
    brush.setAnchorPercent(0.5, 0.5);
    
    bImage  = image.loadImage("image.jpg");
    bDebug  = false;
    timer   = 0;
    blurRadius = 10;
    
    for(int i = 0; i < 2; i++){
        pingpong[i].allocate(width,height);
        pingpong[i].begin();
        ofClear(0);
        image.draw(0, 0);
        pingpong[i].end();
    }
    
    string fragShader = STRINGIFY(uniform sampler2DRect backbuffer;
                                  uniform sampler2DRect normals;
                                  uniform sampler2DRect dampMap;
                                  
                                  void main(){
                                      vec2 st = gl_TexCoord[0].st;
                                      
                                      vec4 newFrame = texture2DRect(backbuffer, st);
                                      vec4 color = vec4(0,0,0,0);
                                      vec2 norm	= ( texture2DRect(normals, st).rg - 0.5 ) * 2.0;
                                      float damp = texture2DRect( dampMap, st ).r;
                                      float inc	= ( abs(norm.x) + abs(norm.y) ) * 0.5;
                                      
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
                                      
                                      float sources = 0.0;
                                      for (int i = 0; i < iTotal; i++){
                                          vec4 goingTo = ( texture2DRect( normals, st + offset[i] ) - 0.5 ) * 2.0;
                                          
                                          if ( dot(goingTo.rg,offset[i]) < -1.0/fTotal ){
                                              sources += 1.0;
                                              color += texture2DRect(backbuffer, st + offset[i]);
                                          }
                                      }
                                      color = color / sources;
                                      inc = 1.0-damp;
                                      
                                      gl_FragColor = color*(1.0-inc) + newFrame*inc ;
                                  }
                                );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
}

//--------------------------------------------------------------
void testApp::update(){
    timer++;

    mask.setTexture(grayscale.getTextureReference(), 1);
    mask.update();
    
    blur.setRadius(blurRadius);
    if ( bImage ){
        grayscale.setTexture( image.getTextureReference() );
        grayscale.update();
        inverse << grayscale;
        inverse.update();
        blur << inverse;
        blur.update();
        normals << blur;
        normals.update();
        
        pingpong[ 0].begin();
        ofSetColor(255);
        image.draw(0, 0);
        pingpong[ 0 ].end();
        
        pingpong[ 1 ].begin();
        ofSetColor(255);
        image.draw(0, 0);
        pingpong[ 1 ].end();
        
        bImage = false;
    } else {
        blur << inverse;
        blur.update();
        normals << blur;
        normals.update();
        
        pingpong[ timer%2 ].begin();
        
        shader.begin();
        shader.setUniformTexture("normals", normals.getTextureReference(), 1);
        shader.setUniformTexture("dampMap", mask.getTextureReference(), 2);
        
        ofSetColor(255);
        pingpong[ (timer+1)%2 ].draw(0, 0);
        
        shader.end();
        pingpong[ timer%2 ].end();
    }

    mask.begin();
    ofClear(0,0);
    mask.end();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()) + " blur: " + ofToString(blurRadius));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    
    pingpong[timer%2].draw(0,0);//,ofGetWidth(),ofGetHeight());
    
    if ( bDebug ){
        ofSetColor(255);
        grayscale.draw(0,0,width*0.25,height*0.25);
        inverse.draw(width*0.25,0,width*0.25,height*0.25);
        normals.draw(width*0.5,0,width*0.25,height*0.25);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' '){
        bImage = true;
    } else if (key == 'd'){
        bDebug = !bDebug;
    } else if (key == 'c'){
        mask.begin();
        ofClear(0,0);
        mask.end();
    } else if (key == OF_KEY_LEFT){
        blurRadius--;
    } else if (key == OF_KEY_RIGHT){
        blurRadius++;
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
    mask.begin(0);
    ofClear(0,0);
    brush.draw(x,y,100,100);//ofMap(mouseX,0,width,0,ofGetWidth()),ofMap(mouseY,0,height,0,ofGetHeight()),30,30);
    mask.end(0);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    if(dragInfo.files.size() > 0){
        bImage = image.loadImage(dragInfo.files[0]);
        if (bImage){
            image.update();
        }
    }
}