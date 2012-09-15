//
//  StopMotion.cpp
//  week1-movingRec
//
//  Created by Patricio Gonzalez Vivo on 9/15/12.
//
//

#include "StopMotion.h"

StopMotion::StopMotion(){
    
}

void StopMotion::allocate( int _width, int _height ){
    width = _width;
    height = _height;
}

void StopMotion::load(string _path){
    
}

void StopMotion::save(string _path){
    
    ofDirectory dirHolder;
    
    for(int i = 0; i < buffer.size(); i++){
        ofPixels pixelsHolder;
        pixelsHolder.allocate(width, height, 3);
        pixelsHolder.setFromPixels( buffer[i].pixels , width, height, OF_IMAGE_COLOR);
        ofSaveImage(pixelsHolder, _path+"/"+ofToString(i)+".jpg");
    }
}

void StopMotion::addFrame( unsigned char * _pixels ){
    
    //  If is not frames means it's the first one
    //
    if (buffer.size() == 0){
        startTime = ofGetElapsedTimeMillis();
    }
    
    //  Copy pixel by pixel otherwise it copy the pointer reference to buffer witch is allways the same
    //  I know it's a C funtion for this... memcopy?
    //
    int totalPixles = width*height*3;
    Frame newFrame;
    newFrame.pixels = new unsigned char[totalPixles];
    
    for(int i = 0; i < totalPixles ; i++){
        newFrame.pixels[i] = _pixels[i];
    }
    
    newFrame.timeStamp = ofGetElapsedTimeMillis() - startTime;
    
    buffer.push_back( newFrame );
}

void StopMotion::nextFrame(){
    
}

void StopMotion::goFrame(){
    
}

void StopMotion::goPct(float _pct){
    
}

void StopMotion::draw(int _x, int _y, int _width, int _height){
    
}
