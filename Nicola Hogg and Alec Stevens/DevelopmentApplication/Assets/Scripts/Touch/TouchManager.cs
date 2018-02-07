using System.Collections;
using System.Collections.Generic;
using UnityEngine;


//struct TrianglePoint {
//	Vector2	position;
//	int[] groupBelonging;
//	bool paired;
//};
//
//struct TriangleTracker {
//	Vector2[] position;
//	int apexPointIndex;
//
//	float orientation;
//
//	float rangeLow;
//	float rangeHigh;
//
//	ofPoint
//
//};

public class TouchManager : MonoBehaviour {

	public static TouchManager Instance { set; get; }

	public List <Transform> fingers;

	// Update is called once per frame
	void Update () {

		#if UNITY_EDITOR 
			if(Input.GetMouseButton(0) || Input.GetMouseButtonDown(0) || Input.GetMouseButtonUp(0)) {
				Vector2 mouse = Input.mousePosition;
				fingers[0].transform.position = mouse;
			}
		#endif

		if(Input.touchCount > 0 && Input.touchCount < 5 ) {
			for(int i = 0; i < Input.touchCount; i++ ) {
				fingers[i].transform.position = new Vector3(Input.touches[i].position.x,Input.touches[i].position.y,0.0f);
			}
		}
	}

	float calculateRotation(List<Touch> touches) {

		Vector2 top;

		return 0.0f;
	}



}
