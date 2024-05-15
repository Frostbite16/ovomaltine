using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class walking : MonoBehaviour
{

    void Update()
    {
        float X = Input.GetAxis("Horizontal");
        float Y = Input.GetAxis("Vertical");
        Vector3 a = new Vector3(0, 0, 0);
        a += transform.position;
        //transform.position += new Vector3(1*Time.deltaTime,1*Time.deltaTime,0);
        for(int i = 0; i <= 5; i++){
            a += new Vector3(i * 0.01f, i * 0.01f, 0);
            Instantiate(gameObject, a , Quaternion.identity);
        }
        Debug.Log(transform.position);
        Debug.Log(X);
        
    }
}
