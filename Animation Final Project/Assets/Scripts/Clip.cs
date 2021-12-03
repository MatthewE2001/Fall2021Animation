using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Clip : MonoBehaviour
{
    string clipName;
    int index;
    int firstKeyframe, finalKeyframe, keyframeCount; //do 1st and final frames need to be keyframe?
    bool keyframeDirection; //this could also be an integer set to 1 or 0
    int durationStep;
    float durationSec, durationInverse;

    //clip transitions?

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void InitializeClip(string tmpName, Keyframe keyframeFirst, Keyframe keyframeFinal)
    {
        clipName = tmpName;
        //firstKeyframe = keyframeFirst.GetComponent<Keyframe>().GetIndex();
        //finalKeyframe = keyframeFinal.GetComponent<Keyframe>().GetIndex();
    }
}
