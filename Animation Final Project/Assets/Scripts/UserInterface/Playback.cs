using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Playback : MonoBehaviour
{
    bool isPaused;

    // Start is called before the first frame update
    void Start()
    {
        isPaused = false;
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void onPlay()
    {
        if (GetComponent<AudioSource>().clip != null)
        {
            isPaused = false;
            GetComponent<AudioSource>().Play();
        }
        else
        {
            Debug.Log("No clip loaded!");
        }
    }

    public void onPause()
    {
        if (isPaused)
        {
            isPaused = false;
            GetComponent<AudioSource>().UnPause();
        }
        else
        {
            isPaused = true;
            GetComponent<AudioSource>().Pause();
        }

    }
}
