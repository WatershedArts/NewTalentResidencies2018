//
//  ParticleHandler.h
//  CargoWall
//
//  Created by David Haylock on 08/03/2018.
//

#ifndef ParticleHandler_h
#define ParticleHandler_h

#include "ofMain.h"
#include "ofxTextAlignTTF.h"
#include "LetterParticle.h"

class ParticleHandler : public ofThread  {
	
public:
	
	
	
	void getPoems(vector<Poem> poems,ofxTextAlignTTF *font) {
		flags |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
		flags |= ofxTextAlign::VERTICAL_ALIGN_MIDDLE;
		
		this->poems.clear();
		
		this->poems = poems;
		this->font = font;
		
		if(!isThreadRunning()) {
			startThread();
		}
	}
	
	void exit() {
		stopThread();
	}
	
	void threadedFunction() {
		while( isThreadRunning() ) {
			if(lock()) {
				for (int i = 0; i < particles.size(); i++) {
					if ( particles[i].isParticleDead() ) {
						particles.erase(particles.begin() + i);
					}
					
					particles[i].update();
				}
				sleep(10);
				unlock();
			}
		}
	}
	
	void nextPoem() {
		currentLineFromPoem = 0;
		if(currentPoem != poems.size()-1) {
			currentPoem++;
		}
	}
	
	void previousPoem() {
		currentLineFromPoem = 0;
		if(currentPoem != 0) {
			currentPoem--;
		}
	}
	
	void isOverCenter() {
		nextLine();
	}
	
	void nextLine() {
		if(currentLineFromPoem != poems[currentPoem].lines.size()-1) {
			vector <string> words = ofSplitString(poems[currentPoem].lines[currentLineFromPoem]," ");
			for (int i = 0; i < words.size(); i++) {
				particles.push_back(LetterParticle(words[i],font,ofVec2f(100+(i*60),10)));
			}
			
			currentLineFromPoem++;
		}
	}
	
	void previousLine() {
		if(currentLineFromPoem != 0) {
			currentLineFromPoem--;
		}
	}
	
	void draw() {
		for (int i = 0; i < particles.size(); i++) {
			particles[i].draw();
		}
	}
	
private:
	
	
	vector <LetterParticle> particles;
	vector<Poem> poems;
	int currentPoem = 0;
	int currentLineFromPoem = 0;
	ofxTextAlignTTF *font;
	unsigned int flags = 0;
};

#endif /* ParticleHandler_h */
