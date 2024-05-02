using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LinearEquation : MonoBehaviour
{
    float a;
    float b;
    float x;
    float y;
    public LinearEquation(float _a, float _b, float _x, float _y){
        a = _a;
        b = _b;
        x = _x;
        y = _y;
    }

    public float calculateLine() => a + (b * x);

    public float quadraticFunction(float n) {
        return n*n;
    }
    static public void drawLine(LinearEquation startPos, LinearEquation endPos, LinearEquation width, Color color){
        GameObject Line = new GameObject();
        LineRenderer lineRenderer = Line.AddComponent<LineRenderer>();

        lineRenderer.material = new Material(Shader.Find("Unlit/Color"));
        lineRenderer.material.color = color;

        lineRenderer.SetPosition(0, new Vector3(startPos.x, startPos.y, 0));
        lineRenderer.SetPosition(1, new Vector3(endPos.x, endPos.y, 0));

        lineRenderer.startWidth = width.x;
        lineRenderer.endWidth = width.y;

    }



    static public void drawCurve(LinearEquation startPos, LinearEquation endPos, LinearEquation width, Color color){
        float s;

        for (s=startPos.x; s <= endPos.x; s += 0.009f){
            
            LinearEquation start = new LinearEquation(0,0,s,startPos.quadraticFunction(s));
            LinearEquation end = new LinearEquation(0,0,s + 0.009f, startPos.quadraticFunction(s + 0.009f));
            drawLine(start, end, width, color);
            
        }

    
    }

    static public void drawLinear(LinearEquation startPos, LinearEquation endPos, LinearEquation width, Color color)
    {
      
        LinearEquation start = new LinearEquation(0, 0, startPos.x, startPos.calculateLine());
        LinearEquation end = new LinearEquation(0, 0, endPos.x, endPos.calculateLine());
        drawLine(start, end, width, color);

    }
}
