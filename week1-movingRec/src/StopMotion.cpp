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
    clear();
    
    width = _width;
    height = _height;
    
    activeFrame.allocate(width, height, OF_IMAGE_COLOR);
    
    nFrame = 0;
}

void StopMotion::clear(){
    //  Erase the pixels array in each frame of the array to prevent memory leaks
    //  that's because they are pointers to stack memory
    //
    for(int i = 0; i < buffer.size(); i++){
        delete [] buffer[i].pixels;
    }
    
    buffer.clear();
}

void StopMotion::load(string _folder){
    clear();
    
    //Read the directory for the images
    // We know that they are named in seq
    //
    ofDirectory dir;
    int nFiles = dir.listDir(_folder);
    if(nFiles) {
        for(int i=0; i<dir.numFiles(); i++) {
            ofPixels pixelsHolder;
            
            //  Load the image
            //
            ofLoadImage( pixelsHolder, dir.getPath(i) );
            
            if (i == 0){
                //  If it's the first one remember the width and height
                //
                allocate(pixelsHolder.getWidth(), pixelsHolder.getHeight());
            }
            
            int totalPixels = width*height*3;
            Frame newFrame;
            newFrame.pixels = new unsigned char[ totalPixels ];
            memcpy(newFrame.pixels, pixelsHolder.getPixels(), totalPixels * sizeof(unsigned char) );
            newFrame.timeStamp = ofToInt(dir.getFile(i).getFileName());
        
            buffer.push_back( newFrame );
        }
    }
}



void StopMotion::save(string _folder){
    
    //  Check if the folder exist
    //
    ofDirectory checkFolder;
    if ( checkFolder.listDir(_folder) ){
        //  if exist, clean it
        //
        for(int i=0; i < checkFolder.numFiles(); i++) {
            checkFolder.getFile(i).remove();
        }
    } else {
        //  If not, create one
        //
        checkFolder.createDirectory(_folder);
    }
    
    for(int i = 0; i < buffer.size(); i++){
        ofPixels pixelsHolder;
        pixelsHolder.allocate(width, height, 3);
        pixelsHolder.setFromPixels( buffer[i].pixels , width, height, OF_IMAGE_COLOR);
        ostringstream name;
        name << fixed << setfill('0') << setw(8) << buffer[i].timeStamp;
        ofSaveImage(pixelsHolder, _folder+"/" + name.str() + ".jpg");
    } 
}

void StopMotion::addFrame( unsigned char * _pixels ){
    
    //  If is not frames means it's the first one
    //
    if (buffer.size() == 0){
        startTime = ofGetElapsedTimeMillis()*0.05;
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
    newFrame.timeStamp = ofGetElapsedTimeMillis()*0.05 - startTime;
    
    //  Add the Frame into the dinamic array of Frames that we call buffer
    //
    buffer.push_back( newFrame );
}

void StopMotion::prevFrame(){
    if ( nFrame > 0 )
        nFrame--;
}

void StopMotion::nextFrame(){
    if ( buffer.size() != 0 ){
        nFrame = (nFrame+1)%buffer.size();
    }
}

void StopMotion::goPct(float _pct){
    
}

void StopMotion::update(){
    if ( buffer.size() != 0 ){
        activeFrame.setFromPixels( buffer[nFrame].pixels, width, height, OF_IMAGE_COLOR );
    }
}

void StopMotion::draw(int _x, int _y, int _width, int _height){
    
    if ( buffer.size() != 0 ){
        int tmpWidth, tmpHeight;
        
        if ( _width == -1)
            tmpWidth = width;
        else
            tmpWidth = _width;
        
        if ( _height == -1)
            tmpHeight = height;
        else
            tmpHeight = _height;
        
        ofPushMatrix();
        ofTranslate(_x, _y);
        
        ofSetColor(255);
        activeFrame.draw(0, 0, tmpWidth, tmpHeight);
        ofDrawBitmapString( "Frame: " + ofToString( nFrame ) + "/" + ofToString( buffer.size() ), 15, 30 );
        ofDrawBitmapString( "TimeStamp: " + ofToString( buffer[nFrame].timeStamp * 2.0 ), 15, 45 );
        
        ofPopMatrix();
    
    }
}
