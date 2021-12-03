using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HierarchyState : MonoBehaviour
{
    //any necessary data here

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public float PoseIntegrateEuler(float x, float dxdt, float dt)
    {
        float euler;

        euler = x + (dxdt / dt) * dt;

        return euler;
    }

    public float PoseIntegrateKinematic(float x, float dxdt, float dxdt2, float dt)
    {
        float kinematic;

        kinematic = x + (dxdt / dt) * dt + (dxdt2 / (dt * dt)) * ((dt * dt) / 2);

        return kinematic;
    }

    public float PoseIntegrateLerp(float x, float xc, float u)
    {
        float lerp;

        lerp = x + (xc - x) * u;

        return lerp;
    }

    public float PoseQuadraticBezier(float p0, float p1, float p2, float t)
    {
        float bezier;

        bezier = p0 * ((1 - t) * (1 - t)) + p1 * 2 * (1 - t) * t + p2 * (t * t);

        return bezier;
    }
}