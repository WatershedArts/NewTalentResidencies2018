#include "ofApp.h"

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
	
	if(font.load("verdana.ttf",10) ) {
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
	names.push_back("Alright ");
	names.push_back("then");
	names.push_back("picture ");
	names.push_back("this ");
	names.push_back("if ");
	names.push_back("you ");
	names.push_back("will");
	names.push_back("10 ");
	names.push_back("to ");
	names.push_back("2AM ");
	names.push_back("X ");
	names.push_back("Yogi ");
	names.push_back("DMT ");
	names.push_back("and ");
	names.push_back("a ");
	names.push_back("box ");
	names.push_back("of ");
	names.push_back("Krispy ");
	names.push_back("Kremes ");
	names.push_back("in ");
	names.push_back("my ");
	names.push_back("need ");
	names.push_back("to ");
	names.push_back("know ");
	names.push_back("pose ");
	names.push_back("just ");
	names.push_back("outside ");
	names.push_back("of ");
	names.push_back("Area ");
	names.push_back("51");
	names.push_back("Contemplating ");
	names.push_back("the ");
	names.push_back("whole ");
	names.push_back("chosen");
	names.push_back("people ");
	names.push_back("thing");
	names.push_back("with ");
	names.push_back("just ");
	names.push_back("a ");
	names.push_back("flaming ");
	names.push_back("stealth ");
	names.push_back("banana ");
	names.push_back("split");
	names.push_back("the ");
	names.push_back("sky ");
	names.push_back("like ");
	names.push_back("one ");
	names.push_back("would ");
	names.push_back("hope ");
	names.push_back("but ");
	names.push_back("never ");
	names.push_back("really ");
	names.push_back("expect ");
	names.push_back("to ");
	names.push_back("see ");
	names.push_back("in ");
	names.push_back("a ");
	names.push_back("place ");
	names.push_back("like ");
	names.push_back("this.");
	names.push_back("Cutting ");
	names.push_back("right ");
	names.push_back("angle ");
	names.push_back("donuts ");
	names.push_back("on ");
	names.push_back("a ");
	names.push_back("dime ");
	names.push_back("and ");
	names.push_back("stopping ");
	names.push_back("right ");
	names.push_back("at ");
	names.push_back("my ");
	names.push_back("Birkenstocks, ");
	names.push_back("and ");
	names.push_back("me ");
	names.push_back("yelping...");
	names.push_back("Holy ");
	names.push_back("fucking ");
	names.push_back("shit!");
	names.push_back("Then ");
	names.push_back("the ");
	names.push_back("X-Files ");
	names.push_back("being");
	names.push_back("looking");
	names.push_back("like ");
	names.push_back("some ");
	names.push_back("kind ");
	names.push_back("of ");
	names.push_back("blue-green ");
	names.push_back("Jackie Chan ");
	names.push_back("with ");
	names.push_back("Isabella ");
	names.push_back("Rossellini ");
	names.push_back("lips ");
	names.push_back("and ");
	names.push_back("breath ");
	names.push_back("that ");
	names.push_back("reeked ");
	names.push_back("of ");
	names.push_back("vanilla ");
	names.push_back("Chig ");
	names.push_back("Champa,");
	names.push_back("Did");
	names.push_back("a ");
	names.push_back("slow-mo");
	names.push_back("Matrix");
	names.push_back("descent");
	names.push_back("out");
	names.push_back("of");
	names.push_back("the");
	names.push_back("butt");
	names.push_back("end");
	names.push_back("of");
	names.push_back("the");
	names.push_back("banana ");
	names.push_back("vessel ");
	names.push_back("and ");
	names.push_back("hovered ");
	names.push_back("above ");
	names.push_back("my ");
	names.push_back("bug-eyes ");
	names.push_back("my ");
	names.push_back("gaping ");
	names.push_back("jaw ");
	names.push_back("and ");
	names.push_back("my ");
	names.push_back("sweaty ");
	names.push_back("L. ");
	names.push_back("Ron Hubbard ");
	names.push_back("upper ");
	names.push_back("lip ");
	names.push_back("and ");
	names.push_back("all ");
	names.push_back("I ");
	names.push_back("could ");
	names.push_back("think ");
	names.push_back("was: ");
	names.push_back("I ");
	names.push_back("hope ");
	names.push_back("Uncle ");
	names.push_back("Martin ");
	names.push_back("here ");
	names.push_back("doesn't ");
	names.push_back("notice");
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
	torch *= ofPoint(1.6,1.6);
	
	
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
	
	auto settings = ofxImGui::Settings();
	
	ofDrawBitmapStringHighlight(currentSelection,15,15);
	
	gui.begin();
	ofxImGui::AddGroup(imageProcessor.imageProc,settings);
	ofxImGui::AddGroup(imageProcessor.cameraSettings,settings);
	gui.end();
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
