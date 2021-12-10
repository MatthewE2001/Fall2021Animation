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
        //SolvePartialForwardKinematics(hierarchyState, hierarchyState.GetComponent<HierarchyState>())
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
        Vector4 tmp1;
        tmp1.x = hierarchyState.GetComponent<HierarchyState>().GetLocalPose().transform.position.x
            * hierarchyState.GetComponent<HierarchyState>().GetObjectPose().transform.position.x;
        tmp1.y = hierarchyState.GetComponent<HierarchyState>().GetLocalPose().transform.position.y
            * hierarchyState.GetComponent<HierarchyState>().GetObjectPose().transform.position.y;
        tmp1.z = hierarchyState.GetComponent<HierarchyState>().GetLocalPose().transform.position.z
            * hierarchyState.GetComponent<HierarchyState>().GetObjectPose().transform.position.z;
        //multiple pose and parent pose parts in Space.World
        //hierarchyState.GetComponent<HierarchyState>().GetLocalPose().transform.position = tmp1;
        //multiply that result by pose in Space.Self

    }

    public void SolveInverseKinematics(GameObject hierarchyState)
    {
        //SolvePartialInverseKinematics();
    }

    public void SolvePartialInverseKinematics(GameObject hierarchyState, int firstIndex, int nodeCount)
    {

    }

    public void SolveSingleInverseKinematics(GameObject hierarchyState, int index, int parentIndex)
    {
        Vector4 tmp1;
        //need to change this to object inverse
        tmp1.x = hierarchyState.GetComponent<HierarchyState>().GetLocalPose().transform.position.x
            * hierarchyState.GetComponent<HierarchyState>().GetObjectPose().transform.position.x;
        tmp1.y = hierarchyState.GetComponent<HierarchyState>().GetLocalPose().transform.position.y
            * hierarchyState.GetComponent<HierarchyState>().GetObjectPose().transform.position.y;
        tmp1.z = hierarchyState.GetComponent<HierarchyState>().GetLocalPose().transform.position.z
            * hierarchyState.GetComponent<HierarchyState>().GetObjectPose().transform.position.z;

        //hierarchyState.GetComponent<HierarchyState>().GetObjectPose().transform.position = tmp1;
    }
}