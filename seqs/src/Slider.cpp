//
//  Slider.cpp
//  pdExample
//
//  Created by Mac on 8/17/20.
//

#include "Slider.h"

Slider::Slider(){
    
}


//--------------------------------------------------------------
void Slider::setup(){
    
    c1 = 10;
  
  
    value=0;
    radiusM=8;
    radiusP=8;
    
    

    
    
}
void Slider::update(){
//
    xPos2 = xPos;
    base2= base-50;
 
    
    
    
}

void Slider::draw(){
    
   ofFill();
    dist = ofDist(xPos, base, ofGetMouseX(), ofGetMouseY());
    dist2 = ofDist(xPos2, base2, ofGetMouseX(), ofGetMouseY());
   
    if(ofGetMousePressed()==true){
        c1--;
    if(dist<=radiusM){
        if(c1<0){
        value--;
        ofGetMousePressed()==false;
        cout << "printing"<<endl;
        cout << value <<endl;
        c1=10;
        }
        }

    }
    
    if(ofGetMousePressed()==true){
        c1--;
    if(dist2<=radiusP){
        if(c1<0){
        value++;
        cout << "printing"<<endl;
        cout << value <<endl;
        c1=10;
        }
        }

    }
    
    if (value<=0){
        
        value = 0;
        
    }
    
    if (value>=24){
          
          value = 24;
          
      }
    
    //buttons and bars

    sMap=ofMap(value,0,24,0,100);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(4, 228, 165);
    ofDrawCircle(xPos,base, radiusM);
    ofDrawCircle(xPos2,base2, radiusP);
    ofDrawRectangle(xPos +30, base+25, 8, -sMap);
    ofDrawRectangle(xPos +28,(base+16)-sMap,12,12);
    
    //symbols inside buttons
    ofPushMatrix();
    ofSetColor(0);
    ofDrawBitmapString(value, xPos-5, base-20);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofDrawRectangle(xPos, base2, 8, 2);
    ofDrawRectangle(xPos, base2, 2, 8);
    ofDrawRectangle(xPos, base, 8, 2);
    ofPopMatrix();
}
