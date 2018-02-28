#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxImGui.h"
#include "ofxCv.h"
#include "ImageProcessing.h"
#include "ofxCenteredTrueTypeFont.h"
#include "VisualManager.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void setupBuffers();
	
		void keyPressed(int key);
	
		void drawGui();
	
		vector <ofPoint> drawPoints;
	
		void drawNames();
	
		ImageProcessing imageProcessor;
		VisualManager visualManager;
	
		ofParameter <bool> showContentPreview { "Show Content Preview", false };
		ofParameter <bool> disableShader { "Disable Mask", false };
	
		ofxAutoReloadedShader shader;
		ofFbo shaderBuffer;
		ofFbo screenInfo;
	
		ofxImGui::Gui gui;
	
		ofImage image;
		ofImage img;
		ofVideoPlayer defaultVideo;
	
		ofxCenteredTrueTypeFont font;
		bool isOpen;
	
		string currentSelection;
	
		vector<string> names;
};
