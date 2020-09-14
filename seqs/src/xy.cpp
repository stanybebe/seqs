#include <stdio.h>



#include "xy.h"
xy::xy(){
    
}
void xy::setup(int xPosIn, int baseIn){
    
    c1 = 5;
    rot= 50;
    radiusM=8;
    radiusP=20;
    //com=true;
    
 
    xMax = xPos+120;
    bMax = base+120;
    
    pX = xPosIn;
    pY = baseIn;
    

    
    
}
void xy::update(){

 
    
    
    
}

void xy::draw(int xPosIn, int baseIn){
   
   xPos=xPosIn;
   base =baseIn;
    
    //( = ! )//
    //   &  //
    // <--> //
    //  ?   //
   
    
    if(ofGetMousePressed()==true){
       
        if(ofGetMouseX()>xPos && ofGetMouseX()<xPos+xMax && ofGetMouseY()>base && ofGetMouseY()< base + bMax){
          //  if(c1<0){
            
            
//            posX= ofMap(ofGetMouseX(), 0, ofGetWidth(), xPos, xMax);
//            posY= ofMap(ofGetMouseY(), 0, ofGetHeight(), base, bMax);
            
            pX =ofGetMouseX();
            pY =ofGetMouseY();
       
//                if (posX <xPos){
//                    posX =xPos;
//                }
//                if (posX >xMax){
//                    posX =xMax;
//                }
//
//                if (posY <base){
//                    posY =base;
//                               }
//                if (posY >bMax){
//                    posY =bMax;
//                }
//
            valueX = floor(ofMap(pX,xPos,xPos+xMax, 0, 1000));
            valueY = floor(ofMap(pY,base,base+bMax, 0, 1000));
            
         
            
            cout << "printing"<<endl;
            cout << valueX  << "xpos" <<endl;
                //c1=10;
           
            }//}
    }
    
   
    ofPushMatrix();
    ofFill();
    ofTranslate(xPos, base);
    ofSetColor(255, 255, 0);
    //ofRectMode(OF_RECTMODE_CORNER);
    ofDrawRectangle(0+(xMax/2), 0+(bMax/2), xMax , bMax );
    
    ofSetColor(0);
    
    ofDrawRectangle(0+(xMax/2), 0+(bMax/2),1,bMax);
    ofDrawRectangle(0+(xMax/2), 0+(bMax/2),xMax,1);
    
   
    
    cout << posX <<endl;
    cout << posY <<endl;
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofNoFill();
    ofTranslate(xPos, base);
       for (int i =0; i<5; i++){
           
           ofDrawRectangle(0+(xMax/2), 0+(bMax/2), xMax-(i*(valueX*.02)) , bMax-(i*(valueY*.02)));
       }
    ofPopMatrix();
    ofFill();
    ofDrawRectangle(pX,pY, 10, 10);
    
    


    
  
    
    ofDrawBitmapString(valueX, xPos-10, base-5);
    ofDrawBitmapString(valueY, xPos+ 116, base+130);
      
}
