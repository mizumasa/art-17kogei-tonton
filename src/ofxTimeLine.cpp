/*
 *
*/

#include "ofxTimeLine.h"

#define f_TimeLineSabi 0.262
#define f_TimeLineLag 0.001
#define SCENE1_START 0
#define SCENE2_START 10



ofxFragment::ofxFragment(){
    b_ActiveOnce = true;
    i_FragId = 0;
    i_nextFragId = 1;
    actId = AID_INIT;
    f_Length = 0;
    i_Param = 0;
}
ofxFragment::ofxFragment(int _i_FragId,ACT_ID _actId,float _f_Length){
    b_ActiveOnce = true;
    i_FragId = _i_FragId;
    i_nextFragId = _i_FragId+1;
    actId = _actId;
    f_Length = _f_Length;
    i_Param = 0;
}
ofxFragment::ofxFragment(int _i_FragId,ACT_ID _actId,float _f_Length,int _i_Param){
    b_ActiveOnce = true;
    i_FragId = _i_FragId;
    i_nextFragId = _i_FragId+1;
    actId = _actId;
    f_Length = _f_Length;
    i_Param = _i_Param;
}
ofxFragment::ofxFragment(int _i_FragId,int _i_nextFragId,ACT_ID _actId,float _f_Length){
    b_ActiveOnce = true;
    i_FragId = _i_FragId;
    i_nextFragId = _i_nextFragId;
    actId = _actId;
    f_Length = _f_Length;
    i_Param = 0;
}
ofxFragment::ofxFragment(int _i_FragId,int _i_nextFragId,ACT_ID _actId,float _f_Length,int _i_Param){
    b_ActiveOnce = true;
    i_FragId = _i_FragId;
    i_nextFragId = _i_nextFragId;
    actId = _actId;
    f_Length = _f_Length;
    i_Param = _i_Param;
}

ofxSequence::ofxSequence(){
    i_TimeUnitMsec = 1000;//1000
    t_FragmentStartTime = 0;
    b_Playing = false;
    b_Changed = false;
}
void ofxSequence::setup(){
    nowFragment = ofxFragment();
}
void ofxSequence::pushData(ofxFragment _fragment){
    vf_Sequence.push_back(_fragment);
}
void ofxSequence::update(){
    if(b_Playing){
        if(b_Interrupt){
            b_Interrupt = false;
            b_Changed = true;
            int nextId = i_InterruptNextId;
            nowFragment = getNextFrag(nextId);
            t_FragmentStartTime = ofGetElapsedTimeMillis();
        }
        if(ofGetElapsedTimeMillis() > (t_FragmentStartTime+nowFragment.f_Length*i_TimeUnitMsec)){
            //cout <<"[Sequence]"<< nowFragment.actId << endl;
            b_Changed = true;
            int nextId = nowFragment.i_nextFragId;
            nowFragment = getNextFrag(nextId);
            t_FragmentStartTime = ofGetElapsedTimeMillis();
        }
    }
}

ofxFragment ofxSequence::getNextFrag(int _i_nextFragId){
    for(int i=0;i<vf_Sequence.size();i++){
        if(_i_nextFragId == vf_Sequence[i].i_FragId){
            return vf_Sequence[i];
        }
    }
    return ofxFragment();
}

void ofxSequence::interrupt(int _i_InterruptNextId){
    b_Interrupt = true;
    i_InterruptNextId = _i_InterruptNextId;
}

void ofxSequence::draw(){
    
}
bool ofxSequence::getChanged(){
    if(b_Changed){
        b_Changed = false;
        return true;
    }else{
        return false;
    }
}
ACT_ID ofxSequence::getIdNow(){
    return nowFragment.actId;
}

int ofxSequence::getFragIdNow(){
    return nowFragment.i_FragId;
}

int ofxSequence::getParamNow(){
    return nowFragment.i_Param;
}
bool ofxSequence::isPlaying(){
    return b_Playing;
}
void ofxSequence::play(){
    b_Playing = true;
    t_FragmentStartTime = ofGetElapsedTimeMillis();
    nowFragment = getNextFrag(1);
}
void ofxSequence::stop(){
    b_Playing = false;
}
void ofxSequence::togglePlay(){
    b_Playing = !b_Playing;
    if(!b_Playing)cout << "Sequence stop" << endl;
    if(b_Playing)cout << "Sequence restart" << endl;
}


