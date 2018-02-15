//--------------------------------------------------------------
//  Timer.h
//  Pinball
//  Created by David Haylock on 14/01/2018.
//! Pinball.
/** This is a Timer Object. */
//--------------------------------------------------------------

#pragma once

#ifndef Timer_h
#define Timer_h


#include <stdio.h>
#include "ofMain.h"

class Timer
{
public:
	
	Timer();
	~Timer();
	//-----------------------------------------------------
	/** \brief Setup the Timer
	 * @param timerLength : timer length ms.
	 * @param timerName : timer name.
	 * @param loop : loop the timer.
	 *
	 * This method generates a timer object
	 * timer name will be referenced on events.
	 */
	//-----------------------------------------------------
	void setup(float timerLength,string timerName,bool bLoop);
	
	//-----------------------------------------------------
	/** \brief Update the Timer
	 * @param event : app event.
	 *
	 * This method updates the timer, should never be
	 * called as it should be handled internally.
	 */
	//-----------------------------------------------------
	void update(ofEventArgs &event);
	
	//-----------------------------------------------------
	/** \brief Start the Timer */
	//-----------------------------------------------------
	void start();
	
	//-----------------------------------------------------
	/** \brief Stop the Timer */
	//-----------------------------------------------------
	void stop();
	
	//-----------------------------------------------------
	/** \brief Reset the Timer */
	//-----------------------------------------------------
	void reset();
	
	//-----------------------------------------------------
	/** \brief Set New Timer Length
	 * @param timerLength : new timer length.
	 *
	 * This method updates the timer length.
	 */
	//-----------------------------------------------------
	void setNewTimerLength(int timerLength);
	
	//-----------------------------------------------------
	/** \brief Get time left
	 * @return timerLength : time left
	 *
	 * This method returns the time left on the timer.
	 */
	//-----------------------------------------------------
	float getTimeLeft();
	
	//-----------------------------------------------------
	/** \brief Has the Timer Finished
	 * @return bTimerReached : timer finished
	 *
	 * This method returns whether the timer has
	 * finished.
	 */
	//-----------------------------------------------------
	bool hasTimerFinished();
	
	//-----------------------------------------------------
	/** \brief Invalidates the Timer */
	//-----------------------------------------------------
	void invalidate();
	
	// Events
	ofEvent<string> timerStarted;
	ofEvent<string> timerFinished;
	ofEvent<string> timerInvalidated;
	
private:
	float startTime;
	float endTime;
	float timeLeft;
	
	bool bTimerReached;
	bool bLoop;
	bool bInvalidate;
	
	float timerLength;
	string timerName;
};
#endif /* Timer_h */

