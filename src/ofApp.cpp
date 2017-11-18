#include "ofApp.h"

#define USE_BLACKMAGIC 0
#define BLACKMAGIC_W 1920
#define BLACKMAGIC_H 1080
#define WEBCAM_W 1280
#define WEBCAM_H 720

#define TITLE_DRAW_THUMB 30
#define TITLE_DRAW_ALL 60
//#define TITLE_DRAW_VANISH 300
#define TITLE_DRAW_VANISH 3000

#define SAMPLE_RADIUS_RATE 1.4

//setup--------------------------------------------------------------
void ofApp::setup(){
    //ofHideCursor();
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    b_ComingIntroduceMode = false;
    i_ComingLogo = 0;
    avs.setup("Works");
    avs.play(0, 1000, 1000);

    
    ofSetWindowShape(1280, 800);
    i_Camera = 2;
    i_test = 0;
    
    ofEnableLighting();
    ofEnableDepthTest();
    ofEnableSmoothing();
    
    ofSetSmoothLighting(true);
    
    areaLight.setup();
    areaLight.enable();
    areaLight.setSpotlight(180,1);
    
    areaLight.setAmbientColor(ofFloatColor(0.5,0.5,0.5));
    areaLight.setDiffuseColor(ofFloatColor(0.5,0.5,0.5));
    areaLight.setSpecularColor(ofFloatColor(0.5,0.5,0.5));
    areaLight.setPosition(RADIUS,-RADIUS,RADIUS);
    areaLight.lookAt(ofVec3f(0,0,0), ofVec3f(0,0,1));
    
    plane.set(400,400,2,2);
    plane.rotate(-90,ofVec3f(1,0,0));
    plane.move(ofVec3f(0,-300,0));
    
    {
        ofxObjectCamera camBuf;
        camBuf.setFarClip(20000);
        camBuf.setPosition(0, 0,0);
        camBuf.setDistance(1.0);
        camBuf.move(-200, -300, 0);
        camBuf.lookAt(ofVec3f(0,RADIUS,0), ofVec3f(0,0,1));
        camBuf.setFov(70);
        v_Camera.push_back(camBuf);
    }
    {
        ofxObjectCamera camBuf;
        camBuf.setFarClip(20000);
        camBuf.setPosition(RADIUS/sqrt(2.0), -RADIUS/sqrt(2.0),0);
        camBuf.lookAt(ofVec3f(0,RADIUS,0), ofVec3f(0,0,1));
        camBuf.setFov(50);
        v_Camera.push_back(camBuf);
    }
    {
        ofxObjectCamera camBuf;
        //camBuf.enableOrtho();
        camBuf.setPosition(RADIUS/sqrt(2.0), -RADIUS/sqrt(2.0),0);
        camBuf.lookAt(ofVec3f(0,RADIUS,0), ofVec3f(0,0,1));
        camBuf.setFov(50);
        //camBuf.setNearClip(0);
        camBuf.setFarClip(20000);
        camBuf.setAutoMove();
        v_Camera.push_back(camBuf);
    }
    
    rollCam.setup();//rollCam's setup.
    rollCam.setPos(0, 45, 0);
    rollCam.setCamSpeed(0.12);//rollCam's speed set;
    rollCam.setScale(2.3);

    b_RollingCam = true;
    b_MovingCam = false;
    
    
    
    bool ret;
    //ret = myFont.loadFont("hiragino.ttc", 0, true, true,0.3,500);

    ret = fontNotoRegu1.loadFont("Fonts/NotoSansCJKjp-Regular.otf", 5, true, true,0,240);
    if(ret){cout << "ok" <<endl;}else{cout << "error" <<endl;}
    ret = fontNotoRegu2.loadFont("Fonts/NotoSansCJKjp-Regular.otf", 5, true, true,0,240);
    if(ret){cout << "ok" <<endl;}else{cout << "error" <<endl;}
    //ret = fontNotoRegu3.loadFont("Fonts/NotoSansCJKjp-Regular.otf", 8, true, true,0,100);
    ret = fontNotoRegu3.loadFont("Fonts/NotoSansCJKjp-Regular.otf", 5, true, true,0,200);
    if(ret){cout << "ok" <<endl;}else{cout << "error" <<endl;}
    ret = fontNotoThin1.loadFont("Fonts/NotoSansCJKjp-Thin.otf", 5, true, true,0,160);
    if(ret){cout << "ok" <<endl;}else{cout << "error" <<endl;}
    ret = fontNotoThin2.loadFont("Fonts/NotoSansCJKjp-Thin.otf", 5, true, true,0,170);
    if(ret){cout << "ok" <<endl;}else{cout << "error" <<endl;}
    //ret = fontNotoThin3.loadFont("Fonts/NotoSansCJKjp-Thin.otf", 8, true, true,0,100);
    ret = fontNotoThin3.loadFont("Fonts/NotoSansCJKjp-Thin.otf", 5, true, true,0,170);
    if(ret){cout << "ok" <<endl;}else{cout << "error" <<endl;}
    
    ret = myFont.loadFont("AxisStd-Light.otf", 4, true, true,0,200);
    if(ret){cout << "ok" <<endl;}else{cout << "error" <<endl;}
    //ret = myFont1.loadFont("AxisStd-Light.otf", 3, true, true,0,500);
    ret = myFont1.loadFont("AxisStd-Light.otf", 4, true, true,0,200);//1920x800 size
    if(ret){cout << "ok" <<endl;}else{cout << "error" <<endl;}
    //ret = myFont2.loadFont("AxisStd-Light.otf", 1, true, true,0,500);
    ret = myFont2.loadFont("AxisStd-Light.otf", 4, true, true,0,200);//1920x800 size
    if(ret){cout << "ok" <<endl;}else{cout << "error" <<endl;}
    

    i_SelectedHuman = 0;
    i_StatusStayNum = 0;
    ofSetCylinderResolution(24, 1);
    
    
    //FboBlur
    ofFbo::Settings s;
    s.width = ofGetWidth();
    s.height = ofGetHeight();
    s.internalformat = GL_RGBA;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;
    gpuBlur.setup(s, false);
    
    gui.setup();
    gui.add(pi_AngleSpeed.setup("Angle Speed", 5, 1, 10));
    b_GuiDraw = false;
    
    background.load("background.png");
    b_CamStart = false;
    
    if(0){
    if(USE_BLACKMAGIC){
        cam.setup(BLACKMAGIC_W, BLACKMAGIC_H, 30);
    }else{
        camMac.setDeviceID(0);
        camMac.initGrabber(WEBCAM_W, WEBCAM_H);
    }
    }
    
    i_FadingMode = 0;
    i_FadeBlackLevel = 0;
    
    timeline.setup();
    b_TimeLineIdle = false;
    
    b_ChromeShow = false;
    b_LogoShow = false;
    i_Logo.load("logo.jpg");
    
    i_TitleDrawCount = 0;
    
    svg.load("new_face.svg");
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        for(int j=0;j<(int)lines.size();j++){
            svgOutlines.push_back(lines[j].getResampledBySpacing(1));
        }
    }
    b_SvgDraw = false;
    b_SvgPngDraw = false;
    i_SvgPng.load("new_face.png");
    if(0){
    int fsize = 5;
    int fsize2 = 200;
    ftest1.loadFont("Fonts/AxisStd-Medium.otf", fsize, true, true,0,fsize2);
    ftest2.loadFont("Fonts/AxisStd-Regular.otf", fsize, true, true,0,fsize2);
    ftest3.loadFont("Fonts/AxisStd-UltraLight.otf", fsize, true, true,0,fsize2);
    ftest4.loadFont("Fonts/koburinaStdNW6.otf", fsize, true, true,0,fsize2);
    ftest5.loadFont("Fonts/koburinaStdNW3.otf", fsize, true, true,0,fsize2);
    ftest6.loadFont("Fonts/koburinaStdNW1.otf", fsize, true, true,0,fsize2);
    ftest7.loadFont("Fonts/NotoSansCJKjp-Medium.otf", fsize, true, true,0,fsize2);
    ftest8.loadFont("Fonts/NotoSansCJKjp-Regular.otf", fsize, true, true,0,fsize2);
    ftest9.loadFont("Fonts/NotoSansCJKjp-Thin.otf", fsize, true, true,0,fsize2);
    }
    i_TotalIntroduceCount = 0;
    
    for(int i=0;i<70;i++){
        ofxObjectParticle bufParticle;
        bufParticle.setPos(ofRandom(-180, 180), ofRandom(-60,60), RADIUS * ofRandom(0.5,4.0));
        v_ObjectParticle.push_back(bufParticle);
    }
    
    lightImage.load("dot.png");
    b_ChromeFriendThumbDraw=true;
    i_FriendIcon.load("friendicon.png");
    
    materialPlane.setAmbientColor(ofFloatColor(0.5,0.5,0.5,1.0));
    materialPlane.setDiffuseColor(ofFloatColor(0.5,0.5,0.5,1.0));
    materialPlane.setSpecularColor(ofFloatColor(1.0,1.0,1.0,1.0));
    materialPlane.setShininess(100000);

    //system("python --version");
    //system(CMD_MAIN_PY); coundn't use python shell
    fadeIn();
    
    b_MouseOn = false;
    b_GrabScreen = false;
    model_base.loadModel("hammer_base.3ds");
    
    /*
    fileName = "movie";
    fileExt = ".mp4";
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("8000k");
    vidRecorder.setAudioCodec("mp3");
    vidRecorder.setAudioBitrate("192k");
    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    soundStream.setup(this, 0, channels, sampleRate, 256, 4);
     */
    
    vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
    vidGrabber.setGrabber(vidRecorder);
    videoDevices = vidRecorder->listVideoDevices();
    audioDevices = vidRecorder->listAudioDevices();
    ofAddListener(vidRecorder->videoSavedEvent, this, &ofApp::videoSaved);
    vidGrabber.setup(1280, 720);
    vidRecorder->initRecording();
    bLaunchInQuicktime = true;
    
    //pattern
    {
        ofVec2f patternBuf;
        vector<ofVec2f> vv_patternBuf;
        for(int i=0;i<30;i++){
            patternBuf = ofVec2f(ofRandom(0.1,0.9),ofRandom(0.1,0.9));
            vv_patternBuf.push_back(patternBuf);
        }
        vvv_pattern.push_back(vv_patternBuf);
        vv_patternBuf.clear();
        for(int i=0;i<230;i++){
            float fBuf;
            fBuf =ofRandom(0,1.0);
            patternBuf = ofVec2f(ofRandom(0.1,0.9),fBuf*fBuf*0.8+0.1);
            vv_patternBuf.push_back(patternBuf);
        }
        vvv_pattern.push_back(vv_patternBuf);
        vv_patternBuf.clear();
        for(int i=0;i<260;i++){
            float fBuf;
            fBuf =ofRandom(0.1,0.4);
            float fBuf2;
            fBuf2 =ofRandom(0,2*3.14);
            patternBuf = ofVec2f( 0.5 + fBuf*sin(fBuf2),0.5+fBuf*cos(fBuf2));
            vv_patternBuf.push_back(patternBuf);
        }
        vvv_pattern.push_back(vv_patternBuf);
        vv_patternBuf.clear();
    }
    iconPhone.load("TonTon_style_iphone.png");
    icon1.load("TonTon_style_full.png");
    icon2.load("TonTon_style_gradient.png");
    icon3.load("TonTon_style_round.png");
}

