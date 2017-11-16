#include "ofxObjectParent.h"

//--------------------------------------------------------------
ofxObjectParent::ofxObjectParent(){
    //init
    f_PosPan = 0;
    f_PosTilt = 0;
    f_PosDistance = 0;
    f_AnglePan = 0;
    f_AngleTilt = 0;
    vf_NormalVec = ofVec3f(0,0,0);
    vf_Pos = ofVec3f(0,0,0);
    //f_PosX = 0;
    //f_PosY = 0;
    //f_PosZ = 0;
    f_speed = ofRandom(-0.3, 0.3);
}
//--------------------------------------------------------------
void ofxObjectParent::setup(){
}
//--------------------------------------------------------------
void ofxObjectParent::set(string s){
}

void ofxObjectParent::setSpeed(float _f_speed){
    f_speed = _f_speed;
}

//--------------------------------------------------------------
void ofxObjectParent::movePan(){
    float pDir;
    pDir = f_PosPan + f_speed;
    if(pDir >= 360)pDir-=360;
    if(pDir < 0)pDir+=360;
    if(!isnan(pDir)){
        setPos(pDir,f_PosTilt,f_PosDistance);
    }
}


//--------------------------------------------------------------
void ofxObjectParent::move(){
    setPos(f_PosPan+f_speed, f_PosTilt, f_PosDistance);
    setNormalVec(- getPos());
}
//--------------------------------------------------------------
void ofxObjectParent::update(int _i_AngleSpeed){
    bool b_AngleChange = false;
    if(abs(f_AnglePanDest - f_AnglePan) < 1.0){
        f_AnglePan = f_AnglePanDest;
        b_AngleChange = true;
    }
    if(abs(f_AngleTiltDest - f_AngleTilt) < 1.0){
        f_AngleTilt = f_AngleTiltDest;
        b_AngleChange = true;
    }
    if(f_AnglePanDest != f_AnglePan){
        f_AnglePan = (f_AnglePanDest + f_AnglePan * _i_AngleSpeed)/ (1 + _i_AngleSpeed);
        b_AngleChange = true;
    }
    if(f_AngleTiltDest != f_AngleTilt){
        f_AngleTilt = (f_AngleTiltDest + f_AngleTilt * _i_AngleSpeed)/ (1 + _i_AngleSpeed);
        b_AngleChange = true;
    }
    if(b_AngleChange){
        vf_NormalVec.x = cos(PI * f_AngleTilt / 180.0) * sin(PI * f_AnglePan / 180.0);
        vf_NormalVec.y = cos(PI * f_AngleTilt / 180.0) * cos(PI * f_AnglePan / 180.0);
        vf_NormalVec.z = sin(PI * f_AngleTilt / 180.0);
    }
}

bool ofxObjectParent::drawLightTo(ofVec3f _dst,float __f_pos,ofImage _image){
    ofVec3f pos;
    ofVec3f stPos,enPos;
    stPos = vf_Pos + ((_dst-vf_Pos).getNormalized()*100);
    enPos = _dst + ((vf_Pos-_dst).getNormalized()*100);

    //float _f_pos = 1 - cos(__f_pos/100.0);
    float _f_pos =  (1+f((__f_pos/3 - 100/8.0)))/2.0;
    //if((_dst-vf_Pos).length()>_f_pos){
    if(0.98 > _f_pos){
        if((_f_pos < 0.3) && (ofRandom(-2.0, 1.0)>0)){
            return false;
        }
        //pos = vf_Pos + ((_dst-vf_Pos).getNormalized()*_f_pos);
        pos = stPos + ((enPos-stPos)*_f_pos);
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(pos);
        //cout <<"angle"<< buf << v_ObjectHuman[i].getName()<<lightImage.getWidth()<<endl;
        ofRotateZ(-f_AnglePan);
        ofRotateX(f_AngleTilt);
        ofRotateX(90);
        ofScale( 3 - cos(PI*_f_pos*2)*2,3 - cos(PI*_f_pos*2)*2);
        ofScale(2, 2);
        //ofScale(10, 10);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableAlphaBlending();
        ofSetColor(255,255,255, 255);
        _image.draw(-10,-10, 0, 20, 20);
        
        ofPopStyle();
        ofPopMatrix();
        return false;
    }else{
        return true;
    }
}

