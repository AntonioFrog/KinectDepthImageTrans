#pragma once

#include "ofMain.h"
#include "pbNetwork.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    int w, h;
	int size;
    ofXml XML;
    
    int port1 ;
    int port2 ;
    int packetSize ;

    ofShortImage image1;
    unsigned short *data1;
    unsigned char *dataChar1;
	pbNetworkReceiver _receiver1;
	int _frameId1;
	float _receiveLastTime1;
	int _receiveCount1;
	float _receiveFPS1;

    ofShortImage image2; 
    unsigned short *data2;
    unsigned char *dataChar2;
    pbNetworkReceiver _receiver2;
    int _frameId2;
    float _receiveLastTime2;
    int _receiveCount2;
    float _receiveFPS2;

	void positionUIs();
	void setUIs();

    bool Connect1;
    bool Connect2;
    ofxDatGuiButton* toggle1;
    ofxDatGuiButton* toggle2;
    void onButtonEvent(ofxDatGuiButtonEvent e);

	float amplify1;
	float amplify2;
	ofxDatGuiSlider* slider1;
	ofxDatGuiSlider* slider2;
	ofParameter<float> ofParamFloat1;
	ofParameter<float> ofParamFloat2;
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void onParamFloat1Changed(float & pFloat);
	void onParamFloat2Changed(float & pFloat);
};
