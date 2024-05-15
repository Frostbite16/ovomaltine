using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Canva : MonoBehaviour
{
    CircleFunction _startPos;
    CircleFunction _endPos;
    Draw _width;
    void Start()
    {
        _startPos = new CircleFunction(-20, 4, 4, 2);
        _endPos = new CircleFunction(20, 0, 0, 2);
        _width = new Draw(0.09f, 0.09f);
        CircleFunction.drawCircle(_startPos, _endPos, _width, Color.green);

    }

}
