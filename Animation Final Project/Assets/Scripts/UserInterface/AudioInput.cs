using UnityEngine;
using UnityEngine.Networking;
using TMPro;
using System;
using System.Collections;
using System.Runtime.InteropServices;

// Runtime file dialog based on solution discussed at
// https://www.dunnies-it.com/unity/open-file-dialog-script-in-unity.php

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
public class OpenFileName
{
    public int structSize = 0;
    public IntPtr dlgOwner = IntPtr.Zero;
    public IntPtr instance = IntPtr.Zero;
    public String filter = null;
    public String customFilter = null;
    public int maxCustFilter = 0;
    public int filterIndex = 0;
    public String file = null;
    public int maxFile = 0;
    public String fileTitle = null;
    public int maxFileTitle = 0;
    public String initialDir = null;
    public String title = null;
    public int flags = 0;
    public short fileOffset = 0;
    public short fileExtension = 0;
    public String defExt = null;
    public IntPtr custData = IntPtr.Zero;
    public IntPtr hook = IntPtr.Zero;
    public String templateName = null;
    public IntPtr reservedPtr = IntPtr.Zero;
    public int reservedInt = 0;
    public int flagsEx = 0;
}

public class DllTest
{
    [DllImport("Comdlg32.dll", SetLastError = true, ThrowOnUnmappableChar = true, CharSet = CharSet.Auto)]
    public static extern bool GetOpenFileName([In, Out] OpenFileName ofn);
    public static bool GetOpenFileName1([In, Out] OpenFileName ofn)
    {
        return GetOpenFileName(ofn);
    }
}

public class AudioInput : MonoBehaviour
{
    public TMP_InputField inputField;
    public string audioPath;
    public GameObject playbackSystem;
    //public AudioClip audioClip;

    IEnumerator GetAudioClipFromFile(string path)
    {
        using (UnityWebRequest www = UnityWebRequestMultimedia.GetAudioClip("file:///" + path, AudioType.WAV))
        {
            yield return www.SendWebRequest();

            if (www.result == UnityWebRequest.Result.ConnectionError)
            {
                Debug.Log(www.error);
            }
            else
            {
                Debug.Log("Successfully Retrieved Audio!");

                // Set the audio of the playback system
                playbackSystem.GetComponent<AudioSource>().clip = DownloadHandlerAudioClip.GetContent(www);
            }
        }
    }

    public void OnButtonPress()
    {
        OpenFileName ofn = new OpenFileName();
        ofn.structSize = Marshal.SizeOf(ofn);
        ofn.filter = "Waveform Audio File\0*.wav\0\0";
        ofn.file = new string(new char[256]);
        ofn.maxFile = ofn.file.Length;
        ofn.fileTitle = new string(new char[64]);
        ofn.maxFileTitle = ofn.fileTitle.Length;
        ofn.initialDir = UnityEngine.Application.dataPath;
        ofn.title = "Open Audio File";
        ofn.defExt = "WAV";
        ofn.flags = 0x00080000 | 0x00001000 | 0x00000800 | 0x00000008;
        // Flags: OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR

        if (DllTest.GetOpenFileName(ofn))
        {
            Debug.Log("Selected File: " + ofn.file);

            // Set the audio path input field to the selected file
            inputField.text = ofn.file;
            audioPath = ofn.file;

            // Load audio clip
            StartCoroutine(GetAudioClipFromFile(audioPath));


        }
        else
        {
            // Clear audio path input field to the selected file
            inputField.text = "";

            Debug.Log("No File Selected!");
        }

        // Stop playback system
        playbackSystem.GetComponent<Playback>().PlaybackStop();

    }
}