float f(float x){
    return x/(1+abs(x));
}


void ofxObjectParent::drawColorLineTo(ofVec3f _pos,ofVec3f _color,ofVec3f _colorTo){
    ofVec3f pos1,pos2,color;
    ofVec3f stPos,enPos;
    stPos = vf_Pos + ((_pos-vf_Pos).getNormalized()*100);
    enPos = _pos + ((vf_Pos-_pos).getNormalized()*100);
    ofPushStyle();
    for(int i=0;i < C_DIV_NUM;i++){
        pos1 = (stPos * i + enPos * (C_DIV_NUM - i))/C_DIV_NUM;
        pos2 = (stPos * (i+1) + enPos * (C_DIV_NUM - (i+1)))/C_DIV_NUM;
        color = (_color * i + _colorTo * ((C_DIV_NUM - 1) - i))/(C_DIV_NUM-1);
        ofSetLineWidth(0.2);
        ofSetColor(ofColor(int(color[0]),int(color[1]),int(color[2])));
        ofDrawLine(pos1, pos2);
    }
    ofPopStyle();
}

void ofxObjectParent::drawGrayLineTo(ofVec3f _pos){
    ofPushStyle();
    ofSetColor(20, 20, 20,100);
    ofSetLineWidth(0.2);
    ofVec3f stPos,enPos;
    stPos = vf_Pos + ((_pos-vf_Pos).getNormalized()*100);
    enPos = _pos + ((vf_Pos-_pos).getNormalized()*100);
    ofDrawLine(stPos, enPos);
    ofPopStyle();
}


void ofxObjectParent::drawColorLineTo(ofVec3f _pos){
    ofVec3f hsv1,hsv2,hsv3;
    hsv1 = ofVec3f(int(pos2angle(_pos[0],_pos[1])+180.0+ofGetElapsedTimeMillis()/10.0),40,40);
    hsv2 = ofVec3f(int(pos2angle(vf_Pos[0],vf_Pos[1])+180.0+ofGetElapsedTimeMillis()/10.0),40,40);
    ofVec3f pos1,pos2;
    ofVec3f stPos,enPos;
    stPos = vf_Pos + ((_pos-vf_Pos).getNormalized()*100);
    enPos = _pos + ((vf_Pos-_pos).getNormalized()*100);
    ofPushStyle();
    for(int i=0;i < C_DIV_NUM;i++){
        pos1 = (stPos * i + enPos * (C_DIV_NUM - i))/C_DIV_NUM;
        pos2 = (stPos * (i+1) + enPos * (C_DIV_NUM - (i+1)))/C_DIV_NUM;
        hsv3 = (hsv1 * i + hsv2 * ((C_DIV_NUM - 1) - i))/(C_DIV_NUM-1);
        int alpha = 150;
        alpha = MIN(150,int(150 * pos1.length() / (900)));
        ofSetHSVColor((int(hsv3[0]))%360,int(hsv3[1]),int(hsv3[2]),alpha);
        ofSetLineWidth(0.2);
        ofDrawLine(pos1, pos2);
    }
    ofPopStyle();
}



void ofxObjectParent::drawLineTo(ofVec3f _pos){
    ofDrawLine(vf_Pos, _pos);
}

void ofxObjectParent::drawLineDir(ofVec3f _direction){
    ofDrawLine(vf_Pos, vf_Pos+_direction);
}


//--------------------------------------------------------------
void ofxObjectParent::setPos(float _f_Pan,float _f_Tilt,float _f_Distance){
    f_PosPan = _f_Pan;
    f_PosTilt = _f_Tilt;
    f_PosDistance = _f_Distance;
    vf_Pos.x = _f_Distance * cos(PI * _f_Tilt / 180.0) * sin(PI * _f_Pan / 180.0);
    vf_Pos.y = _f_Distance * cos(PI * _f_Tilt / 180.0) * cos(PI * _f_Pan / 180.0);
    vf_Pos.z = _f_Distance * sin(PI * _f_Tilt / 180.0);
}
//--------------------------------------------------------------
ofVec3f ofxObjectParent::getPos(){
    return vf_Pos;
}

//--------------------------------------------------------------
ofVec3f ofxObjectParent::getPosTheta(){
    return ofVec3f(f_PosPan,f_PosTilt,f_PosDistance);
}

