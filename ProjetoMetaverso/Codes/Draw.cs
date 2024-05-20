using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Draw 
{   double x,y;
    public Draw(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
    static public void drawLine(Draw startPos, Draw endPos, Draw width, Color color)
    {
        GameObject Line = new GameObject();
        LineRenderer lineRenderer = Line.AddComponent<LineRenderer>();

        lineRenderer.material = new Material(Shader.Find("Unlit/Color"));
        lineRenderer.material.color = color;

        lineRenderer.SetPosition(0, new Vector3((float)startPos.x, (float)startPos.y, 0));
        lineRenderer.SetPosition(1, new Vector3((float)endPos.x, (float)endPos.y, 0));

        lineRenderer.startWidth = (float)width.x;
        lineRenderer.endWidth = (float)width.y;

    }
}
