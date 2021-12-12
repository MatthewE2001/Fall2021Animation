using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class Playback : MonoBehaviour
{

    public Button playButton;
    public Button pauseButton;

    bool isPaused;
    bool isPlaying;

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
        if (isPlaying)
        {
            // Stop playing
            isPaused = false;
            isPlaying = false;

            // Set the play button to inactive (Gray)
            playButton.GetComponent<Image>().color = new Color(88f / 255f, 88f / 255f, 88f / 255f);
            // Set the pause button to inactive (Gray 88)
            pauseButton.GetComponent<Image>().color = new Color(88f / 255f, 88f / 255f, 88f / 255f);

            // Stop playing audio and reset
            if (GetComponent<AudioSource>().clip != null)
            {
                GetComponent<AudioSource>().Stop();
            }
            else
            {
                Debug.Log("No audio clip loaded!");
            }

            // Stop playing animation and reset
        }
        else
        {
            // Start playing
            isPaused = false;
            isPlaying = true;

            // Set the play button to active (Blue)
            playButton.GetComponent<Image>().color = new Color(35f / 255f, 74f / 255f, 108f / 255f);
            // Set the pause button to inactive (Gray 88)
            pauseButton.GetComponent<Image>().color = new Color(88f / 255f, 88f / 255f, 88f / 255f);

            // Start playing audio from beginning
            if (GetComponent<AudioSource>().clip != null)
            {
                GetComponent<AudioSource>().Play();
            }
            else
            {
                Debug.Log("No audio clip loaded!");
            }

            // Start playing animation from beginning
        }


    }

    public void onPause()
    {
        // Pause button only works in play state
        if (isPlaying)
        {
            if (isPaused)
            {
                // Stop pause state
                isPaused = false;

                // Set the pause button to inactive (Gray 88)
                pauseButton.GetComponent<Image>().color = new Color(88f / 255f, 88f / 255f, 88f / 255f);

                // Unpause audio
                GetComponent<AudioSource>().UnPause();

                // Unpause animation
            }
            else
            {
                // Start pause state
                isPaused = true;

                // Set the pause button to active (Gray 106)
                pauseButton.GetComponent<Image>().color = new Color(106f / 255f, 106f / 255f, 106f / 255f);

                // Pause audio
                GetComponent<AudioSource>().Pause();

                // Pause animation
            }
        }
        else
        {
            Debug.Log("Not in play state!");
        }

    }
}
