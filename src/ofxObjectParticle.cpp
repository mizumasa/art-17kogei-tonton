#include "ofxObjectParticle.h"

//--------------------------------------------------------------
ofxObjectParticle::ofxObjectParticle(){
    lightImage.load("dot.png");

    //f_size = ofRandom(2.0, 5.0);
    f_size = ofRandom(15.0, 40.0);
    setSpeed((int(ofRandom(0.0, 1.9999))-0.5) *2* ofRandom(0.01, 0.2));
    //color = ofColor(int(ofRandom(250, 255)),int(ofRandom(164, 244)) ,int(ofRandom(62, 142)) );
    color = ofColor(int(ofRandom(250, 255)),int(ofRandom(250, 255)) ,int(ofRandom(250, 255)) );
}

void ofxObjectParticle::setCameraDistance(ofVec3f _cameraPos){
    ofVec3f buf;
    buf = _cameraPos - getPos();
    f_CameraDistance = buf.length();
}

float ofxObjectParticle::getCameraDist(){
    return f_CameraDistance;
}


//--------------------------------------------------------------
void ofxObjectParticle::update(){
    //ofxObjectParent::update(_i_AngleSpeed);
    ofxObjectParent::movePan();
}

void ofxObjectParticle::draw(float _scale){
    ofPushMatrix();
    ofPushStyle();
    //ofNoFill();
    ofSetColor(color, 180);
    //ofDrawCircle(vf_Pos, f_size);
    ofTranslate(vf_Pos);
    ofScale(_scale, _scale,_scale);
    ofDrawSphere(f_size);
    //ofTranslate(vf_Pos);
    //lightImage.draw(-80,-80,0,160,160);
    ofPopStyle();
    ofPopMatrix();
    
}



ofxObjectParticleControl::ofxObjectParticleControl(){
    i_Mode = 0;
    startMsec = ofGetElapsedTimeMillis();
}

float ofxObjectParticleControl::translate(){
    switch (i_Mode) {
        case 0:
            ofTranslate(ofVec3f(0,f_value,0));
            if(f_value<-30)return 1;
            if(f_value>=-30 && f_value<0)return (1.9 - 0.9*cos(PI * (f_value+30)/30.0));
            if(f_value>=0 && f_value<40)return 2.8;
            if(f_value>=40 && f_value<70)return (2.3 + cos(PI * (f_value-40)/30.0)/2);
            if(f_value>=70)return 1.8;
            
            //if(f_value<0){return MAX(1.0,MIN(3.0,1/abs(f_value/30)));}
            //else{return MAX(2.0,MIN(3.0,1/abs(f_value/50)));}
            //return sin(f_value);
            break;
        case 1:
            ofTranslate(ofVec3f(0,0,f_value));
            break;
        case 2:
            ofTranslate(ofVec3f(f_value,0,0));
            //return MAX(1.0,MIN(3.0,1/f_value));
            //return MAX(1.0,MIN(3.0,1/abs(f_value/30)));
            //return sin(f_value);
            break;
        default:
            break;
    }
    return 1.0;
}

void ofxObjectParticleControl::update(){
    float theta;
    switch (i_Mode) {
        case 0:
            theta = 1.0*(ofGetElapsedTimeMillis()-startMsec)/800 - PI/2;
            f_value = tan(theta)*100;
            if((f_value>3000.0) || (theta > PI/2)){
                i_Mode = 1;
                startMsec = ofGetElapsedTimeMillis();
            }
            break;
        case 1:
            f_value = (ofGetElapsedTimeMillis()-startMsec)-5000.0;
            if(f_value>4000.0){
                i_Mode = 2;
                startMsec = ofGetElapsedTimeMillis();
            }
            break;
        case 2:
            theta = 1.0*(ofGetElapsedTimeMillis()-startMsec)/800 - PI/2;
            f_value = tan(theta)*100;
            if((f_value>3000.0) || (theta > PI/2)){
                i_Mode = 0;
                startMsec = ofGetElapsedTimeMillis();
            }
            break;
        default:
            break;
    }
}

