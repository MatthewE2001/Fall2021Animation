using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Node : MonoBehaviour
{
    //these could all theoretically be public variables
    public Transform location; //this is just the transform of the gameobject this is assigned to probably
    public int index;
    public int parentIndex;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void SetIndex(int newVal)
    {
        index = newVal;
    }

    public void SetParentIndex(int parentVal)
    {
        parentIndex = parentVal;
    }

    public int GetIndex()
    {
        return index;
    }

    public int GetParentIndex()
    {
        return parentIndex;
    }
}