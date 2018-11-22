#pragma once

#include "ofMain.h"
#include "ofxObjectHuman.h"
#include "ofxObjectParticle.h"
#include "ofxObjectCamera.h"
#include "ofxGui.h"
#include "ofxVideoRecorder.h"

#include "ofxOsc.h"

#include "ofxFboBlur.h"
#include "ofxRollingCam.h"

#include "ofxBlackMagic.h"
#include "ofxTimeLine.h"
#include "ofxAVString.h"
#include "ofxSvg.h"
#include "ofxOpenCv.h"

#include "ofxAssimpModelLoader.h"

/*#define CANVAS_MARGIN_TOP 303
#define CANVAS_MARGIN_LEFT 163
#define CANVAS_SIZE 120*/

#define CANVAS_MARGIN_TOP 304
#define CANVAS_MARGIN_LEFT 173
#define CANVAS_SIZE 156

//#define CANVAS_MARGIN_TOP 382
//#define CANVAS_MARGIN_LEFT 75
//#define CANVAS_SIZE 280

#define ICON_SIZE 50

#define SUZUGAMI_SIZE 1200
#define SUZUGAMI_BIT_SCALE 0.1

#define BIT_SIZE 10
#define BIT_HEIGHT 100
#define BIT_BASE_HEIGHT 150
#define BIT_MARGIN 5

#define PATTERN_NUM 3

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
    ofxCvColorImage canvasColorImage;
    ofxCvColorImage canvasColorImageBuf;
    ofxCvColorImage canvasColorImageBuf2;
    ofxCvGrayscaleImage canvasGrayImage;
    bool b_MouseOn;
    bool b_GrabScreen;
    vector<ofVec2f> vv_MousePoint;
    vector<vector<ofVec2f>> vvv_MousePoint;
    
    ofxAssimpModelLoader model_base;

    
    int i_InitializeCount;
/*
    ofxVideoRecorder    vidRecorder;
    ofSoundStream       soundStream;
    bool bRecording;
    bool bRecordingPre;
    int sampleRate;
    int channels;
    string fileName;
    string fileExt;
    
    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
    string s_SaveName;
    void audioIn(float * input, int bufferSize, int nChannels);
*/
    
    
    ofVideoGrabber 			vidGrabber;
    ofPtr<ofQTKitGrabber>	vidRecorder;
    
    ofVideoPlayer recordedVideoPlayback;
    
    void videoSaved(ofVideoSavedEventArgs& e);
    
    vector<string> videoDevices;
    vector<string> audioDevices;
    
    bool bLaunchInQuicktime;


    int i_PatternMode;
    vector<vector<ofVec2f>> vvv_pattern;
    int i_pattern[SUZUGAMI_SIZE][SUZUGAMI_SIZE];

    ofImage iconPhone;
    ofImage icon1;
    ofImage icon2;
    ofImage icon3;
    string guimsg;
    bool b_HammerOrBit;
};


