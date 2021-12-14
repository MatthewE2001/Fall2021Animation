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

    public void SpatialPoseConcat(GameObject left, GameObject right)
    {
        rotation.x = left.GetComponent<SpatialPose>().rotation.x + right.GetComponent<SpatialPose>().rotation.x;
        rotation.y = left.GetComponent<SpatialPose>().rotation.y + right.GetComponent<SpatialPose>().rotation.y;
        rotation.z = left.GetComponent<SpatialPose>().rotation.z + right.GetComponent<SpatialPose>().rotation.z;

        scale.x = left.GetComponent<SpatialPose>().scale.x + right.GetComponent<SpatialPose>().scale.x;
        scale.y = left.GetComponent<SpatialPose>().scale.y + right.GetComponent<SpatialPose>().scale.y;
        scale.z = left.GetComponent<SpatialPose>().scale.z + right.GetComponent<SpatialPose>().scale.z;

        translation.x = left.GetComponent<SpatialPose>().translation.x + right.GetComponent<SpatialPose>().translation.x;
        translation.y = left.GetComponent<SpatialPose>().translation.y + right.GetComponent<SpatialPose>().translation.y;
        translation.z = left.GetComponent<SpatialPose>().translation.z + right.GetComponent<SpatialPose>().translation.z;
    }

    public void SpatialPoseDeConcat(GameObject left, GameObject right)
    {
        rotation.x = left.GetComponent<SpatialPose>().rotation.x - right.GetComponent<SpatialPose>().rotation.x;
        rotation.y = left.GetComponent<SpatialPose>().rotation.y - right.GetComponent<SpatialPose>().rotation.y;
        rotation.z = left.GetComponent<SpatialPose>().rotation.z - right.GetComponent<SpatialPose>().rotation.z;

        scale.x = left.GetComponent<SpatialPose>().scale.x - right.GetComponent<SpatialPose>().scale.x;
        scale.y = left.GetComponent<SpatialPose>().scale.y - right.GetComponent<SpatialPose>().scale.y;
        scale.z = left.GetComponent<SpatialPose>().scale.z - right.GetComponent<SpatialPose>().scale.z;

        translation.x = left.GetComponent<SpatialPose>().translation.x - right.GetComponent<SpatialPose>().translation.x;
        translation.y = left.GetComponent<SpatialPose>().translation.y - right.GetComponent<SpatialPose>().translation.y;
        translation.z = left.GetComponent<SpatialPose>().translation.z - right.GetComponent<SpatialPose>().translation.z;
    }

    public void SpatialPoseConvert(SpatialPoseOrder order, Vector4 rotation)
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
                gameObject.transform.Rotate(rotation, Space.World); //world space clarification?
                

                break;
            case SpatialPoseOrder.XZY:

                break;
            case SpatialPoseOrder.YXZ:

                break;
            case SpatialPoseOrder.YZX:

                break;
            case SpatialPoseOrder.ZXY:

                break;
            case SpatialPoseOrder.ZYX:

                break;
            default:
                //maybe default should just be xyz

                break;
        }
    }
}