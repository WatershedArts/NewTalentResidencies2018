//
//  LetterParticle.h
//  CargoWall
//
//  Created by David Haylock on 08/03/2018.
//

#ifndef LetterParticle_h
#define LetterParticle_h

#include "ofMain.h"
#include "ofxTextAlignTTF.h"
class LetterParticle {
	public:
		LetterParticle() {}
		~LetterParticle() {}
	
		LetterParticle(string letter, ofxTextAlignTTF *text, ofVec2f pos) {
			isDead = false;
			this->pos = pos;
			this->letter = letter;
			this->text = text;
			acceleration = ofVec2f(0,0.05);
			velocity = ofVec2f(ofRandom(-0.5,0.5),ofRandom(-1.0,0));
		}

		void update() {
			velocity += acceleration;
			pos += velocity;
		}
	
		void draw() {
			
			ofSetColor(ofColor::white);
			ofDrawBitmapString(ofToString(letter), pos.x, pos.y);
//			text->drawString(ofToString(letter),pos.x,pos.y);
			
			if(pos.x < 0 || pos.x > ofGetWidth()) {
				isDead = true;
			}
			else if(pos.y < 0 || pos.y > ofGetHeight()) {
				isDead = true;
			}
		}
	
		bool isParticleDead() {
			return isDead;
		}
	
		bool isDead;
	
		string letter;
		ofVec2f pos;
		ofVec2f velocity;
		ofVec2f acceleration;
		ofxTextAlignTTF *text;
};

#endif /* LetterParticle_h */
