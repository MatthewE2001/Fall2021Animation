using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class Playback : MonoBehaviour
{

    public Button playButton;
    public Button pauseButton;

    public GameObject mouth;

    public bool isPaused;
    public bool isPlaying;

    public float timer = 0.0f;

    // Start is called before the first frame update
    void Start()
    {
        isPaused = false;
    }

    // Update is called once per frame
    void Update()
    {
        // Update the playback timer
        if (isPlaying)
        {
            if (!isPaused)
            {
                // Update the timer
                timer += Time.deltaTime;

                // Update the animation

                // Loop if the timer runs longer than the audio
                if (timer >= GetComponent<AudioSource>().clip.length)
                {
                    // Stop the current playback session
                    PlaybackStop();
                    // Restart playback from the beginning
                    PlaybackStart();
                }
            }
        }
    }

    /// <summary>
    /// Start playback.
    /// </summary>
    public void PlaybackStart()
    {
        Debug.Log("Start Playback!");

        // Reset timer
        timer = 0.0f;

        // Set active play state
        isPlaying = true;
        // Set inactive pause state
        isPaused = false;

        // Set active play button (Blue)
        playButton.GetComponent<Image>().color = new Color(35f / 255f, 74f / 255f, 108f / 255f);
        // Set inactive pause button (Gray 88)
        pauseButton.GetComponent<Image>().color = new Color(88f / 255f, 88f / 255f, 88f / 255f);

        // Start audio playback
        if (GetComponent<AudioSource>().clip != null)
        {
            Debug.Log("Trying to play");
            GetComponent<AudioSource>().Play();
        }
        else
        {
            Debug.Log("No audio clip loaded!");
        }

    }

    /// <summary>
    /// Stop playback.
    /// </summary>
    public void PlaybackStop()
    {
        Debug.Log("Stop Playback!");

        // Reset timer
        timer = 0.0f;

        // Set inactive play state
        isPlaying = false;
        // Set inactive pause state
        isPaused = false;

        // Set inactive play button (Gray 88)
        playButton.GetComponent<Image>().color = new Color(88f / 255f, 88f / 255f, 88f / 255f);
        // Set inactive pause button (Gray 88)
        pauseButton.GetComponent<Image>().color = new Color(88f / 255f, 88f / 255f, 88f / 255f);


        // Stop audio playback
        if (GetComponent<AudioSource>().clip != null)
        {
            GetComponent<AudioSource>().Stop();
        }
        else
        {
            Debug.Log("No audio clip loaded!");
        }
    }

    /// <summary>
    /// Start/stop playback.
    /// </summary>
    public void OnPlayButton()
    {
        if (isPlaying)
        {
            PlaybackStop();
        }
        else
        {
            PlaybackStart();
        }


    }

    /// <summary>
    /// Pause the playback.
    /// </summary>
    public void PlaybackPause()
    {
        // Set active pause state
        isPaused = true;

        // Set active pause button (Gray 106)
        pauseButton.GetComponent<Image>().color = new Color(106f / 255f, 106f / 255f, 106f / 255f);

        // Pause audio
        GetComponent<AudioSource>().Pause();

        // Pause animation

    }

    /// <summary>
    /// Unpause the playback.
    /// </summary>
    public void PlaybackUnpause()
    {
        // Set inactive pause state
        isPaused = false;

        // Set inactive pause button (Gray 88)
        pauseButton.GetComponent<Image>().color = new Color(88f / 255f, 88f / 255f, 88f / 255f);

        // Unpause audio
        GetComponent<AudioSource>().UnPause();

        // Unpause animation

    }

    /// <summary>
    /// Pause/unpause the playback.
    /// </summary>
    public void OnPauseButton()
    {
        // Pause button only works in play state
        if (isPlaying)
        {
            if (isPaused)
            {
                PlaybackUnpause();
            }
            else
            {
                PlaybackPause();
            }
        }
        else
        {
            Debug.Log("Not in play state!");
        }

    }
}
