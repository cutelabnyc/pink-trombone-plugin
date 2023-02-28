//
//  ADSRUI.cpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#include "ADSRUI.hpp"
#include <limits>

ADSRUI::ADSRUI(AudioProcessorValueTreeState &instate,
               ParameterIdentifiers identifiers)
    : state(&instate)
    , initialParam(instate.getParameter(identifiers.initial))
    , peakParam(instate.getParameter(identifiers.peak))
    , attackParam(instate.getParameter(identifiers.attack))
    , decayParam(instate.getParameter(identifiers.decay))
    , sustainParam(instate.getParameter(identifiers.sustain))
    , releaseParam(instate.getParameter(identifiers.release))
{
    initial = initialParam->getValue();
    attackTimeNormalized = attackParam->getValue();
    decayTimeNormalized = decayParam->getValue();
    sustainRatio = sustainParam->getValue();
    releaseTimeNormalized = releaseParam->getValue();
    
    instate.addParameterListener(initialParam->paramID, this);
    instate.addParameterListener(peakParam->paramID, this);
    instate.addParameterListener(attackParam->paramID, this);
    instate.addParameterListener(decayParam->paramID, this);
    instate.addParameterListener(sustainParam->paramID, this);
    instate.addParameterListener(releaseParam->paramID, this);
    
    repositionHandles();
}

ADSRUI::~ADSRUI()
{
    state->removeParameterListener(initialParam->paramID, this);
    state->removeParameterListener(peakParam->paramID, this);
    state->removeParameterListener(attackParam->paramID, this);
    state->removeParameterListener(decayParam->paramID, this);
    state->removeParameterListener(sustainParam->paramID, this);
    state->removeParameterListener(releaseParam->paramID, this);
}

void ADSRUI::paint(Graphics &g)
{
    g.fillAll(Colours::black);
    
    Point<float> initialPoint, attackPeak, decayEnd, releaseStart, endPoint;
    int width = getWidth();
    int height = getHeight();
    Path p;
    
    initialPoint.setXY(0,  height - initial * height);
    attackPeak.setXY(attackTimeNormalized * width / 3.0, height - peak * height);
    decayEnd.setXY(attackPeak.getX() + decayTimeNormalized * width / 3.0, height - sustainRatio * height);
    releaseStart.setXY(width * 2.0 / 3.0, height - sustainRatio * height);
    endPoint.setXY(width * (2.0 + releaseTimeNormalized) / 3.0, height - end * height);
    
    p.startNewSubPath(initialPoint);
    p.lineTo(attackPeak);
    p.quadraticTo(
                  attackPeak.getX(),
                  decayEnd.getY(),
                  decayEnd.getX(),
                  decayEnd.getY());
//    p.lineTo(decayEnd);
    p.lineTo(releaseStart);
    p.quadraticTo(
                  releaseStart.getX(),
                  endPoint.getY(),
                  endPoint.getX(),
                  endPoint.getY());
//    p.lineTo(endPoint);
    
    g.setColour(Colours::yellowgreen);
    g.strokePath(p, PathStrokeType(2.0));
    
    for (int i = 0; i < HANDLES_COUNT; i++) {
        if (i == highlightedHandleIndex) {
            g.setColour(Colours::white);
        } else {
            g.setColour(Colours::yellow);
        }
        
        g.drawRect(handles[i].x - HANDLE_WIDTH / 2, handles[i].y - HANDLE_WIDTH / 2, HANDLE_WIDTH, HANDLE_WIDTH);
    }
}

void ADSRUI::mouseDown(const MouseEvent& e)
{
    activeHandleIndex = highlightedHandleIndex;
    
    changeValuesForEvent(e);
    
    repaint();
}

void ADSRUI::mouseDrag(const MouseEvent& e)
{
    changeValuesForEvent(e);
}

void ADSRUI::mouseMove(const MouseEvent& e)
{
    Point<int> p = e.getPosition();
    int closestIndex = -1;
    float distance = __FLT_MAX__;
    
    for (int i = 0; i < HANDLES_COUNT; i++) {
        float d = p.getDistanceFrom(handles[i]);
        if (d <= distance) {
            distance = d;
            closestIndex = i;
        }
    }
    
    highlightedHandleIndex = closestIndex;
    
    repaint();
}

void ADSRUI::mouseExit(const MouseEvent&)
{
    highlightedHandleIndex = -1;
    
    repaint();
}

void ADSRUI::mouseUp(const MouseEvent& e)
{
    activeHandleIndex = -1;
    
    repaint();
}

void ADSRUI::parameterChanged (const juce::String& parameter, float newValue)
{
    auto param = state->getParameter(parameter);
    bool needsRedraw = false;
    if (parameter == initialParam->paramID) {
        initial = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == peakParam->paramID) {
        peak = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == attackParam->paramID) {
        attackTimeNormalized = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == decayParam->paramID) {
        decayTimeNormalized = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == sustainParam->paramID) {
        sustainRatio = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == releaseParam->paramID) {
        releaseTimeNormalized = param->convertTo0to1(newValue);
        needsRedraw = true;
    }
    
    if (needsRedraw) repaint();
}


void ADSRUI::changeValuesForEvent(const MouseEvent &e)
{
    float boundedHeight = getHeight() - fmax(0, fmin(getHeight(), e.getPosition().getY()));
    if (activeHandleIndex == 0) {
        initial = (boundedHeight / getHeight());
        initialParam->setValue(initial);
    } else if (activeHandleIndex == 1) {
        float xLimit = getWidth() / 3.0;
        float xBounded = fmax(0, fmin(e.getPosition().getX(), xLimit));
        attackTimeNormalized = xBounded / xLimit;
        peak = (boundedHeight / getHeight());
        attackParam->setValue(attackTimeNormalized);
        peakParam->setValue(peak);
    } else if (activeHandleIndex == 2) {
        float xMin = handles[1].getX();
        float xMax = xMin + getWidth() / 3.0;
        float xBounded = fmin(xMax, fmax(xMin, e.getPosition().getX()));
        decayTimeNormalized = (xBounded - xMin) / (xMax - xMin);
        sustainRatio = (boundedHeight / getHeight());
        decayParam->setValue(decayTimeNormalized);
        sustainParam->setValue(sustainRatio);
    } else if (activeHandleIndex == 3) {
        float xMin = getWidth() * 2.0 / 3.0;
        float xBounded = fmin(getWidth(), fmax(xMin, e.getPosition().getX()));
        releaseTimeNormalized = (xBounded - xMin) / (getWidth() / 3.0);
//        end = ((float) e.getPosition().getY() / getHeight());
        end = 0.0;
        releaseParam->setValue(releaseTimeNormalized);
    }
    
    repositionHandles();
    repaint();
}

void ADSRUI::repositionHandles()
{
    int width = getWidth();
    int height = getHeight();
    
    handles[0].setXY(0, height - initial * height);
    handles[1].setXY(attackTimeNormalized * width / 3.0, height - peak * height);
    handles[2].setXY(handles[1].getX() + decayTimeNormalized * width / 3.0, height - sustainRatio * height);
    handles[3].setXY(width * (2.0 + releaseTimeNormalized) / 3.0, height - end * height);
    
    repaint();
}
