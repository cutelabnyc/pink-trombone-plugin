//
//  TractUI.cpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 9/30/19.
//

#include "TractUI.hpp"

TractUI::TractUI(PinkTromboneAudioProcessor &p):
processor(p),
angleOffset(-0.24),
angleScale(0.64),
originX(0),
originY(0),
radius(100),
scale(20)
{
}

TractUI::~TractUI() { }

void TractUI::paint(Graphics &g)
{
	t_tractProps *props = this->processor.getTractProps();
	this->drawTongueControl(g, props);
	this->drawTract(g, props);
}

void TractUI::drawTongueControl(Graphics &g, t_tractProps *p)
{
	Path path;
	this->originX = getWidth() / 2.0;
	this->originY = getHeight() / 2.0;
	this->radius = getWidth() * this->fillRatio;
	this->scale = this->radius / 5.0;
	PathStrokeType strokeType(this->scale * 0.75);
	double tongueLowerIndexBound = p->bladeStart + 2;
	double tongueUpperIndexBound = p->tipStart - 3;
	double tongueIndexCentre = 0.5 * (tongueLowerIndexBound + tongueUpperIndexBound);
	
	strokeType.setEndStyle(PathStrokeType::EndCapStyle::rounded);
	strokeType.setJointStyle(PathStrokeType::JointStyle::curved);
	
	//outline
	this->moveTo(g, p, path, tongueLowerIndexBound, this->innerTongueControlRadius);
	for (int i = (int) tongueLowerIndexBound + 1; i <= (int) tongueUpperIndexBound; i++) {
		this->lineTo(g, p, path, i, this->innerTongueControlRadius);
	}
	this->lineTo(g, p, path, tongueIndexCentre, outerTongueControlRadius);
	path.closeSubPath();
	g.setColour(Colour::fromString("FFFFEEF5"));
	g.strokePath(path, strokeType);
	g.fillPath(path);
	
	double a = this->innerTongueControlRadius;
	double c = this->outerTongueControlRadius;
	double b = 0.5 * (a + c);
	double r = 3;
	g.setColour(Colours::orchid);
	g.setOpacity(0.3);
	this->drawCircle(g, p, tongueIndexCentre, a, r);
	this->drawCircle(g, p, tongueIndexCentre - 4.25, a, r);
	this->drawCircle(g, p, tongueIndexCentre - 8.5, a, r);
	this->drawCircle(g, p, tongueIndexCentre + 4.25, a, r);
	this->drawCircle(g, p, tongueIndexCentre + 8.5, a, r);
	this->drawCircle(g, p, tongueIndexCentre - 6.1, b, r);
	this->drawCircle(g, p, tongueIndexCentre + 6.1, b, r);
	this->drawCircle(g, p, tongueIndexCentre, b, r);
	this->drawCircle(g, p, tongueIndexCentre, c, r);

	g.setOpacity(1.0);
	
	//circle for tongue position
	double angle = this->angleOffset + p->tongueIndex * this->angleScale * M_PI / (p->lipStart - 1);
	double rad = this->radius - this->scale * (p->tongueDiameter);
	double x = this->originX - rad * cos(angle);
	double y = this->originY - rad * sin(angle);
	double cr = this->scale * 18.0 / 60.0;
	strokeType.setStrokeThickness(this->scale * 4.0 / 60.0);
	g.setColour(Colours::black);
	g.setOpacity(0.7);
	Path cp;
	cp.addEllipse(x, y, cr, cr);
	g.strokePath(cp, strokeType);
	g.setOpacity(0.15);
	g.fillPath(cp);
	g.setOpacity(1.0);
}

