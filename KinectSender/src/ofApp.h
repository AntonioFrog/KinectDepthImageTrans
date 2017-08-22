#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "pbNetwork.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();



	ofxKFW2::Device kinect;
	int w, h;						//Width and height of image
	int size;						//Size of image data in bytes

	ofVideoGrabber 	grabber;		//Video grabber

	pbNetworkSenderSync _sender;	//Network sender
	int _frameId;					//id of sent frame

	ofXml XML;



	unsigned short st;
	unsigned char low_byte;
	unsigned char high_byte;
	unsigned short the_value;
	unsigned char *data;

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
};
