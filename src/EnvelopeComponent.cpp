//
//  EnvelopeComponent_hpp.cpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#include "EnvelopeComponent.hpp"
#include "PluginProcessor.h"

#define MOD_SLIDER_WIDTH        (150)
#define MOD_SLIDER_HEIGHT       (24)
#define ADSR_UI_MARGIN          (4)
#define TITLE_HEIGHT            (20)
#define TEXT_INPUT_HEIGHT       (46)

EnvelopeComponent::EnvelopeComponent(AudioProcessorValueTreeState &state,
                                     ADSRUI::ParameterIdentifiers identifiers)
: _adsrui(state, identifiers)
, _attackSliderAttachment(state, PinkTromboneAudioProcessor::attack, _attackSlider)
, _decaySliderAttachment(state, PinkTromboneAudioProcessor::decay, _decaySlider)
, _releaseSliderAttachment(state, PinkTromboneAudioProcessor::release, _releaseSlider)
, _initialSliderAttachment(state, PinkTromboneAudioProcessor::initial, _initialSlider)
, _peakSliderAttachment(state, PinkTromboneAudioProcessor::peak, _peakSlider)
, _sustainSliderAttachment(state, PinkTromboneAudioProcessor::sustain, _sustainSlider)
, _tongueModXSlider(state, PinkTromboneAudioProcessor::envModTongueX)
, _tongueModYSlider(state, PinkTromboneAudioProcessor::envModTongueY)
, _constrictionModXSlider(state, PinkTromboneAudioProcessor::envModConstrictionX)
, _constrictionModYSlider(state, PinkTromboneAudioProcessor::envModConstrictionY)
, _pitchModSlider(state, PinkTromboneAudioProcessor::envModPitch)
{
    addAndMakeVisible(_adsrui);
    
    _titleLabel.setText("ENVELOPE", juce::dontSendNotification);
    _titleLabel.setFont (juce::Font ("Myriad Pro", 8.0f, juce::Font::plain));
    addAndMakeVisible(_titleLabel);
    _tongueModXLabel.setText("TongueX", juce::dontSendNotification);
    _tongueModXLabel.setFont (juce::Font ("Myriad Pro", 8.0f, juce::Font::plain));
    addAndMakeVisible(_tongueModXLabel);
    _tongueModYLabel.setText("TongueY", juce::dontSendNotification);
    _tongueModYLabel.setFont (juce::Font ("Myriad Pro", 8.0f, juce::Font::plain));
    addAndMakeVisible(_tongueModYLabel);
    _constrictionModXLabel.setText("ConX", juce::dontSendNotification);
    _constrictionModXLabel.setFont (juce::Font ("Myriad Pro", 8.0f, juce::Font::plain));
    addAndMakeVisible(_constrictionModXLabel);
    _constrictionModYLabel.setText("ConY", juce::dontSendNotification);
    _constrictionModYLabel.setFont (juce::Font ("Myriad Pro", 8.0f, juce::Font::plain));
    addAndMakeVisible(_constrictionModYLabel);
    _pitchModLabel.setText("Pitch", juce::dontSendNotification);
    _pitchModLabel.setFont (juce::Font ("Myriad Pro", 8.0f, juce::Font::plain));
    addAndMakeVisible(_pitchModLabel);

    // _attackSlider.setLookAndFeel(&_hiddenLookAndFeel);
    // _decaySlider.setLookAndFeel(&_hiddenLookAndFeel);
    // _releaseSlider.setLookAndFeel(&_hiddenLookAndFeel);
    // _initialSlider.setLookAndFeel(&_hiddenLookAndFeel);
    // _peakSlider.setLookAndFeel(&_hiddenLookAndFeel);
    // _sustainSlider.setLookAndFeel(&_hiddenLookAndFeel);

    addAndMakeVisible(_attackSlider);
    addAndMakeVisible(_decaySlider);
    addAndMakeVisible(_releaseSlider);
    addAndMakeVisible(_initialSlider);
    addAndMakeVisible(_peakSlider);
    addAndMakeVisible(_sustainSlider);
    
    addAndMakeVisible(_tongueModXSlider);
    addAndMakeVisible(_tongueModYSlider);
    addAndMakeVisible(_constrictionModXSlider);
    addAndMakeVisible(_constrictionModYSlider);
    addAndMakeVisible(_pitchModSlider);
}

