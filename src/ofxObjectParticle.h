/*
 *
 */
#pragma once

#ifndef __ofxObjectParticle__
#define __ofxObjectParticle__

#include "ofMain.h"
#include "ofxObjectParent.h"
#include "ofxTrueTypeFontUC.h"



class ofxObjectParticle :public ofxObjectParent{
public:
    ofxObjectParticle();
    void draw(float _scale);
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

class ofxObjectParticleControl{
public:
    ofxObjectParticleControl();
    float translate();
    void update();
    
private:
    int i_Mode;
    float f_value;
    uint64_t startMsec;
};





#endif
