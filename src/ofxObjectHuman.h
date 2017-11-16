/*
 *
 */
#pragma once

#ifndef __ofxObjectHuman__
#define __ofxObjectHuman__

#include "ofMain.h"
#include "ofxObjectParent.h"
#include "ofxObjectLight.h"
#include "ofxTrueTypeFontUC.h"

#define DISTANCE_FADE 35
#define DISTANCE_FADE_BIAS 1200

#define THUMB_SIZE 50
#define THUMB_SIZE_TITLE 100
#define MARGIN_SIZE 4

#define RADIUS 900


enum H_STATE{
    H_STATE_COMING = 0,
    H_STATE_STAY,
    H_STATE_LEFT,
    H_STATE_CANCEL,
};


class ofxObjectHuman :public ofxObjectParent{
public:
    ofxObjectHuman();
    void draw();
    void drawThumb(int _alpha=0);
    void drawName();
    void update(int _i_AngleSpeed);
    /*
     In this case
     Pan+: to right
     Tilt+: go up
     
     In general
     Roll+: right side down
     Pitch+: go up
     Yaw+: to right
     */
    void regist(int _id, string _faceId, string _image, string _name,string _group,string _job);
    void registPath(int _id, string _faceId, string _image, vector<ofPath> _name,vector<ofPath> _group,vector<ofPath> _job);
    H_STATE getState();
    void setState(H_STATE _state);
    bool getStateChanged();
    string getName();
    string getJob();
    string getGroup();
    vector<string> vs_WorksName;
    vector<string> vs_WorksUrl;
    vector<int> vi_FriendsId;
    vector<string> vs_FriendsFaceId;
    int i_Id;
    string s_faceId;
    void setCameraDistance(ofVec3f _cameraPos);
    ofImage i_Thumb;

    bool b_NewFaceIntroductionFinished;
    int64_t t_LastIntroducedTime;
    int i_IntroducedCount;
    float getCameraDist();

    int getLightCount();
    void setLightCount(int _count);
    vector<int> vi_LightFriendsId;
    
private:
    
    string s_Name;
    string s_Job;
    string s_Group;
    string s_Comment;
    H_STATE t_State;
    bool b_StateChanged;
    vector<ofPath> vp_Name;
    vector<ofPath> vp_Job;
    vector<ofPath> vp_Group;
    
    float f_CameraDistance;//distance to camera
    
    ofImage lightImage;
    int i_lightCount;
    
};




#endif
