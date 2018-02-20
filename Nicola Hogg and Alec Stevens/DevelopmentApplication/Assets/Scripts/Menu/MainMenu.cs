using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


public class MainMenu : MonoBehaviour {

    public void StartApplication() {
        print("Starting Application");
        SceneManager.LoadScene("Main");
    }

    public void OpenOptions() {
        print("Opening Options");
    }
}
