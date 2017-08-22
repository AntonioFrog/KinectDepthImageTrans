#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(255,255,255);
    w = 512;
    h = 424;

	ofSetWindowShape(w * 2 + 60, h + 135);
	ofSetWindowTitle("KinectDepthShortImageReciever");

    size = w * h * 2;
    data1 = new unsigned short[w*h];
    data2 = new unsigned short[w*h];
    
    Connect1 = false;
    Connect2 = false;
    toggle1 = new ofxDatGuiToggle("TOGGLE CONNECT1", false);
    toggle2 = new ofxDatGuiToggle("TOGGLE CONNECT2", false);
    toggle1->onButtonEvent(this, &ofApp::onButtonEvent);
    toggle2->onButtonEvent(this, &ofApp::onButtonEvent);

	amplify1 = 10.0f;
	slider1 = new ofxDatGuiSlider(ofParamFloat1.set("AMPLIFY1", 10.0f, 1.0f, 20.0f));
	slider1->setPrecision(4, false);
	slider1->onSliderEvent(this, &ofApp::onSliderEvent);
	ofParamFloat1.addListener(this, &ofApp::onParamFloat1Changed);
	amplify2 = 10.0f;
	slider2 = new ofxDatGuiSlider(ofParamFloat2.set("AMPLIFY2", 10.0f, 1.0f, 20.0f));
	slider2->setPrecision(4, false);
	slider2->onSliderEvent(this, &ofApp::onSliderEvent);
	ofParamFloat2.addListener(this, &ofApp::onParamFloat2Changed);

	positionUIs();
	setUIs();
    
    //load XML
    if( XML.load("settings.xml") ){
    }else{
        XML.addChild("ADDRESS");
        XML.setTo("ADDRESS");
        XML.addChild("PORT1");
        XML.setValue("//PORT1", "12345");
        XML.addChild("PORT2");
        XML.setValue("//PORT2", "12346");
        XML.addChild("PORT3");
        XML.setValue("//PORT3", "12347");
        XML.save("settings.xml");
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    toggle1->update();
    toggle2->update();
	slider1->update();
	slider2->update();
    
    if ( Connect1 && _receiver1.frame() != _frameId1 ) {  //Check if new frame was received
        _frameId1 = _receiver1.frame();
        if ( _receiver1.size() == size ) {
            dataChar1 = &_receiver1.data()[0];
            for (int i = 0; i < w*h;i++) {
                data1[i] = (dataChar1[i*2+1]<<8|dataChar1[i*2])*amplify1;
            }
            image1.setFromPixels(  data1, w, h, OF_IMAGE_GRAYSCALE );
        }
        else {
            cout << "ERROR: Network1 received data with bad size, check image sizes" << endl;
        }
        //Update FPS counter`
        _receiveCount1 ++;
        float fpsUpdateSec = 1.0;	//Time in seconds for accumulating FPS
        float time = ofGetElapsedTimef();
        if ( time >= _receiveLastTime1 + fpsUpdateSec ) {
            _receiveFPS1 = _receiveCount1 / ( time - _receiveLastTime1 );
            _receiveLastTime1 = time;
            _receiveCount1 = 0;
        }
    }
    
    if ( Connect2 && _receiver2.frame() != _frameId2 ) {  //Check if new frame was received
        _frameId2 = _receiver2.frame();
        if ( _receiver2.size() == size ) {
            dataChar2 = &_receiver2.data()[0];
            for (int i = 0; i < w*h;i++) {
                data2[i] = (dataChar2[i*2+1]<<8|dataChar2[i*2])*amplify2;
            }
            image2.setFromPixels(  data2, w, h, OF_IMAGE_GRAYSCALE );
        }
        else {
            cout << "ERROR: Network2 received data with bad size, check image sizes" << endl;
        }
        //Update FPS counter`
        _receiveCount2 ++;
        float fpsUpdateSec = 1.0;	//Time in seconds for accumulating FPS
        float time = ofGetElapsedTimef();
        if ( time >= _receiveLastTime2 + fpsUpdateSec ) {
            _receiveFPS2 = _receiveCount2 / ( time - _receiveLastTime2 );
            _receiveLastTime2 = time;
            _receiveCount2 = 0;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground( 255, 255, 255 );
    toggle1->draw();
    toggle2->draw();
	slider1->draw();
	slider2->draw();
    
    ofPushMatrix();
    ofTranslate(20,60);
    if ( image1.isAllocated() && Connect1 ) {
        ofSetColor( 255, 255, 255 );
        image1.draw( 0, 0 );
        ofSetColor( 0, 0, 0 );
        ofDrawBitmapString( "Received frame id = " + ofToString( _frameId1 ), 0, h + 20 );
        ofDrawBitmapString( "Receiving FPS = " + ofToString( _receiveFPS1, 1 ), 0, h + 40 );
        XML.reset();
        XML.setTo("//ADDRESS");
        ofDrawBitmapString( "PORT = " + XML.getValue("PORT1"), 0, h + 60);
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(w+40, 60);
    if ( image2.isAllocated() && Connect2) {
        ofSetColor( 255, 255, 255 );
        image2.draw( 0, 0 );
        ofSetColor( 0, 0, 0 );
        ofDrawBitmapString( "Received frame id = " + ofToString( _frameId2 ), 0, h + 20 );
        ofDrawBitmapString( "Receiving FPS = " + ofToString( _receiveFPS2, 1 ), 0, h + 40 );
        XML.reset();
        XML.setTo("//ADDRESS");
        ofDrawBitmapString( "PORT = " + XML.getValue("PORT2"), 0, h + 60);
    }
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target->getLabel() == "TOGGLE CONNECT1"){
        Connect1 =!Connect1;
        if(Connect1){
            //Starting network1
            XML.reset();
            XML.setTo("//ADDRESS");
            port1 = XML.getValue<int>("PORT1");
            packetSize = 1024;
            _receiver1.setup( port1, packetSize, true );
            _frameId1 = -1;
            _receiveLastTime1 = 0;
            _receiveCount1 = 0;
            _receiveFPS1 = 0;
        }else{
            _receiver1.close();
        }
        positionUIs();
    }else if(e.target->getLabel() == "TOGGLE CONNECT2"){
        Connect2 =!Connect2;
        if(Connect2){
            //Starting network2
            XML.reset();
            XML.setTo("//ADDRESS");
            port2 = XML.getValue<int>("PORT2");
            packetSize = 1024;
            _receiver2.setup( port2, packetSize, true );
            _frameId2 = -1;
            _receiveLastTime2 = 0;
            _receiveCount2 = 0;
            _receiveFPS2 = 0;
        }else{
            _receiver2.close();
        }
        positionUIs();
    }
}

//--------------------------------------------------------------
void ofApp::positionUIs()
{
    toggle1->setPosition(20 ,20);
	slider1->setPosition(0.5*w + 25,20);
    toggle2->setPosition(40 + w,20);
	slider2->setPosition(1.5*w + 45, 20);
}

//--------------------------------------------------------------
void ofApp::setUIs()
{
	toggle1->setWidth(0.5*w - 5);
	slider1->setWidth(0.5*w - 5, 0.3);
	toggle2->setWidth(0.5*w - 5);
	slider2->setWidth(0.5*w - 5, 0.3);
}

//--------------------------------------------------------------
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
	 if (e.target == slider1) {
		amplify1 = slider1->getValue();
	}
	else if (e.target == slider2) {
		amplify2 = slider2->getValue();
	}
}

//--------------------------------------------------------------
void ofApp::onParamFloat1Changed(float & f)
{
}

//--------------------------------------------------------------
void ofApp::onParamFloat2Changed(float & f)
{
}