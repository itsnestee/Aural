#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : nState(Stopped)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    nOpenFile.setButtonText ("Open...");
    nOpenFile.onClick = [this] { nOpenFileClicked(); };
    
    
    nPlay.setButtonText ("Play");
    nPlay.onClick = [this] { nPlayClicked(); };
    nPlay.setColour (juce::TextButton::buttonColourId, juce::Colours::green);
    nPlay.setEnabled (false);
    
    
    nStop.setButtonText ("Stop");
    nStop.onClick = [this] { nStopClicked(); };
    nStop.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
    nStop.setEnabled (false);
    
    
    addAndMakeVisible(&nPlay);
    addAndMakeVisible(&nStop);
    //addAndMakeVisible(&nPause);
    addAndMakeVisible(&nOpenFile);
    
    nManager.registerBasicFormats();
    nTSource.addChangeListener(this);
    
    setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    nTSource.prepareToPlay(samplesPerBlockExpected,  sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (nReaderSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;;
    }
    nTSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    nTSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (nBlack);

    g.setFont(50.0f);
    
    g.setColour(nOffWhite);
     
    g.drawFittedText("Aural.", getWidth() / 2 - 250, getHeight() / 2 - 300, 500, 100, juce::Justification::centred, 1);
    
}

void MainComponent::resized()
{
    nOpenFile.setBounds(getWidth() / 2 - 300, getHeight() / 2 - 200, 600, 25);
    nPlay.setBounds(getWidth() / 2 - 300, getHeight() / 2 - 150, 600, 25);
    nPause.setBounds(getWidth() / 2 - 300, getHeight() / 2 - 100, 600, 25);
    nStop.setBounds(getWidth() / 2 - 300, getHeight() / 2 - 50, 600, 25);
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &nTSource)
    {
        if (nTSource.isPlaying())
                changeState(Playing);
            
        
        else
                changeState(Stopped);
        
    }
}

void MainComponent::changeState(nAudioState newState)
{
    if (nState != newState)
    {
        nState = newState;
        
        switch (nState)
        {
            case Stopped:
                nPlay.setEnabled(true);
                nStop.setEnabled(false);
                nPause.setEnabled(false);
                nTSource.setPosition(0.0);
                break;
            
            case Starting:
                nPlay.setEnabled(false);
                nStop.setEnabled(true);
                nPause.setEnabled(true);
                nTSource.start();
            
            case Playing:
                nPlay.setEnabled(false);
                nStop.setEnabled(true);
                nPause.setEnabled(true);
                break;
                
       
                
            case Stopping:
                nPlay.setEnabled(true);
                nStop.setEnabled(false);
                nPause.setEnabled(false);
                nTSource.stop();
                break;
        }
    }
}

void MainComponent::nOpenFileClicked()
{
    nChooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...", juce::File{}, "*.wav");
    
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    
    nChooser -> launchAsync(chooserFlags, [this] (const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        
        if (file != juce::File{})
        {
            auto* reader = nManager.createReaderFor (file);
             
            if (reader != nullptr)
            {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                nTSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                nPlay.setEnabled (true);
                nReaderSource.reset (newSource.release());
            }
        }
    });
}


void MainComponent::nPlayClicked()
{
    changeState (Starting);
}

void  MainComponent::nStopClicked()
{
    changeState (Stopping);
}
