using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GPSRender : MonoBehaviour {

	public Text latitude;
	public Text longitude;

	// Update is called once per frame
	void Update () {
		latitude.text = GPSManager.Instance.lat.ToString();
		longitude.text = GPSManager.Instance.lng.ToString();
	}
}
