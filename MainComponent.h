#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
   
    
    enum nAudioState
    {
        Stopped,
        Starting,
        Playing,
  //      Paused,
  //      Resume,
        Stopping
    };
    
    nAudioState nState;
    
    void changeState(nAudioState newState);
    void nOpenFileClicked();
    void nPlayClicked();
    void nStopClicked();
    
private:
    //==============================================================================
    // Your private member variables go here...
    //Nestee Colours
     juce::Colour nBlack = juce::Colour::fromFloatRGBA(0.071f, 0.071f, 0.071f, 1.0);
     juce::Colour nOffWhite = juce::Colour::fromFloatRGBA(0.98f, 0.976f, 0.965f, 1.0);
    
    //Buttons
     juce::TextButton nPlay;
     juce::TextButton nStop;
     juce::TextButton nPause;
     juce::TextButton nOpenFile;
    
    //Classes
    std::unique_ptr<juce::FileChooser> nChooser;
    
    juce::AudioFormatManager nManager;
    std::unique_ptr<juce::AudioFormatReaderSource> nReaderSource;
    juce::AudioTransportSource nTSource;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
