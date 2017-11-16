#include "ofxObjectCamera.h"

//--------------------------------------------------------------
ofxObjectCamera::ofxObjectCamera(){
    b_AutoMove=false;
    i_Mode = 0;
    i_AutoCount = 0;
    f_PosPan = 0;
    f_PosTilt = 0;
    f_PosDistance = 0;
    randtheta = 0.0;

}

//--------------------------------------------------------------
void ofxObjectCamera::setAutoMove(){
    b_AutoMove=true;
    changeMode();
}

//--------------------------------------------------------------
void ofxObjectCamera::changeMode(){
    //i_Mode = int(ofRandom(0, 2))%2;
    i_Mode = (i_Mode+1)%2;
    float distanceBuf;
    distanceBuf = ofRandom(-1, 1);
    if(distanceBuf>0){
        distanceBuf = ofRandom(MOVE_RADIUS_MIN,MOVE_RADIUS_MAX);
    }else{
        distanceBuf = ofRandom(MOVE_RADIUS_MIN2,MOVE_RADIUS_MAX2);
    }
    switch (i_Mode) {
        case 0:
            setPos(ofRandom(20, 180), 20, distanceBuf);
            randtheta = ofRandom(0.0, 2*PI);
            lookAt(ofVec3f(LOOKAT_RADIUS*sin(randtheta),LOOKAT_RADIUS*cos(randtheta),0), ofVec3f(0,0,1));
            break;
        case 1:
            setPos(ofRandom(-20, -180), 20, distanceBuf);
            randtheta = ofRandom(0.0, 2*PI);
            lookAt(ofVec3f(LOOKAT_RADIUS*sin(randtheta),LOOKAT_RADIUS*cos(randtheta),0), ofVec3f(0,0,1));
            break;
        default:
            break;
    }
}



//--------------------------------------------------------------
void ofxObjectCamera::setPos(float _f_Pan,float _f_Tilt,float _f_Distance){
    f_PosPan = _f_Pan;
    f_PosTilt = _f_Tilt;
    f_PosDistance = _f_Distance;
    vf_Pos.x = _f_Distance * cos(PI * _f_Tilt / 180.0) * sin(PI * _f_Pan / 180.0);
    vf_Pos.y = _f_Distance * cos(PI * _f_Tilt / 180.0) * cos(PI * _f_Pan / 180.0);
    vf_Pos.z = _f_Distance * sin(PI * _f_Tilt / 180.0);
    setPosition(vf_Pos);
}

//--------------------------------------------------------------
ofVec3f ofxObjectCamera::getPos(){
    return vf_Pos;
}

//--------------------------------------------------------------
void ofxObjectCamera::update(){
    if(b_AutoMove){
        switch (i_Mode) {
            case 0:
                setPos(f_PosPan+0.5, f_PosTilt, f_PosDistance-2.1);
                lookAt(ofVec3f(LOOKAT_RADIUS*sin(randtheta),LOOKAT_RADIUS*cos(randtheta),0), ofVec3f(0,0,1));
                //lookAt(ofVec3f(0,MOVE_RADIUS_MAX,0), ofVec3f(0,0,1));
                break;
            case 1:
                setPos(f_PosPan-0.5, f_PosTilt, f_PosDistance+2.1);
                lookAt(ofVec3f(LOOKAT_RADIUS*sin(randtheta),LOOKAT_RADIUS*cos(randtheta),0), ofVec3f(0,0,1));
                //lookAt(ofVec3f(0,MOVE_RADIUS_MAX,0), ofVec3f(0,0,1));
                break;
            default:
                break;
        }
        i_AutoCount += 1;
        if(i_AutoCount > MOVE_CUT_COUNT){
            i_AutoCount = 0;
            changeMode();
        }
    }
}


