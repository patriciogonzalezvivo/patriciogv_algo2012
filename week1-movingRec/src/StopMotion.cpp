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
    
    //  For geting a copy of this frame we have to copy every single pixel
    //  So we start by knowing the total amount of pixels
    //
    int totalPixles = width*height*3;
    
    //  Making a new frame with the right amount of pixels on the "pixles" array
    //
    Frame newFrame;
    newFrame.pixels = new unsigned char[totalPixles];
    
    //  For copying each single pixel of the image we can use this C function
    //
    
    memcpy(newFrame.pixels, _pixels, totalPixles * sizeof(unsigned char) );
    
    //
    //  Witch is the same to say:
    //
    
//    for(int i = 0; i < totalPixles ; i++){
//        newFrame.pixels[i] = _pixels[i];
//    }
    
    //  Put a timeStamp on it.
    //  This could be handy if we record in other speed that is not 24 per second
    //
    newFrame.timeStamp = ofGetElapsedTimeMillis() - startTime;
    
    //  Add the Frame into the dinamic array of Frames that we call buffer
    //
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
