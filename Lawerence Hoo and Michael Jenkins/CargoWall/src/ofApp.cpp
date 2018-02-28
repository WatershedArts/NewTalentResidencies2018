#include "ofApp.h"
float scaleW = 1.0;
float scaleH = 1.0;

static int scaleFactor = 4;
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
	
	visualManager.setup();
	
	
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
}

//--------------------------------------------------------------
void ofApp::update() {
	defaultVideo.update();
	imageProcessor.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofPoint torch = imageProcessor.getBiggestCoordinate();
	torch *= ofPoint(scaleW,scaleH);
	
	screenInfo.begin();
		ofClear(0, 0, 0, 255);
		visualManager.draw();
	screenInfo.end();
	
	// Background to Black
	ofBackground(0);

	if(!disableShader) {
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
	}
	screenInfo.draw(0,0);

	if(!disableShader) {
		// Close both the buffer and shader
		shader.end();
		shaderBuffer.end();

		// Draw the Shaded Buffer
		shaderBuffer.draw(0,0);
	}
	
	ofSetColor(ofColor::red,50);
	ofDrawCircle(torch.x, torch.y, 10);
	
	ofSetColor(ofColor::white);
	
	// Draw Stuff
	imageProcessor.draw();
	
	
	if(showContentPreview)
		screenInfo.draw(ofGetWidth() - ofGetWidth()/scaleFactor,ofGetHeight() - ofGetHeight()/scaleFactor,ofGetWidth()/scaleFactor,ofGetHeight()/scaleFactor);
	
	drawGui();
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
void ofApp::drawGui() {
	auto mainSettings = ofxImGui::Settings();
	mainSettings.windowPos = ofVec2f(ofGetWidth()-400,10);
	mainSettings.windowSize = ofVec2f(400,10);
	gui.begin();
	
	if (ofxImGui::BeginWindow("Settings", mainSettings, false))
	{
		ofxImGui::AddParameter(showContentPreview);
		ofxImGui::AddParameter(disableShader);
		
		
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
		
		ofParameterGroup d {"Visual Manager",this->visualManager.currentMode};
		if(ofxImGui::BeginTree(d, mainSettings)) {
			
			static const vector<string> labels = { "Video", "Text", "Animation","Shaders" };
			ofxImGui::AddRadio(this->visualManager.currentMode, labels, 4);
		
			ImGui::Separator();
			if( ImGui::Button("Previous Poem") ) {
				visualManager.poemHandler.previousPoem();
			}
			
			if( ImGui::Button("Next Poem") ) {
				visualManager.poemHandler.nextPoem();
			}
			ofxImGui::EndTree(mainSettings);
		}
	}
	ofxImGui::EndWindow(mainSettings);
	gui.end();
}
