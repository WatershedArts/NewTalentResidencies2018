﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

//----------------------------------------------------
struct Position {
	
	public Position(double lat,double lng) {
		Latitude = lat;
		Longitude = lng;
	}

	public double Latitude;
	public double Longitude;
};

public static class ExtensionMethods {

	public static float Map (this float value, float from1, float to1, float from2, float to2) {
		return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
	}

	public static double toRadians(this double value) {
		return value * Mathf.PI / 180.0f;
	}

	public static double toDegrees(this double value) {
		return value * 180.0f / Mathf.PI;
	}

	public static float toRadians(this float value) {
		return value * Mathf.PI / 180.0f;
	}

	public static float toDegrees(this float value) {
		return value * 180.0f / Mathf.PI;
	}
}

//----------------------------------------------------
// Collection 
//----------------------------------------------------
[Serializable]
public class BeaconCollection {
	public BeaconData[] beacons;
};

//----------------------------------------------------
// Beacon Data
//----------------------------------------------------
[Serializable]
public class BeaconData {
	public string beaconid;
	public string audiofile;
	public double lat;
	public double lng;
	public double radius;
	public float tuning;
	public int genericid; 
    public bool isPlaying = false;
};

//----------------------------------------------------
public class Beacon : MonoBehaviour {

	public BeaconData data;

	// User calculation
	private double userlat;
	private double userlng;
	private double radiusKilometres;

	// Beacon Latches
	private bool hasUserEntered = false;
	public bool isPlaying = false;

	//---------------------------------------------------------------
	// Init Beacon
	//---------------------------------------------------------------
	public Beacon(BeaconData d) {
		this.data = d;
	}

	public void SetBeaconData(BeaconData d) {
		this.data = d;
	}

	//---------------------------------------------------------------
	// Get the Beacon ID
	//---------------------------------------------------------------
	public string getBeaconId() {
		return data.beaconid;
	}
				
	//---------------------------------------------------------------
	// Get the Current User GPS coords
	// 
	//---------------------------------------------------------------
	public void setCurrentGPSCoords(double userlat,double userlng) {
		
		// Compute the Distance
		double result = computeDistanceFromUserToSelf(new Position(data.lat,data.lng),new Position(userlat,userlng));

		string pr = string.Format("Beacon {0} is {1} from User", data.beaconid, result);
	
		// Convert to Kilometers
		radiusKilometres = (double)(data.radius / 1000.0f);

		// Check that the user has entered
		if(!hasUserEntered && (result < radiusKilometres)) {
			BeaconManager.Instance.gameObject.SendMessage("OnEnteredBeacon",data,SendMessageOptions.DontRequireReceiver);
			hasUserEntered = true;
		}
		else if (hasUserEntered && (result > radiusKilometres)) {
			BeaconManager.Instance.gameObject.SendMessage("OnExitBeacon",data,SendMessageOptions.DontRequireReceiver);
			hasUserEntered = false;
		}
	}
		
	//---------------------------------------------------------------
	// Compute Haversine Distance between the beacon and the user
	// @param: userLat: users latitude
	// @param: userLng: users longitude
	// @return c: the distance in meters
	//---------------------------------------------------------------
	double computeDistanceFromUserToSelf(Position pos1, Position pos2) {

		// Earth's Radius in Kilometres
		double R = 6371;  

		// Calculate the Theata
		double dLat = (pos2.Latitude - pos1.Latitude).toRadians();  
		double dLon = (pos2.Longitude - pos1.Longitude).toRadians();  

		// Do Conversion to Globe coordinates
		double a = Mathf.Sin((float)(dLat / 2)) * 
			Mathf.Sin((float)(dLat / 2)) +  
			Mathf.Cos((float)( pos1.Latitude.toRadians() )) * 
			Mathf.Cos((float)( pos2.Latitude.toRadians() )) *  
			Mathf.Sin((float)(dLon / 2)) * 
			Mathf.Sin((float)(dLon / 2));  

		double c = 2 * Mathf.Asin(Mathf.Min(1, Mathf.Sqrt((float)(a))));  

		double d = R * c;  
		return d; 
	}
}