using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Hierarchy : MonoBehaviour
{
    int index;
    int parentIndex;

    Transform[] nodes; //Should this be an array

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void CreateHierarchy(int numberOfNodes, string name)
    {
        for (int i = 0; i < numberOfNodes; i++)
        {
            //nodes[i].name = name;
        }
    }

    public void SetHierarchyNode(int index, int parentIndex, string name)
    {

    }

    public int GetNodeIndex(string name)
    {
        for (int i = 0; i < nodes.Length; i++)
        {
            if (nodes[i].name == name)
            {
                return i;
            }
        }

        return -1; //should only reach this if no other name is found
    }

    public string GetNodeName(int index)
    {
        if (index < nodes.Length && index > -1)
        {
            return nodes[index].name;
        }
        else
        {
            return null;
        }
    }
}
