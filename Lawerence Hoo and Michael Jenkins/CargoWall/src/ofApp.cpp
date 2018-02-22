#include "ofApp.h"
float scaleW = 1.0;
float scaleH = 1.0;
//--------------------------------------------------------------
void ofApp::setupBuffers() {
	
	// Load the Cargo Shader which is essentially a Alpha Masking Shader
	shader.load("Shaders/cargoshader");
	
	// Setup the Image Processing
	imageProcessor.setup();
	
	// Allocate the Screen Space Buffer
	shaderBuffer.allocate(ofGetWidth(), ofGetHeight());
	shaderBuffer.begin();
	ofClear(0, 0, 0, 255);
	shaderBuffer.end();
	
	// Allocate the Screen Space Buffer
	screenInfo.allocate(ofGetWidth(), ofGetHeight());
	screenInfo.begin();
	ofClear(0, 0, 0, 255);
	screenInfo.end();
	
	// Enable the Alpha Channel
	ofEnableAlphaBlending();
	
	// Load Some Developement Images
	img.load("B.jpg");
	image.load("test.jpg");
	
	// Load a Dev Video
	defaultVideo.load("Shadowing.mp4");
	defaultVideo.play();
	
	if(font.load("verdana.ttf",15) ) {
		cout << "Font Loaded"  << endl;
	}
	
}

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	// Do the Above Setup
	setupBuffers();
	
	isOpen = true;
	currentSelection = "";
	
	int spacingWidth = ofGetWidth() / 12;
	int spacingHeight = ofGetHeight() / 12;
	
	scaleW = ofGetWidth() / 640;
	scaleH = ofGetHeight() / 480;
	
	for(int y = 0; y < 12; y++)
	{
		for(int x = 0; x < 12; x++)
		{
			int offset = 10;
			if (y % 2) {
				offset = 60;
			}
			
			ofPoint p = ofPoint(offset+(x*spacingWidth),10+(y*spacingHeight));
			drawPoints.push_back(p);
		}
	}
	names.push_back("And");
	names.push_back("you");
	names.push_back("can");
	names.push_back("fake");
	names.push_back("smiles");
	names.push_back("to");
	names.push_back("make");
	names.push_back("you");
	names.push_back("feel");
	names.push_back("When");
	names.push_back("its");
	names.push_back("time");
	names.push_back("to");
	names.push_back("break");
	names.push_back("the");
	names.push_back("deal");
	names.push_back("Cos");
	names.push_back("it's");
	names.push_back("easy");
	names.push_back("now,");
	names.push_back("i'm");
	names.push_back("easy");
	names.push_back("now");
	names.push_back("And");
	names.push_back("buy");
	names.push_back("a");
	names.push_back("new");
	names.push_back("disease");
	names.push_back("When");
	names.push_back("it");
	names.push_back("pays");
	names.push_back("to");
	names.push_back("sell");
	names.push_back("the");
	names.push_back("cure");
	names.push_back("It's");
	names.push_back("easy");
	names.push_back("now,");
	names.push_back("im");
	names.push_back("easy");
	names.push_back("now");
	names.push_back("Cos");
	names.push_back("i");
	names.push_back("can");
	names.push_back("climb");
	names.push_back("mountains");
	names.push_back("I");
	names.push_back("can");
	names.push_back("spit");
	names.push_back("further");
	names.push_back("than");
	names.push_back("you");
	names.push_back("can");
	names.push_back("hurl");
	names.push_back("And");
	names.push_back("i");
	names.push_back("can");
	names.push_back("swallow");
	names.push_back("failure");
	names.push_back("just");
	names.push_back("to");
	names.push_back("learn");
	names.push_back("And");
	names.push_back("i");
	names.push_back("can");
	names.push_back("build");
	names.push_back("bridges");
	names.push_back("from,");
	names.push_back("what");
	names.push_back("you");
	names.push_back("burn");
	names.push_back("And");
	names.push_back("you");
	names.push_back("can");
	names.push_back("fake");
	names.push_back("smiles");
	names.push_back("to");
	names.push_back("make");
	names.push_back("you");
	names.push_back("feel");
	names.push_back("When");
	names.push_back("its");
	names.push_back("time");
	names.push_back("to");
	names.push_back("close");
	names.push_back("the");
	names.push_back("deal");
	names.push_back("And");
	names.push_back("it's");
	names.push_back("easy");
	names.push_back("now,");
	names.push_back("i'm");
	names.push_back("easy");
	names.push_back("now");
	names.push_back("And");
	names.push_back("buy");
	names.push_back("a");
	names.push_back("new");
	names.push_back("disease");
	names.push_back("When");
	names.push_back("it");
	names.push_back("pays");
	names.push_back("to");
	names.push_back("sell");
	names.push_back("the");
	names.push_back("cure");
	names.push_back("It's");
	names.push_back("easy");
	names.push_back("now,");
	names.push_back("i'm");
	names.push_back("easy");
	names.push_back("now");
	names.push_back("Cos");
	names.push_back("i");
	names.push_back("can");
	names.push_back("run,");
	names.push_back("i");
	names.push_back("can");
	names.push_back("hide");
	names.push_back("I");
	names.push_back("can");
	names.push_back("take");
	names.push_back("a");
	names.push_back("world");
	names.push_back("of");
	names.push_back("pain");
	names.push_back("in");
	names.push_back("a");
	names.push_back("stride");
	names.push_back("And");
	names.push_back("i");
	names.push_back("don't");
	names.push_back("need");
	names.push_back("fairy");
	names.push_back("tales");
	names.push_back("to");
	names.push_back("pretend");
	names.push_back("And");
	names.push_back("i");
	names.push_back("don't");
	names.push_back("need");
	names.push_back("heroes");
}

