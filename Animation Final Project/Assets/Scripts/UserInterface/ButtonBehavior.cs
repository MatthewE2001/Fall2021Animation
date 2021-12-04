using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using UnityEditor.Scripting.Python;
using UnityEditor;

public class ButtonBehavior : MonoBehaviour
{
    public void OnButtonPress()
    {
        PythonRunner.RunString(@"
                import UnityEngine;
                UnityEngine.Debug.Log('Hello World')
                ");
    }
}
