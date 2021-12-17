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
    public AudioSource audioSource;

    public bool isPaused;
    public bool isPlaying;

    public float timer = 0.0f;

    private int sampleDataLength = 1024;
    public float clipLoudness;
    private float[] clipSampleData;


    // Start is called before the first frame update
    void Start()
    {
        isPaused = false;
        clipSampleData = new float[sampleDataLength];
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

                // Loop if the timer runs longer than the audio
                if (timer >= audioSource.clip.length)
                {
                    Debug.Log("Duration Exceeded!");
                    // Stop the current playback session
                    PlaybackStop();
                    // Restart playback from the beginning
                    PlaybackStart();
                }

                // Update the sound amplitude information
                if (audioSource.clip != null)
                {
                    audioSource.clip.GetData(clipSampleData, audioSource.timeSamples);
                    clipLoudness = 0f;
                    foreach (var sample in clipSampleData)
                    {
                        clipLoudness += Mathf.Abs(sample);
                    }
                    clipLoudness /= sampleDataLength;
                }
                else
                {
                    clipLoudness = 0.0f;
                }

                // Update the mouth bone rotation
                mouth.transform.Find("Root/Jaw_Start").gameObject.transform.rotation = transform.parent.rotation;
                mouth.transform.Find("Root/Jaw_Start").gameObject.transform.Rotate(clipLoudness * 32, 0, 0, Space.World);

                // Workaround for the phoneme system since we have only two viseme blendshapes
                float phoneme_control = Mathf.Clamp(clipLoudness * 400.0f, 0.0f, 100.0f);
                if (phoneme_control <= 25.0f)
                {
                    // AE controls
                    mouth.transform.Find("Preview_Mouth").gameObject.GetComponent<SkinnedMeshRenderer>().SetBlendShapeWeight(0, 0.0f);
                    // AO controls
                    mouth.transform.Find("Preview_Mouth").gameObject.GetComponent<SkinnedMeshRenderer>().SetBlendShapeWeight(1, phoneme_control);
                }
                else
                {
                    // AO control
                    mouth.transform.Find("Preview_Mouth").gameObject.GetComponent<SkinnedMeshRenderer>().SetBlendShapeWeight(1, 0.0f);
                    // AE controls
                    mouth.transform.Find("Preview_Mouth").gameObject.GetComponent<SkinnedMeshRenderer>().SetBlendShapeWeight(0, phoneme_control);
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

        // Reset transformation
        mouth.transform.Find("Root/Jaw_Start").gameObject.transform.rotation = transform.parent.rotation;

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
            PlaybackStop();
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

        // Reset transformation
        mouth.transform.Find("Root/Jaw_Start").gameObject.transform.rotation = transform.parent.rotation;

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