//--------------------------------------------------------------
void ofApp::update(){
    i_StatusStayNum=0;
    i_MaxIntroducedNum = 0;
    i_MinIntroducedNum = 100000;
    
    if(b_UpdateFbo){
        b_UpdateFbo = false;
        ofFbo::Settings s;
        s.width = ofGetWidth();
        s.height = ofGetHeight();
        s.internalformat = GL_RGBA;
        s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        s.maxFilter = GL_LINEAR; GL_NEAREST;
        s.numSamples = 0;
        s.numColorbuffers = 1;
        s.useDepth = false;
        s.useStencil = false;
        gpuBlur.setup(s, false);
    }
    
    gpuBlur.blurOffset = 130 * ofMap(mouseY, 0, ofGetHeight(), 1, 0, true);
    gpuBlur.blurOffset = 4;
    gpuBlur.blurPasses = 10 * ofMap(mouseX, 0, ofGetWidth(), 0, 1, true);
    gpuBlur.blurPasses = 9.5;
    gpuBlur.numBlurOverlays = 1;
    gpuBlur.blurOverlayGain = 255;
    
    rollCam.update();   //rollCam's rotate update.

    if(b_ChromeShow){
        if(USE_BLACKMAGIC){
            if(cam.update()){
                timer.tick();
                b_CamStart=true;
                camPixels=cam.getColorPixels();
                camImg.setFromPixels(camPixels.getData(), BLACKMAGIC_W, BLACKMAGIC_H, OF_IMAGE_COLOR_ALPHA);
            }
        }else{
            //camMac.update();
            vidGrabber.update();
            b_CamStart=true;
            camPixels = vidGrabber.getPixels();
            //camPixels = camMac.getPixels();
            camImg.setFromPixels(camPixels.getData(), WEBCAM_W, WEBCAM_H, OF_IMAGE_COLOR);
            //camPixels.resize( BLACKMAGIC_W, BLACKMAGIC_H);
            //camImg.setFromPixels(camPixels.getData(), BLACKMAGIC_W, BLACKMAGIC_H, OF_IMAGE_COLOR);
        }
    }
    
    //updateTitleDraw();
    
    switch(i_FadingMode){
        case 0:
            break;
        case 1://fadein
            i_FadeBlackLevel = i_FadeBlackLevel - 5;
            if(i_FadeBlackLevel == 0){
                i_FadingMode = 0;
            }
            break;
        case 2://fadeout
            i_FadeBlackLevel = i_FadeBlackLevel + 5;
            if(i_FadeBlackLevel == 255){
                i_FadingMode = 0;
            }
            break;
    }
    
    /*
    if(bRecordingPre and bRecording){
        bool success = vidRecorder.addFrame(camImg);
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }
    bRecordingPre = bRecording;

    if (vidRecorder.hasVideoError()) {
        ofLogWarning("The video recorder failed to write some frames!");
    }
    if (vidRecorder.hasAudioError()) {
        ofLogWarning("The video recorder failed to write some audio samples!");
    }*/

    if(recordedVideoPlayback.isLoaded()){
        recordedVideoPlayback.update();
    }

}