ofVec2f ofxObjectParent::getAnglePanTilt(){
    return ofVec2f(f_AnglePan, f_AngleTilt);
}

ofVec2f ofxObjectParent::getSekiryoku(ofVec3f _posTheta){
    float tiltForce = 0;
    if((abs(f_PosPan - _posTheta[0]) < 20) || (abs(f_PosPan - _posTheta[0]) > 340)){
        int tiltDist = (int)(f_PosTilt - _posTheta[1]);
        if(tiltDist > 0 && tiltDist < 40 && f_PosTilt<40){
            tiltForce = SEKIRYOKU/(tiltDist+1);
        }
        if(tiltDist > -40 && tiltDist < 0 && f_PosTilt > -40){
            tiltForce = SEKIRYOKU/(tiltDist-1);
        }
    }
    ofVec2f retBuf=ofVec2f(0,tiltForce);
    if(abs(f_PosTilt - _posTheta[1]) < 40){
        int panDist = (int)(f_PosPan - _posTheta[0]);
        if(panDist > 0 && panDist < 90){
            retBuf=ofVec2f( SEKIRYOKU/(panDist+1),tiltForce);
        }
        if(panDist > -90 && panDist < 0){
            retBuf=ofVec2f( SEKIRYOKU/(panDist-1),tiltForce);
        }
        if(panDist > 270){
            retBuf=ofVec2f( -SEKIRYOKU/((360-panDist)+1),tiltForce);
        }
        if(panDist < -270){
            retBuf=ofVec2f(SEKIRYOKU/((360+panDist)+1),tiltForce);
        }
    }
    return retBuf;
}

void ofxObjectParent::moveAngleDir(float _f_Pan,float _f_Tilt){
    float pDir,tDir,dDir;
    float m_f_Pan,m_f_Tilt;
    m_f_Pan = MAX(-4,MIN(_f_Pan,4));
    m_f_Tilt = MAX(-4,MIN(_f_Tilt,4));
    pDir = f_PosPan + _f_Pan;
    tDir = f_PosTilt + _f_Tilt;
    if(pDir >= 360)pDir-=360;
    if(pDir < 0)pDir+=360;
    if(tDir > 80)tDir=80;
    if(tDir < -80)tDir=-80;
    if(!isnan(pDir) && !isnan(tDir)){
        setPos(pDir,tDir,f_PosDistance);
    }
}


//--------------------------------------------------------------
ofVec3f ofxObjectParent::getNorm(){
    return vf_NormalVec;
}
//--------------------------------------------------------------
void ofxObjectParent::setAngle(float _f_Pan,float _f_Tilt){
    f_AnglePan = _f_Pan;
    f_AngleTilt = _f_Tilt;
    f_AnglePanDest = _f_Pan;
    f_AngleTiltDest = _f_Tilt;
    vf_NormalVec.x = cos(PI * _f_Tilt / 180.0) * sin(PI * _f_Pan / 180.0);
    vf_NormalVec.y = cos(PI * _f_Tilt / 180.0) * cos(PI * _f_Pan / 180.0);
    vf_NormalVec.z = sin(PI * _f_Tilt / 180.0);
}
//--------------------------------------------------------------
void ofxObjectParent::setRandomAngle(){
    moveAngle(f_PosPan + ofRandom(-10, 10), f_PosTilt + ofRandom(-10, 10));
}
//--------------------------------------------------------------
void ofxObjectParent::moveAngle(float _f_Pan,float _f_Tilt){
    f_AnglePanDest = _f_Pan;
    f_AngleTiltDest = _f_Tilt;
}
//--------------------------------------------------------------
void ofxObjectParent::setNormalVec(ofVec3f _norm){
    vf_NormalVec = _norm/_norm.length();
    //鏡の法線で指定
    //default (0,-1,0)
    //atan();
    if(_norm.y==0){
        f_AnglePan = 90.0;
    }else{
        if(_norm.y > 0){
            f_AnglePan = 180.0 * atan(_norm.x / _norm.y) / PI;
        }else{
            if(_norm.x > 0){
                f_AnglePan = 180 + 180.0 * atan(_norm.x / _norm.y) / PI;
            }else{
                f_AnglePan = 180.0 * atan(_norm.x / _norm.y) / PI - 180;
            }
        }
    }
    if((_norm.y ==0) and (_norm.x == 0)){
        f_AngleTilt = 90.0;
    }else{
        f_AngleTilt = 180.0 * atan(_norm.z / ofVec2f(_norm.x,_norm.y).length()) / PI;
    }
    //cout << "set" << f_AnglePan << " " << f_AngleTilt << endl;
    f_AngleTiltDest = f_AngleTilt;
    f_AnglePanDest = f_AnglePan;
}

