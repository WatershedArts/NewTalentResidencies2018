//
//  Poems.h
//  CargoWall
//
//  Created by David Haylock on 28/02/2018.
//

#ifndef Poems_h
#define Poems_h

#include "ofMain.h"
#include "ofxTextAlignTTF.h"

struct Poem {
	Poem() {}
	Poem(string title, vector <string> lines) {
		this->title = title;
		this->lines = lines;
	}
	
	string title;
	vector <string> lines;
};

class Poems {
	public:

		void loadPoems(string dirPath) {
			_poems.clear();
			flags |= ofxTextAlign::HORIZONTAL_ALIGN_CENTER;
			flags |= ofxTextAlign::VERTICAL_ALIGN_MIDDLE;
			font = new ofxTextAlignTTF();
			font->load(OF_TTF_SANS,25);
			ofDirectory dir(dirPath);
			dir.sort();
			
			for(int i = 0; i < (int)dir.size(); i++){
				Poem p;
				p.title = dir.getName(i);
				ofBuffer buffer = ofBufferFromFile(dir.getPath(i));
				if(buffer.size()) {
					for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
						string line = *it;
					
						if(line.empty() == false) {
							p.lines.push_back(line);
						}else {
							p.lines.push_back("\n");
						}
					}
				}
				_poems.push_back(p);
			}
		}
	
		void nextPoem() {
			if(currentPoem != _poems.size()-1) {
				currentPoem++;
			}
		}
	
		void previousPoem() {
			if(currentPoem != 0) {
				currentPoem--;
			}
		}
	
		void draw() {
			
			if(!_poems.empty()) {
				int startingPoint = ofGetHeight()/2 - (_poems[currentPoem].lines.size() * 25) / 2 ;
				for(int i = 0; i < _poems[currentPoem].lines.size(); i++) {
					font->draw(_poems[currentPoem].lines[i],ofGetWidth()/2, startingPoint + ( i * 25 ),flags);
				}
			}
		}
	
	
		vector <Poem> getPoems() {
			return _poems;
		}
	
		ofxTextAlignTTF *font;
	
	protected:
	
		vector <Poem> _poems;
		int currentPoem = 0;
		unsigned int flags = 0;
};

#endif /* Poems_h */