void ofApp::dumpOSC(ofxOscMessage m) {
    string msg_string;
    msg_string = m.getAddress();
    for (int i=0; i<m.getNumArgs(); i++ ) {
        msg_string += " ";
        if(m.getArgType(i) == OFXOSC_TYPE_INT32)
            msg_string += ofToString( m.getArgAsInt32(i));
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT)
            msg_string += ofToString( m.getArgAsFloat(i));
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING)
            msg_string += m.getArgAsString(i);
    }
    cout << msg_string << endl;
}

void ofApp::updateFriendship(){
    for(int i = 0; i<v_ObjectHuman.size(); i++){
        //cout << "about" << v_ObjectHuman[i].s_faceId <<endl;
        v_ObjectHuman[i].i_Id = i;
        v_ObjectHuman[i].vi_FriendsId.clear();
    }
    for(int i = 0; i<v_ObjectHuman.size(); i++){
        for(int j = 0; j<v_ObjectHuman[i].vs_FriendsFaceId.size(); j++){
            for(int k = 0; k<v_ObjectHuman.size(); k++){
                if(v_ObjectHuman[i].vs_FriendsFaceId[j] == v_ObjectHuman[k].s_faceId){
                    if(! inVector(v_ObjectHuman[k].vi_FriendsId,v_ObjectHuman[i].i_Id)){
                        v_ObjectHuman[k].vi_FriendsId.push_back(v_ObjectHuman[i].i_Id);
                        v_ObjectHuman[i].vi_FriendsId.push_back(v_ObjectHuman[k].i_Id);
                    }
                }
            }
        }
    }
    for(int i = 0; i<v_ObjectHuman.size(); i++){
        //cout << v_ObjectHuman[i].i_Id <<":"<< v_ObjectHuman[i].s_faceId <<endl;
        for(int j = 0; j<v_ObjectHuman[i].vi_FriendsId.size(); j++){
            int friendId;
            friendId = v_ObjectHuman[i].vi_FriendsId[j];
            //cout << ">>" << v_ObjectHuman[friendId].i_Id <<":"<< v_ObjectHuman[friendId].s_faceId <<endl;
        }
    }
}


void ofApp::updateTitleDraw(){
    if(i_TitleDrawCount > 0){
        i_TitleDrawCount +=1;
        if(b_ComingIntroduceMode){
            i_ComingLogo +=1;
            if(i_ComingLogo > 0 and i_ComingLogo < 30){
                i_TitleDrawCount -= 1;
            }
            if(i_ComingLogo==80){
                //avsName = "";
            }
            if(i_ComingLogo == 10){
                svgStep = 0;
                //avsName.setup("NEW FACE !!!");
                //avsName.play(0, 100, 100);
                b_SvgDraw = true;
                
            }
            if(i_ComingLogo >= 10 && i_ComingLogo <=29){
                svgStep += 0.05;
            }
            if(i_ComingLogo == 75){
                b_SvgDraw = false;
            }
            if(i_ComingLogo >= 30 && i_ComingLogo <=62){
                b_SvgPngDraw = ((i_ComingLogo % 2) == 0);
            }
            if(i_ComingLogo >= 75){
                b_SvgPngDraw = false;
            }
        }
        if(i_TitleDrawCount > TITLE_DRAW_VANISH){
            i_TitleDrawCount = 0;
            i_ComingLogo = 0;
        }
        switch(i_TitleDrawCount){
            case 2:
                avsName="";
                avsJob="";
                avsGroup="";
                avsWork1="";
                avsWork2="";
                avsWork3="";
                avsWorkUrl1="";
                avsWorkUrl2="";
                avsWorkUrl3="";
                i_TotalIntroduceCount+=1;
                v_ObjectHuman[i_SelectedHuman].i_IntroducedCount+=1;
                v_ObjectHuman[i_SelectedHuman].t_LastIntroducedTime=ofGetElapsedTimeMillis();
                //v_ObjectHuman[i_SelectedHuman].b_NewFaceIntroductionFinished=true;
            case 40:
                cout << "AVS set" << v_ObjectHuman[i_SelectedHuman].getName() << ":" << v_ObjectHuman[i_SelectedHuman].getJob() << ":" << v_ObjectHuman[i_SelectedHuman].getGroup() << endl;
                avsJob.setup(limitStr(v_ObjectHuman[i_SelectedHuman].getJob()));
                avsName.setup(limitStr(v_ObjectHuman[i_SelectedHuman].getName()));
                avsGroup.setup(limitStr(v_ObjectHuman[i_SelectedHuman].getGroup()));
                if(v_ObjectHuman[i_SelectedHuman].vs_WorksUrl.size()>0){
                    avsWork1.setup(limitStr(v_ObjectHuman[i_SelectedHuman].vs_WorksName[0]));
                    avsWorkUrl1.setup(limitStr(v_ObjectHuman[i_SelectedHuman].vs_WorksUrl[0]));
                }
                if(v_ObjectHuman[i_SelectedHuman].vs_WorksUrl.size()>1){
                    avsWork2.setup(limitStr(v_ObjectHuman[i_SelectedHuman].vs_WorksName[1]));
                    avsWorkUrl2.setup(limitStr(v_ObjectHuman[i_SelectedHuman].vs_WorksUrl[1]));
                }
                if(v_ObjectHuman[i_SelectedHuman].vs_WorksUrl.size()>2){
                    avsWork3.setup(limitStr(v_ObjectHuman[i_SelectedHuman].vs_WorksName[2]));
                    avsWorkUrl3.setup(limitStr(v_ObjectHuman[i_SelectedHuman].vs_WorksUrl[2]));
                }
                avsName.play(0, 1000, 1000);
                break;
            case 50:
                avsGroup.play(0, 1000, 1000);
                break;
            case 60:
                avsJob.play(0, 1000, 1000);
                break;
            case 70:
                if(!b_ComingIntroduceMode)avsWork1.play(0, 900, 900);
                break;
            case 75:
                if(!b_ComingIntroduceMode)avsWorkUrl1.play(0, 900, 900);
                break;
            case 80:
                if(!b_ComingIntroduceMode)avsWork2.play(0, 800, 800);
                break;
            case 85:
                if(!b_ComingIntroduceMode)avsWorkUrl2.play(0, 800, 800);
                break;
            case 90:
                if(!b_ComingIntroduceMode)avsWork3.play(0, 700, 700);
                break;
            case 95:
                if(!b_ComingIntroduceMode)avsWorkUrl3.play(0, 700, 700);
                break;
        }
    }else{
        i_ComingLogo = 0;
        b_SvgDraw = false;
        b_SvgPngDraw = false;
    }
}