//--------------------------------------------------------------
ofVec3f ofxObjectParent::convPanTilt2Vec(float _f_Pan,float _f_Tilt){
    ofVec3f out;
    out.x = cos(PI * _f_Tilt / 180.0) * sin(PI * _f_Pan / 180.0);
    out.y = cos(PI * _f_Tilt / 180.0) * cos(PI * _f_Pan / 180.0);
    out.z = sin(PI * _f_Tilt / 180.0);
    return out;
}
//--------------------------------------------------------------
ofVec2f ofxObjectParent::convVec2PanTilt(ofVec3f _norm){
    float _f_AnglePan,_f_AngleTilt;
    if(_norm.y==0){
        _f_AnglePan = 90.0;
    }else{
        if(_norm.y > 0){
            _f_AnglePan = 180.0 * atan(_norm.x / _norm.y) / PI;
        }else{
            if(_norm.x > 0){
                _f_AnglePan = 180 + 180.0 * atan(_norm.x / _norm.y) / PI;
            }else{
                _f_AnglePan = 180.0 * atan(_norm.x / _norm.y) / PI - 180;
            }
        }
    }
    if((_norm.y ==0) and (_norm.x == 0)){
        _f_AngleTilt = 90.0;
    }else{
        _f_AngleTilt = 180.0 * atan(_norm.z / ofVec2f(_norm.x,_norm.y).length()) / PI;
    }
    return ofVec2f(_f_AnglePan,_f_AngleTilt);
}

//--------------------------------------------------------------
void ofxObjectParent::setAngleBetween(ofVec3f _pos1,ofVec3f _pos2){
    //鏡の法線を定める
    ofVec3f buf1,buf2;
    buf1 = _pos1 - vf_Pos;
    buf2 = _pos2 - vf_Pos;
    //cout<<buf1 << ":" << buf2<<endl;
    buf1 /= buf1.length();
    buf2 /= buf2.length();
    //cout<<buf1 << ":" << buf2<<endl;
    setNormalVec(buf1 + buf2);
}


float pos2angle(float x,float y){
    float f_AnglePan;
    if(y==0){
        if(x > 0){
            f_AnglePan = 90.0;
        }else{
            f_AnglePan = -90.0;
        }
    }else{
        if(y > 0){
            f_AnglePan = 180.0 * atan(x / y) / PI;
        }else{
            if(x > 0){
                f_AnglePan = 180 + 180.0 * atan(x / y) / PI;
            }else{
                f_AnglePan = 180.0 * atan(x / y) / PI - 180;
            }
        }
    }
    return f_AnglePan;
}

void ofSetHSVColor(int _h, int _s, int _v, int _a){
    
    float s = (float)_s/100, v = (float)_v/100;
    if (s==0) {
        ofSetColor(255*v, 255*v, 255*v);
        return;
    }
    float r,g,b;
    int hi = (int)(floor((double)_h/60))%6;
    float f = (float)_h/60-(float)hi;
    float p = v*(1-s);
    float q = v*(1-f*s);
    float t = v*(1-(1-f)*s);
    switch (hi) {
        case 0:
            r=v;
            g=t;
            b=p;
            break;
        case 1:
            r=q;
            g=v;
            b=p;
            break;
        case 2:
            r=p;
            g=v;
            b=t;
            break;
        case 3:
            r=p;
            g=q;
            b=v;
            break;
        case 4:
            r=t;
            g=p;
            b=v;
            break;
        case 5:
            r=v;
            g=p;
            b=q;
            break;
        default:
            break;
    }
    ofSetColor(255*r, 255*g, 255*b, _a);
}

void ofSetHSVColor(int _h, int _s, int _v){
    ofSetHSVColor(_h, _s, _v, 255);
}
