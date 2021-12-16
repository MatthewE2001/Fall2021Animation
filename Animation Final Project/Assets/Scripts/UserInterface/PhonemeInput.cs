using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using TMPro;
using UnityEditor;
using UnityEditor.Scripting.Python;

 [System.Serializable]
 public class SerializedPhonemes
 {
    public int length;
    public List<string> phonemes;
    public void Add(string data)
    {
        phonemes = new List<string>();
        phonemes = data.Split(',').ToList();
        length = phonemes.Count();
    }
 }


public class PhonemeInput : MonoBehaviour
{
    public TMP_InputField InputField;
    public List<SerializedPhonemes> words;

    [HideInInspector]
    public string dataString;
    private string[] split;
    

    void Awake()
    {
        Debug.Log("Running");
        // For each word, add a SerializedPhonemes
        // words.Add(new SerializedPhonemes());
        // For each phoneme, add said phoneme to the corresponding index list
        // words[0].Add("Test");

    }

    public void CopyText()
    {
        // When button is pressed, run an external Python script
        PythonRunner.RunFile($"{Application.dataPath}/Scripts/Python/PhonemeGeneration.py");
        // Check if an error has been communicated
        if (dataString != "invalid")
        {
            words.Clear();
            // Convert the phoneme output into a string and trim
            split = dataString.Split('[');
            split = split.Skip(1).ToArray();
            // Remove extra data indicating the end of phoneme groups
            for (int i = 0; i < split.Length; i++)
            {
                split[i] = split[i].Remove(split[i].Length - 1, 1);
            }

            for (int i = 0; i < split.Length; i++)
            {
                words.Add(new SerializedPhonemes());
                words[i].Add(split[i]);
            }
        }
        else
        {
            // Clear the words list
            words.Clear();
        }
    }
}
