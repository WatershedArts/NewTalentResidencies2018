//
//  Button.h
//  InstagramTv
//
//  Created by David Haylock on 05/03/2018.
//

#ifndef Button_h
#define Button_h

#include <stdio.h>
#include "ofMain.h"

#ifndef TARGET_RASPBERRY_PI
	#include "ofxTextAlignTTF.h"
#else
	#include "ofxGPIO.h"
#endif

class Button {
	public:
		Button();
		~Button();
		void setupButton(string gpioNumber, string buttonName = "Replay");
		void draw();
	
	#ifndef TARGET_RASPBERRY_PI
		void mousePressed(ofMouseEventArgs &e);
		void mouseReleased(ofMouseEventArgs &e);
	#endif
	
	
		ofEvent<string> buttonPressed;
		ofEvent<string> buttonReleased;
	private:
	#ifndef TARGET_RASPBERRY_PI
		ofRectangle fakeButton;
		ofxTextAlignTTF text;
		bool buttonPushed;
	#else
		GPIO gpioPin;
		string lastState;
	#endif
		string gpioNumber;
		string buttonName;
};


#endif /* Button_h */
