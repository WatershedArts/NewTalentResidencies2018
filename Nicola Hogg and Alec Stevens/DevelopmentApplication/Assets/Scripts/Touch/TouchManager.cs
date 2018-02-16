using System.Collections;
using System.Collections.Generic;
using UnityEngine;
	
public class TouchManager : MonoBehaviour {

	private static float maxDistance = 100.0f;
	public static TouchManager Instance { set; get; }

	public Transform[] fingers;

	// Update is called once per frame
	void Update () {

		#if UNITY_EDITOR 
			if(Input.GetMouseButton(0) || Input.GetMouseButtonDown(0) || Input.GetMouseButtonUp(0)) {
				Vector2 mouse = Input.mousePosition;
				fingers[0].transform.position = mouse;
			}
		#endif

//		if(Input.touchCount > 0 && Input.touchCount < 5 ) {
//			for(int i = 0; i < Input.touchCount; i++ ) {
//				fingers[i].transform.position = new Vector3(Input.touches[i].position.x,Input.touches[i].position.y,0.0f);
//			}
//		}

//		if( Input.touchCount == 3 ) {
			calculateDistanceBetweenTouches(fingers);
//		}
	}

	float calculateDistanceBetweenTouches(Transform[] touches) {

		Vector2 i = touches[0].position;
		Vector2 k = touches[1].position;
		Vector2 j = touches[2].position;

		float dist1 = Vector2.Distance(i,k);
		float dist2 = Vector2.Distance(i,j);
		float dist3 = Vector2.Distance(k,j);


//		print("Distance between 0 and 1 " + dist1.ToString());
//		print("Distance between 0 and 2 " + dist2.ToString());
//		print("Distance between 1 and 2 " + dist3.ToString());

		return 0.0f;
	}

	float calculateRotation(List<Touch> touches) {

		Vector2 top;

		return 0.0f;
	}



}
