using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DrawLinear : MonoBehaviour
{
    LinearEquation _startPos;
    LinearEquation _endPos;

    LinearEquation _width;

    void Start(){

        _startPos = new LinearEquation(2, 1, -20, 0);
        _endPos = new LinearEquation(2, 1, 20, 0);

        _width = new LinearEquation(0, 0, 0.09f, 0.09f);

        LinearEquation.drawLinear(_startPos, _endPos, _width, Color.white);

        LinearEquation.drawCurve(_startPos, _endPos, _width, Color.cyan);

        
    }
}
