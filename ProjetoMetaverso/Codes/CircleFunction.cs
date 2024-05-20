using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CircleFunction 
{
    float x, x0, y0, r;
    public CircleFunction(float _x, float _x0, float _y0, float _r)
    {
        x = _x;
        x0 = _x0;
        y0 = _y0;
        r = _r;
    }

    public float CircleFunct(float x, float x0, float r)
    {
        float y = (float)Math.Sqrt(Math.Pow(r, 2) - Math.Pow(x-x0, 2));
        if ((x+0.009f <= r + x0) && (x-0.009f >= -r + x0))
        {
            return y;
        }
        return 0;
    }
    static public void drawCircle(CircleFunction Startpos, CircleFunction EndPos, Draw width, Color color)
    {

        float i, j;
        for (i = Startpos.x; i <= EndPos.x; i += 0.009f)
        {
 
            Draw start = new Draw(i, Startpos.CircleFunct(i, Startpos.x0, Startpos.r) + Startpos.y0);
            Draw end = new Draw(i + 0.009f, Startpos.CircleFunct(i + 0.009f, Startpos.x0, Startpos.r)+Startpos.y0);
            Draw start2 = new Draw(i, -Startpos.CircleFunct(i, Startpos.x0, Startpos.r) + Startpos.y0);
            Draw end2 = new Draw(i + 0.009f, -Startpos.CircleFunct(i + 0.009f, Startpos.x0, Startpos.r)+Startpos.y0);
            if ((i + 0.009f <= Startpos.r + Startpos.x0) && (i + 0.009f >= -Startpos.r + Startpos.x0))
            {
                Draw.drawLine(start, end, width, color);
                Draw.drawLine(start2, end2, width, color);
            }
        }

    }

}
