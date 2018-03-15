#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxImGui.h"
#include "ofxCv.h"
#include "ImageProcessing.h"
#include "ofxCenteredTrueTypeFont.h"
#include "VisualManager.h"
#include "ofxTextAlignTTF.h"


class ofApp : public ofBaseApp{

	public:
		void setupBuffers();
		void setup();
		void update();
		void draw();
		void drawGui();
		void keyPressed(int key);

		ImageProcessing imageProcessor;
		VisualManager visualManager;
	
		ofParameter <bool> showContentPreview { "Show Content Preview", false };
		ofParameter <bool> disableShader { "Disable Mask", false };
	
		ofxAutoReloadedShader shader;
		ofFbo shaderBuffer;
		ofFbo screenInfo;

		ofxImGui::Gui gui;
		ofxTextAlignTTF *text;
	
		bool isOpen;
		bool bDrawGui;
};
