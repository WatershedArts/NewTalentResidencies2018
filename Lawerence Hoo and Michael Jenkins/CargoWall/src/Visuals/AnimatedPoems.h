//
//  AnimatedPoems.h
//  CargoWall
//
//  Created by David Haylock on 28/02/2018.
//

#ifndef AnimatedPoems_h
#define AnimatedPoems_h

#include "Poems.h"

class AnimatedPoems {
	public:
		void getPoems(vector<Poem> poems,ofxTextAlignTTF *font) {
			flags |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
			flags |= ofxTextAlign::VERTICAL_ALIGN_MIDDLE;
			
			this->poems = poems;
			this->font = font;
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
				currentLineFromPoem++;
			}
		}
	
		void previousLine() {
			if(currentLineFromPoem != 0) {
				currentLineFromPoem--;
			}
		}
	
		void draw() {
			if(!poems.empty()) {
				font->draw(poems[currentPoem].lines[currentLineFromPoem],ofGetWidth()/2, ofGetHeight()/2,flags);
			}
		}
	
	private:
	
		vector<Poem> poems;
		int currentPoem = 0;
		int currentLineFromPoem = 0;
		ofxTextAlignTTF *font;
		unsigned int flags = 0;
};

#endif /* AnimatedPoems_h */
