using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CartesianPlane : MonoBehaviour
{
    Coordinates _startPointX;
    Coordinates _startPointY;

    Coordinates _endPointX;
    Coordinates _endPointY;

    Coordinates _Width;
  
    void Start()
    {
        _startPointX = new Coordinates(260, 0);
        _startPointY = new Coordinates(0, 100);
        _endPointX = new Coordinates(-260, 0);
        _endPointY = new Coordinates(0, -100);
        _Width = new Coordinates(0, 0.1f);

        Coordinates.DrawCartesianPlane(_startPointX, _endPointX,_Width, Color.red);
        Coordinates.DrawCartesianPlane(_startPointY, _endPointY,_Width, Color.blue);

    }

    void Update()
    {
        
    }
}
