//
//  VisualManager.h
//  CargoWall
//
//  Created by David Haylock on 22/02/2018.
//

#ifndef VisualManager_h
#define VisualManager_h

#include <stdio.h>
#include "Videos.h"
#include "ofxImGui.h"

enum class VisualMode {
	VIDEOS,
	TEXT,
	POEM_ANIMATIONS,
	SHADERS
};

class VisualManager {
	public:
	
		void setup();
		void update();
		void draw();
	
//		VisualMode currentMode;
	
		Videos videoHandler;
	
		ofParameter<int> currentMode { "Visual Mode", static_cast<int>(VisualMode::VIDEOS) };
};

#endif /* VisualManager_h */
