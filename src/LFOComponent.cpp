//
//  LFOComponent.cpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 3/25/23.
//

#include "LFOComponent.hpp"
#include "PluginProcessor.h"

#define MOD_SLIDER_WIDTH        (150)
#define MOD_SLIDER_HEIGHT       (24)
#define PARAM_WIDTH             (150)
#define TITLE_HEIGHT            (20)
#define TEXT_INPUT_HEIGHT       (46)

LFOComponent::LFOComponent(AudioProcessorValueTreeState &state)
: _tongueModXSlider(state, PinkTromboneAudioProcessor::lfoModTongueX)
, _tongueModYSlider(state, PinkTromboneAudioProcessor::lfoModTongueY)
, _constrictionModXSlider(state, PinkTromboneAudioProcessor::lfoModConstrictionX)
, _constrictionModYSlider(state, PinkTromboneAudioProcessor::lfoModConstrictionY)
, _pitchModSlider(state, PinkTromboneAudioProcessor::lfoModPitch)
, _rateSliderAttachment(state, PinkTromboneAudioProcessor::lfoRate, _rateSlider)
, _shapePickerAttachment(state, PinkTromboneAudioProcessor::lfoShape, _shapePicker)
, _modePickerAttachment(state, PinkTromboneAudioProcessor::lfoMode, _modePicker)
{   
    _titleLabel.setText("LFO", juce::dontSendNotification);
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
    
    addAndMakeVisible(_tongueModXSlider);
    addAndMakeVisible(_tongueModYSlider);
    addAndMakeVisible(_constrictionModXSlider);
    addAndMakeVisible(_constrictionModYSlider);
    addAndMakeVisible(_pitchModSlider);

    addAndMakeVisible(_rateSlider);
    auto shapeParam = state.getParameter(PinkTromboneAudioProcessor::lfoShape);
    _shapePicker.addItemList(shapeParam->getAllValueStrings(), 1);
    addAndMakeVisible(_shapePicker);
    auto modeParam = state.getParameter(PinkTromboneAudioProcessor::lfoMode);
    _modePicker.addItemList(modeParam->getAllValueStrings(), 1);
    addAndMakeVisible(_modePicker);
}

LFOComponent::~LFOComponent()
{
    
}

void LFOComponent::paint(Graphics &)
{
    
}

void LFOComponent::resized()
{
    _titleLabel.setBounds(0, 0, getWidth(), TITLE_HEIGHT);
    
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

    auto paramX = getWidth() - (MOD_SLIDER_WIDTH + PARAM_WIDTH);
    _rateSlider.setBounds(paramX, 0, PARAM_WIDTH, MOD_SLIDER_HEIGHT);
    _shapePicker.setBounds(paramX, MOD_SLIDER_HEIGHT, PARAM_WIDTH, MOD_SLIDER_HEIGHT);
    _modePicker.setBounds(paramX, 2.0f * MOD_SLIDER_HEIGHT, PARAM_WIDTH, MOD_SLIDER_HEIGHT);
}
