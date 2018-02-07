using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TouchPoint : MonoBehaviour {

	public Color defaultColor;
	public Color selectedColor;

	private Material mat;

	// Use this for initialization
	void Start () {
		mat = GetComponent<Renderer>().material;
	}

	void OnTouchDown() {
		mat.color = selectedColor;
	}

	void OnTouchUp() {
		mat.color = defaultColor;
	}

	void OnTouchStay() {
		mat.color = selectedColor;
	}

	void OnTouchExit() {
		mat.color = defaultColor;
	}
}
