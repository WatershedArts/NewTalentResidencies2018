using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class GPSRender : MonoBehaviour {

    public TextMeshProUGUI latitude;
    public TextMeshProUGUI longitude;

	// Update is called once per frame
	void Update () {
        latitude.SetText( GPSManager.Instance.lat.ToString() );
        longitude.SetText( GPSManager.Instance.lng.ToString() );
	}
}