//--------------------------------------------------------------
void ofApp::update() {
	defaultVideo.update();
	imageProcessor.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	screenInfo.begin();
	ofClear(0, 0, 0, 255);
	ofPoint torch = imageProcessor.getBiggestCoordinate();
	torch *= ofPoint(scaleW,scaleH);
	
	
	for (int i = 0; i < names.size(); i++) {
		if (ofDist(torch.x, torch.y, drawPoints[i].x, drawPoints[i].y) < 20) {
			ofSetColor(255, 0, 0, 255);
			currentSelection = names[i];
		}
		else {
			ofSetColor(255, 255, 255, 200);
		}

		font.drawStringCentered(names[i], drawPoints[i].x, drawPoints[i].y);
	}
	screenInfo.end();
	
	// Background to Black
	ofBackground(0);

	// Open the Buffer
	shaderBuffer.begin();
	
	// Clear it to Black
	ofClear(0, 0, 0, 255);
	
	// Open the Shader
	shader.begin();
	
	// Set the time and resolution variables
	shader.setUniform1f("u_time", ofGetElapsedTimef());
	shader.setUniform2f("u_resolution", ofVec2f(ofGetWidth(),ofGetHeight()));
	
	// This is where we pass through the masking texture
	shader.setUniformTexture("maskTex", imageProcessor.getMaskImage(), 1);
	
	screenInfo.draw(0,0);

	// Close both the buffer and shader
	shader.end();
	shaderBuffer.end();

	// Draw the Shaded Buffer
	shaderBuffer.draw(0,0);
	
	ofSetColor(ofColor::red,50);
	ofDrawCircle(torch.x, torch.y, 10);
	
	ofSetColor(ofColor::white);
	
	// Draw so
	imageProcessor.draw();
	
	auto mainSettings = ofxImGui::Settings();
	mainSettings.windowPos = ofVec2f(ofGetWidth()-200,10);
	mainSettings.windowSize = ofVec2f(200,10);
	gui.begin();
	
	if (ofxImGui::BeginWindow("Settings", mainSettings, false))
	{
		if(ofxImGui::BeginTree(this->imageProcessor.imageProc, mainSettings)) {
			
			ofxImGui::AddParameter(this->imageProcessor.bShowCv);
			ofxImGui::AddParameter(this->imageProcessor.bShowCvCalibration);
			ofxImGui::AddParameter(this->imageProcessor.bFillArea);
			ofxImGui::AddParameter(this->imageProcessor.cvImagesOpacity);
			ofxImGui::AddParameter(this->imageProcessor.fadeLevel);
			ofxImGui::AddParameter(this->imageProcessor.brushScale);
			ofxImGui::AddParameter(this->imageProcessor.thresholdAmount);
			ofxImGui::AddParameter(this->imageProcessor.maxArea);
			ofxImGui::AddParameter(this->imageProcessor.minArea);
			ofxImGui::AddParameter(this->imageProcessor.contourThreshold);
			ofxImGui::EndTree(mainSettings);
		}
		
		if(ofxImGui::BeginTree(this->imageProcessor.cameraSettings, mainSettings)) {
			ofxImGui::AddParameter(this->imageProcessor.cameraBrightness);
			ofxImGui::AddParameter(this->imageProcessor.cameraExposure);
			ofxImGui::AddParameter(this->imageProcessor.cameraGain);
			ofxImGui::AddParameter(this->imageProcessor.cameraContrast);
			ofxImGui::AddParameter(this->imageProcessor.cameraFramerate);
			ofxImGui::AddParameter(this->imageProcessor.cameraVFlip);
			ofxImGui::AddParameter(this->imageProcessor.cameraHFlip);
			ofxImGui::EndTree(mainSettings);
		}
		
//		ofxImGui::AddGroup(imageProcessor.imageProc,settings);
//		ofxImGui::AddGroup(imageProcessor.cameraSettings,settings);
	}
	ofxImGui::EndWindow(mainSettings);
	gui.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	switch (key) {
//		case 'd': imageProcessor.setMode(DEBUG_PROCESSING); break;
//		case 'v': imageProcessor.setMode(VIDEO_PROCESSING); break;
//		case 's': imageSelection.saveToFile("w.xml"); break;
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
//	imageSelection.updatePoint(x, y, 0, 0, 640, 480);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//	imageSelection.selectPoint(x, y, 0, 0, 640, 480, 30);
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
