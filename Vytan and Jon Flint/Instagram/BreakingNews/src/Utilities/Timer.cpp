//--------------------------------------------------------------
//  Timer.cpp
//  Pinball
//  Created by David Haylock on 14/01/2018.
//! Pinball.
/** This is a Timer Object. */
//--------------------------------------------------------------

#include "Timer.h"

//--------------------------------------------------------------
Timer::Timer() {
	bTimerReached = true;
	bInvalidate = false;
}

//--------------------------------------------------------------
Timer::~Timer() {
	
}

//--------------------------------------------------------------
void Timer::setup(float timerLength,string timerName,bool bLoop)
{
	bTimerReached = true;
	bInvalidate = false;
	this->bLoop = bLoop;
	this->timerLength = timerLength;
	this->timerName = timerName;
	cout << this->timerName << endl;
	ofAddListener(ofEvents().update, this, &Timer::update);
}

//--------------------------------------------------------------
void Timer::update(ofEventArgs &event)
{
	float timer = ofGetElapsedTimeMillis() - startTime;
	
	if (!bTimerReached) {
		timeLeft = timerLength - timer;
	}
	
	if (timer >= timerLength && !bTimerReached) {
		bTimerReached = true;
		if (!bInvalidate) {
			string evt = timerName + " Finished";
			ofNotifyEvent(timerFinished, evt, this);
		}
		
		if (bLoop) {
			start();
		}
	}
}

//--------------------------------------------------------------
float Timer::getTimeLeft()
{
	return timeLeft;
}

//--------------------------------------------------------------
bool Timer::hasTimerFinished()
{
	return bTimerReached;
}

//--------------------------------------------------------------
void Timer::setNewTimerLength(int timerLength)
{
	this->timerLength = timerLength;
}

//--------------------------------------------------------------
void Timer::start()
{
	bInvalidate = false;
	if (bTimerReached) {
		string evt = timerName + " Started";
		ofNotifyEvent(timerStarted, evt, this);
		bTimerReached = false;
		startTime = ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void Timer::stop()
{
	bTimerReached = true;
	bLoop = false;
}

//--------------------------------------------------------------
void Timer::invalidate()
{
	bInvalidate = true;
	string evt = timerName + " Invalidated";
	ofNotifyEvent(timerInvalidated, evt, this);
}

