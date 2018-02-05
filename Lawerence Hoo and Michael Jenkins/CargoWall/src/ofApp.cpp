#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setupBuffers() {
	
	shader.load("Shaders/cargoshader");
	imageProcessor.setup();
	
	shaderBuffer.allocate(ofGetWidth(), ofGetHeight());
	shaderBuffer.begin();
	ofClear(0, 0, 0, 255);
	shaderBuffer.end();
	ofEnableAlphaBlending();
	img.load("B.jpg");
	
	defaultVideo.load("Shadowing.mp4");
	defaultVideo.play();
}

//--------------------------------------------------------------
void ofApp::setup() {
	
	bShowCVImages = false;
	bDoDebugControl = false;

	setupBuffers();
}

//--------------------------------------------------------------
void ofApp::update() {
	defaultVideo.update();
	imageProcessor.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	imageProcessor.draw();
	
	shaderBuffer.begin();
	ofClear(0, 0, 0, 0);
	shader.begin();
	shader.setUniform1f("u_time", ofGetElapsedTimef());
	shader.setUniform2f("u_resolution", ofVec2f(ofGetWidth(),ofGetHeight()));
	shader.setUniformTexture("maskTex", imageProcessor.getMaskImage(), 1);
	ofSetColor(255, 255, 255);
	defaultVideo.draw(0, 0);
	shader.end();
	shaderBuffer.end();

	shaderBuffer.draw(0,0);
	imageProcessor.drawGui();
}

//--------------------------------------------------------------
void ofApp::drawCv() {
	ofPushStyle();
	ofSetColor(255);
	
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	switch (key) {
		case 'd': imageProcessor.setMode(DEBUG_PROCESSING); break;
		case 'v': imageProcessor.setMode(VIDEO_PROCESSING); break;
		case 'l': imageProcessor.setMode(LIVE_PROCESSING); break;
		default:break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