void TractUI::drawTract(Graphics &g, t_tractProps *props)
{
	double velum = props->noseDiameter[0];
	double velumAngle = velum * 4.0;
	
	// first draw fill
	Path p;
	PathStrokeType stroke(2);
	g.setColour(Colours::pink);
	this->moveTo(g, props, p, 1, 0);
	for (int i = 1; i < props->n; i++) this->lineTo(g, props, p, i, props->tractDiameter[i]);
	for (int i = props->n - 1; i >= 2; i--) this->lineTo(g, props, p, i, 0);
	p.closeSubPath();
	g.strokePath(p, stroke);
	g.fillPath(p);

	//for nose
	p.clear();
	stroke.setStrokeThickness((this->scale * 2.0 / 60.0));
	this->moveTo(g, props, p, props->noseStart, -props->noseOffset);
	for (int i = 0; i < props->noseLength; i++) {
		this->lineTo(g, props, p, i + props->noseStart, -props->noseOffset - props->noseDiameter[i] * 0.9);
	}
	for (int i = props->noseLength - 1; i >= 1; i--) {
		this->lineTo(g, props, p, i + props->noseStart, -props->noseOffset);
	}
	p.closeSubPath();
	g.fillPath(p);

	//velum
	p.clear();
	stroke.setStrokeThickness((this->scale * 2.0 / 60.0));
	this->moveTo(g, props, p, props->noseStart - 2, 0);
	this->lineTo(g, props, p, props->noseStart, -props->noseOffset);
	this->lineTo(g, props, p, props->noseStart + velumAngle, -props->noseOffset);
	this->lineTo(g, props, p, props->noseStart + velumAngle - 2, 0);
	p.closeSubPath();
	g.strokePath(p, stroke);
	g.fillPath(p);
	
	//white text
//	this.ctx.fillStyle = "white";
//	this.ctx.font="20px Arial";
//	this.ctx.textAlign = "center";
//	this.ctx.globalAlpha = 1.0;
//	this.drawText(Tract.n*0.10, 0.425, "throat");
//	this.drawText(Tract.n*0.71, -1.8, "nasal");
//	this.drawText(Tract.n*0.71, -1.3, "cavity");
//	this.ctx.font="22px Arial";
//	this.drawText(Tract.n*0.6, 0.9, "oral");
//	this.drawText(Tract.n*0.7, 0.9, "cavity");


	// TODO: Actually draw amplitudes, which would be cool
	this->drawAmplitudes(g, props);

	//then draw lines
	p.clear();
	stroke.setStrokeThickness((this->scale * 5.0 / 60.0));
	stroke.setEndStyle(PathStrokeType::EndCapStyle::rounded);
	stroke.setJointStyle(PathStrokeType::JointStyle::curved);
	g.setColour(Colour::fromString("FFC070C6"));
	this->moveTo(g, props, p, 1, props->tractDiameter[0]);
	for (int i = 2; i < props->n; i++) this->lineTo(g, props, p, i, props->tractDiameter[i]);
	this->moveTo(g, props, p, 1, 0);
	for (int i = 2; i < props->noseStart - 2; i++) this->lineTo(g, props, p, i, 0);
	this->moveTo(g, props, p, props->noseStart + velumAngle - 2, 0);
	for (int i = props->noseStart + ceil(velumAngle) - 2; i < props->n; i++) this->lineTo(g, props, p, i, 0);
	g.strokePath(p, stroke);

	//for nose
	p.clear();
	stroke.setStrokeThickness((this->scale * 5.0 / 60.0));
	this->moveTo(g, props, p, props->noseStart, -props->noseOffset);
	for (int i = 1; i < props->noseLength; i++) {
		this->lineTo(g, props, p, i + props->noseStart, -props->noseOffset - props->noseDiameter[i] * 0.9);
	}
	this->moveTo(g, props, p, props->noseStart + velumAngle, -props->noseOffset);
	for (int i = ceil(velumAngle); i < props->noseLength; i++) this->lineTo(g, props, p, i + props->noseStart, -props->noseOffset);
	g.strokePath(p, stroke);

	//velum
	g.setOpacity(fmin(1.0, velum * 5));
	p.clear();
	this->moveTo(g, props, p, props->noseStart - 2, 0);
	this->lineTo(g, props, p, props->noseStart, -props->noseOffset);
	this->moveTo(g, props, p, props->noseStart + velumAngle - 2, 0);
	this->lineTo(g, props, p, props->noseStart + velumAngle, -props->noseOffset);
	g.strokePath(p, stroke);

	// TODO: text?
//	this.ctx.fillStyle = "orchid";
//	this.ctx.font="20px Arial";
//	this.ctx.textAlign = "center";
//	this.ctx.globalAlpha = 0.7;
//	this.drawText(Tract.n*0.95, 0.8+0.8*Tract.diameter[Tract.n-1], " lip");
//
//	this.ctx.globalAlpha=1.0;
//	this.ctx.fillStyle = "black";
//	this.ctx.textAlign = "left";
//	this.ctx.fillText(UI.debugText, 20, 20);
	//this.drawPositions();
}

void TractUI::moveTo(Graphics &g, t_tractProps *props, Path &p, double i, double d)
{
	double angle = this->angleOffset + i * this->angleScale * M_PI / (props->lipStart - 1);
	
	// TODO: Figure out wobble
//	double time = 0.0;
//	double wobble = (Tract.maxAmplitude[Tract.n-1]+Tract.noseMaxAmplitude[Tract.noseLength-1]);
//	wobble *= 0.03 sin(2 * i - 50 * time) * i / (double) props->n;
//	angle += wobble;
	double wobble = 0.0;
	double r = this->radius - this->scale * d + 100 * wobble;
	p.startNewSubPath(this->originX - r * cos(angle), this->originY - r * sin(angle));
}

void TractUI::lineTo(Graphics &g, t_tractProps *props, Path &p, double i, double d)
{
	double angle = this->angleOffset + i * this->angleScale * M_PI / (props->lipStart - 1);
	
	// TODO: Figure out wobble
//	double time = 0.0;
//	var wobble = (Tract.maxAmplitude[Tract.n-1]+Tract.noseMaxAmplitude[Tract.noseLength-1]);
//	wobble *= 0.03*Math.sin(2*i-50*time)*i/Tract.n;
//	angle += wobble;
	double wobble = 0.0;
	double r = this->radius - this->scale * d + 100 * wobble;
	p.lineTo(this->originX - r * cos(angle), this->originY - r * sin(angle));
}

void TractUI::drawCircle(Graphics &g, t_tractProps *props, double i, double d, double radius)
{
	Path p;
	double angle = this->angleOffset + i * this->angleScale * M_PI / (props->lipStart - 1);
	double r = this->radius - this->scale * d;
	p.addEllipse(this->originX - r * cos(angle), this->originY - r * sin(angle), radius, radius);
	g.fillPath(p);
}

void TractUI::drawAmplitudes(Graphics &g, t_tractProps *props)
{
    Path p;
    g.setColour(Colours::orchid);
    PathStrokeType stroke(this->scale * 0.75);
    stroke.setEndStyle(PathStrokeType::EndCapStyle::butt);
    g.setOpacity(0.3);
    
    for (int i = 2; i < props->n; i++) {
        stroke.setStrokeThickness(sqrt(props->maxAmplitude[i]) *3);
        
        this->moveTo(g, props, p, i, 0);
        this->lineTo(g, props, p, i, props->tractDiameter[i]);
        g.strokePath(p, stroke);
    }
}
