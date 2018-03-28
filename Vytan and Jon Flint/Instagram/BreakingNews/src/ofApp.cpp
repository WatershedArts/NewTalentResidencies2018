#include "ofApp.h"

bool ofApp::str_to_uint16(const char *str, uint16_t &res) {
	char *end;
	errno = 0;
	long val = strtol(str, &end, 10);
	if (errno || end == str || *end != '\0' || val < 0 || val >= 0x10000) {
		return false;
	}
	res = (uint16_t)val;
	return true;
}

//--------------------------------------------------------------
void ofApp::setup() {

	// Just Tell Me Which Platform we are running on.
	switch (ofGetTargetPlatform()) {
		case OF_TARGET_OSX: cout << "Running on Mac Osx" << endl; break;
		case OF_TARGET_WINVS: cout << "Running on Windows" << endl; break;
		case OF_TARGET_IOS: cout << "Running on iOS" << endl; break;
		case OF_TARGET_ANDROID: cout << "Running on Android" << endl; break;
		case OF_TARGET_LINUX: cout << "Running on Linux 32" << endl; break;
		case OF_TARGET_LINUX64: cout << "Running on Linux 64" << endl; break;
		case OF_TARGET_LINUXARMV6L: cout << "Running on Linux Arm 6" << endl; break;
		case OF_TARGET_LINUXARMV7L: cout << "Running on Linux Arm 7" << endl; break;
		case OF_TARGET_EMSCRIPTEN: cout << "Running on Emscripten" << endl; break;
		default:break;
	}
	
	
	// This is our Config Loader
	// Given more time we could make this it to a json loader
	// but a key value txt file works just as well
	
	// These are defaults
	uint16_t displayaddress = 0x70;
	string buttonpin = "4";
	int gracetime = 1000;
	int tweentime = 750;
	string folderpath = "/Users/david/Desktop/PM Studio Residents/New Talent 2018/Vytan and Jon Flint/Instagram/media/";

	
	ofBuffer buffer = ofBufferFromFile(ofToDataPath("conf.txt"));
	
	// If there is no data use defaults
	if(buffer.size()) {
		for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
			string line = *it;
			
			if (!line.empty()) {
				vector<string> split = ofSplitString(line, ":");
				if(split[0] == "displayaddress") str_to_uint16(split[1].c_str(), displayaddress);
				else if(split[0] == "pin") buttonpin = split[1];
				else if(split[0] == "folder") folderpath = split[1];
				else if(split[0] == "gracetime") gracetime = ofToInt(split[1]);
				else if(split[0] == "tweentime") tweentime = ofToInt(split[1]);
			}
		}
	}
	else {
		cout << "Using Defaults" << endl;
	}
	
	videoHandler.setup(gracetime,tweentime);
	
	watcher.registerAllEvents(this);
	watcher.addPath(folderpath, true, &fileFilter);
	
	display.setupDisplay(displayaddress);
	button.setupButton(buttonpin);
	
	ofAddListener(button.buttonPressed, this, &ofApp::gotButtonPushedEvent);
	ofAddListener(button.buttonReleased, this, &ofApp::gotButtonReleasedEvent);
	
#ifndef TARGET_RASPBERRY_PI
	gui.setup();
#endif
	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(BACKGROUND_COLOR_1,BACKGROUND_COLOR_2,OF_GRADIENT_CIRCULAR);
	ofSetColor(ofColor::white);
	
#ifndef TARGET_RASPBERRY_PI
	videoHandler.draw(showDebug);
	ofPushStyle();
	ofFill();
	drawGui();
	ofPopStyle();
#else
	videoHandler.draw();
#endif
	
	char buffer[5];
	sprintf(buffer,"%04d",videoHandler.getPlaysForCurrentVideo());
	string ss = buffer;
	display.setDisplay(ss);
	button.draw();
}

#ifndef TARGET_RASPBERRY_PI
//--------------------------------------------------------------
void ofApp::drawGui() {
	gui.begin();
	ImGui::Checkbox("Show Debug", &showDebug);
	gui.end();
}
#endif

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
	int height = ofGetHeight();
	
	std::size_t numLines = static_cast<std::size_t>(height / 14);
	messages.push_front(msg.message);
	
	while (messages.size() > numLines) {
		messages.pop_back();
	}
}

//--------------------------------------------------------------
void ofApp::onDirectoryWatcherItemAdded(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt) {
	if(ofIsStringInString(evt.item.path(), ".mp4")) {
		videoHandler.addNewVideoToQueue(evt.item.path());
		cout << "Added: " << evt.item.path() << endl;
	}
}

//--------------------------------------------------------------
void ofApp::onDirectoryWatcherItemRemoved(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt) {
	ofSendMessage("Removed:  " + evt.item.path());
}

//--------------------------------------------------------------
void ofApp::onDirectoryWatcherItemModified(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt) {
	ofSendMessage("Modified: " + evt.item.path());
}

//--------------------------------------------------------------
void ofApp::onDirectoryWatcherItemMovedFrom(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt) {
	ofLogNotice("ofApp::onDirectoryWatcherItemMovedFrom") << "Moved From: " << evt.item.path();
}

//--------------------------------------------------------------
void ofApp::onDirectoryWatcherItemMovedTo(const ofxIO::DirectoryWatcherManager::DirectoryEvent& evt) {
	ofLogNotice("ofApp::onDirectoryWatcherItemMovedTo") << "Moved To: " << evt.item.path();
}

//--------------------------------------------------------------
void ofApp::onDirectoryWatcherError(const Poco::Exception& exc) {
	ofLogError("ofApp::onDirectoryWatcherError") << "Error: " << exc.displayText();
}

//--------------------------------------------------------------
void ofApp::gotButtonPushedEvent(string &evt) {
	cout << evt << " Pushed" << endl;
}

//--------------------------------------------------------------
void ofApp::gotButtonReleasedEvent(string &evt) {
	cout << evt << " Released" << endl;
	videoHandler.replayVideo();
}
