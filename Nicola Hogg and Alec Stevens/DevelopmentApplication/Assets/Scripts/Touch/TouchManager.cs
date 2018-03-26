using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

using DigitalRuby.Tween;

//----------------------------------------------------
// Touch Dial Object
//----------------------------------------------------
[System.Serializable]
public class TouchDial {

    public float orientation;

    [HideInInspector]
	public bool exists;
	
    [HideInInspector]
    public int apex;

    [HideInInspector]
	public Vector2 center;

    [HideInInspector]
	public float width;

    [HideInInspector]
    public float height;
	
    [HideInInspector]
    public float apexAngle;

    [HideInInspector]
    public float objectAngle;
}

//----------------------------------------------------
// Touch Manager Class
//----------------------------------------------------
public class TouchManager : MonoBehaviour {

    public ParticleSystem ps;
    public TextMeshProUGUI angle;
	public static TouchManager Instance { set; get; }
	public Slider tunerSlider;
	public Transform[] fingers;
	public TouchDial dial;
    [Range(0f, 2f)]
    public float tuningThreshold = 0.2f;
    bool isPlaying = false;
    bool isTunerHidden = true;

    private void Start()
	{
        Debug.Log("Touch Manager: Started");
        FindObjectOfType<AudioManager>().Play("Static");


        fingers[0].gameObject.SetActive(false);
        fingers[1].gameObject.SetActive(false);
        fingers[2].gameObject.SetActive(false);
	}

	//----------------------------------------------------
	// Check if the user has tuned to any of the frequencies
	//----------------------------------------------------
	void checkTuningStatus(float d) {

        foreach (Beacon b in BeaconManager.Instance.beacons)
        {
            if ((d > (b.data.tuning - tuningThreshold) && d < (b.data.tuning + tuningThreshold) )) {
                // We are in range but are we playing
                if (!b.data.isPlaying)
                {
                    FindObjectOfType<AudioManager>().Stop("Static");
                    FindObjectOfType<AudioManager>().Play(b.data.beaconid);
                    Debug.Log("Tuned to Beacon: " + b.data.beaconid);
                    b.data.isPlaying = true;
                }
            }
            else if (b.data.isPlaying)
            {
                // Out of Range but still playing
                FindObjectOfType<AudioManager>().Play("Static");
                FindObjectOfType<AudioManager>().Stop(b.data.beaconid);
                Debug.Log("Detuned from Beacon: " + b.data.beaconid);
                b.data.isPlaying = false;
            }
        }
    }

    //----------------------------------------------------
    // Update Loop
    //----------------------------------------------------
    void Update()
    {
#if UNITY_EDITOR
        if (Input.GetMouseButtonDown(0) || Input.GetMouseButton(0))
        {
            if (ps.isPlaying)
            {
                ps.Stop();
            }

            if (!fingers[0].gameObject.activeSelf)
            {
                fingers[0].gameObject.SetActive(true);
                fingers[1].gameObject.SetActive(true);
                fingers[2].gameObject.SetActive(true);
            }
            Vector2 mouse = Input.mousePosition;
            fingers[0].transform.position = mouse;
        }
        else if (Input.GetMouseButtonUp(0) && fingers[0].gameObject.activeSelf)
        {
            if (!ps.isPlaying)
            {
                ps.Play();
            }

            fingers[0].gameObject.SetActive(false);
            fingers[1].gameObject.SetActive(false);
            fingers[2].gameObject.SetActive(false);
        }

        float d = dial.orientation.Map(0f, 360f, 88.0f, 108.0f);
        angle.SetText(" Orig Rot: {0} Mapped Rot: {1:1} ", dial.orientation, d);
        tunerSlider.value = d;

        checkTuningStatus(d);
#else 
        if (Input.touchCount == 3)
        {

            calculateDial(Input.touches);
            
            for (int i = 0; i < Input.touchCount; i++)
            {
                fingers[i].transform.position = new Vector3(Input.touches[i].position.x, Input.touches[i].position.y, 0.0f);
            }

            if(ps.isPlaying) 
            {
                ps.Stop();
            }

            if (fingers.Length != 0)
            {
                if (!fingers[0].gameObject.activeSelf)
                {
                    fingers[0].gameObject.SetActive(true);
                    fingers[1].gameObject.SetActive(true);
                    fingers[2].gameObject.SetActive(true);
                }
            }
        }
        else if (Input.touchCount < 3) {

            if(!ps.isPlaying) {
                ps.Play();
            }

            if (fingers.Length != 0)
            {
                if (fingers[0].gameObject.activeSelf)
                {
                    fingers[0].gameObject.SetActive(false);
                    fingers[1].gameObject.SetActive(false);
                    fingers[2].gameObject.SetActive(false);
                }
            }
        }
#endif


    }

    //----------------------------------------------------
    // Hide the Tuner
    //----------------------------------------------------
    public void hideTuner(){
        if (!isTunerHidden) 
        {
            RectTransform r = tunerSlider.GetComponent<RectTransform>();
            float currentPos = -150f;//r.transform.position.y;
            float endPos = 80f;//r.transform.position.y + 78f;

            TweenFactory.Tween("Hide Tuner", currentPos, endPos, 1.0f, TweenScaleFunctions.CubicEaseInOut, (t) =>
            {
                Vector3 e = new Vector3(r.anchoredPosition.x, t.CurrentValue);
                tunerSlider.GetComponent<RectTransform>().anchoredPosition = e;
            }, (t) =>
            {
                FindObjectOfType<AudioManager>().Stop("Static");
                Debug.Log("Done Hide");
                isTunerHidden = true;
            });
        }
    }

    //----------------------------------------------------
    // Show the Tuner
    //----------------------------------------------------
    public void showTuner()
    {
        if (isTunerHidden) 
        {
            RectTransform r = tunerSlider.GetComponent<RectTransform>();
            float currentPos = 80f;//r.transform.position.y;
            float endPos = -150f;//r.transform.position.y - 78f;

            TweenFactory.Tween("Show Tuner", currentPos, endPos, 1.0f, TweenScaleFunctions.CubicEaseInOut, (t) =>
            {
                Vector3 e = new Vector3(r.anchoredPosition.x, t.CurrentValue);
                tunerSlider.GetComponent<RectTransform>().anchoredPosition = e;
                //tunerSlider.transform.SetPositionAndRotation(e, Quaternion.identity);
            }, (t) =>
            {
                FindObjectOfType<AudioManager>().Play("Static");
                Debug.Log("Done Show");
                isTunerHidden = false;
            });
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
        angle.SetText("Orig Rot: {0} Mapped Rot: {1:1}", dial.orientation,d);
        tunerSlider.value = d;

        checkTuningStatus(d);
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
        Vector2 cb = new Vector2((b.x - c.x), (b.y - c.y));

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
