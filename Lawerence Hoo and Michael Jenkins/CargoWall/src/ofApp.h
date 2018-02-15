#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxImGui.h"
#include "ofxCv.h"
#include "ImageProcessing.h"
#include "ofxCenteredTrueTypeFont.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void setupBuffers();
		void drawGui();
	
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		vector <ofPoint> drawPoints;
	
		void drawNames();
	
		ImageProcessing imageProcessor;
	
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
