using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEditor;
using UnityEditor.Scripting.Python;

 [System.Serializable]
 public class SerializedPhonemes
 {
      public List<string> phonemes;

      public void Add(string input)
      {
          if(phonemes == null)
          {
              phonemes = new List<string>();
              phonemes.Add(input);
          }
          else
          {
          phonemes.Add(input);
          }
      }
 }


public class PhonemeInput : MonoBehaviour
{
    public TMP_InputField InputField;
    public List<SerializedPhonemes> words;
    

    void Awake()
    {
        Debug.Log("Running");
        // For each word, add a SerializedPhonemes
        words.Add(new SerializedPhonemes());
        // For each phoneme, add said phoneme to the corresponding index list
        words[0].Add("Test");

    }

    public void CopyText()
    {
        // When button is pressed, run an external Python script

        PythonRunner.RunFile($"{Application.dataPath}/Scripts/Python/Test.py");

        // Feed the text through the pronouncing library via Python API
        // This can be converted to an external Python file later

        /*
        string command = @"
            import UnityEngine;
            import pronouncing;
            phones_list = pronouncing.phones_for_word('{text}')
            return 
            # Retrieve the game object the script is connected to
            main = UnityEngine.GameObject.Find('GeneratePhonemes_Setup')
            
            UnityEngine.Debug.Log(main.name)
            ".Replace("{text}", text);
        */

        //PythonRunner.RunString(command);
    }

}
