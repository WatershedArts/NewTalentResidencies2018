using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Beacon : MonoBehaviour {

	private static float EarthRadius = 6371.0f;

	private float lat = 54.2541245f;
	private float lng = -2.41346f;
	private float radius = 20.0f;

	private bool hasUserEntered = false;
	private bool isPlaying = false;

	private string audioFile = "";
	private string beaconId = "";

	//-----------------------------------------------------------------------------
	// Init Beacon
	// @param float lat: zone lat
	// @param float lng: zone lng
	// @param string audiopath: where the audio content is stored and which to load
	//-----------------------------------------------------------------------------
	Beacon(string beaconId, float lat, float lng, float radius, string audioFile) {
		this.audioFile = audioFile;
		this.lat = lat;
		this.lng = lng;
		this.radius = radius;
		this.beaconId = beaconId;
	}

	//-----------------------------------------------------------------------------
	// Helper Function for Converting Degrees to Radians
	//
	//-----------------------------------------------------------------------------
	float toRadians(float degrees) {
		return degrees * Mathf.PI / 180.0f;
	}

	//-----------------------------------------------------------------------------
	// Helper Function for Converting radians to Degrees
	//
	//-----------------------------------------------------------------------------
	float toDegrees(float radians) {
		return radians * 180.0f / Mathf.PI;
	}

	//-----------------------------------------------------------------------------
	// Beacon Starting Up
	// 
	//-----------------------------------------------------------------------------
	void Start() {
		print("Hello from the Beacon " + beaconId);
	}

	//-----------------------------------------------------------------------------
	// Update
	// 
	//-----------------------------------------------------------------------------
	void Update() {

		// Get the GPS from the Manager
		float uLat = GPSManager.Instance.lat;
		float uLng = GPSManager.Instance.lng;

		// Compute the Distance
		float result = computeDistanceFromUserToSelf(uLat,uLng);

		// Tell me how far you are away from the center
		print(beaconId + " is " + result.ToString() + " metres away from the user");
	}

	//-----------------------------------------------------------------------------
	// Compute the Azimuth Distance between the beacon and the user
	// @param: userLat: users latitude
	// @param: userLng: users longitude
	// @return c: the distance in meters
	//
	//-----------------------------------------------------------------------------
	float computeDistanceFromUserToSelf(float userLat, float userLng) {
		float deltaP = (toRadians(lat) - toRadians(userLat));
		float deltaL = (toRadians(lng) - toRadians(userLng));

		float a = Mathf.Sin(deltaP/2f) * 
				  Mathf.Sin(deltaP/2f) + 
				  Mathf.Cos(toRadians(userLat)) * 
				  Mathf.Cos(toRadians(lat)) * 
				  Mathf.Sin(deltaL/2f) * 
				  Mathf.Sin(deltaL/2f);
		
		float b = 2.0f * Mathf.Atan2(Mathf.Sqrt(a), Mathf.Sqrt(1.0f-a));
		float c = EarthRadius * b;

		return c;
	}
}