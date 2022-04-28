#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    
    addAndMakeVisible(&nPlay);
    addAndMakeVisible(&nStop);
    addAndMakeVisible(&nPause);
    addAndMakeVisible(&nOpenFile);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
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
    
}
