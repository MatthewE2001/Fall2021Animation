using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum SpatialPoseOrder
{
    XYZ,
    YZX,
    ZXY,
    YXZ,
    XZY,
    ZYX
};

public class SpatialPose : MonoBehaviour
{
    //values for the spatial pose
    Vector4 scale;
    Vector4 translation;
    Vector4 rotation;
    //Vector4 user; //unsure what user is exactly
    Matrix4x4 transformMat; //need to set this to value for no warning

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
        rotation = tmpRotate;
    }

    public void SetTranslation(Vector4 tmpTranslate)
    {
        //translation = tmpTranslate;
        gameObject.transform.position = tmpTranslate;
    }

    public void SetScale(Vector4 tmpScale)
    {
        gameObject.transform.localScale = tmpScale;
    }

    public void CopyPose(GameObject poseToCopy)
    {
        //set the values of this pose to those of the gameobject parameter
        //gameObject.this = poseToCopy;
    }

    public void SpatialPoseLerp()
    {

    }

    public void SpatialPoseConcat()
    {

    }

    public void SpatialPoseConvert(SpatialPoseOrder order)
    {
        //a3mat4 rx, ry, rz, r;
        //a3real4x4SetRotateX(rx.m, a3trigValid_sind(spatialPose->rotate.x));
        //a3real4x4SetRotateY(ry.m, a3trigValid_sind(spatialPose->rotate.y));
        //a3real4x4SetRotateZ(rz.m, a3trigValid_sind(spatialPose->rotate.z));
       
        switch (order)
        {
            case SpatialPoseOrder.XYZ:
                //a3real4x4Product(r.m, rx.m, ry.m);
                //a3real4x4Product(spatialPose->transformMat.m, r.m, rz.m);

                break;
            default:

                break;
        }
    }
}