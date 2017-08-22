#include "ofApp.h"

int previewWidth = 640;
int previewHeight = 480;

//--------------------------------------------------------------
void ofApp::setup() {
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();


	w = 512;
	h = 424;
	size = w * h * 1;
	ofSetWindowShape(w + 40, h + 80);

	data = new unsigned char[w*h * 2];

	int port ;
	int packetSize ;
	string addr;

	XML.load("settings.xml");
	if (XML.load("settings.xml")) {
	}
	else {
		XML.addChild("ADDRESS");
		XML.setTo("ADDRESS");

		XML.addChild("ADDRESS1");
		XML.setTo("ADDRESS1");
		XML.addChild("IP");
		XML.addChild("PORT");
		XML.setValue("//IP", "192.168.1.107");
		XML.setValue("//PORT", "12345");

		XML.setToParent();
		XML.addChild("ADDRESS2");
		XML.setTo("ADDRESS2");
		XML.addChild("IP");
		XML.addChild("PORT");
		XML.setValue("//IP", "192.168.1.108");
		XML.setValue("//PORT", "12346");
		XML.save("settings.xml");
	}

	XML.setTo("//ADDRESS/ADDRESS1");
	port = XML.getValue<int>("PORT");
	addr = XML.getValue<string>("IP");
	packetSize = 1024;

	ofSetWindowTitle("KinectDepthShortImageSender "+addr+":"+ ofToString(port));

	_sender.setup(addr, port, packetSize);
	_frameId = 0;

	grabber.setDeviceID(0);
	grabber.setDesiredFrameRate(30);
	grabber.initGrabber(w, h);

}

//--------------------------------------------------------------
void ofApp::update() {
	kinect.update();

	{
		auto bodies = kinect.getBodySource()->getBodies();
		for (auto body : bodies) {
			for (auto joint : body.joints) {
				//now do something with the joints
			}
		}
	}

	{
		// Note that for this we need a reference of which joints are connected to each other.
		// We call this the 'boneAtlas', and you can ask for a reference to this atlas whenever you like
		auto bodies = kinect.getBodySource()->getBodies();
		auto boneAtlas = ofxKinectForWindows2::Data::Body::getBonesAtlas();

		for (auto body : bodies) {
			for (auto bone : boneAtlas) {
				auto firstJointInBone = body.joints[bone.first];
				auto secondJointInBone = body.joints[bone.second];

				//now do something with the joints
			}
		}
	}

	//grabber.update();	//Update grabber state

	//if (grabber.isFrameNew()) {	//Check for new frame
	//							//Send frame via network
	//	unsigned char *pixels = grabber.getPixels().getData();
	//	_sender.send(pixels, size, _frameId++);
	//}

	//ofImage img;
	//img.getImageType();

	//ofSetColor(0, 0, 0);
	//ofDrawBitmapString(img.getImageType(), 20, 40);

	//img.setFromPixels(kinect.getDepthSource()->getPixels());
	//img.setImageType(OF_IMAGE_GRAYSCALE);

	//kinect.getDepthSource()->getPixels();

	unsigned short *spixels = kinect.getDepthSource()->getPixels().getData();

	if (spixels != nullptr) {
		for (int i = 0; i < w*h; i++) {
			data[i * 2] = spixels[i] & 0xFF;
			data[i * 2 + 1] = spixels[i] >> 8;
		}
		_sender.send(data, w * h * 2, _frameId++);
	}

	//unsigned char *pixels = img.getPixels().getData();
	//_sender.send(pixels, size, _frameId++);



}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);
	kinect.getDepthSource()->draw(20, 20, w, h);  // note that the depth texture is RAW so may appear dark

	unsigned short *spixels = kinect.getDepthSource()->getPixels().getData();

	int port;
	string addr;
	XML.setTo("//ADDRESS/ADDRESS1");
	port = XML.getValue<int>("PORT");
	addr = XML.getValue<string>("IP");

	ofSetColor(0,0,0);
	ofDrawBitmapString(addr+" :: "+ ofToString(port), 20, h + 40);
	ofDrawBitmapString("Sent frame id = " + ofToString(_frameId), 20, h + 60);



	//the_value = 65530;
	//low_byte = *((unsigned char *)&the_value);
	//high_byte = *((unsigned char *)((&the_value) + 1));

	//the_value = st;
	//low_byte = the_value & 0xFF;
	//high_byte = the_value >> 8;

	//ofSetColor(255, 255, 255);
	//ofDrawBitmapString(int(low_byte), 20, h + 130);
	//ofDrawBitmapString(int(high_byte), 20, h + 160);
	//ofDrawBitmapString(int((high_byte << 8) | low_byte), 20, h + 190);

	// Color is at 1920x1080 instead of 512x424 so we should fix aspect ratio
	//float colorHeight = previewWidth * (kinect.getColorSource()->getHeight() / kinect.getColorSource()->getWidth());
	//float colorTop = (previewHeight - colorHeight) / 2.0;

	//kinect.getColorSource()->draw(previewWidth, 0 + colorTop, previewWidth, colorHeight);
	//kinect.getBodySource()->drawProjected(previewWidth, 0 + colorTop, previewWidth, colorHeight);

	//kinect.getInfraredSource()->draw(0, previewHeight, previewWidth, previewHeight);

	//kinect.getBodyIndexSource()->draw(previewWidth, previewHeight, previewWidth, previewHeight);
	//kinect.getBodySource()->drawProjected(previewWidth, previewHeight, previewWidth, previewHeight, ofxKFW2::ProjectionCoordinates::DepthCamera);

	//ofBackground(200, 255, 200);

	////Draw camera image
	//ofSetColor(255, 255, 255);
	//grabber.draw(20, 20);

	////Print frame id
	//ofSetColor(0, 0, 0);
	//ofDrawBitmapString("Sent frame id = " + ofToString(_frameId), 20, h + 40);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
