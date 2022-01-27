//
//  TractUI.hpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 9/30/19.
//

#ifndef TractUI_hpp
#define TractUI_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Tract.hpp"

class TractUI : public Component, public Timer {
public:
    TractUI(PinkTromboneAudioProcessor &);
	~TractUI();
	void paint(Graphics &g) override;
    
    void timerCallback() override;
    
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
private:
    void setConstriction(t_tractProps *props, double index, double diameter);
    void setTongue(t_tractProps *props, double index, double diameter);
    double * getEventPosition(t_tractProps *p, double x, double y);
    bool isNearTongue(t_tractProps *p, double index, double diameter);
	void drawTongueControl(Graphics &g, t_tractProps *p);
	void drawTract(Graphics &g, t_tractProps *p);
	void moveTo(Graphics &g, t_tractProps *props, Path &p, double index, double diameter);
	void lineTo(Graphics &g, t_tractProps *props, Path &p, double i, double d);
	void drawCircle(Graphics &g, t_tractProps *props, double i, double d, double radius);
    void drawAmplitudes(Graphics &g, t_tractProps *props);
	
	PinkTromboneAudioProcessor &processor;
	double angleOffset;
	double angleScale;
	double originX, originY;
	double radius;
	double scale;
	double innerTongueControlRadius = 2.05;
	double outerTongueControlRadius = 3.5;
	double fillRatio = 1.0 / 6.0;
    bool isTongue;
};

#endif /* TractUI_hpp */
