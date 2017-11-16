/*
 *
 */
#pragma once

#ifndef __ofxObjectCamera__
#define __ofxObjectCamera__

#include "ofMain.h"

#define MOVE_RADIUS_MIN 1300
#define MOVE_RADIUS_MAX 1500
#define LOOKAT_RADIUS 300
#define MOVE_RADIUS_MIN2 2000
#define MOVE_RADIUS_MAX2 2400
#define MOVE_CUT_COUNT 180

class ofxObjectCamera :public ofEasyCam{
public:
    ofxObjectCamera();
    void setAutoMove();
    void changeMode();
    void update();
    void setPos(float _f_Pan,float _f_Tilt,float _f_Distance);
    ofVec3f getPos();
private:
    bool b_AutoMove;
    int i_AutoCount;
    
    int i_Mode;
    float f_PosPan;
    float f_PosTilt;
    float f_PosDistance;
    ofVec3f vf_Pos;
    float randtheta;
};


#endif
