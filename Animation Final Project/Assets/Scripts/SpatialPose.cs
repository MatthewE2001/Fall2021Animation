using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpatialPose : MonoBehaviour
{
    //values for the spatial pose
    Vector4 scale;
    Vector4 translation;
    Vector4 rotation;
    //Vector4 user; //unsure what user is exactly
    Matrix4x4 transform; //need to set this to value for no warning

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void SetRotation(Vector4 tmpRotate)
    {

    }

    public void SetTranslation(Vector4 tmpTranslate)
    {

    }

    public void SetScale(Vector4 tmpScale)
    {

    }

    public void CopyPose(GameObject poseToCopy)
    {
        //set the values of this pose to those of the gameobject parameter
    }

    //public void SpatialPoseLerp()
}