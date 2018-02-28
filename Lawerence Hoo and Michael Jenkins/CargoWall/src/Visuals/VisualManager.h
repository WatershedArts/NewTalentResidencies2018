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
#include "AnimatedPoems.h"
#include "Photos.h"
#include "ofxImGui.h"

enum class VisualMode {
	VIDEOS,
	POEMS,
	PHOTOS,
	ANIMATED_POEMS
};

class VisualManager {
	public:
	
		void setup();
		void update(float x, float y);
		void draw();
	
//		VisualMode currentMode;
	
		Videos videoHandler;
		Poems poemHandler;
		Photos photoHandler;
		AnimatedPoems animatedPoemHandler;
	
		ofParameter<int> currentMode { "Visual Mode", static_cast<int>(VisualMode::VIDEOS) };
	
		ofParameter<bool> previousPoem {"Previous Poem",false};
		ofParameter<bool> nextPoem {"Next Poem",false};
};

#endif /* VisualManager_h */
