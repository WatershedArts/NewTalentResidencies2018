#pragma once

#include "ofMain.h"
#include "ofxIO.h"
#include "VideoHandler.h"
#include "AlphanumericDisplay.h"
#include "Button.h"

#ifndef TARGET_RASPBERRY_PI
	#include "ofxImGui.h"
#endif

#define BACKGROUND_COLOR_1 ofColor(0,0,0)
#define BACKGROUND_COLOR_2 ofColor(50,50,50)

//	Br34kingVi3ws!

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	
	#ifndef TARGET_RASPBERRY_PI
		void drawGui();
		ofxImGui::Gui gui;
		bool showDebug;
	#endif
	
		void keyPressed(int key);
		void keyReleased(int key);
		void gotMessage(ofMessage msg);
	
		ofxIO::DirectoryWatcherManager watcher;
		ofxIO::HiddenFileFilter fileFilter;
		void onDirectoryWatcherItemAdded(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt);
		void onDirectoryWatcherItemRemoved(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt);
		void onDirectoryWatcherItemModified(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt);
		void onDirectoryWatcherItemMovedFrom(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt);
		void onDirectoryWatcherItemMovedTo(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt);
		void onDirectoryWatcherError(const Poco::Exception& exc);
	
		deque <string> messages;
		deque <VideoData> videoFiles;
	
		VideoHandler videoHandler;
		AlphanumericDisplay display;
	
		Button button;
		void gotButtonPushedEvent(string &evt);
		void gotButtonReleasedEvent(string &evt);
	
		bool str_to_uint16(const char *str, uint16_t &res);
};
