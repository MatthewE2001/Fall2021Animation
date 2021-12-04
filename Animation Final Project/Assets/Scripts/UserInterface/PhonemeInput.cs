using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEditor;
using UnityEditor.Scripting.Python;

public class PhonemeInput : MonoBehaviour
{
    public TMP_InputField InputField;
    public List<string> Phonemes;

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
