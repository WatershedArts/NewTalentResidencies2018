using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

//----------------------------------------------------
// Touch Dial Object
//----------------------------------------------------
[System.Serializable]
public class TouchDial {

	public bool exists;
	public float orientation;
	public int apex;

	public Vector2 center;

	public float width;
	public float height;
	public float apexAngle;
	public float objectAngle;
}

//----------------------------------------------------
// Touch Manager Class
//----------------------------------------------------
public class TouchManager : MonoBehaviour {

    public TextMeshProUGUI angle;
	public static TouchManager Instance { set; get; }
	public Slider tunerSlider;
	public Transform[] fingers;
	public TouchDial dial;
    bool isPlaying = false;
    //----------------------------------------------------
    // Update Loop
    //----------------------------------------------------
    void Update()
    {
#if UNITY_EDITOR
        if (Input.GetMouseButton(0) || Input.GetMouseButtonDown(0) || Input.GetMouseButtonUp(0))
        {
            Vector2 mouse = Input.mousePosition;
            fingers[0].transform.position = mouse;
        }

        float d = dial.orientation.Map(0f, 360f, 88.0f, 108.0f);
        angle.SetText(" Orig Rot: {0} Mapped Rot: {1:1} ", dial.orientation, d);
        tunerSlider.value = d;

        if ((d > 101.5f && d < 102.5) && !isPlaying)
        {
            FindObjectOfType<AudioManager>().Play("Beacon1");
            isPlaying = true;
        }
        else if ((d < 101.5f && d > 102.5) && isPlaying)
        {
            FindObjectOfType<AudioManager>().Stop("Beacon1");
            isPlaying = false;
        }

#endif


        if (Input.touchCount == 3)
        {
            calculateDial(Input.touches);
            for (int i = 0; i < Input.touchCount; i++)
            {
                fingers[i].transform.position = new Vector3(Input.touches[i].position.x, Input.touches[i].position.y, 0.0f);
            }
        }
    }

	//----------------------------------------------------
	// Calculate the Dial from the Touches
	//----------------------------------------------------
	void calculateDial(Touch[] touches) 
    {
		int topIndex = calculateTop(touches);

		dial.exists = true;		
		dial.width = calculateWidth(touches,topIndex);
		dial.height = calculateHeight(touches,topIndex);
		dial.orientation = calculateOrientation(touches,topIndex);
		dial.apex = topIndex;

        float d = dial.orientation.Map(0f, 360f, 88.0f, 108.0f);
        angle.SetText("Orig Rot: {0} Mapped Rot: {1.0}", dial.orientation,d);
        tunerSlider.value = d;


        if ((d > 101.5f && d < 102.5) && !isPlaying) {
            FindObjectOfType<AudioManager>().Play("Beacon1");
            isPlaying = true;
        }
        else if((d < 101.5f && d > 102.5) && isPlaying) {
            FindObjectOfType<AudioManager>().Stop("Beacon1");
            isPlaying = false;
        }
	}

	//----------------------------------------------------
	// Major thanks to Paul Ferragut 
	// https://github.com/paul-ferragut/oscTouchObject/blob/master/src/touchObject.cpp
	//----------------------------------------------------

	//----------------------------------------------------
	// Calculate the Top Touch
	//----------------------------------------------------
	int calculateTop(Touch[] touches) 
    {
		Vector2 i = touches[0].position;
		Vector2 k = touches[1].position;
		Vector2 j = touches[2].position;

		float dist0to1 = Vector2.Distance(i,k);
		float dist0to2 = Vector2.Distance(i,j);
		float dist1to2 = Vector2.Distance(k,j);

		float diff0to1m0to2 = Mathf.Abs(dist0to1 - dist0to2);
		float diff0to2m1to2 = Mathf.Abs(dist0to2 - dist1to2);
		float diff0to1m1to2 = Mathf.Abs(dist0to1 - dist1to2);

		if (diff0to1m0to2 < diff0to2m1to2 && diff0to1m0to2 < diff0to1m1to2) 
        {
			return 0;
		}
		else if (diff0to1m1to2 < diff0to1m0to2 && diff0to1m1to2 < diff0to2m1to2) 
        {
			return 1;
		}
		else if (diff0to2m1to2 < diff0to1m0to2 && diff0to2m1to2 < diff0to1m1to2) 
        {
			return 2;
		}

		return 0;
	}

