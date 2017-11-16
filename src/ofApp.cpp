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
    
    if(USE_BLACKMAGIC){
        cam.setup(BLACKMAGIC_W, BLACKMAGIC_H, 30);
    }else{
        camMac.setDeviceID(0);
        camMac.initGrabber(WEBCAM_W, WEBCAM_H);
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
}

//--------------------------------------------------------------
void ofApp::update(){
    if(b_Auto){
        if(b_TimeLineIdle)timelineIdle();
    }
    i_StatusStayNum=0;
    i_MaxIntroducedNum = 0;
    i_MinIntroducedNum = 100000;
    for(int i = 0; i<v_ObjectHuman.size(); i++){
        v_ObjectHuman[i].update(0);
        if(v_ObjectHuman[i].getState() == H_STATE_STAY){
            i_StatusStayNum += 1;
            if(!(v_ObjectHuman[i].b_NewFaceIntroductionFinished)){
                v_ObjectHuman[i].b_NewFaceIntroductionFinished = true;
                vi_IntroduceQue.push_back(i);
                vi_NewFaceQue.push_back(i);
                cout << "pushed to NewFace And Introduce que:" << v_ObjectHuman[i].getName()<<endl;
            }
            if(i_MaxIntroducedNum < v_ObjectHuman[i].i_IntroducedCount)i_MaxIntroducedNum = v_ObjectHuman[i].i_IntroducedCount;
            if(i_MinIntroducedNum > v_ObjectHuman[i].i_IntroducedCount)i_MinIntroducedNum = v_ObjectHuman[i].i_IntroducedCount;
        }
        if(v_ObjectHuman[i].getState() == H_STATE_STAY && (i!=i_SelectedHuman)){
            ofVec2f moveDir = ofVec2f(0,0);
            for(int j = 0; j<v_ObjectHuman.size(); j++){
                if(v_ObjectHuman[j].getState() == H_STATE_STAY){
                    moveDir += v_ObjectHuman[i].getSekiryoku(v_ObjectHuman[j].getPosTheta());
                }
            }
            if(abs(moveDir[0]) > 0.05){
                v_ObjectHuman[i].moveAngleDir(moveDir[0], moveDir[1]);
            }
        }
        if(v_ObjectHuman[i].getState() == H_STATE_LEFT){
            for(int j=0;j < vi_IntroduceQue.size();j++){
                if(vi_IntroduceQue[j]==i){
                    vi_IntroduceQue.erase(vi_IntroduceQue.begin()+j);
                }
            }
        }
        
        if(b_RollingCam){
            v_ObjectHuman[i].setNormalVec(-(rollCam.vf_Pos));
            v_ObjectHuman[i].setCameraDistance(rollCam.vf_Pos);
        }else{
            v_ObjectHuman[i].setNormalVec((v_Camera[i_Camera].getLookAtDir()));
            v_ObjectHuman[i].setCameraDistance(v_Camera[i_Camera].getPos());
        }
        //if(b_MovingCam)v_ObjectHuman[i].move();
    }
    for(int i = 0; i<v_Camera.size(); i++){
        v_Camera[i].update();
    }
    
    ObjectParticleControl.update();
    for(int i = 0; i<v_ObjectParticle.size(); i++){
        v_ObjectParticle[i].update();
    }
    
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
    
    if(b_MovingCam){
        ofVec3f posTheta;
        posTheta = v_ObjectHuman[i_SelectedHuman].getPosTheta();
        rollCam.setPos(posTheta.x,posTheta.y,0);
    }
    
    if(b_ChromeShow){
        if(USE_BLACKMAGIC){
            if(cam.update()){
                timer.tick();
                b_CamStart=true;
                camPixels=cam.getColorPixels();
                camImg.setFromPixels(camPixels.getData(), BLACKMAGIC_W, BLACKMAGIC_H, OF_IMAGE_COLOR_ALPHA);
            }
        }else{
            camMac.update();
            b_CamStart=true;
            camPixels = camMac.getPixels();
            camPixels.resize( BLACKMAGIC_W, BLACKMAGIC_H);
            camImg.setFromPixels(camPixels.getData(), BLACKMAGIC_W, BLACKMAGIC_H, OF_IMAGE_COLOR);
        }
    }
    
    updateTitleDraw();

    
    
    
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
    

}

