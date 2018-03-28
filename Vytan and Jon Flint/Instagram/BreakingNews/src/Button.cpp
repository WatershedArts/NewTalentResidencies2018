//
//  Button.cpp
//  InstagramTv
//
//  Created by David Haylock on 05/03/2018.
//

#include "Button.h"

//--------------------------------------------------------------
Button::Button() {}

//--------------------------------------------------------------
Button::~Button() {}

//--------------------------------------------------------------
void Button::setupButton(string gpioNumber,string buttonName) {
	
	#ifndef TARGET_RASPBERRY_PI
		text.load(OF_TTF_SANS, 12);
		fakeButton.set(ofGetWidth()-75, ofGetHeight()-75, 50, 50);
		ofAddListener(ofEvents().mousePressed, this, &Button::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &Button::mouseReleased);
		buttonPushed = false;
	#else
		gpioPin.setup(gpioNumber);
		gpioPin.export_gpio();
		gpioPin.setdir_gpio("in");
	#endif
	
	this->gpioNumber = gpioNumber;
	this->buttonName = buttonName;
}

//--------------------------------------------------------------
void Button::draw() {
	#ifndef TARGET_RASPBERRY_PI
		ofPushStyle();
		ofFill();
		if(!buttonPushed)
			ofSetColor(ofColor::red);
		else
			ofSetColor(ofColor::red,100);
		ofDrawCircle(fakeButton.getCenter(), 25);
		ofSetColor(ofColor::white);
		text.draw("Replay", fakeButton.getCenter().x, fakeButton.getCenter().y,0x00000005);
		ofPopStyle();
	#else
		string stateButton;
		gpioPin.getval_gpio(stateButton);
	
		if (stateButton != lastState) {
			if(stateButton == "1") {
				ofNotifyEvent(buttonPressed, buttonName, this);
			}
			else if(stateButton == "0") {
				ofNotifyEvent(buttonReleased, buttonName, this);
			}
			
			lastState = stateButton;
		}
	
		usleep(500);
	#endif
}
#ifndef TARGET_RASPBERRY_PI
//--------------------------------------------------------------
void Button::mousePressed(ofMouseEventArgs &e) {
	if (fakeButton.inside(e.x, e.y)) {
		buttonPushed = true;
		ofNotifyEvent(buttonPressed, buttonName, this);
	}
}

//--------------------------------------------------------------
void Button::mouseReleased(ofMouseEventArgs &e) {
	if (fakeButton.inside(e.x, e.y)) {
		buttonPushed = false;
		ofNotifyEvent(buttonReleased, buttonName, this);
	}
}
#endif