//--------------------------------------------------------------
ofxTimeLine::ofxTimeLine(){
}
//--------------------------------------------------------------
void ofxTimeLine::setup(){
    
    float timeLineData[STEP_NUM][4] = {
        {SCENE1_START+1 ,SCENE1_START+1 +1,AID_INIT,0},
        {SCENE1_START+2 ,SCENE1_START+2 +1,AID_CHANGE_ANGLE,3},
        {SCENE1_START+3 ,SCENE1_START+3 +1,AID_MOVE_ANGLE,4},
        {SCENE1_START+4 ,SCENE1_START+4 +1,AID_CHANGE_ANGLE,5},
        {SCENE1_START+5 ,SCENE1_START+5 +1,AID_MOVE_ANGLE,6},
        
        {SCENE1_START+5 ,SCENE1_START+5 +1,AID_MOVE_HUMAN,7},
        {SCENE1_START+6 ,SCENE1_START+6 +1,AID_MOVE_HUMAN,8},
        {SCENE1_START+7 ,SCENE1_START+7 +1,AID_MOVE_HUMAN,9},
        {SCENE1_START+8 ,SCENE1_START+8 +1,AID_HUMAN_ZOOM_IN,10},
        {SCENE1_START+9 ,SCENE1_START+9 +1,AID_FADEOUT,11},
        
        {SCENE2_START,SCENE2_START+1,AID_CHANGE_SCENE_TO_CHROME,2},
        {SCENE2_START+1,SCENE2_START+1+1,AID_CHANGE_SITE,0},
        {SCENE2_START+2,SCENE2_START+2 +1,AID_FADEIN,1},
        { SCENE2_START+3,SCENE2_START+3 +1,AID_SCROLL,0},
        { SCENE2_START+4,SCENE2_START+4 +1,AID_SCROLL,0},
        
        { SCENE2_START+5,SCENE2_START+5 +1,AID_SCROLL,0},
        { SCENE2_START+6,SCENE2_START+6 +1,AID_CHANGE_SITE,0},
        { SCENE2_START+7,SCENE2_START+7 +1,AID_SCROLL,0},
        { SCENE2_START+8,SCENE2_START+8 +1,AID_SCROLL,0},
        { SCENE2_START+9,SCENE2_START+9 +1,AID_SCROLL,0},
        
        { SCENE2_START+10,SCENE2_START+10 +1,AID_CHANGE_SITE,0},
        { SCENE2_START+11,SCENE2_START+11 +1,AID_SCROLL,0},
        { SCENE2_START+12,SCENE2_START+12 +1,AID_SCROLL,0},
        { SCENE2_START+13,SCENE2_START+13 +1,AID_SCROLL,0},
        { SCENE2_START+14,SCENE2_START+14 +1,AID_FADEOUT,11},
        
        { SCENE2_START+15,SCENE2_START+15 +1,AID_CHANGE_SCENE_TO_WORLD,2},
        { SCENE2_START+16,SCENE2_START+16 +1,AID_FADEIN,1},
        { SCENE2_START+17,SCENE2_START+17 +1,AID_HUMAN_ZOOM_OUT,1},
        { SCENE2_START+18,SCENE2_START+18 +1,AID_LOOP_BACK,1},
    };

    bool onceFlag[ACT_ID_NUM] = {true,true,true,true,true,true,true,true,true,true,true,true};
    for(int i=0;i<STEP_NUM;i++){
        vvf_TimeLineTime.push_back(ofVec2f(timeLineData[i][0],timeLineData[i][1]));
        vi_TimeLineActId.push_back(timeLineData[i][2]);
        vi_TimeLineParam.push_back(timeLineData[i][3]);
        vb_TimeLineUsedFlag.push_back(false);
    }
    for(int i=0;i<ACT_ID_NUM;i++){
        vb_ActState.push_back(false);
        vb_ActOnceFlag.push_back(onceFlag[i]);
        vb_ActParam.push_back(-1);
    }
}

//--------------------------------------------------------------
void ofxTimeLine::update(float pos){
    for(int i=0;i<STEP_NUM;i++){
        if((vvf_TimeLineTime[i][0] < pos) && (pos < vvf_TimeLineTime[i][1])){
            if(vb_ActOnceFlag[vi_TimeLineActId[i]] && (vb_TimeLineUsedFlag[i])){
            }else{
                vb_ActState[vi_TimeLineActId[i]]=true;
                vb_ActParam[vi_TimeLineActId[i]]=vi_TimeLineParam[i];
                vb_TimeLineUsedFlag[i]=true;
            }
        }else if (vb_TimeLineUsedFlag[i] && (pos >= vvf_TimeLineTime[i][1])){
            vb_TimeLineUsedFlag[i]=false;
            vb_ActState[vi_TimeLineActId[i]]=false;
        }
    }
}
//--------------------------------------------------------------

void ofxTimeLine::draw(){
    for(int i=0;i<ACT_ID_NUM;i++){
        bool buf = getState(i);
        ofDrawBitmapString(buf, 800, (i+1)*30);
    }
    for(int i=0;i<STEP_NUM;i++){
        ofDrawBitmapString(vb_TimeLineUsedFlag[i], 850, (i+1)*30);
    }
}
//--------------------------------------------------------------
void ofxTimeLine::clear(){
    for(int i=0;i<STEP_NUM;i++){
        vb_TimeLineUsedFlag[i]=false;
    }
    for(int i=0;i<ACT_ID_NUM;i++){
        vb_ActState[i]=false;
    }
}
//--------------------------------------------------------------

bool ofxTimeLine::getState(int actId){
    if(vb_ActState[actId]){
        if(vb_ActOnceFlag[actId]){
            vb_ActState[actId]=false;
        }
        return true;
    }else{
        return false;
    }
}

int ofxTimeLine::getParam(int actId){
    return vb_ActParam[actId];
}