EnvelopeComponent::~EnvelopeComponent()
{
    
}

void EnvelopeComponent::paint(Graphics &)
{
    
}

void EnvelopeComponent::resized()
{
    _titleLabel.setBounds(0, 0, getWidth(), TITLE_HEIGHT);
    
    _adsrui.setBounds(
        ADSR_UI_MARGIN,
        TITLE_HEIGHT + ADSR_UI_MARGIN,
        getWidth() - (2 * ADSR_UI_MARGIN + MOD_SLIDER_WIDTH),
        getHeight() - (TITLE_HEIGHT + 2 * ADSR_UI_MARGIN + TEXT_INPUT_HEIGHT)
    );

    auto textInputAreaY = getHeight() - TEXT_INPUT_HEIGHT;
    auto textInputAreaWidth = getWidth() - (2 * ADSR_UI_MARGIN + MOD_SLIDER_WIDTH);
    _attackSlider.setBounds(ADSR_UI_MARGIN, textInputAreaY, textInputAreaWidth / 3.0, TEXT_INPUT_HEIGHT / 2.0);
    _decaySlider.setBounds(ADSR_UI_MARGIN + textInputAreaWidth / 3.0, textInputAreaY, textInputAreaWidth / 3.0, TEXT_INPUT_HEIGHT / 2.0);
    _releaseSlider.setBounds(ADSR_UI_MARGIN + 2.0 * textInputAreaWidth / 3.0, textInputAreaY, textInputAreaWidth / 3.0, TEXT_INPUT_HEIGHT / 2.0);
    _initialSlider.setBounds(ADSR_UI_MARGIN, textInputAreaY + TEXT_INPUT_HEIGHT / 2.0, textInputAreaWidth / 3.0, TEXT_INPUT_HEIGHT / 2.0);
    _peakSlider.setBounds(ADSR_UI_MARGIN + textInputAreaWidth / 3.0, textInputAreaY + TEXT_INPUT_HEIGHT / 2.0, textInputAreaWidth / 3.0, TEXT_INPUT_HEIGHT / 2.0);
    _sustainSlider.setBounds(ADSR_UI_MARGIN + 2.0 * textInputAreaWidth / 3.0, textInputAreaY + TEXT_INPUT_HEIGHT / 2.0, textInputAreaWidth / 3.0, TEXT_INPUT_HEIGHT / 2.0);
    
    auto modSliderX = getWidth() - (MOD_SLIDER_WIDTH);
    _tongueModXSlider.setBounds(modSliderX, 0, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);
    _tongueModYSlider.setBounds(modSliderX, MOD_SLIDER_HEIGHT, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);
    _constrictionModXSlider.setBounds(modSliderX, 2 * MOD_SLIDER_HEIGHT, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);
    _constrictionModYSlider.setBounds(modSliderX, 3 * MOD_SLIDER_HEIGHT, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);
    _pitchModSlider.setBounds(modSliderX, 4 * MOD_SLIDER_HEIGHT, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);

    _tongueModXLabel.setBounds(modSliderX + 0.5 * MOD_SLIDER_WIDTH, 0, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);
    _tongueModYLabel.setBounds(modSliderX + 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);
    _constrictionModXLabel.setBounds(modSliderX + 0.5 * MOD_SLIDER_WIDTH, 2 * MOD_SLIDER_HEIGHT, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);
    _constrictionModYLabel.setBounds(modSliderX + 0.5 * MOD_SLIDER_WIDTH, 3 * MOD_SLIDER_HEIGHT, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);
    _pitchModLabel.setBounds(modSliderX + 0.5 * MOD_SLIDER_WIDTH, 4 * MOD_SLIDER_HEIGHT, 0.5 * MOD_SLIDER_WIDTH, MOD_SLIDER_HEIGHT);
}
