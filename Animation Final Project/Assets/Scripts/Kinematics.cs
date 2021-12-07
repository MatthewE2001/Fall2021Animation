using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Kinematics : MonoBehaviour
{
    //any needed data up here

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void SolveForwardKinematics(GameObject hierarchyState) //parameter hierarchy state?
    {
        //hierarchyState
    }

    public void SolvePartialForwardKinematics(GameObject hierarchyState, int firstIndex, int nodeCount) //parameter hierarchy state?
    {
        Hierarchy tmp = hierarchyState.GetComponent<HierarchyState>().GetHierarchy();

        for (int i = firstIndex; i < nodeCount; i++)
        {
            //if (tmp.GetNode(i). )
        }
    }

    public void SolveSingleForwardKinematics(int index, int parentIndex) //parameter hierarchy state?
    {
        //need to differentiate between object space and local space (Space.World and Space.Self)
        //I think this will be two vector4 multiplied together
        
    }

    public void SolveInverseKinematics() //parameter hierarchy state?
    {

    }

    public void SolvePartialInverseKinematics(int firstIndex, int nodeCount) //parameter hierarchy state?
    {

    }

    public void SolveSingleInverseKinematics(int index, int parentIndex) //parameter hierarchy state?
    {

    }
}