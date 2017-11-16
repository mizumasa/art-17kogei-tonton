/*
 *  ofxRollingCam.cpp
 *
 *  Copyright (c) 2014, saebashi, https://twitter.com/saebashi
 *  All rights reserved.
 */

#include "ofxRollingCam.h"
//--------------------------------------------------------------

ofxRollingCam::ofxRollingCam(){
    cam.disableMouseInput();
    scaleN=1.0;
    scaleT=scaleN;
    //cam.setscaleance(ofGetHeight());
    
    camSpeed=DEFAULT_CAM_SP;
    
}
//--------------------------------------------------------------
void ofxRollingCam::setup(float _camSpeed,float _scaleance){
    cam.disableMouseInput();
    scaleN=_scaleance;
    scaleT=scaleN;
    camSpeed=ofClamp(_camSpeed,0,1);
    cam.setNearClip(500.0);
    cam.setFov(30);
    //cout <<"fo"<< cam.getFov()<<endl;
}

//--------------------------------------------------------------
void ofxRollingCam::update(){
    posS.set(posT.x-posN.x,posT.y-posN.y,posT.z-posN.z);
    posS.set(posS.x*camSpeed,posS.y*camSpeed,posS.z*camSpeed);
    posN.set(posN.x+posS.x, posN.y+posS.y,posN.z+posS.z);
    
    scaleS=scaleT-scaleN;
    scaleS=scaleS*camSpeed;
    scaleN=scaleN+scaleS;
    //cam.setscaleance(scaleN);
    vf_Pos.x = scaleN * CAM_RADIUS * cos(PI * posN.y / 180.0) * sin(PI * posN.x / 180.0);
    vf_Pos.y = scaleN * CAM_RADIUS * cos(PI * posN.y / 180.0) * cos(PI * posN.x / 180.0);
    vf_Pos.z = scaleN * CAM_RADIUS * sin(PI * posN.y / 180.0);
    cam.setPosition(vf_Pos);
    cam.lookAt(ofVec3f(0,0,0), ofVec3f(0,0,1));
}
//--------------------------------------------------------------

void ofxRollingCam::begin(){
    ofPushMatrix();
    cam.begin();
    //ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    //ofRotateX(-posN.y);
    //ofRotateY(posN.x);
    //ofRotateZ(posN.z);
    //ofRotateX(90);
    //ofScale(scaleN, scaleN,scaleN);
}
void ofxRollingCam::begin(ofRectangle view){
    ofPushMatrix();
    cam.begin(view);
    //ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    //ofRotateX(-posN.y);
    //ofRotateY(posN.x);
    //ofRotateZ(posN.z);
    //ofRotateX(90);
    //ofScale(scaleN, scaleN,scaleN);
}

//--------------------------------------------------------------

void ofxRollingCam::end(){
    cam.end();
    ofPopMatrix();
    
}
//--------------------------------------------------------------

void ofxRollingCam::setCamSpeed(float _camSpeed){
    camSpeed=ofClamp(_camSpeed,0,1);
}
//--------------------------------------------------------------

void ofxRollingCam::setRandomPos(float _randomAngle){
    ofVec3f ram3f;
    int ramSize=_randomAngle;
    ram3f.set(ofRandom(-ramSize, ramSize),ofRandom(-ramSize, ramSize),ofRandom(-ramSize, ramSize));
    posT.set(ofRandom(posN.x,(int)(posN.x+ram3f.x)%360), ofRandom(posN.y,(int)(posN.y+ram3f.y)%360),ofRandom(posN.z,(int)(posN.z+ram3f.z)%360));
}
//--------------------------------------------------------------

void ofxRollingCam::setPos(float _pan,float _tilt,float _roll){
    if(isinf(_pan) || isnan(_pan)){
        cout << "NAN********************************"<<endl;
        return;
    }
    if(isinf(_tilt) || isnan(_tilt)){
        cout << "NAN********************************"<<endl;
        return;
    }
    if(isinf(_roll) || isnan(_roll)){
        cout << "NAN********************************"<<endl;
        return;
    }
    posT.set(_pan,_tilt,_roll);
}
//--------------------------------------------------------------

void ofxRollingCam::setRandomScale(float _min,float _max){
    scaleT=ofRandom(_min,_max);
}
//--------------------------------------------------------------

void ofxRollingCam::setScale(float _scale){
    scaleT=_scale;
}

//--------------------------------------------------------------
//void testApp::mousePressed(int x, int y, int button){
//    camSetRandom();
//}

