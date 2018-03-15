#include "ofApp.h"
float scaleW = 1.0;
float scaleH = 1.0;

static int scaleFactor = 4;
//--------------------------------------------------------------
void ofApp::setupBuffers() {
	
	// Setup the universal font file
	unsigned int flags;
	flags |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
	flags |= ofxTextAlign::VERTICAL_ALIGN_MIDDLE;
	text = new ofxTextAlignTTF();
	text->load(OF_TTF_SANS,15);
	
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
}

//--------------------------------------------------------------
void ofApp::setup() {
	bDrawGui = true;
	ofSetDataPathRoot("../Resources/data/");
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	// Do the Above Setup
	setupBuffers();
	
	isOpen = true;
	visualManager.setup();
}

//--------------------------------------------------------------
void ofApp::update() {
	imageProcessor.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	scaleW = ofGetWidth() / 640;
	scaleH = ofGetHeight() / 480;
	
	ofPoint torch = imageProcessor.getBiggestCoordinate();
	visualManager.update(ofMap(torch.x,0,640,0,ofGetWidth()), ofMap(torch.y,0,480,0,ofGetHeight()));
	
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
	
	ofSetColor(ofColor::white);
	
	// Draw Stuff
	imageProcessor.draw();

	// Draw a Small Segment which show whats behind the mask
	if(showContentPreview)
		screenInfo.draw(ofGetWidth() - ofGetWidth()/scaleFactor,ofGetHeight() - ofGetHeight()/scaleFactor,ofGetWidth()/scaleFactor,ofGetHeight()/scaleFactor);
	
	// Draw the GUI
	if(bDrawGui) {
		drawGui();
		ofPushStyle();
		ofSetColor(ofColor::red,250);
		ofDrawCircle(ofMap(torch.x,0,640,0,ofGetWidth()), ofMap(torch.y,0,480,0,ofGetHeight()), 10);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == OF_KEY_TAB) {
		bDrawGui = !bDrawGui;
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
		if( ImGui::Button("Show Content Folder") ) {
			ofSystem("open " + ofToDataPath("Visuals",false));
		}
		
		if( ImGui::Button("Reload Content") ) {
			visualManager.reloadContent();
		}
		ImGui::Separator();
		
		ofxImGui::AddParameter(showContentPreview);
		ofxImGui::AddParameter(disableShader);
		
		if(ofxImGui::BeginTree(this->imageProcessor.imageProc, mainSettings)) {
			ofxImGui::AddParameter(this->imageProcessor.bShowCv);
			ofxImGui::AddParameter(this->imageProcessor.bShowCvCalibration);
			ofxImGui::AddParameter(this->imageProcessor.bFillArea);
			ImGui::Separator();
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
			
			static const vector<string> labels = { "Video", "Poems", "Photos","Animated Poems","Textures","Particles"};
			ofxImGui::AddRadio(this->visualManager.currentMode, labels, 1);
		
			
			ImGui::Separator();
		
			if( ImGui::Button("Previous Line") ) {
				visualManager.animatedPoemHandler.previousLine();
			}
			ImGui::SameLine();
			if( ImGui::Button("Next Line") ) {
				visualManager.animatedPoemHandler.nextLine();
			}
			
			ImGui::Separator();
			if( ImGui::Button("Previous Texture") ) {
				visualManager.textureHandler.previousTexture();
			}
			ImGui::SameLine();
			if( ImGui::Button("Next Texture") ) {
				visualManager.textureHandler.nextTexture();
			}
			
			ImGui::Separator();
			if( ImGui::Button("Previous Poem") ) {
				visualManager.poemHandler.previousPoem();
				visualManager.animatedPoemHandler.previousPoem();
			}
			ImGui::SameLine();
			if( ImGui::Button("Next Poem") ) {
				visualManager.poemHandler.nextPoem();
				visualManager.animatedPoemHandler.nextPoem();
			}
			ImGui::Separator();
			if( ImGui::Button("Previous Particles") ) {
				visualManager.particleHandler.previousLine();
			}
			ImGui::SameLine();
			if( ImGui::Button("Next Particle") ) {
				visualManager.particleHandler.nextLine();
			}
			ImGui::Separator();
			if( ImGui::Button("Previous Video") ) {
				visualManager.videoHandler.previousVideo();
			}
			ImGui::SameLine();
			if( ImGui::Button("Next Video") ) {
				visualManager.videoHandler.nextVideo();
			}
			ofxImGui::EndTree(mainSettings);
		}
	}
	ofxImGui::EndWindow(mainSettings);
	gui.end();
	ofDrawBitmapStringHighlight("Current FPS: " + ofToString(ofGetFrameRate()), 10,ofGetHeight()-25);
}
