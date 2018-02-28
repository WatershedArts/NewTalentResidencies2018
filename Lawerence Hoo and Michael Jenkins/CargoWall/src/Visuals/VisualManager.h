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
#include "Poems.h"
#include "Photos.h"
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
		Poems poemHandler;
		Photos photoHandler;
	
		ofParameter<int> currentMode { "Visual Mode", static_cast<int>(VisualMode::VIDEOS) };
	
		ofParameter<bool> previousPoem {"Previous Poem",false};
		ofParameter<bool> nextPoem {"Next Poem",false};
};

#endif /* VisualManager_h */
