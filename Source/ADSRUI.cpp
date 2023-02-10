//
//  ADSRUI.cpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#include "ADSRUI.hpp"

void ADSRUI::paint(Graphics &g)
{
    g.fillAll(Colours::rebeccapurple);
    
    Point<float> initialPoint, attackPeak, decayEnd, releaseStart, endPoint;
    int width = getWidth();
    int height = getHeight();
    Path p;
    
    initialPoint.setXY(0,  initial * height);
    attackPeak.setXY(attackTimeNormalized * width / 3.0, peak * height);
    decayEnd.setXY(attackPeak.getX() + decayTimeNormalized * width / 3.0, sustainRatio * height);
    releaseStart.setXY(width * 2.0 / 3.0, sustainRatio * height);
    endPoint.setXY(width, end * height);
    
    p.startNewSubPath(initialPoint);
    p.lineTo(attackPeak);
    p.lineTo(decayEnd);
    p.lineTo(releaseStart);
    p.lineTo(endPoint);
    
    g.setColour(Colours::yellowgreen);
    g.strokePath(p, PathStrokeType(2.0));
}

void ADSRUI::mouseDown(const MouseEvent& e)
{
    
}

void ADSRUI::mouseDrag(const MouseEvent& e)
{
    
}

void ADSRUI::mouseMove(const MouseEvent& e)
{
    
}

void ADSRUI::mouseUp(const MouseEvent& e)
{
    
}
