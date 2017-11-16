/*
 *
 */
#pragma once

#ifndef __ofxObjectLight__
#define __ofxObjectLight__

#include "ofMain.h"
#include "ofxObjectParent.h"



class ofxObjectLight :public ofxObjectParent{
public:
    ofxObjectLight();
    void draw();
    void update();
    /*
     In this case
     Pan+: to right
     Tilt+: go up
     
     In general
     Roll+: right side down
     Pitch+: go up
     Yaw+: to right
     */
    void setCameraDistance(ofVec3f _cameraPos);
    float getCameraDist();
    
private:
    
    float f_CameraDistance;//distance to camera
    float f_size;
    ofColor color;
    ofImage lightImage;
};




#endif
