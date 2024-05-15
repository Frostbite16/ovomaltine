using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Coordinates 
{
    float _x;
    float _y;

    public Coordinates(float x, float y)
    {
        _x = x;
        _y = y;
    }

    static public void DrawCartesianPlane(Coordinates StartPos, Coordinates EndPos, Coordinates Width, Color color)
    {
        GameObject Line = new GameObject();
        LineRenderer lineRenderer = Line.AddComponent<LineRenderer>();
        lineRenderer.material = new Material(Shader.Find("Unlit/Color"));
        lineRenderer.material.color = color;
        lineRenderer.SetPosition(0, new Vector3(StartPos._x, StartPos._y, 0));
        lineRenderer.SetPosition(1, new Vector3(EndPos._x, EndPos._y, 0));
        lineRenderer.startWidth = Width._x;
        lineRenderer.endWidth = Width._y;

    }
   
}
