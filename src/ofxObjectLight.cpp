#include "ofxObjectLight.h"

//--------------------------------------------------------------
ofxObjectLight::ofxObjectLight(){
    lightImage.load("dot.png");
}

void ofxObjectLight::setCameraDistance(ofVec3f _cameraPos){
    ofVec3f buf;
    buf = _cameraPos - getPos();
    f_CameraDistance = buf.length();
}

float ofxObjectLight::getCameraDist(){
    return f_CameraDistance;
}


//--------------------------------------------------------------
void ofxObjectLight::update(){
}



void ofxObjectLight::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(vf_Pos);
    ofRotateZ(-f_AnglePan);
    ofRotateX(f_AngleTilt);
    ofRotateX(90);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofEnableAlphaBlending();
    ofSetColor(30,30,30, 100);
    lightImage.draw(0, 0, 0, 20, 20);
    ofPopStyle();
    ofPopMatrix();
}

