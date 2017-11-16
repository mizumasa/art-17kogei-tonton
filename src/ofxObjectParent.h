/*
 *
 */
#pragma once

#ifndef __ofxObjectParent__
#define __ofxObjectParent__

#include "ofMain.h"
#define C_DIV_NUM 10
#define SEKIRYOKU 1.0


class ofxObjectParent{
public:
    ofxObjectParent();
    void setup();
    void set(string s);
    void setSpeed(float _f_speed);
    void movePan();
    void move();
    void update(int _i_AngleSpeed);
    void drawColorLineTo(ofVec3f _pos,ofVec3f _color,ofVec3f _colorTo);
    void drawColorLineTo(ofVec3f _pos);
    void drawGrayLineTo(ofVec3f _pos);
    void drawLineTo(ofVec3f _pos);
    void drawLineDir(ofVec3f _direction);
    bool drawLightTo(ofVec3f _dst,float _f_pos,ofImage _image);

    virtual void setPos(float _f_Pan,float _f_Tilt,float _f_Distance);
    ofVec3f getPos();
    ofVec3f getPosTheta();
    ofVec2f getSekiryoku(ofVec3f _posTheta);
    void moveAngleDir(float _f_Pan,float _f_Tilt);
    ofVec3f getNorm();
    void setAngle(float _f_Pan,float _f_Tilt);
    void setRandomAngle();
    void moveAngle(float _f_Pan,float _f_Tilt);
    void setNormalVec(ofVec3f _norm);
    void setAngleBetween(ofVec3f _pos1,ofVec3f _pos2);
    /*
     In this case
     Pan+: to right
     Tilt+: go up
     
     In general
     Roll+: right side down
     Pitch+: go up
     Yaw+: to right
     */
    ofVec3f convPanTilt2Vec(float _f_Pan,float _f_Tilt);
    ofVec2f convVec2PanTilt(ofVec3f _vec);
    ofVec2f getAnglePanTilt();

protected:
    float f_PosPan;
    float f_PosTilt;
    float f_PosDistance;
    float f_AnglePan;
    float f_AngleTilt;
    float f_AnglePanDest;
    float f_AngleTiltDest;
    ofVec3f vf_NormalVec;
    ofVec3f vf_Pos;
    float f_speed;
    //float f_PosX;
    //float f_PosY;
    //float f_PosZ;
};

//h: 0-360 , s: 0-100, v: 0-100, a: 0-255
void ofSetHSVColor(int _h, int _s, int _v, int _a);
void ofSetHSVColor(int _h, int _s, int _v);

float pos2angle(float x,float y);

float f(float x);

#endif
