//
//  toggle.cpp
//  pdExample
//
//  Created by Mac on 8/22/20.
//

#include "toggle.h"
toggle::toggle(){
    
}
void toggle::setup(){
    
      c1 = 10;
    
    radiusM=8;
    radiusP=10;
    //com=true;
    value = false;
    
    

    
    
}
void toggle::update(){

 
    
    
    
}

void toggle::draw(){
   ofFill();
   c1--;
   dist = ofDist(xPos, base, ofGetMouseX(), ofGetMouseY());
    
 
    //( = ! )//
    //   &  //
    // <--> //
    //  ?   //
   
    
    if(ofGetMousePressed()==true){
       
          
        if(dist <= radiusP){
            if(c1<0){
            value=!value;
            cout << "printing"<<endl;
            cout << value <<endl;
            c1 = 10;
           
            }}

    }
      
   
    
    if (value == true){
           ofPushMatrix();
           ofSetColor(255, 255, 0);
           ofDrawCircle(xPos,base, radiusP);
           ofSetColor(0);
           ofDrawCircle(xPos,base, radiusM);
           ofDrawBitmapString("on", xPos-10, base-20);
           ofPopMatrix();
           
           
    }
       
       if (value == false){
           
           ofPushMatrix();
           ofSetColor(255, 255, 0);
           ofDrawCircle(xPos,base, radiusP);
           ofDrawBitmapString("off", xPos-10, base-20);
           ofPopMatrix();
             
    }
    
    
  
   
   
   

  
    //buttons and bars

//    sMap=ofMap(value,0,24,0,100);
//    ofSetRectMode(OF_RECTMODE_CORNER);
//    ofSetColor(255, 255, 0);
//    ofDrawCircle(xPos,base, radiusM);
//    ofDrawCircle(xPos2,base2, radiusP);
//    ofDrawRectangle(xPos +30, base+25, 8, -sMap);
//    ofDrawRectangle(xPos +28,(base+16)-sMap,12,12);
//
//    //symbols inside buttons
//    ofPushMatrix();
//    ofSetColor(0);
//    ofDrawBitmapString(value, xPos-5, base-20);
//    ofSetRectMode(OF_RECTMODE_CENTER);
//    ofDrawRectangle(xPos, base2, 8, 2);
//    ofDrawRectangle(xPos, base2, 2, 8);
//    ofDrawRectangle(xPos, base, 8, 2);
//    ofPopMatrix();
}
