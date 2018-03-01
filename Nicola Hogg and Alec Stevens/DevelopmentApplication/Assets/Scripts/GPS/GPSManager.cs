using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GPSManager : MonoBehaviour {

	public static GPSManager Instance { set; get; }
	public float lat;
	public float lng;
    IEnumerator coroutine;

    private void Start() {
        Instance = this;
        DontDestroyOnLoad(gameObject);
        StartCoroutine(StartLocationService());
    }	

	private IEnumerator StartLocationService() {

		coroutine = updateGPS();
		if(!Input.location.isEnabledByUser){ 
			yield break;
		}

		Input.location.Start(10.0f,5.0f);

		int maxWait = 20;

		while(Input.location.status == LocationServiceStatus.Initializing && maxWait > 0) {
			yield return new WaitForSeconds(1);
			maxWait--;
		}

		if(maxWait < 1) {
			print("Timed Out");
			yield break;
		}

		if(Input.location.status == LocationServiceStatus.Failed) {
			print("Unable to Determine Device Location");
			yield break;
		} else { 
			print("Location: " + Input.location.lastData.latitude + " " + Input.location.lastData.longitude + " " + Input.location.lastData.altitude + " " + Input.location.lastData.horizontalAccuracy + " " + Input.location.lastData.timestamp);
			lat = Input.location.lastData.latitude;
			lng = Input.location.lastData.longitude;
			StartCoroutine(coroutine);
		}
	}

	IEnumerator updateGPS() {
		const float UPDATE_TIME = 0.5f; //Every 0.5 seconds
		WaitForSeconds updateTime = new WaitForSeconds(UPDATE_TIME);

		while (true) {
			print("Location: " + Input.location.lastData.latitude + " " + Input.location.lastData.longitude + " " + Input.location.lastData.altitude + " " + Input.location.lastData.horizontalAccuracy + " " + Input.location.lastData.timestamp);
			lat = Input.location.lastData.latitude;
			lng = Input.location.lastData.longitude;

			yield return updateTime;
		}
	}

	void stopGPS() {
		Input.location.Stop();
		StopCoroutine(coroutine);
	}

    void OnDisable() {
		stopGPS();
	}
}
