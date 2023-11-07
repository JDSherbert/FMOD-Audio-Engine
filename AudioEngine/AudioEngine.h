// ©2023 JDSherbert. All rights reserved.

#pragma once

/// @file AudioEngine.h
/// 
/// FMOD wrapper which can utilize audio files (.wav, .mp3, .ogg etc) and FMOD soundbanks (.bank files).
/// Supports playback of loops or one-shots in stereo, and customizable spatialization.
///
/// @author JDSherbert
/// @dependencies FMOD Studio & Core

#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Source/Data/AudioData.h"

/**
 * Error Handling Function for FMOD Errors
 * @param result - the FMOD_RESULT generated during every FMOD function call 
 */
void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

/**
 * Class that handles the process of loading and playing sounds by wrapping FMOD's functionality.
 * Deals with all FMOD calls so that FMOD-specific code does not need to be used outside this class.
 * Only one AudioEngine should be constructed for an application.
 */
class AudioEngine 
{
public:
    /**
     * Default AudioEngine constructor. 
     * AudioEngine::Init() must be called before using the Audio Engine 
     */
    AudioEngine();

    /**
     * Initializes Audio Engine Studio and Core systems to default values. 
     */
    void Init();

    /**
     * Halts the engine instance and frees all held memory.
     */
    void Terminate();

    /**
    * Should be called each frame.
    */
    void Update();
    
    /**
     * Loads a sound from disk using provided settings
     * Prepares for later playback with Play()
     * Only reads the audio file and loads into the audio engine
     * if the sound file has already been added to the cache.
     */
    void Load(AudioData audioData);

    /**
    * Plays a sound file using FMOD's low level audio system. If the sound file has not been
    * previously loaded using Load(), a console message is displayed.
    */
    void Play(AudioData audioData);
    
    /**
     * Stops a looping sound if it's currently playing.
     */
    void Stop(AudioData audioData);

    /**
     * Method that updates the volume of a soundloop that is playing. This can be used to create audio 'fades'
     * where the volume ramps up or down to the provided new volume.
     * @param fadeSampleLength the length in samples of the intended volume sample. If less than 64 samples, the default 
     *                         FMOD fade out is used
     */
    void UpdateVolume(AudioData &audioData, float newVolume, unsigned int fadeSampleLength = 0);

   

    /**
    * Updates the position of a looping 3D sound that has already been loaded and is playing back.
    * The AudioData object's position coordinates will be used for the new sound position, so
    * SoundInfo::set3DCoords(x,y,z) should be called before this method to set the new desired location.
    */
    void Update3DPosition(AudioData audioData); 
      
    /**
     * Checks if a looping sound is playing.
     */
    bool IsPlaying(AudioData audioData); 
   

    /**
     * Sets the position of the listener in the 3D scene.
     * @param posX, posY, posZ - 3D translation of listener
     * @param forwardX, forwardY, forwardZ - forward angle character is looking in
     * @param upX, upY, upZ - up which must be perpendicular to forward vector
     */
    void Set3DListenerPosition(float posX,     float posY,     float posZ,
                               float forwardX, float forwardY, float forwardZ,
                               float upX,      float upY,      float upZ);

    /**
    * Utility method that returns the length of a AudioData's audio file in milliseconds
    * If the sound hasn't been loaded, returns 0
    */
    unsigned int GetLengthMS(AudioData audioData);

    /**
     * Loads an FMOD Studio soundbank 
     * TODO Fix
     */
    void LoadBank(const char* filePath);
    
    /**
     * Loads an FMOD Studio Event. The Soundbank that this event is in must have been loaded before
     * calling this method.
     * TODO Fix
     */
    void LoadEvent(const char* eventName, std::vector<std::pair<const char*, float>> paramsValues = { });
    
    /**
     * Sets the parameter of an FMOD Soundbank Event Instance.
     */
    void SetEventParamValue(const char* eventName, const char* parameterName, float value);
    