void ofApp::titleDraw(){
    ofPushMatrix();
    //ofTranslate(ofVec3f(ofGetWidth()/2-100, ofGetHeight()/2 - 100,505));
    ofTranslate(ofVec3f(ofGetWidth()/2, ofGetHeight()/2,505));//1920x800 size
    ofPushStyle();
    vector<string> vs_Title;
    vs_Title.push_back(avsName);
    vs_Title.push_back(avsGroup);
    vs_Title.push_back(avsJob);
        vs_Title.push_back(avsWork1);
        vs_Title.push_back(avsWorkUrl1);
        vs_Title.push_back(avsWork2);
        vs_Title.push_back(avsWorkUrl2);
        vs_Title.push_back(avsWork3);
        vs_Title.push_back(avsWorkUrl3);
    
    vector<ofPath> avsTest;
    if(i_TitleDrawCount>TITLE_DRAW_THUMB && i_TitleDrawCount<TITLE_DRAW_VANISH){
        //v_ObjectHuman[i_SelectedHuman].i_Thumb.draw(-THUMB_SIZE*2.3, 0, THUMB_SIZE*2, THUMB_SIZE*2);
        v_ObjectHuman[i_SelectedHuman].i_Thumb.draw(-THUMB_SIZE_TITLE*1.1 *0.9, -THUMB_SIZE_TITLE*0.87+8, THUMB_SIZE_TITLE*0.9, THUMB_SIZE_TITLE*0.9);//1920x800 size
    }
    /*if(i_ComingLogo > 10 && i_ComingLogo <100){
        ofTranslate(ofVec3f(0,100,0));
        avsTest = myFont1.getStringAsPoints(avsName);
        ofSetColor(255,255,255,255);
        vector<ofPath>::iterator iter2;
        iter2 = avsTest.begin();
        for (; iter2 != avsTest.end(); ++iter2){
            (*iter2).setStrokeWidth(0);
            (*iter2).setPolyWindingMode(OF_POLY_WINDING_NONZERO);
            (*iter2).draw(0, 0);
        }
    }*/
    ofTranslate(ofVec3f(0,-53,0));
    if(i_TitleDrawCount>TITLE_DRAW_ALL && i_TitleDrawCount<TITLE_DRAW_VANISH){
        for(int i= 0; i<vs_Title.size(); i++){
            ofPushMatrix();
            ofPushStyle();
            ofSetColor(255,255,255,255);
            if(i<3){
                ofScale(0.9, 0.9);
            }
            if(vs_Title[i].size()>0){
                if(i==0){
                    avsTest = fontNotoRegu1.getStringAsPoints(vs_Title[i]);
                }else{
                    if(i==1 || i==2){
                        avsTest = fontNotoThin1.getStringAsPoints(vs_Title[i]);
                    }else{
                        ofScale(0.5, 0.5);
                        if(i%2==0){
                            ofSetColor(77,77,77,255);
                            avsTest = fontNotoThin2.getStringAsPoints(vs_Title[i]);
                        }else{
                            avsTest = fontNotoRegu2.getStringAsPoints(vs_Title[i]);
                        }
                        //ofTranslate(-3.0*vs_Title[i].size(), 0);
                        if(avsTest.size()>0){
                            vector<ofPolyline> gomi;
                            gomi = avsTest[avsTest.size()-1].getOutline();
                            if(gomi[gomi.size()-1].size()){
                                ofTranslate(-(gomi[gomi.size()-1][gomi[gomi.size()-1].size()-1].x )/2.0, 0);
                            }
                        }
                    }
                }
                vector<ofPath>::iterator iter;
                iter = avsTest.begin();
                for (; iter != avsTest.end(); ++iter){
                    (*iter).setStrokeWidth(0);
                    (*iter).setPolyWindingMode(OF_POLY_WINDING_NONZERO);
                    (*iter).draw(0, 0);
                }
            }
            ofPopStyle();
            ofPopMatrix();
            if(i<3){
                //ofTranslate(ofVec3f(0,30,0));
                ofTranslate(ofVec3f(0,20,0));
                if(i==0){
                    ofTranslate(ofVec3f(0,3,0));
                }
                if(i==2){
                    ofTranslate(ofVec3f(0,18,0));
                }
            }else{
                if(i%2==0){
                    ofTranslate(ofVec3f(0,9,0));
                }
                ofTranslate(ofVec3f(0,7,0));
            }
        }
    }
    
    //test
    if(0){
    vector<string> vs_fontTest;
    vs_fontTest.push_back("南 陽平 IMJ Front-end Engineer");
    vs_fontTest.push_back("南 陽平 IMJ Front-end Engineer");
    vs_fontTest.push_back("南 陽平 IMJ Front-end Engineer");
    vector<ofPath> fontTest;
    for(int i = 0;i<9;i++){
        if(i==0)fontTest = ftest1.getStringAsPoints(vs_fontTest[i%3]);
        if(i==1)fontTest = ftest2.getStringAsPoints(vs_fontTest[i%3]);
        if(i==2)fontTest = ftest3.getStringAsPoints(vs_fontTest[i%3]);
        if(i==3)fontTest = ftest4.getStringAsPoints(vs_fontTest[i%3]);
        if(i==4)fontTest = ftest5.getStringAsPoints(vs_fontTest[i%3]);
        if(i==5)fontTest = ftest6.getStringAsPoints(vs_fontTest[i%3]);
        if(i==6)fontTest = ftest7.getStringAsPoints(vs_fontTest[i%3]);
        if(i==7)fontTest = ftest8.getStringAsPoints(vs_fontTest[i%3]);
        if(i==8)fontTest = ftest9.getStringAsPoints(vs_fontTest[i%3]);
        vector<ofPath>::iterator iterTest;
        iterTest = fontTest.begin();
        for (; iterTest != fontTest.end(); ++iterTest){
            (*iterTest).setStrokeWidth(0);
            (*iterTest).setPolyWindingMode(OF_POLY_WINDING_NONZERO);
            (*iterTest).draw(0, 0);
        }
        ofTranslate(0, 14);
    }
    }
    
    ofPopStyle();
    ofPopMatrix();
}

