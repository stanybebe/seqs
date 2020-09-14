//
//  knob.cpp
//  pdExample
//
//  Created by Mac on 8/22/20.
//

#include <stdio.h>
#include "knob.h"
knob::knob(){
    
}
void knob::setup(){
    
    c1 = 10;
    rot= 50;
    radiusM=8;
    radiusP=30;
    //com=true;
    
  
   
    
    

    
    
}
void knob::update(){

 
    
    
    
}

void knob::draw(int xPosIn, int baseIn, int range){
   
   ofFill();
   xPos=xPosIn;
   base =baseIn;
   c1--;
   dist = ofDist(xPos, base, ofGetMouseX(), ofGetMouseY());
    
 
    //( = ! )//
    //   &  //
    // <--> //
    //  ?   //
   
    
    if(ofGetMousePressed()==true){
       
        
        rot = ofGetMouseX();
        if(dist <= radiusP){
            
            if(c1<0){
                
          
           rot = ofGetMouseX();
            
            mP = ofMap(rot ,xPos-radiusP, xPos+radiusP, 0, 360);
                
         
            value = floor(ofMap(mP, 0, 360, 0, range));
            
            
            cout << "printing"<<endl;
            cout << value <<endl;
                c1=10;
           
            }}

    }
    
   // ofRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();
    ofSetColor(255, 255, 0);
    ofDrawCircle(xPos, base, radiusP);
    ofTranslate(xPos, base);
    ofRotateDeg(mP);
    cout << mP <<endl;
    ofSetColor(0);
    ofDrawCircle(0, 0, radiusP-18);
    ofDrawRectangle(0, 10, 8,30);
    ofPopMatrix();
    
    ofDrawBitmapString(value, xPos-10, base-radiusP-8);
      
}

