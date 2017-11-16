#include "ofxObjectHuman.h"

//--------------------------------------------------------------
ofxObjectHuman::ofxObjectHuman(){
    i_Id = -1;
    s_Name = "";
    s_Job = "";
    s_Group = "";
    b_StateChanged = false;
    b_NewFaceIntroductionFinished = false;
    t_LastIntroducedTime = 0;
    i_IntroducedCount = 0;
    t_State = H_STATE_COMING;
    lightImage.load("dot.png");
    i_lightCount = 0;
}

void ofxObjectHuman::regist(int _id,string _faceId, string _image, string _name,string _group,string _job){
    i_Id = _id;
    s_faceId = _faceId;
    i_Thumb.load(_image);
    s_Name = _name;
    s_Job = _job;
    s_Group = _group;
}

void ofxObjectHuman::registPath(int _id, string _faceId, string _image, vector<ofPath> _name,vector<ofPath> _group,vector<ofPath> _job){
    i_Id = _id;
    s_faceId = _faceId;
    i_Thumb.load(_image);
    
    vp_Name = _name;
    vp_Job = _job;
    vp_Group = _group;
}

void ofxObjectHuman::setCameraDistance(ofVec3f _cameraPos){
    ofVec3f buf;
    buf = _cameraPos - getPos();
    f_CameraDistance = buf.length();
}

float ofxObjectHuman::getCameraDist(){
    return f_CameraDistance;
}

H_STATE ofxObjectHuman::getState(){
    return t_State;
}

void ofxObjectHuman::setState(H_STATE _state){
    cout << s_Name << "(" << s_faceId << ") state-> " <<_state << endl;
    if(t_State != _state){
        b_StateChanged = true;
        switch(_state){
            case H_STATE_COMING:
                setPos(ofRandom(-180, 180), ofRandom(-80,80), RADIUS * ofRandom(4.0,4.2));
                break;
            case H_STATE_STAY:
                setPos(ofRandom(-180, 180), ofRandom(-40,40), RADIUS * ofRandom(1.2,1.55));
                break;
            case H_STATE_LEFT:
                break;
            case H_STATE_CANCEL:
                break;
        }
    }
    t_State = _state;
}

bool ofxObjectHuman::getStateChanged(){
    if(b_StateChanged){
        b_StateChanged = false;
        return true;
    }else{
        return false;
    }
}

string ofxObjectHuman::getName(){
    return s_Name;
}
string ofxObjectHuman::getJob(){
    return s_Job;
}
string ofxObjectHuman::getGroup(){
    return s_Group;
}

//--------------------------------------------------------------
void ofxObjectHuman::drawThumb(int _alpha){
    ofPushMatrix();
    if((_alpha==0) && (t_State == H_STATE_COMING))ofScale(0.6, 0.6);
    ofPushStyle();
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    int color;
    if(_alpha==0){
        color = int(255 - (f_CameraDistance-DISTANCE_FADE_BIAS)/DISTANCE_FADE);
        if(color<0)color=0;
        if(color>255)color=255;
    }else{
        color = _alpha;
    }
    ofSetColor(255,255,255, color);
    //ofSetColor(255,255,255, 255);
    //ofDrawCircle(0,0,0,40);
    //ofDrawCylinder(0, 0, 0, 50, 50);
    i_Thumb.draw(-THUMB_SIZE/2, -THUMB_SIZE/2, THUMB_SIZE, THUMB_SIZE);
    
    /*if(i_lightCount>0){
        if(i_lightCount%3){
            lightImage.draw(-THUMB_SIZE/2, -THUMB_SIZE/2,-1, THUMB_SIZE,THUMB_SIZE);
        }
    }*/
    
    //ofDrawBitmapString(s_Name, 0, 0);
    //ofDrawBitmapString(s_Group, 0, -THUMB_SIZE/2);
    //ofDrawBitmapString(s_Job, 0, THUMB_SIZE/2);
    ofPopStyle();
    ofPopMatrix();
}
//--------------------------------------------------------------
void ofxObjectHuman::drawName(){
    ofPushStyle();
    int color;
    color = int(255 - (f_CameraDistance-DISTANCE_FADE_BIAS)/DISTANCE_FADE);
    if(color<0)color=0;
    if(color>255)color=255;
    ofSetColor(255,255,255, color);
    vector<ofPath>::iterator iter;
    iter = vp_Name.begin();
    ofTranslate(THUMB_SIZE/2+10, -2);
    for (; iter != vp_Name.end(); ++iter){
        (*iter).setStrokeWidth(0);
        (*iter).setPolyWindingMode(OF_POLY_WINDING_NONZERO);
        (*iter).draw(0, -THUMB_SIZE/4+2);
    }
    iter = vp_Group.begin();
    for (; iter != vp_Group.end(); ++iter){
        (*iter).setStrokeWidth(0);
        (*iter).setPolyWindingMode(OF_POLY_WINDING_NONZERO);
        (*iter).draw(0, THUMB_SIZE/8+2);
    }
    iter = vp_Job.begin();
    for (; iter != vp_Job.end(); ++iter){
        (*iter).setStrokeWidth(0);
        (*iter).setPolyWindingMode(OF_POLY_WINDING_NONZERO);
        (*iter).draw(0, THUMB_SIZE/2);
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofxObjectHuman::update(int _i_AngleSpeed){
    //ofxObjectParent::update(_i_AngleSpeed);
    if(i_lightCount>0){
        i_lightCount+=1;
    }else{
        /*if(ofRandom(1.0)>0.99){
            i_lightCount = 1;
        }*/
    }
}


int ofxObjectHuman::getLightCount(){
    return i_lightCount;
}
void ofxObjectHuman::setLightCount(int _count){
    i_lightCount = _count;
    return;
}


//draw--------------------------------------------------------------
void ofxObjectHuman::draw(){
    // drawColorLineTo(ofVec3f(0,0,0),ofVec3f(255,0,0),ofVec3f(0,255,0));
    // drawColorLineTo(ofVec3f(300,0,0));
    if((t_State == H_STATE_COMING)|| (t_State == H_STATE_STAY)){
        ofPushMatrix();
        ofPushStyle();
        ofSetColor(255, 255, 255);
        ofTranslate(vf_Pos);
        ofRotateZ(-f_AnglePan);
        ofRotateX(f_AngleTilt);
        ofRotateX(90);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableAlphaBlending();
        ofSetColor(30,30,30, 100);
        //ofDrawRectangle(-THUMB_SIZE-MARGIN_SIZE, -THUMB_SIZE/2-MARGIN_SIZE, (THUMB_SIZE+MARGIN_SIZE)*2, (THUMB_SIZE+MARGIN_SIZE*2));
        ofTranslate(ofVec3f(0,0,1));
        ofScale(3.0, 3.0);
        drawThumb();
        ofRotateX(180);
        ofScale(0.8, 0.8);
        if(t_State == H_STATE_STAY){
            ofTranslate(ofVec3f(8,0,0));
            drawName();
        }
        ofPopStyle();
        ofPopMatrix();
    }
}

