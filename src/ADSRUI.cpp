//
//  ADSRUI.cpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#include "ADSRUI.hpp"
#include <limits>

#define ADSRUI_PADDING      (5.0f)

ADSRUI::ADSRUI(AudioProcessorValueTreeState &instate,
               ParameterIdentifiers identifiers)
    : state(&instate)
    , _identifiers(identifiers)
{
    initial = instate.getParameter(_identifiers.initial)->getValue();
    attackTimeNormalized = instate.getParameter(_identifiers.attack)->getValue();
    decayTimeNormalized = instate.getParameter(_identifiers.decay)->getValue();
    sustainRatio = instate.getParameter(_identifiers.sustain)->getValue();
    releaseTimeNormalized = instate.getParameter(_identifiers.release)->getValue();

    instate.addParameterListener(identifiers.initial, this);
    instate.addParameterListener(identifiers.peak, this);
    instate.addParameterListener(identifiers.attack, this);
    instate.addParameterListener(identifiers.decay, this);
    instate.addParameterListener(identifiers.sustain, this);
    instate.addParameterListener(identifiers.release, this);
    
    repositionHandles();

    auto self = this;
    destructor = [&] {
        instate.removeParameterListener(identifiers.initial, self);
        instate.removeParameterListener(identifiers.peak, self);
        instate.removeParameterListener(identifiers.attack, self);
        instate.removeParameterListener(identifiers.decay, self);
        instate.removeParameterListener(identifiers.sustain, self);
        instate.removeParameterListener(identifiers.release, self);
    };
}

ADSRUI::~ADSRUI()
{
    destructor();
}

void ADSRUI::paint(Graphics &g)
{
    g.fillAll(Colours::black);
    
    Point<float> initialPoint, attackPeak, decayEnd, releaseStart, endPoint;
    int width = getWidth() - 2.0f * ADSRUI_PADDING;
    int height = getHeight() - 2.0f * ADSRUI_PADDING;
    int offset = ADSRUI_PADDING;
    Path p;
    
    initialPoint.setXY(0,  height - initial * height);
    attackPeak.setXY(attackTimeNormalized * width / 3.0, height - peak * height);
    decayEnd.setXY(attackPeak.getX() + decayTimeNormalized * width / 3.0, height - sustainRatio * height);
    releaseStart.setXY(width * 2.0 / 3.0, height - sustainRatio * height);
    endPoint.setXY(width * (2.0 + releaseTimeNormalized) / 3.0, height - end * height);

    initialPoint.addXY(offset, offset);
    attackPeak.addXY(offset, offset);
    decayEnd.addXY(offset, offset);
    releaseStart.addXY(offset, offset);
    endPoint.addXY(offset, offset);
    
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
        
        g.drawRect(handles[i].x - HANDLE_WIDTH / 2, handles[i].y - HANDLE_WIDTH / 2, HANDLE_WIDTH, HANDLE_WIDTH, 2.0);
    }
}

void ADSRUI::resized()
{
    repositionHandles();
    repaint();
}

void ADSRUI::mouseDown(const MouseEvent& e)
{
    activeHandleIndex = highlightedHandleIndex;
    
    changeValuesForEvent(e);
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
    if (parameter == _identifiers.initial) {
        initial = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == _identifiers.peak) {
        peak = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == _identifiers.attack) {
        attackTimeNormalized = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == _identifiers.decay) {
        decayTimeNormalized = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == _identifiers.sustain) {
        sustainRatio = param->convertTo0to1(newValue);
        needsRedraw = true;
    } else if (parameter == _identifiers.release) {
        releaseTimeNormalized = param->convertTo0to1(newValue);
        needsRedraw = true;
    }
    
    if (needsRedraw) {
        repositionHandles();
        repaint();
    }
}


void ADSRUI::changeValuesForEvent(const MouseEvent &e)
{
    float boundedHeight = getHeight() - fmax(0, fmin(getHeight(), e.getPosition().getY()));
    if (activeHandleIndex == 0) {
        initial = (boundedHeight / getHeight());
        state->getParameter(_identifiers.initial)->setValue(initial);
    } else if (activeHandleIndex == 1) {
        float xLimit = getWidth() / 3.0;
        float xBounded = fmax(0, fmin(e.getPosition().getX(), xLimit));
        attackTimeNormalized = xBounded / xLimit;
        peak = (boundedHeight / getHeight());
        state->getParameter(_identifiers.attack)->setValue(attackTimeNormalized);
        state->getParameter(_identifiers.peak)->setValue(peak);
    } else if (activeHandleIndex == 2) {
        float xMin = handles[1].getX();
        float xMax = xMin + getWidth() / 3.0;
        float xBounded = fmin(xMax, fmax(xMin, e.getPosition().getX()));
        decayTimeNormalized = (xBounded - xMin) / (xMax - xMin);
        sustainRatio = (boundedHeight / getHeight());
        state->getParameter(_identifiers.decay)->setValue(decayTimeNormalized);
        state->getParameter(_identifiers.sustain)->setValue(sustainRatio);
    } else if (activeHandleIndex == 3) {
        float xMin = getWidth() * 2.0 / 3.0;
        float xBounded = fmin(getWidth(), fmax(xMin, e.getPosition().getX()));
        releaseTimeNormalized = (xBounded - xMin) / (getWidth() / 3.0);
//        end = ((float) e.getPosition().getY() / getHeight());
        end = 0.0;
        state->getParameter(_identifiers.release)->setValue(releaseTimeNormalized);
    }
    
    repositionHandles();
    repaint();
}

void ADSRUI::repositionHandles()
{
    int width = getWidth() - 2 * ADSRUI_PADDING;
    int height = getHeight() - 2 * ADSRUI_PADDING;
    int offset = ADSRUI_PADDING;
    
    handles[0].setXY(0, height - initial * height);
    handles[1].setXY(attackTimeNormalized * width / 3.0, height - peak * height);
    handles[2].setXY(handles[1].getX() + decayTimeNormalized * width / 3.0, height - sustainRatio * height);
    handles[3].setXY(width * (2.0 + releaseTimeNormalized) / 3.0, height - end * height);

    handles[0].addXY(offset, offset);
    handles[1].addXY(offset, offset);
    handles[2].addXY(offset, offset);
    handles[3].addXY(offset, offset);
    
    repaint();
}
