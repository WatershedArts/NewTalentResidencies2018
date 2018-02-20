using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class BeaconManager : MonoBehaviour {

	public Beacon prefab;
	public static BeaconManager Instance { set; get; }

	public List<Beacon> beacons = new List<Beacon>();
	public BeaconCollection beaconsData;

	//---------------------------------------------------------------
	// Load the Beacons from the Json File
	//---------------------------------------------------------------
	void LoadBeacons(string path) {
		path = Application.streamingAssetsPath + "/" + path;

		string jsonstring = File.ReadAllText(path);
		beaconsData = JsonUtility.FromJson<BeaconCollection> (jsonstring);

		try {
			if (beaconsData.beacons.Length != 0) {
				print("Loaded Beacon File: found  " + beaconsData.beacons.Length);		

				foreach(BeaconData d in beaconsData.beacons) {
					Beacon bd = Instantiate(prefab,transform);
					bd.transform.parent = transform;
					bd.SetBeaconData(d);
					beacons.Add(bd);
				}
			}
			else {
				
			}
		} 
		catch (System.Exception ex) {
			Debug.LogException(ex,this);
		}
	}

	//----------------------------------------------------
	// Use this for initialization
	//----------------------------------------------------
	void Start () {
		print("Beacon Manager Started");
		Instance = this; 
		LoadBeacons("beacons.json");
	}

	//----------------------------------------------------
	// Update
	//----------------------------------------------------
	void Update() {
		// Get the GPS Coords
		double dlt = GPSManager.Instance.lat;
		double dlg = GPSManager.Instance.lng;

		// Loop through the Beacons and tell them where we are.
		foreach(Beacon b in beacons) {
			b.setCurrentGPSCoords(dlt,dlg);
		}
	}

	//----------------------------------------------------
	// Has the User Entered the Beacon
	//----------------------------------------------------
	void OnEnteredBeacon(string t) {
		print("Heya " + t);
		FindObjectOfType<AudioManager>().Play(t);
	}

	//----------------------------------------------------
	// Has the User Exited the Beacon
	//----------------------------------------------------
	void OnExitBeacon(string t) {
		print("See Ya " + t);
		FindObjectOfType<AudioManager>().Stop(t);
	}
}
