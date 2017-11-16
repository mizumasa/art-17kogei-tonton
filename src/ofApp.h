#pragma once

#include "ofMain.h"
#include "ofxObjectHuman.h"
#include "ofxObjectParticle.h"
#include "ofxObjectCamera.h"
#include "ofxGui.h"

#include "ofxOsc.h"

#include "ofxFboBlur.h"
#include "ofxRollingCam.h"

#include "ofxBlackMagic.h"
#include "ofxTimeLine.h"
#include "ofxAVString.h"
#include "ofxSvg.h"


#define CMD_MAIN_PY "/usr/local/bin/python ~/programming/of_v0.9.8_osx/apps/Art2017/BaPABar/bin/data/main.py"
#define CMD_KILL_PY "/usr/local/bin/python ~/programming/of_v0.9.8_osx/apps/Art2017/BaPABar/bin/data/kill.py"

#define CANVAS_MARGIN_TOP 120
#define CANVAS_MARGIN_LEFT 120
#define CANVAS_SIZE 120


#define MIR_X_NUM 5*4
#define MIR_X_ANG 9*1.5
#define MIR_Y_NUM 5*4
#define MIR_Y_ANG 9*1.5

#define NUM 500
#define SIZE 10

class RateTimer {
protected:
    float lastTick, averagePeriod, smoothing;
    bool secondTick;
public:
    RateTimer() :
    smoothing(.9) {
        reset();
    }
    void reset() {
        lastTick = 0, averagePeriod = 0, secondTick = false;
    }
    void setSmoothing(float smoothing) {
        this->smoothing = smoothing;
    }
    float getFramerate() {
        return averagePeriod == 0 ? 0 : 1 / averagePeriod;
    }
    void tick() {
        float curTick = ofGetElapsedTimef();
        if(lastTick == 0) {
            secondTick = true;
        } else {
            float curDiff = curTick - lastTick;;
            if(secondTick) {
                averagePeriod = curDiff;
                secondTick = false;
            } else {
                averagePeriod = ofLerp(curDiff, averagePeriod, smoothing);
            }
        }
        lastTick = curTick;
    }
};


class ofApp : public ofBaseApp{
public:

    void setup();
    void timelineIdle();
    void update();
    void updateFriendship();
    void draw();
    void drawFriendLines();
    void drawFriendLights();
    void drawLogo();
    void startAuto();
    void exit();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void dumpOSC(ofxOscMessage m);
    void fadeIn();
    void fadeOut();
    void updateTitleDraw();
    void titleDraw();
    void changeSelectedHuman(bool _b_count=true);
    void changeSelectedHuman2NewFace();
    void addSampleData();
    string limitStr(string _str);
    
    bool inVector(vector<int> _v ,int _i);
    
    bool isRemoteBusy();//Remote Browser loading state
    
    bool b_UpdateFbo;
    bool b_GuiDraw;
    ofVec3f getHumanPos(string _humanId);

    ofLight areaLight;

    ofPlanePrimitive plane;
    
    ofMaterial materialBackGround;
    ofMaterial materialPlane;
    ofMaterial materialPlaneBlack;
    
    int i_test;
    
    ofImage background;
    
    vector<ofxObjectHuman> v_ObjectHuman;
    vector<ofxObjectParticle> v_ObjectParticle;
    ofxObjectParticleControl ObjectParticleControl;

    int i_SelectedHuman;
    
    
    int i_Camera;
    bool b_RollingCam;
    bool b_MovingCam;
    
    vector<ofxObjectCamera> v_Camera;
    ofxRollingCam rollCam;

    
    
    ofxFboBlur gpuBlur;
    
    ofxPanel gui;
    ofxIntSlider pi_AngleSpeed;
    ofxTrueTypeFontUC myFont;
    ofxTrueTypeFontUC myFont1;//profile Name title
    ofxTrueTypeFontUC myFont2;//profile works title
    
    /*Blackmagic*/
    ofxBlackMagic cam;
    ofVideoGrabber camMac;
    RateTimer timer;
    ofPixels camPixels;
    ofImage camImg;
    bool b_CamStart;
    int i_FadingMode;
    int i_FadeBlackLevel;
    
    bool b_Auto;
    uint64_t i_AutoStartMsec;
    uint64_t i_AutoLastMsec;
    ofxTimeLine timeline;
    bool b_TimeLineIdle;
        
    bool b_ChromeShow;
    bool b_LogoShow;
    ofImage i_Logo;
    
    ofxAVString avs;
    ofxAVString avsName;
    ofxAVString avsJob;
    ofxAVString avsGroup;
    ofxAVString avsWork1;
    ofxAVString avsWork2;
    ofxAVString avsWork3;
    ofxAVString avsWorkUrl1;
    ofxAVString avsWorkUrl2;
    ofxAVString avsWorkUrl3;
    
    int i_TitleDrawCount;
    bool b_ComingIntroduceMode;
    int i_ComingLogo;
    vector<int> vi_NewFaceQue;//TODO when new face came, add to new face que
    vector<int> vi_IntroduceQue;
    
    int i_StatusStayNum;
    
    vector<ofxTrueTypeFontUC> vfont;
    ofxTrueTypeFontUC ftest1;
    ofxTrueTypeFontUC ftest2;
    ofxTrueTypeFontUC ftest3;
    ofxTrueTypeFontUC ftest4;
    ofxTrueTypeFontUC ftest5;
    ofxTrueTypeFontUC ftest6;
    ofxTrueTypeFontUC ftest7;
    ofxTrueTypeFontUC ftest8;
    ofxTrueTypeFontUC ftest9;
    ofxTrueTypeFontUC fontNotoRegu1;
    ofxTrueTypeFontUC fontNotoThin1;
    ofxTrueTypeFontUC fontNotoRegu2;
    ofxTrueTypeFontUC fontNotoThin2;
    ofxTrueTypeFontUC fontNotoRegu3;
    ofxTrueTypeFontUC fontNotoThin3;

    int i_MaxIntroducedNum;
    int i_MinIntroducedNum;
    
    ofxSVG svg;
    float svgStep;
    vector<ofPolyline> svgOutlines;
    bool b_SvgDraw;
    bool b_SvgPngDraw;
    ofImage i_SvgPng;
    
    int i_TotalIntroduceCount;
    vector<ofPath> vp_WorkNamePlaying;
    int i_WorkNamePlayingWidth;
    
    ofImage lightImage;
    
    bool b_ChromeFriendThumbDraw;
    ofImage i_FriendIcon;
    
    ofImage canvas;
    bool b_MouseOn;
    bool b_GrabScreen;
    vector<ofVec2f> vv_MousePoint;
    vector<vector<ofVec2f>> vvv_MousePoint;
};

