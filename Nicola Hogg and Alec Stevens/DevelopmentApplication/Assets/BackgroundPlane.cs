using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//https://gist.github.com/pyrobot/4363640

public class BackgroundPlane : MonoBehaviour {

    static BackgroundPlane _this;

    public Color topLeftColor, topRightColor, btmRightColor, btmLeftColor;
    public Material mat;

    Mesh thisMesh;
    Camera thisCam;
    GameObject backgroundPlane;

    public static Color TopLeftColor
    {
        get { return _this.topLeftColor; }
        set { _this.topLeftColor = value; _this.setMeshColors(); }
    }

    public static Color TopRightColor
    {
        get { return _this.topRightColor; }
        set { _this.topRightColor = value; _this.setMeshColors(); }
    }

    public static Color BtmLeftColor
    {
        get { return _this.btmLeftColor; }
        set { _this.btmLeftColor = value; _this.setMeshColors(); }
    }

    public Color BtmRightColor
    {
        get { return _this.btmRightColor; }
        set { _this.btmRightColor = value; _this.setMeshColors(); }
    }

    void Awake()
    {
        _this = this;
        thisCam = Camera.main;
    }

    void Start()
    {
        if (thisCam == null)
        {
            Debug.LogWarning("Script must be able to access the Camera");
            return;
        }

        float farClip = thisCam.farClipPlane - 0.01f;

        Vector3
            topLeftPosition = thisCam.ViewportToWorldPoint(new Vector3(0, 1, farClip)),
            topRightPosition = thisCam.ViewportToWorldPoint(new Vector3(1, 1, farClip)),
            btmLeftPosition = thisCam.ViewportToWorldPoint(new Vector3(0, 0, farClip)),
            btmRightPosition = thisCam.ViewportToWorldPoint(new Vector3(1, 0, farClip));

        Vector3[] verts = new Vector3[] {
            topLeftPosition, topRightPosition, btmLeftPosition, btmRightPosition
        };

        int[] tris = new int[] {
            0, 1, 2, 2, 1, 3
        };

        thisMesh = new Mesh();
        thisMesh.vertices = verts;
        thisMesh.triangles = tris;

        backgroundPlane = new GameObject("_backgroundPlane");
        backgroundPlane.transform.parent = transform;
        backgroundPlane.AddComponent<MeshFilter>().mesh = thisMesh;
        backgroundPlane.AddComponent<MeshRenderer>().material = mat;

        setMeshColors();
    }

    void setMeshColors()
    {
        thisMesh.colors = new Color[] {
            topLeftColor, topRightColor, btmLeftColor, btmRightColor
        };
    }
}
