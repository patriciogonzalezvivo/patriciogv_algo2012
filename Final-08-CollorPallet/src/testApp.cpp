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
    
    canvas.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    canvas.begin();
    ofClear(0,0);
    canvas.end();
    
    width = ofGetScreenWidth();
    height = 230;
    
    palleteImg.loadImage("palette.png");
    
    pallete.allocate(width, height);
    pallete.begin();
    ofClear(0, 0);
    pallete.end();
    
    grayscale.allocate(width, height);
    blur.allocate(width, height);
    normals.allocate(width, height);
    
    noise.allocate(width, height);
    noise.setZoom(60);
    noise.update();
    
    for(int i = 0; i < 2; i++){
        pingpong[i].allocate(width,height);
        pingpong[i].begin();
        ofClear(0,0);
        palleteImg.draw(0, 0, width, (width/palleteImg.getWidth()) * palleteImg.getHeight() );
        pingpong[i].end();
    }
    
    string fragShader = STRINGIFY(uniform sampler2DRect normals;
                                  uniform sampler2DRect pallete;
                                  uniform sampler2DRect backbuffer;
                                  uniform sampler2DRect dampMap;
                                  
                                  void main(){
                                      vec2 st = gl_TexCoord[0].st;
                                      
                                      vec4 newFrame = texture2DRect(backbuffer, st);
                                      vec4 color = texture2DRect(pallete,st);
                                      vec2 norm	= ( texture2DRect(normals, st).rg - 0.5 ) * 2.0;
                                      float damp = texture2DRect( dampMap, st ).r;
                                      
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
                                          
                                          if ( dot(goingTo.rg,offset[i]) < -1.0/fTotal ) {
                                              vec4 target = texture2DRect(backbuffer, st + offset[i]);
                                              
                                              if (target.a > 0.5){
                                                  sources += 1.0;
                                                  color += target;
                                              }
                                          }
                                      }
                                      
                                      color = color / sources;
                                      float inc = 1.0-damp;
                                      
                                      gl_FragColor = color*(1.0-inc) + newFrame*inc ;
                                  });
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    timer   = 0;
    bDebug  = false;
    bPallete= true;
    bShader = true;
}

//--------------------------------------------------------------
void testApp::update(){
    brush.update();

    canvas.begin();
    brush.draw();
    canvas.end();
    
    if (bPallete){
        blur.setRadius(blurRadius);
        
        pallete.begin();
        
        ofSetColor(255);
        palleteImg.draw(0, 0, width, (width/palleteImg.getWidth()) * palleteImg.getHeight() );
        
        brush.draw();
        
        pallete.end();
        
        grayscale.begin();
        ofSetColor(255);
    
        noise.draw();
        grayscale.end();
        grayscale.update();
        blur << grayscale;
        
        blur.update();
        normals << blur;
        normals.update();
        
        pingpong[ timer%2 ].begin();

        shader.begin();
        shader.setUniformTexture("normals", normals.getTextureReference(), 0);
        shader.setUniformTexture("backbuffer", pingpong[(timer+1)%2], 2);
        shader.setUniformTexture("dampMap", grayscale.getTextureReference(), 3);
        
        pallete.draw(0,0);
        
        shader.end();
        
        pingpong[ timer%2 ].end();
        
        timer++;
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    canvas.draw(0, 0);
    
    if (bPallete){
        ofSetColor(255);
        pingpong[(timer+1)%2].draw(0, 0);
        ofSetColor(255,100);
        palleteImg.draw(0, 0, width, (width/palleteImg.getWidth()) * palleteImg.getHeight() );
    }
    
    if (bDebug){
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
            ofClear(0,0);
            pingpong[i].end();
        }
    } else if (key == 'f'){
        ofToggleFullscreen();
    } else if (key == 'n'){
        noise.setZoom(60);
        noise.update();
    } else if (key == 's'){
        bShader = !bShader;
    } else if ( key == 'p'){
        bPallete = !bPallete;
        
        brush.clear();
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
    if (bPallete){
        noise.update();
        brush.pickColorFrom( pingpong[(timer+1)%2].getTextureReference(),0.1,0.01 );
    }
    brush.set(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    brush.init(brushNumber);
    brush.setWidth(brushWidth);
    
    if (bPallete){
        brush.pickColorFrom( pingpong[(timer+1)%2].getTextureReference(),1.0,0.01 );
    }
    
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
//    width = w;
//    
//    grayscale.allocate(width, height);
//    blur.allocate(width, height);
//    normals.allocate(width, height);
//    
//    noise.allocate(width, height);
//    for(int i = 0; i < 2; i++){
//        pingpong[i].allocate(width,height);
//    }
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}