	//----------------------------------------------------
	// Calculate the Orientation of the Dial
	//----------------------------------------------------
	float calculateOrientation(Touch[] touches,int topIndex) 
    {
		Vector2 a = new Vector2();
		Vector2 b = new Vector2();
		Vector2 c = new Vector2();

		b = touches[topIndex].position;

		if(topIndex == 0) 
        {
			a = touches[1].position;
			c = touches[2].position;
		}
		else if(topIndex == 1) 
        {
			a = touches[0].position;
			c = touches[2].position;
		}
		else if(topIndex == 2) 
        {
			a = touches[0].position;
			c = touches[1].position;
		}

		Vector2 middlePoint = new Vector2(Mathf.Lerp(a.x,c.x,0.5f),Mathf.Lerp(a.y,c.y,0.5f));

		Vector2 diff = b - middlePoint;
		diff.Normalize();

        float radians = Mathf.Atan2(diff.x, diff.y);

		if (radians < 0.0) 
        {
			radians += 2.0f * Mathf.PI;
		}

		float degrees = radians.toDegrees();

		return degrees;
	}

	//----------------------------------------------------
	// Calculate the Apex Rotation
	//----------------------------------------------------
	float calculateApexAngle(Touch[] touches, int top) 
    {
		Vector2 a = new Vector2();
		Vector2 b = new Vector2();
		Vector2 c = new Vector2();

		b = touches[top].position;
		if (top == 0) 
        {
			a = touches[1].position;
			c = touches[2].position;
		}
		else if (top == 1) 
        {
			a = touches[0].position;
			c = touches[2].position;
		}
		else if (top == 2) 
        {
			a = touches[0].position;
			c = touches[1].position;
		}

		Vector2 ab = new Vector2((b.x - a.x),(b.y - a.y));
		Vector2 cb = new Vector2((b.x - c.x),(b.y - c.y));


		float dotProduct = Vector2.Dot(ab,cb);
		float crossProduct = (ab.x * cb.y - ab.y * cb.x);

		float alpha = Mathf.Atan2(crossProduct,dotProduct);
		float apexAngle = Mathf.Floor(alpha * 180.0f / Mathf.PI + 0.5f);
		return apexAngle;
	}

	//----------------------------------------------------
	// Calculate the Width of the Dial From the touches
	//----------------------------------------------------
	float calculateWidth(Touch[] touches, int top) 
    {
		Vector2 a = new Vector2();
		Vector2 b = new Vector2();
		Vector2 c = new Vector2();

		b = touches[top].position;
		if (top == 0) 
        {
			a = touches[1].position;
			c = touches[2].position;
		}
		else if (top == 1) 
        {
			a = touches[0].position;
			c = touches[2].position;
		}
		else if (top == 2) 
        {
			a = touches[0].position;
			c = touches[1].position;
		}

		return Vector2.Distance(a,c);
	}

	//----------------------------------------------------
	// Calculate the Height of the Dial From the touches
	//----------------------------------------------------
	float calculateHeight(Touch[] touches, int top) 
    {
		Vector2 a = new Vector2();
		Vector2 b = new Vector2();
		Vector2 c = new Vector2();

		b = touches[top].position;
		if (top == 0) 
        {
			a = touches[1].position;
			c = touches[2].position;
		}
		else if (top == 1) 
        {
			a = touches[0].position;
			c = touches[2].position;
		}
		else if (top == 2) 
        {
			a = touches[0].position;
			c = touches[1].position;
		}

		Vector2 middlePoint = new Vector2(Mathf.Lerp(a.x,c.x,0.5f),Mathf.Lerp(a.y,c.y,0.5f));

		return Vector2.Distance(middlePoint,b);
	}
}