ofVec3f ofApp::getHumanPos(string _humanId){
    for(int i = 0; i<v_ObjectHuman.size(); i++){
        if(v_ObjectHuman[i].s_faceId == _humanId){
            return v_ObjectHuman[i].getPos();
        }
    }
    return ofVec3f(0,0,0);
}


//--------------------------------------------------------------
void ofApp::drawFriendLines(){
    for(int i = 0; i<v_ObjectHuman.size(); i++){
        for(int j = 0; j<v_ObjectHuman[i].vi_FriendsId.size(); j++){
            if(v_ObjectHuman[i].i_Id > v_ObjectHuman[i].vi_FriendsId[j]){
                if((v_ObjectHuman[i].getState() == H_STATE_STAY) ||
                   (v_ObjectHuman[v_ObjectHuman[i].vi_FriendsId[j]].getState() == H_STATE_STAY)){
                    if((v_ObjectHuman[i].getState() == H_STATE_STAY) &&
                       (v_ObjectHuman[v_ObjectHuman[i].vi_FriendsId[j]].getState() == H_STATE_STAY)){
                        if((i_StatusStayNum < 20) || ((i % 8) == ((int)(ofGetElapsedTimeMillis()/2000)%8))){
                        v_ObjectHuman[i].drawColorLineTo(v_ObjectHuman[v_ObjectHuman[i].vi_FriendsId[j]].getPos());
                        }else{
                            //v_ObjectHuman[i].drawGrayLineTo(v_ObjectHuman[v_ObjectHuman[i].vi_FriendsId[j]].getPos());
                        }
                    }
                    else{
                        //v_ObjectHuman[i].drawGrayLineTo(v_ObjectHuman[v_ObjectHuman[i].vi_FriendsId[j]].getPos());
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawFriendLights(){
    bool nobody = true;
    bool noStayFriend = true;
    for(int i = 0; i<v_ObjectHuman.size(); i++){
        ofPushMatrix();
        ofTranslate(v_ObjectHuman[i].getPos());
        ofVec2f buf;
        buf =v_ObjectHuman[i].getAnglePanTilt();
        //cout <<"angle"<< buf << v_ObjectHuman[i].getName()<<lightImage.getWidth()<<endl;
        ofRotateZ(-buf[0]);
        ofRotateX(buf[1]);
        ofRotateX(90);
        ofScale(10, 10);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofEnableAlphaBlending();
        ofSetColor(255,255,255, 255);
        //lightImage.draw(-10,-10, 0, 20, 20);
        ofPopMatrix();
        int count = v_ObjectHuman[i].getLightCount();
        if(count>0){
            nobody = false;
            if(v_ObjectHuman[i].vi_FriendsId.size()==0){
                v_ObjectHuman[i].setLightCount(0);
            }
            for(int j = 0; j<v_ObjectHuman[i].vi_FriendsId.size(); j++){
                if(v_ObjectHuman[v_ObjectHuman[i].vi_FriendsId[j]].getState() == H_STATE_STAY){
                    noStayFriend = false;
                    bool ret = v_ObjectHuman[i].drawLightTo(v_ObjectHuman[v_ObjectHuman[i].vi_FriendsId[j]].getPos(), count, lightImage);
                    if(ret){
                        v_ObjectHuman[i].setLightCount(0);
                        //v_ObjectHuman[int(ofRandom(0.9999)* v_ObjectHuman.size())].setLightCount(1);
                    }
                }
            }
            if(noStayFriend){
                v_ObjectHuman[i].setLightCount(0);
            }
        }
    }
    if(v_ObjectHuman.size()>0 && nobody){
        int bufID;
        bufID = int(ofRandom(0.9999)* v_ObjectHuman.size());
        if(v_ObjectHuman[bufID].getState()==H_STATE_STAY)v_ObjectHuman[bufID].setLightCount(1);
    }
}


//--------------------------------------------------------------
void ofApp::timelineIdle(){
    i_AutoStartMsec += (ofGetElapsedTimeMillis() - i_AutoLastMsec);
}




//draw--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255, 255, 255);
    ofEnableAlphaBlending();
    
    ofBackground(255,255,255);
    
    if(b_ChromeShow and b_CamStart){
        
        
        ofPushMatrix();
        ofPushStyle();

        camImg.draw(0, 0,WEBCAM_W/2,WEBCAM_H/2);
        if(vp_WorkNamePlaying.size()>0){
            vector<ofPolyline> gomi;
            gomi = vp_WorkNamePlaying[vp_WorkNamePlaying.size()-1].getOutline();
            if(gomi.size()>0){
                if(gomi[gomi.size()-1].size()){
                    i_WorkNamePlayingWidth = gomi[gomi.size()-1][gomi[gomi.size()-1].size()-1].x;
                }
            }
            ofDrawRectRounded(-100, -60,-1, 140+i_WorkNamePlayingWidth*2, 60,10);
            ofScale(1.5, 1.5);
            ofSetColor(255,255,255);
            vector<ofPath>::iterator iter;
            iter = vp_WorkNamePlaying.begin();
            ofTranslate(20,-15);
            for (; iter != vp_WorkNamePlaying.end(); ++iter){
                (*iter).setStrokeWidth(0);
                (*iter).setPolyWindingMode(OF_POLY_WINDING_NONZERO);
                (*iter).draw(0,0);
            }
        }
        ofPopStyle();
        ofPopMatrix();

        ofPushMatrix();
        ofPushStyle();
        ofTranslate(ofGetWidth()-330, 0);
        ofSetColor(0, 0, 0,255);
        ofFill();
        ofDrawRectRounded(-70, -60,-1, 420, 120,10);
        ofPopStyle();
        ofPopMatrix();
    }else{
        if(b_LogoShow){
            drawLogo();
        }else{
            
            ofPushMatrix();
            ofPushStyle();
            iconPhone.draw(0, 0,-2,ofGetHeight()*iconPhone.getWidth() / iconPhone.getHeight(),ofGetHeight());
            
            ofSetColor(190, 192,194,255);
            ofDrawRectangle(CANVAS_MARGIN_LEFT, CANVAS_MARGIN_TOP,-1, CANVAS_SIZE, CANVAS_SIZE);
            ofSetColor(0, 0, 0,255);
            ofSetLineWidth(30);
            ofFill();
            //ofDrawRectangle(50, 100,10, 200, 200);
            for(int i = 0; i<vvv_MousePoint.size(); i++){
                if(vvv_MousePoint[i].size()>3){
                    for(int j = 0; j<vvv_MousePoint[i].size()-1; j++){
                        //ofDrawCircle(vvv_MousePoint[i][j][0], vvv_MousePoint[i][j][1],10, 3);
                        ofDrawLine(vvv_MousePoint[i][j][0], vvv_MousePoint[i][j][1],
                                   vvv_MousePoint[i][j+1][0], vvv_MousePoint[i][j+1][1]);
                    }
                }
            }
            ofPopStyle();
            ofPopMatrix();

            if(b_GrabScreen){
                b_GrabScreen = false;
                canvas.allocate(CANVAS_SIZE, CANVAS_SIZE, OF_IMAGE_COLOR);
                canvas.grabScreen(CANVAS_MARGIN_LEFT, CANVAS_MARGIN_TOP, CANVAS_SIZE,CANVAS_SIZE);
                canvasColorImage.setFromPixels( canvas.getPixels());
                canvasColorImageBuf.setFromPixels( canvas.getPixels());
                canvasColorImage.blurGaussian(5);
                canvasGrayImage = canvasColorImage;
            }
            if(0){
            canvas.draw(CANVAS_MARGIN_LEFT+CANVAS_SIZE, CANVAS_MARGIN_TOP);
            canvasGrayImage.draw(CANVAS_MARGIN_LEFT+CANVAS_SIZE*2, CANVAS_MARGIN_TOP, CANVAS_SIZE, CANVAS_SIZE);
            }
            icon1.draw(CANVAS_MARGIN_LEFT+CANVAS_SIZE+3, CANVAS_MARGIN_TOP,CANVAS_SIZE/3,CANVAS_SIZE/3);
            icon2.draw(CANVAS_MARGIN_LEFT+CANVAS_SIZE+3, CANVAS_MARGIN_TOP+CANVAS_SIZE/3,CANVAS_SIZE/3,CANVAS_SIZE/3);
            icon3.draw(CANVAS_MARGIN_LEFT+CANVAS_SIZE+3, CANVAS_MARGIN_TOP+CANVAS_SIZE*2/3,CANVAS_SIZE/3,CANVAS_SIZE/3);
            ofPushStyle();
            ofSetColor(255, 0,0);
            ofNoFill();
            ofDrawRectangle(CANVAS_MARGIN_LEFT+CANVAS_SIZE+3, CANVAS_MARGIN_TOP+CANVAS_SIZE*(i_PatternMode)/3,3,CANVAS_SIZE/3,CANVAS_SIZE/3);
            ofPopStyle();
            
            if(canvasGrayImage.getWidth() > 0){
                ofPixels roiBuf;
                canvasColorImageBuf.setROI(5, 5, CANVAS_SIZE-10, CANVAS_SIZE-10);
                roiBuf = canvasColorImageBuf.getRoiPixels();
                canvasColorImageBuf2.setFromPixels(roiBuf);

                ofPushMatrix();
                ofPushStyle();
                //ofTranslate(CANVAS_MARGIN_LEFT, CANVAS_MARGIN_TOP + CANVAS_SIZE*1.5 );
                ofTranslate(99, 439);
                //ofFill();
                ofScale(1.9*CANVAS_SIZE/SUZUGAMI_SIZE,1.9*CANVAS_SIZE/SUZUGAMI_SIZE,1.9*CANVAS_SIZE/SUZUGAMI_SIZE);
                for(int k =0;k < vvv_pattern[i_PatternMode].size();k++){
                    int x,y;
                    x = int(SUZUGAMI_SIZE*vvv_pattern[i_PatternMode][k][0]);
                    y = int(SUZUGAMI_SIZE*vvv_pattern[i_PatternMode][k][1]);
                    //cout << x<<":"<<y<<endl;
                    canvasColorImageBuf2.draw(x-CANVAS_SIZE/2,y-CANVAS_SIZE/2,CANVAS_SIZE,CANVAS_SIZE);
                    //canvasGrayImage.draw(x,y,CANVAS_SIZE,CANVAS_SIZE);
                }
                ofSetColor(190,192,194);

                ofDrawRectangle(0, 0, SUZUGAMI_SIZE, SUZUGAMI_SIZE);
                ofPopStyle();
                ofPopMatrix();
            }
            
            ofPushStyle();
            ofSetColor(180, 180, 180);
            background.draw(-5*ofGetWidth(),-5*ofGetHeight(),-6100,11*ofGetWidth(), 11*ofGetHeight());
            ofPopStyle();
 
            ofRectangle viewport3D;
            viewport3D.x = ofGetWidth()/2;
            viewport3D.y = 0;
            viewport3D.width = ofGetWidth()/2;
            viewport3D.height = ofGetHeight()/2;
            if(b_RollingCam){
                rollCam.begin(viewport3D);
            }else{
                v_Camera[i_Camera].begin();
            }
            //background.draw(ofGetWidth(), ofGetHeight());
            areaLight.enable();
            materialPlane.begin();
            ofPixels canvasPixels;
            canvasPixels = canvasGrayImage.getPixels();
            unsigned char* canvasChar;
            canvasChar = canvasPixels.getData();

            
            ofSetColor(255, 255, 255);
            int i_height = 0;
            
            ofRotateZ(ofGetElapsedTimeMillis()/100.0);
            //ofRotateY(ofGetElapsedTimeMillis()/100.0);
            if(canvasGrayImage.getWidth() > 0){
                for(int i = BIT_MARGIN; i< (CANVAS_SIZE - BIT_MARGIN); i++){
                    for(int j = BIT_MARGIN; j<(CANVAS_SIZE - BIT_MARGIN); j++){
                        i_height = BIT_BASE_HEIGHT;
                        i_height += int(BIT_HEIGHT * (255 - int(canvasChar[i+j*CANVAS_SIZE])) / 255.0);
                        ofDrawBox((i-CANVAS_SIZE/2)*BIT_SIZE,(j-CANVAS_SIZE/2)*BIT_SIZE,i_height,BIT_SIZE,BIT_SIZE,i_height*2);
                    }
                }
                
            }
            //model_base.drawFaces();

            
            
            materialPlane.end();

            
            if(b_RollingCam){
                rollCam.end();
            }else{
                v_Camera[i_Camera].end();
            }
            
            
            
            
            
            if(0){
            
            viewport3D.x = ofGetWidth()/2;
            viewport3D.y = ofGetHeight()/2;
            viewport3D.width = ofGetWidth()/2;
            viewport3D.height = ofGetHeight()/2;
            if(b_RollingCam){
                rollCam.begin(viewport3D);
            }else{
                v_Camera[i_Camera].begin();
            }
            areaLight.enable();
            materialPlane.begin();
            
            ofSetColor(255, 255, 255);
            
            ofRotateZ(ofGetElapsedTimeMillis()/100.0);

            //ofDrawBox(0,0,0,1000);
            
            
            

            if(canvasGrayImage.getWidth() > 0 and 0){
                if(0){
                ofDrawBox(0, 0, 0, SUZUGAMI_SIZE, SUZUGAMI_SIZE, 10);
                for(int k =0;k < vvv_pattern[i_PatternMode].size();k++){
                    ofPushMatrix();
                    ofTranslate(SUZUGAMI_SIZE*vvv_pattern[i_PatternMode][k][0],SUZUGAMI_SIZE*vvv_pattern[i_PatternMode][k][1]);
                    ofScale(SUZUGAMI_BIT_SCALE, SUZUGAMI_BIT_SCALE);
                    for(int i = BIT_MARGIN; i< (CANVAS_SIZE - BIT_MARGIN); i++){
                        for(int j = BIT_MARGIN; j<(CANVAS_SIZE - BIT_MARGIN); j++){
                            if(int(canvasChar[i+j*CANVAS_SIZE]) < 200){
                                i_height = int(BIT_HEIGHT * (255 - int(canvasChar[i+j*CANVAS_SIZE])) / 255.0);
                                ofDrawBox((i-CANVAS_SIZE/2)*BIT_SIZE,(j-CANVAS_SIZE/2)*BIT_SIZE,i_height,BIT_SIZE,BIT_SIZE,i_height*2);
                            }
                        }
                    }
                    ofPopMatrix();
                }
                }else{
                    for(int i = 0; i< SUZUGAMI_SIZE; i++){
                        for(int j = 0; j<SUZUGAMI_SIZE; j++){
                            i_pattern[i][j]=100;
                        }
                    }
                    /*
                    for(int k =0;k < vvv_pattern[i_PatternMode].size();k++){
                        int x,y;
                        x = SUZUGAMI_SIZE*vvv_pattern[i_PatternMode][k][0];
                        y = SUZUGAMI_SIZE*vvv_pattern[i_PatternMode][k][1];
                        for(int i = BIT_MARGIN; i< (CANVAS_SIZE - BIT_MARGIN); i++){
                            for(int j = BIT_MARGIN; j<(CANVAS_SIZE - BIT_MARGIN); j++){
                                if(int(canvasChar[i+j*CANVAS_SIZE]) < 200){
                                    i_height = int(BIT_HEIGHT * (255 - int(canvasChar[i+j*CANVAS_SIZE])) / 255.0);
                                    i_pattern[int(x+(i-CANVAS_SIZE/2)*SUZUGAMI_BIT_SCALE)][int(y+(j-CANVAS_SIZE/2)*SUZUGAMI_BIT_SCALE)]-=i_height;
                                }
                            }
                        }
                    }*/
                    for(int i = 0; i< SUZUGAMI_SIZE; i++){
                        for(int j = 0; j<SUZUGAMI_SIZE; j++){
                            ofDrawBox((i-SUZUGAMI_SIZE/2),(j-SUZUGAMI_SIZE/2),i_pattern[i][j],1,1,i_pattern[i][j]*2);
                        }
                    }
                }
                
            }

            
            materialPlane.end();
            
            
            if(b_RollingCam){
                rollCam.end();
            }else{
                v_Camera[i_Camera].end();
            }

            }
                
            if(b_GuiDraw){
                ofPushStyle();
                ofPushMatrix();
                gui.draw();
                ofPopMatrix();
                ofPopStyle();

                ofPushStyle();
                ofPushMatrix();
                ofSetColor(255,0,0);
                ofDisableLighting();
                string info = "";
                info += "Now Angle : "+ofToString(rollCam.posN)+"\n";
                info += "Framerate : "+ofToString(ofGetFrameRate())+"\n";
                info += "msg: "+guimsg+"\n";
                ofDrawBitmapString(info, 10,100);
                ofPopMatrix();
                ofPopStyle();
            }

            
            ofPushStyle();
            int zoomGrayShade = 0;
            zoomGrayShade = (255 - (rollCam.scaleN - 1.2)*300);
            if(zoomGrayShade<0)zoomGrayShade=0;
            if(zoomGrayShade>255)zoomGrayShade=255;
            ofSetColor(0, 0, 0, zoomGrayShade);
            ofFill();
            ofDrawRectangle(-100,-100,500, ofGetWidth()+200, ofGetHeight()+200);
            ofPopStyle();
            titleDraw();
        }
    }
    
    
    ofPushStyle();
    ofSetColor(0, 0, 0, i_FadeBlackLevel);
    ofFill();
    ofDrawRectangle(-100,-100,510, ofGetWidth()+200, ofGetHeight()+200);
    ofPopStyle();

    ofDisableLighting();
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        
        //if it is recording, stop
        if(vidRecorder->isRecording()){
            vidRecorder->stopRecording();
        }
        else {
            // otherwise start a new recording.
            // before starting, make sure that the video file
            // is already in use by us (i.e. being played), or else
            // we won't be able to record over it.
            if(recordedVideoPlayback.isLoaded()){
                recordedVideoPlayback.close();
            }
            vidRecorder->startRecording("MyMovieFile"+ofGetTimestampString()+".mov");
        }
    }
    switch(key){
        case 't':
            b_LogoShow = !b_LogoShow;
            break;
        case 'i':
            b_ComingIntroduceMode = !b_ComingIntroduceMode;
            break;
        case 'f':
            ofToggleFullscreen();
            b_UpdateFbo = true;
            break;
        case 'g':
            b_GuiDraw = !b_GuiDraw;
            break;
        case 'u':
            updateFriendship();
            break;
        case OF_KEY_LEFT:
            break;
        case OF_KEY_RIGHT:
            break;
        case OF_KEY_RETURN:
            addSampleData();
            break;
        case '2':
            i_test += 1;
            break;
        case '1':
            i_test -= 1;
            break;
        case 'c':
            if(b_RollingCam){
                changeSelectedHuman();
            }else{
                i_Camera = (i_Camera +1)%3;
                for(int i = 0;i<v_Camera.size();i++){
                    if(i_Camera == i){
                        v_Camera[i].enableMouseInput();
                    }else{
                        v_Camera[i].disableMouseInput();
                    }
                }
            }
            break;
        case 'b':
            b_ChromeShow = !b_ChromeShow;
            break;
        case 'v':
            b_RollingCam = !b_RollingCam;
            break;
        case 'm':
            b_MovingCam = !b_MovingCam;
            break;
        case 'l':
            rollCam.setScale(4);
            break;
        case '6':
            fadeIn();
            break;
        case '7':
            fadeOut();
            break;
        case 'o':
            rollCam.setScale(2.3);
            i_TitleDrawCount = 0;
            break;
        case 'z':
            rollCam.setScale(1.6);
            i_TitleDrawCount = 1;
            break;
        case 'r':
            /*
            bRecording = !bRecording;
            if(bRecording && !vidRecorder.isInitialized()) {
                s_SaveName = fileName+ofGetTimestampString()+fileExt;
                vidRecorder.setup(s_SaveName, BLACKMAGIC_W, BLACKMAGIC_H, 30, sampleRate, channels);
                vidRecorder.start();
            }
            else if(!bRecording && vidRecorder.isInitialized()) {
                vidRecorder.setPaused(true);
            }
            else if(bRecording && vidRecorder.isInitialized()) {
                vidRecorder.setPaused(false);
            }*/
            
            break;
            /*
        case 'e':
            bRecording = false;
            vidRecorder.close();
            break;
             */
        case 'n':
            i_PatternMode = (i_PatternMode+1)%PATTERN_NUM;
        case 'a':
            b_Auto = !b_Auto;
            if(b_Auto){
                startAuto();
            }
            break;
    }
    if (key=='1') {//All Random.
        rollCam.setRandomScale(0.5, 1.5);
        rollCam.setRandomPos(270);
    }
    if (key=='2') {//Random rotate.
        rollCam.setRandomPos(270);
    }
    if (key=='3') {//Inputting optional rotate.
        rollCam.setPos(0, 0, 0);
    }
    if (key=='4') {//Random distance.
        rollCam.setRandomScale(1.7, 2.7);
        cout << rollCam.scaleT << ";" <<rollCam.scaleS << ";" <<rollCam.scaleN <<endl;
    }
    if (key=='5') {//Inputting optional distance.
        rollCam.setScale(1.5);
    }
}

void ofApp::changeSelectedHuman(bool _b_count){
    rollCam.setRandomPos();
}

void ofApp::changeSelectedHuman2NewFace(){
    i_SelectedHuman = vi_NewFaceQue[0];
    vi_NewFaceQue.erase(vi_NewFaceQue.begin());
    ofVec3f posTheta;
    posTheta = v_ObjectHuman[i_SelectedHuman].getPosTheta();
    cout << "move to NewFace:"<<v_ObjectHuman[i_SelectedHuman].getName()<<endl;
    rollCam.setPos(posTheta.x,posTheta.y,0);
}

bool ofApp::isRemoteBusy(){
    
}//Remote Browser loading state


//--------------------------------------------------------------
void ofApp::fadeIn(){
    if(i_FadingMode==0 and i_FadeBlackLevel==255){
        i_FadingMode = 1;
    }
}
//--------------------------------------------------------------
void ofApp::fadeOut(){
    if(i_FadingMode==0 and i_FadeBlackLevel==0){
        i_FadingMode = 2;
    }
}

//--------------------------------------------------------------
void ofApp::startAuto(){
    i_AutoStartMsec = ofGetElapsedTimeMillis();
    timeline.clear();
    cout << "start auto mode" <<endl;
}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(b_MouseOn){
        vv_MousePoint.push_back(ofVec2f(x,y));
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    b_MouseOn = true;
    cout << x<<":"<< y << endl;
    guimsg = ofToString(x)+":"+ofToString(y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    b_MouseOn = false;
    vvv_MousePoint.push_back(vv_MousePoint);
    vv_MousePoint.clear();
    b_GrabScreen=true;
    //canvas.grabScreen(CANVAS_MARGIN_LEFT, CANVAS_MARGIN_TOP, CANVAS_SIZE,CANVAS_SIZE);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

void ofApp::exit(){
    //ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    //vidRecorder.close();
    if(USE_BLACKMAGIC){
        cam.close();
    }
}

bool ofApp::inVector(vector<int> _v,int _i){
    bool b_flag =false;
    for(int i=0;i < _v.size();i++){
        if(_i==_v[i]){
            b_flag = true;
        }
    }
    return b_flag;
}

void ofApp::addSampleData(){
}

string ofApp::limitStr(string _str){
    int maxsize = 60;
    if(_str.size() > maxsize){
        string buf="";
        for(int i=0;i<maxsize;i++){
            buf += _str[i];
        }
        return buf+"...";
    }else{
        return _str;
    }
}

void ofApp::drawLogo(){
    ofPushStyle();
    ofEnableAlphaBlending();
    int buf;
    buf = ofGetElapsedTimeMillis()%2000;
    int sPos,ePos,sVal,eVal;
    if(buf>=2000*0 && buf<=2000*0.03){
        sPos=0;
        ePos=2000*0.03;
        sVal=255*0.75;
        eVal=255*1;
    }
    if(buf>=2000*0.03 && buf<=2000*0.06){
        sPos=2000*0.03;
        ePos=2000*0.06;
        sVal=255*0.75;
        eVal=255*1;
    }
    if(buf>=2000*0.06 && buf<=2000*0.09){
        sPos=2000*0.06;
        ePos=2000*0.09;
        sVal=255*0.75;
        eVal=255*1;
    }
    if(buf>=2000*0.09 && buf<=2000*0.30){
        sPos=2000*0.09;
        ePos=2000*0.3;
        sVal=255*1;
        eVal=255*1;
    }
    if(buf>=2000*0.3 && buf<=2000*0.33){
        sPos=2000*0.3;
        ePos=2000*0.33;
        sVal=255*1;
        eVal=255*0.75;
    }
    if(buf>=2000*0.33 && buf<=2000*0.36){
        sPos=2000*0.33;
        ePos=2000*0.36;
        sVal=255*0.75;
        eVal=255*1;
    }
    if(buf>=2000*0.36 && buf<=2000*1.0){
        sPos=2000*0.36;
        ePos=2000*1.0;
        sVal=255*1;
        eVal=255*1;
    }
    ofSetColor(255, 255, 255,(int)(sVal+1.0*(eVal-sVal)*(buf-sPos)/(ePos-sPos)));
    i_Logo.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
    /*0%{ opacity: 0.75; }
    3%{ opacity: 1; }
    6%{ opacity: 0.75; }
    9%{ opacity: 1; }
    30%{ opacity: 1; }
    33%{ opacity: 0.75; }
    36%{ opacity: 1; }*/
}

/*
//--------------------------------------------------------------
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    
    if(bRecording)
        vidRecorder.addAudioSamples(input, bufferSize, nChannels);
    
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}
*/


//--------------------------------------------------------------
void ofApp::videoSaved(ofVideoSavedEventArgs& e){
    // the ofQTKitGrabber sends a message with the file name and any errors when the video is done recording
    if(e.error.empty()){
        recordedVideoPlayback.load(e.videoPath);
        recordedVideoPlayback.play();
        
        if(bLaunchInQuicktime) {
            ofSystem("open " + e.videoPath);
        }
    }
    else {
        ofLogError("videoSavedEvent") << "Video save error: " << e.error;
    }
}
