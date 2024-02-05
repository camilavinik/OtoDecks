/*
  ==============================================================================

    DeckGUI.h
    Created: 4 Feb 2024 10:02:41pm
    Author:  Camila

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component
{
public:
    DeckGUI();
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
