using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class LinearEquation
{
    double a;
    double b;
    double x;
    double y;
    public LinearEquation(double _a, double _b, double _x, double _y)
    {
        a = _a;
        b = _b;
        x = _x;
        y = _y;
    }


    public double calculateLine() => a + (b * x);

    public double quadraticFunction(double n)
    {
        return n * n * n;
    }

    static public bool conicFunction(double a, double b, double c, double d, double f, double e, double x, double y)
    {

        return (a * (x * x) + d * x + f) == (-b * (y * y) - c * x * y - e * y);

    }

    public double CircleFunction(double x, double x0, double y0, double r)
    {
        y = (Math.Sqrt(Math.Pow(r, 2) - Math.Pow(x, 2)));
        if ((x <= r) && (x >= -r))
        {
            return y;
        }
        return 0;
    }


    /*public void drawConicFunction(LinearEquation Startpos, LinearEquation EndPos, LinearEquation width, Color color){

        double i,j;
        for(i=Startpos.x; i<=EndPos.x; i += 0.009f){
            for (j = Startpos.y; j <= EndPos.y; j += 0.009f){
                if (conicFunction(1,1,0,0,4,0,i,j)){

                    LinearEquation start = new LinearEquation(0, 0, i, j);
                    LinearEquation end = new LinearEquation(0, 0, i + 0.009, j + 0.009f);
                    drawLine(start, end, width, color);
                }
            }
        } 

    }*/

    static public void drawCircle(LinearEquation Startpos, LinearEquation EndPos, LinearEquation width, Color color)
    {

        double i, j;
        for (i = Startpos.x; i <= EndPos.x; i += 0.009f)
        {
            int r = 4;
            LinearEquation start = new LinearEquation(0, 0, i, Startpos.CircleFunction(i, 0, 0, r));
            LinearEquation end = new LinearEquation(0, 0, i + 0.009f, Startpos.CircleFunction(i + 0.009f, 0, 0, r));
            LinearEquation start2 = new LinearEquation(0, 0, i, -Startpos.CircleFunction(i, 0, 0, r));
            LinearEquation end2 = new LinearEquation(0, 0, i + 0.009f, -Startpos.CircleFunction(i + 0.009f, 0, 0, r));
            if ((i <= r) && (i + 0.009f >= -r))
            {
                drawLine(start, end, width, color);
                drawLine(start2, end2, width, color);
            }
        }

    }

    static public void drawLine(LinearEquation startPos, LinearEquation endPos, LinearEquation width, Color color)
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

    static public void drawCurve(LinearEquation startPos, LinearEquation endPos, LinearEquation width, Color color)
    {
        double s;

        for (s = startPos.x; s <= endPos.x; s += 0.009f)
        {

            LinearEquation start = new LinearEquation(0, 0, s, startPos.quadraticFunction(s));
            LinearEquation end = new LinearEquation(0, 0, s + 0.009f, startPos.quadraticFunction(s + 0.009f));
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

