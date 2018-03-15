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
#include "Textures.h"
#include "ofxImGui.h"
#include "ParticleHandler.h"

enum class VisualMode {
	VIDEOS,
	POEMS,
	PHOTOS,
	ANIMATED_POEMS,
	TEXTURES
};

class VisualManager {
	public:
	
		void setup();
		void update(float x, float y);
		void draw();
	
		// Reloads the Contents folder
		void reloadContent();
	
		Videos videoHandler;
		Poems poemHandler;
		Photos photoHandler;
		AnimatedPoems animatedPoemHandler;
		Textures textureHandler;
		ParticleHandler particleHandler;
	
		ofParameter<int> currentMode { "Visual Mode", static_cast<int>(VisualMode::VIDEOS) };
		ofParameter<bool> previousPoem {"Previous",false};
		ofParameter<bool> nextPoem {"Next",false};
};

#endif /* VisualManager_h */