//OSCメッセージをコンソールに出力する関数
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
    
    ofBackground(0,0,0);
    
    if(b_ChromeShow and b_CamStart){
        
        
        ofPushMatrix();
        ofPushStyle();

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
            ofSetColor(255, 255,255,255);
            ofDrawRectangle(CANVAS_MARGIN_LEFT, CANVAS_MARGIN_TOP,-1, CANVAS_SIZE, CANVAS_SIZE);
            ofSetColor(0, 0, 0,255);
            ofSetLineWidth(30);
            ofFill();
            //ofDrawRectangle(50, 100,10, 200, 200);
            for(int i = 0; i<vvv_MousePoint.size(); i++){
                for(int j = 0; j<vvv_MousePoint[i].size()-1; j++){
                    //ofDrawCircle(vvv_MousePoint[i][j][0], vvv_MousePoint[i][j][1],10, 3);
                    ofDrawLine(vvv_MousePoint[i][j][0], vvv_MousePoint[i][j][1],
                               vvv_MousePoint[i][j+1][0], vvv_MousePoint[i][j+1][1]);
                }
            }
            ofPopStyle();
            ofPopMatrix();

            if(b_GrabScreen){
                b_GrabScreen = false;
                canvas.allocate(CANVAS_SIZE, CANVAS_SIZE, OF_IMAGE_COLOR);
                canvas.grabScreen(CANVAS_MARGIN_LEFT, CANVAS_MARGIN_TOP, CANVAS_SIZE,CANVAS_SIZE);
            }
            canvas.draw(CANVAS_MARGIN_LEFT, CANVAS_MARGIN_TOP+CANVAS_SIZE);
            
            
            ofPushStyle();
            ofSetColor(180, 180, 180);
            background.draw(-5*ofGetWidth(),-5*ofGetHeight(),-6100,11*ofGetWidth(), 11*ofGetHeight());
            ofPopStyle();
            ofRectangle viewport3D;
            viewport3D.x = ofGetWidth()/2;
            viewport3D.y = 0;
            viewport3D.width = ofGetWidth()/2;
            viewport3D.height = ofGetHeight();
            if(b_RollingCam){
                rollCam.begin(viewport3D);
            }else{
                v_Camera[i_Camera].begin();
            }
            //background.draw(ofGetWidth(), ofGetHeight());
            areaLight.enable();
            materialPlane.begin();

            
            //ofTranslate(ofVec3f(0,tan(ofGetElapsedTimeMillis()/1000),0));
            //ofTranslate(ofVec3f(0,tan(1.0*ofGetElapsedTimeMillis()/300)*100,0));
            ofSetColor(255, 255, 255);

            ofDrawBox(0, 0, 0, 100);

            materialPlane.end();

            
            if(b_RollingCam){
                rollCam.end();
            }else{
                v_Camera[i_Camera].end();
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
    switch(key){
        case 'n':
            vi_NewFaceQue.clear();
            break;
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
    if(1){
        ofxObjectHuman bufHuman;
        bufHuman.setPos(150, 0, RADIUS*SAMPLE_RADIUS_RATE);
        
        bufHuman.setNormalVec(- bufHuman.getPos());
        bufHuman.regist(0,"face****", "./thumb/2.jpg", "林あいうえお", "AAAAAAAAA", "******* engineer");
        bufHuman.registPath(0,"face****", "./thumb/2.png",
                            myFont.getStringAsPoints("林あいうえお"),
                            myFont.getStringAsPoints("AAAAAAAAA"),
                            myFont.getStringAsPoints("******* engineer"));
        bufHuman.vs_FriendsFaceId.push_back("face****1");
        bufHuman.vs_FriendsFaceId.push_back("face****2");
        bufHuman.vs_FriendsFaceId.push_back("face****3");
        bufHuman.setState(H_STATE_STAY);
        v_ObjectHuman.push_back(bufHuman);
        
        ofxObjectHuman bufHuman1;
        bufHuman1.setPos(0, 0, RADIUS*SAMPLE_RADIUS_RATE);
        bufHuman1.setNormalVec(- bufHuman1.getPos());
        bufHuman1.regist(0,"face****", "./thumb/3.jpg", "南 陽平", "IMJ", "Front-end Engineer");
        bufHuman1.registPath(0,"face****", "./thumb/3.png",
                             fontNotoRegu3.getStringAsPoints("南 陽平"),
                             fontNotoThin3.getStringAsPoints("IMJ"),
                             fontNotoThin3.getStringAsPoints("Front-end Engineer"));
        bufHuman1.setState(H_STATE_STAY);
        bufHuman1.vs_FriendsFaceId.push_back("face****4");
        bufHuman1.vs_FriendsFaceId.push_back("face****5");
        bufHuman1.vs_FriendsFaceId.push_back("face****");
        bufHuman1.vs_WorksName.push_back("MASS RHYTHM");
        bufHuman1.vs_WorksName.push_back("Nike Unlimited Stadium");
        bufHuman1.vs_WorksName.push_back("Window with Intelligence");
        bufHuman1.vs_WorksUrl.push_back("https://www.youtube.com/watch?v=idUodas3EBw");
        bufHuman1.vs_WorksUrl.push_back("http://prty.jp/work/unlimited-stadium");
        bufHuman1.vs_WorksUrl.push_back("http://prty.jp/work/wwi");
        v_ObjectHuman.push_back(bufHuman1);
        
        ofxObjectHuman bufHuman2;
        bufHuman2.setPos(-120, 30, RADIUS*SAMPLE_RADIUS_RATE);
        bufHuman2.setNormalVec(- bufHuman2.getPos());
        bufHuman2.regist(0,"face****1", "./thumb/5.jpg", "馬場あいうえお", "CCCCCCCCC", "******* director");
        bufHuman2.registPath(0,"face****1", "./thumb/5.png",
                             myFont.getStringAsPoints("馬場あいうえお"),
                             myFont.getStringAsPoints("CCCCCCCCC"),
                             myFont.getStringAsPoints("******* director"));
        bufHuman2.setState(H_STATE_STAY);
        v_ObjectHuman.push_back(bufHuman2);
        
        ofxObjectHuman bufHuman3;
        bufHuman3.setPos(30, 0, RADIUS*SAMPLE_RADIUS_RATE);
        bufHuman3.setNormalVec(- bufHuman3.getPos());
        bufHuman3.regist(0,"face****2", "./thumb/5.jpg", "kojima", "CCCCCCCCC", "******* director");
        bufHuman3.registPath(0,"face****2", "./thumb/6.png",
                             myFont.getStringAsPoints("kojima"),
                             myFont.getStringAsPoints("AAAAAAAAA"),
                             myFont.getStringAsPoints("******* engineer"));
        bufHuman3.setState(H_STATE_STAY);
        bufHuman3.vs_WorksName.push_back("MASS RHYTHM");
        bufHuman3.vs_WorksName.push_back("Nike Unlimited Stadium");
        bufHuman3.vs_WorksUrl.push_back("https://www.youtube.com/watch?v=idUodas3EBw");
        bufHuman3.vs_WorksUrl.push_back("http://prty.jp/work/unlimited-stadium");
        v_ObjectHuman.push_back(bufHuman3);
        
        ofxObjectHuman bufHuman4;
        bufHuman4.setPos(-30, 10, RADIUS*SAMPLE_RADIUS_RATE);
        bufHuman4.setNormalVec(- bufHuman4.getPos());
        bufHuman4.regist(0,"face****3", "./thumb/5.jpg", "馬場あいうえお", "CCCCCCCCC", "******* director");
        bufHuman4.registPath(0,"face****3", "./thumb/7.png",
                             myFont.getStringAsPoints("あいうえお"),
                             myFont.getStringAsPoints("AAAAAAAAA"),
                             myFont.getStringAsPoints("******* engineer"));
        bufHuman4.setState(H_STATE_STAY);
        v_ObjectHuman.push_back(bufHuman4);
        ofxObjectHuman bufHuman5;
        bufHuman5.setPos(180, 20, RADIUS*SAMPLE_RADIUS_RATE);
        bufHuman5.setNormalVec(- bufHuman5.getPos());
        bufHuman5.regist(0,"face****4", "./thumb/5.jpg", "馬場あいうえお", "CCCCCCCCC", "******* director");
        bufHuman5.registPath(0,"face****4", "./thumb/8.png",
                             myFont.getStringAsPoints("あいうえお"),
                             myFont.getStringAsPoints("AAAAAAAAA"),
                             myFont.getStringAsPoints("******* engineer"));
        bufHuman5.setState(H_STATE_STAY);
        bufHuman5.vs_FriendsFaceId.push_back("face****1");
        bufHuman5.vs_FriendsFaceId.push_back("face****2");
        bufHuman5.vs_FriendsFaceId.push_back("face****3");

        v_ObjectHuman.push_back(bufHuman5);
        ofxObjectHuman bufHuman6;
        bufHuman6.setPos(-60, -30, RADIUS*SAMPLE_RADIUS_RATE);
        bufHuman6.setNormalVec(- bufHuman6.getPos());
        bufHuman6.regist(0,"face****5", "./thumb/5.jpg", "馬場あいうえお", "CCCCCCCCC", "******* director");
        bufHuman6.registPath(0,"face****5", "./thumb/9.png",
                             myFont.getStringAsPoints("あいうえお"),
                             myFont.getStringAsPoints("AAAAAAAAA"),
                             myFont.getStringAsPoints("******* engineer"));
        bufHuman6.setState(H_STATE_STAY);
        v_ObjectHuman.push_back(bufHuman6);
    }

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
