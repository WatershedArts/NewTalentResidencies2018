using UnityEngine.Audio;
using UnityEngine;

//---------------------------------------------------------------
public class AudioManager : MonoBehaviour {

	public Sound[] sounds;
	public static AudioManager instance;

	//---------------------------------------------------------------
	// On Awake Action
	//---------------------------------------------------------------
	void Awake() {

		if( instance == null ) {
			instance = this;
		}
		else {
			Destroy(gameObject);
		}

		DontDestroyOnLoad(gameObject);

		foreach(Sound s in sounds) {
			s.source = gameObject.AddComponent<AudioSource>();
			s.source.clip = s.clip;
			s.source.volume = s.volume;
			s.source.pitch = s.pitch;
		}
	}

	//---------------------------------------------------------------
	// Play Specific Beacon Audio 
	//---------------------------------------------------------------
	public void Play(string name) {
		Sound s = System.Array.Find(sounds,sound => sound.name == name);
		if( s == null ) {
			Debug.LogWarning("Cannot Find Audio File " + name);
			return;
		}
		s.source.Play();
	}

	//---------------------------------------------------------------
	// Stop Beacon Audio
	//---------------------------------------------------------------
	public void Stop(string name) {
		Sound s = System.Array.Find(sounds,sound => sound.name == name);
		if( s == null ) {
			Debug.LogWarning("Cannot Find Audio File " + name);
			return;
		}
		s.source.Stop();
	}
}