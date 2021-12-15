using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Keyframe : MonoBehaviour
{
    int index;
    int durationStep;
    int sampleIndex0, sampleIndex1;
    float durationSeconds;
    float durationInverse;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void InitializeKeyframe(int tmpSample0, int tmpSample1, int timeStep, float playbackPerSec)
    {
        sampleIndex0 = tmpSample0;
        sampleIndex1 = tmpSample1;
        durationStep = timeStep;
        durationSeconds = playbackPerSec;
    }

    public int GetIndex()
    {
        return index;
    }
}