    /**
     * Plays the specified instance of an event
     * TODO support playback of multiple event instances
     * TODO Fix playback
     */
    void PlayEvent(const char* eventName, int instanceIndex = 0);
    
    /**
     * Stops the specified instance of an event, if it is playing.
     */
    void StopEvent(const char* eventName, int instanceIndex = 0);
 
    /**
     * Sets the volume of an event.
     * @param normalizedVolume - volume of the event, from 0 (min vol) to 1 (max vol)
     */
    void SetEventVolume(const char* eventName, float normalizedVolume = 0.75f);

    /**
     * Checks if an event is playing.
     */
    bool IsPlaying(const char* eventName, int instance = 0);

    /**
     * Mutes all sounds.
     */
	void MuteAll();

    /**
     * Unmutes all sounds.
     */
	void UnmuteAll();

    /**
     * Returns true if the audio engine is muted, false if not
     */
	bool IsMute();

    // The audio sampling rate of the audio engine
    static const int AUDIO_SAMPLE_RATE = 44100;

private:  

    /**
     * Checks if a sound file is in the soundCache
     */
    bool IsLoaded(AudioData audioData);

    /**
     * Sets the 3D position of a sound
     */
    void Set3DChannelPosition(AudioData audioData, FMOD::Channel* channel);

    /**
     * Initializes the reverb effect
     */
    void InitializeReverb();

    /**
     * Prints debug info about an FMOD event description
     */
    void DebugEventInfo(FMOD::Studio::EventDescription* eventDescription);

    // FMOD Studio API system, which can play FMOD sound banks (*.bank)
    FMOD::Studio::System* studioSystem = nullptr;       
    
    // FMOD's low-level audio system which plays audio files and is obtained from Studio System
    FMOD::System* lowLevelSystem = nullptr;          

    // Max FMOD::Channels for the audio engine 
    static const unsigned int MAX_AUDIO_CHANNELS = 255; 
    
    // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
    const float DISTANCEFACTOR = 1.0f;  
 
    // Listener head position, initialized to default value
    FMOD_VECTOR listenerPosition = { 0.0f, 0.0f, -1.0f * DISTANCEFACTOR };
    
    // Listener forward vector, initialized to default value
    FMOD_VECTOR forward     = { 0.0f, 0.0f, 1.0f };
    
    // Listener upwards vector, initialized to default value
    FMOD_VECTOR up          = { 0.0f, 1.0f, 0.0f };

    // Main group for low level system which all sounds go though
    FMOD::ChannelGroup* mastergroup = 0;

    // Low-level system reverb TODO add multi-reverb support
	FMOD::Reverb3D* reverb;

	// Reverb origin position
	FMOD_VECTOR revPos = { 0.0f, 0.0f, 0.0f };

	// reverb min, max distances
	float revMinDist = 10.0f, revMaxDist = 50.0f;

    // flag tracking if the Audio Engin is muted
    bool muted = false;

    /*
     * Map which caches FMOD Low-Level sounds
     * Key is the AudioData's uniqueKey field.
     * Value is the FMOD::Sound* to be played back.
     * TODO Refactor to use numeric UID as key
     */
    std::map<std::string, FMOD::Sound*> sounds;

    /*
     * Map which stores the current playback channels of any playing sound loop
     * Key is the AudioData's uniqueKey field.
     * Value is the FMOD::Channel* the FMOD::Sound* is playing back on.
     */
    std::map<std::string, FMOD::Channel*> loopsPlaying;

    /*
     * Map which stores the soundbanks loaded with loadFMODStudioBank()
     */
    std::map<std::string, FMOD::Studio::Bank*> soundBanks;
    
    /*
     * Map which stores event descriptions created during loadFMODStudioEvent()
     */
    std::map<std::string, FMOD::Studio::EventDescription*> eventDescriptions;
    
    /*
     * Map which stores event instances created during loadFMODStudioEvent()
     */
    std::map<std::string, FMOD::Studio::EventInstance*> eventInstances;
};
