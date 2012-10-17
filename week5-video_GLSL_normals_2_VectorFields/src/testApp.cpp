#include "testApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    width = 640;
    height = 480;
    scale = 5;
    video.initGrabber(width, height);
    
    grayscale.allocate(width, height);
    blur.allocate(width, height);
    blur.setRadius(5);
    normals.allocate(width, height);
    
    timer = 0;
    
    for(int i = 0; i < 2; i++){
        pingpong[i].allocate(width,height);
        pingpong[i].begin();
        ofClear(0);
        pingpong[i].end();
    }
    
    string fragShader = STRINGIFY(uniform sampler2DRect backbuffer;
                                  uniform sampler2DRect texture;
                                  uniform sampler2DRect normals;
                                  
                                  vec2 offset[9];
                                  
                                  void main(){
                                      vec2 st = gl_TexCoord[0].st;
                                      
                                      vec4 newFrame = texture2DRect(texture, st);
                                      vec4 color ;
                                      vec2 norm	= ( texture2DRect(normals, st).rg - 0.5 ) * 2.0;
                                      float inc	= ( abs(norm.x) + abs(norm.y) ) * 0.5;
                                      
                                      offset[0] = vec2(-1.0, -1.0);
                                      offset[1] = vec2(0.0, -1.0);
                                      offset[2] = vec2(1.0, -1.0);
                                      offset[3] = vec2(-1.0, 0.0);
                                      offset[4] = vec2(0.0, 0.0);
                                      offset[5] = vec2(1.0, 0.0);
                                      offset[6] = vec2(-1.0, 1.0);
                                      offset[7] = vec2(0.0, 1.0);
                                      offset[8] = vec2(1.0, 1.0);
                                      
                                      float sources = 0.0;
                                      for (int i = 0; i < 9; i++){
                                          if ( i != 4 ){
                                              vec4 goingTo = ( texture2DRect( normals, st + offset[i] ) - 0.5 ) * 2.0;
                                              vec2 match = vec2(goingTo.x + offset[9-i-1].x, goingTo.y + offset[9-i-1].y);
                                              
                                              if ( (match.x <= 1.0) && (match.x >= -1.0) && (match.y <= 1.0) && (match.y >= -1.0) ){
                                                  sources += 1.0;
                                                  color += texture2DRect(backbuffer, st + offset[i]);
                                              }
                                          }
                                      }
                                      
                                      color = color / sources;
                                      
                                      gl_FragColor = color*(1.0-inc) + newFrame*inc ;
                                  }
                                );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    VF.setupField(width/scale,height/scale,ofGetWidth(), ofGetHeight());
    bDrawField = false;
}

//--------------------------------------------------------------
void testApp::update(){
    timer++;
    video.update();
    
    if ( video.isFrameNew() ){
        grayscale.setTexture( video.getTextureReference() );
        grayscale.update();
        blur << grayscale;
        blur.update();
        normals << blur;
        normals.update();
        
        if ( bDrawField ){
            ofPixels normPixels;
            normals.getTextureReference().readToPixels(normPixels);
            ofPixels brightPixels;
            grayscale.getTextureReference().readToPixels(brightPixels);
            
            int scaledWidth = width/scale;
            int scaledHeight = height/scale;
            for(int x = 0; x <= scaledWidth; x++){
                for(int y = 0; y <= scaledHeight; y++){
                    ofFloatColor normalColor = normPixels.getColor(x*scale, y*scale);
                    ofFloatColor brightColor = brightPixels.getColor(x*scale, y*scale);
                    
                    int index = x + y * scaledWidth;
                    ofVec2f norm = ofVec2f((normalColor.r - 0.5) * 2.0,
                                           (normalColor.g - 0.5) * 2.0);
                    
                    float pct = 0.2;//brightColor.r;
                    VF[index] = VF[index]*(1.0-pct) + norm * pct;
                }
            }
        }
    }

    pingpong[ timer%2 ].begin();
    ofClear(0,0);
    
    shader.begin();
    shader.setUniformTexture("texture", video.getTextureReference(), 1);
    shader.setUniformTexture("normals", normals.getTextureReference(), 2);
    
    ofSetColor(255);
    pingpong[ (timer+1)%2 ].draw(0, 0);
    
    shader.end();
    pingpong[ timer%2 ].end();
    
    VF.fadeField(0.99f);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    pingpong[timer%2].draw(0,0,ofGetWidth(),ofGetHeight());
    
    if ( bDrawField ){
        ofSetColor(130,0,0, 200);
        VF.draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    bDrawField = !bDrawField;
    
    ofToggleFullscreen();
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

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

}