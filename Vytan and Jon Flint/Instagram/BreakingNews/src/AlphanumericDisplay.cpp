//
//  AlphanumericDisplay.cpp
//  InstagramTv
//
//  Created by David Haylock on 05/03/2018.
//

#include "AlphanumericDisplay.h"
AlphanumericDisplay::AlphanumericDisplay() {}
AlphanumericDisplay::~AlphanumericDisplay() {}

//--------------------------------------------------------------
void AlphanumericDisplay::setupDisplay(uint16_t address) {
	
#ifndef TARGET_RASPBERRY_PI
	text.load(OF_TTF_SANS, 60);
#else
	// Open the Bus
	bus = new I2c("/dev/i2c-1");
	
	// With the Current address
	bus->addressSet(address);
	
	// Check the Python Setup Proceedure
	bus->write(HT16K33_SYSTEM_SETUP | HT16K33_OSCILLATOR);
	
	// Set the Blinking Status
	bus->write(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | HT16K33_BLINK_OFF);
	
	// Set the Brightness and the Blink Rate
	bus->write(0xE0 | 0x15);
	
	// Get Ready for Sending data
	bus->write(0x00);
#endif
	
	displayBuffer[0] = characterList.at(" ");
	displayBuffer[1] = characterList.at(" ");
	displayBuffer[2] = characterList.at(" ");
	displayBuffer[3] = characterList.at(" ");
}

//--------------------------------------------------------------
void AlphanumericDisplay::setDisplay(string val) {
	
	if(val.size() > 4) {
		cout << "Woah there too many characters here" << endl;
		cout << val << endl;
		return;
	}
	copyofstring = val;
	for(int i = 0; i < 4; i++) {
		if(characterList.count(ofToString(val[i])) > 0) {
			displayBuffer[i] = characterList.at(ofToString(val[i]));
		}
		else {
			displayBuffer[i] = characterList.at(" ");
		}
	}
	
	writeDisplay();
}

//--------------------------------------------------------------
void AlphanumericDisplay::clear() {
	displayBuffer[0] = characterList.at(" ");
	displayBuffer[1] = characterList.at(" ");
	displayBuffer[2] = characterList.at(" ");
	displayBuffer[3] = characterList.at(" ");
	writeDisplay();
}

//--------------------------------------------------------------
void AlphanumericDisplay::exit() {
	
}

//--------------------------------------------------------------
void AlphanumericDisplay::writeDisplay() {
#ifndef TARGET_RASPBERRY_PI
	unsigned int flags = 0;
	flags |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
	flags |= ofxTextAlign::VERTICAL_ALIGN_MIDDLE;
	ofSetColor(255, 255, 255);
	text.draw(copyofstring, ofGetWidth()/2, ofGetHeight()-40, flags);
	
#else
	bus->writeBlockData(0x00,BUFFER_LENGTH,(__u8*)displayBuffer);
	usleep(500);
#endif
}
