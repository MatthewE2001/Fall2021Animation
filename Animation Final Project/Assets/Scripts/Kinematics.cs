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
            if (tmp.GetNode(i).GetParentIndex() >= 0)
            {
                SolveSingleForwardKinematics(hierarchyState, tmp.GetNode(i).GetIndex(), tmp.GetNode(i).GetParentIndex());
            }
            else
            {
                hierarchyState.GetComponent<HierarchyState>().SetObjectPose(
                    hierarchyState.GetComponent<HierarchyState>().GetLocalPose());
            }

        }
    }

    public void SolveSingleForwardKinematics(GameObject hierarchyState, int index, int parentIndex) //parameter hierarchy state?
    {
        //need to differentiate between object space and local space (Space.World and Space.Self)
        //I think this will be two vector4 multiplied together
        //hierarchyState.GetComponent<HierarchyState>().getLocalPose()
        //hierarchyState.GetComponent<HierarchyState>().getObjectPose(),
        //multiple pose and parent pose parts in Space.World
        //hierarchyState->localSpace->pose[index].transformMat.m
        //multiply that result by pose in Space.Self

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