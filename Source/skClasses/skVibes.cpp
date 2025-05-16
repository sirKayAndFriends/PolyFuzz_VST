#pragma once
#include "JuceHeader.h"
#include "skVibes.h"

//SKVibes::windowParams* SKVibes::aspectRatio(double width, double quotient1, double quotient2)
//{
//    wParams.width = width;
//    wParams.height = (width / quotient1) * quotient2;
//
//    wParams.ratio = quotient2 / quotient1;
//    wParams.unitsNumX = width / 50.0;
//    wParams.unit = width / wParams.unitsNumX;
//    wParams.unitsNumY = wParams.unitsNumX * wParams.ratio;
//
//    return &wParams;
//}

SKVibes::windowParams* SKVibes::aspectRatio(double unitSize, int numX, int numY)
{    
    wParams.width = unitSize * numX;
    wParams.height = unitSize * numY;

    wParams.unitsNumX = numX;
    wParams.unit = unitSize;
    wParams.unitsNumY = numY;

    return &wParams;
}

void SKVibes::setupSKSliders(std::vector<juce::Slider*>& knobs, std::vector<int>& colours, std::vector<juce::Label*>& labels, std::vector<juce::String>& text)
{
    for (int i = 0; i < knobs.size(); i++)
    {
        knobs[i]->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        knobs[i]->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        knobs[i]->setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour(colours[i]));
        knobs[i]->setSize(wParams.unit * 2, wParams.unit * 2);
        knobs[i]->setDoubleClickReturnValue(true, 0.5f);

        labels[i]->setText(text[i], juce::NotificationType::dontSendNotification);
        labels[i]->setFont(SKVibes::getPluginFont().boldened().withHeight(24.0f));
        labels[i]->setJustificationType(juce::Justification::centred);
        labels[i]->setColour(juce::Label::ColourIds::textColourId, juce::Colour(juce::Colours::black));
    }

    knobSize = wParams.unit * 2;
}

void SKVibes::setupSKButtons(std::vector<juce::TextButton*>& buttons)
{
    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(SKVibes::skStrawbYog));
        buttons[i]->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(SKVibes::skBerryPink));
        buttons[i]->setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colour(juce::Colours::black));
        buttons[i]->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colour(juce::Colours::black));
        buttons[i]->setColour(juce::ComboBox::outlineColourId, juce::Colour(SKVibes::skClear));
        buttons[i]->setSize(wParams.unit, wParams.unit);
        buttons[i]->setClickingTogglesState(true);
    }

    buttonSize = wParams.unit;
}


void SKVibes::setupSKTitle(juce::Label* title)
{
    title->setSize(wParams.unit * 6, wParams.unit * 2);
    title->setFont(SKVibes::getPluginFont().boldened().withHeight(38.0f));
    title->setJustificationType(juce::Justification::centred);
    title->setColour(juce::Label::ColourIds::textColourId, juce::Colour(juce::Colours::white));

    titleSize.push_back(title->getWidth());
    titleSize.push_back(title->getHeight());
}

float SKVibes::getKnobSize()
{
    return knobSize;
}

float SKVibes::getButtonSize()
{
    return buttonSize;
}

std::vector<float> SKVibes::getTitleSize()
{
    return titleSize;
}

void SKVibes::createDivisions()
{  
    float startX = 0.0f;
    float startY = 0.0f;

    for (int j = 0; j < wParams.unitsNumY; j++)
    {
        for (int i = 0; i < wParams.unitsNumX; i++)
        {
            gridSquare.push_back(juce::Rectangle<float>(startX, startY, wParams.unit, wParams.unit));
            startX += wParams.unit;
        }
        startX = 0.0;
        startY += wParams.unit;
    }
}

std::vector<juce::Rectangle<float>>& SKVibes::getGrid()
{
    return gridSquare;
}

juce::Point<float>& SKVibes::getGridRectPoint(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX(), gridSquare[index].getY());
}

juce::Point<float>& SKVibes::getGridCentrePoint(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getCentre();
}

juce::Rectangle<float>& SKVibes::getGridRect(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index];
}

juce::Point<float> SKVibes::getGridRectBottomLeft(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getBottomLeft();
}

juce::Point<float> SKVibes::getGridRectBottomRight(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getBottomRight();
}

juce::Point<float> SKVibes::getGridRectTopLeft(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getTopLeft();
}

juce::Point<float> SKVibes::getGridRectTopRight(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getTopRight();
}

float SKVibes::getUnitSize()
{
    return wParams.unit;
}

juce::Point<float> SKVibes::drawOnIntersection(int row, int col, juce::Component* object)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX() - object->getWidth() / 2.0, gridSquare[index].getY()  - object->getHeight() / 2.0);
}

juce::Point<float> SKVibes::drawOnIntersection(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX(), gridSquare[index].getY());
}

juce::Point<float> SKVibes::drawOnCentre(int row, int col, juce::Component* object)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX() - object->getWidth() / 2.0, gridSquare[index].getCentreY() - object->getHeight() / 2.0);
}

juce::Point<float> SKVibes::drawOnCentre(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX(), gridSquare[index].getCentreY());
}

juce::Point<float> SKVibes::drawFromIntersection(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX(), gridSquare[index].getY());
}

juce::Point<float> SKVibes::drawFromCentre(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX(), gridSquare[index].getCentreY());
}

juce::Point<float> SKVibes::drawOnHorizontal(int row, int col, juce::Component* object)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX() - object->getWidth() / 2.0, gridSquare[index].getY() - object->getHeight() / 2.0);
}

juce::Point<float> SKVibes::drawOnHorizontal(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX(), gridSquare[index].getY());
}

juce::Point<float> SKVibes::drawOnVertical(int row, int col, juce::Component* object)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX() - object->getWidth() / 2.0, gridSquare[index].getCentreY() - object->getHeight() / 2.0);
}

juce::Point<float> SKVibes::drawOnVertical(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX(), gridSquare[index].getY());
}

void SKVibes::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
   
    auto radius = (float)juce::jmin(width / 2, height / 2);
    auto knobRect = juce::Rectangle<float>(x, y, width, height);
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);

    juce::Point<float> centre(knobRect.getCentreX(), knobRect.getCentreY());

    g.setColour(fill);
    g.fillEllipse(knobRect);

    juce::Path p;
    auto pointerThickness = 4.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, radius);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centre));

    auto centreDot = juce::Rectangle<float>(pointerThickness, pointerThickness).withCentre(centre);

    g.setColour(juce::Colours::black);
    g.fillPath(p);
    g.fillEllipse(centreDot);

}

SKVibes::panelParams SKVibes::setupPanels(float x, float y, double width, double height, int col)
{
    panelParams params;

    params.width = width;
    params.height = height;
    params.position = juce::Point<float>(x, y);
    params.colour = col;

    return params;
}

SKVibes::doublePanelParams SKVibes::setupDoublePanels(float x, float y, double width, double height, int margin, int outerCol, int innerCol)
{
    doublePanelParams params;

    params.outerColour = outerCol;
    params.innerColour = innerCol;

    params.position = juce::Point<float>(x, y);
    params.width = width;
    params.height = height;
    params.margin = margin;

    return params;
}

void SKVibes::drawPanel(juce::Graphics& g, panelParams* panel)
{
    juce::Rectangle<float> bgPanel(panel->position.x, panel->position.y, panel->width, panel->height);
    g.setColour(juce::Colour(panel->colour));
    g.fillRect(bgPanel);
}

void SKVibes::drawDoublePanel(juce::Graphics& g, doublePanelParams* panel)
{
    float cornerRound = 6.0f;
    juce::Rectangle<float> outerPanel(panel->position.x, panel->position.y, panel->width, panel->height);
    juce::Rectangle<float> innerPanel(panel->position.x + panel->margin, panel->position.y + panel->margin, panel->width - (panel->margin * 2.0f), panel->height - (panel->margin * 2.0f));
    g.setColour(juce::Colour(panel->outerColour));
    g.fillRoundedRectangle(outerPanel, cornerRound);
    g.setColour(juce::Colour(panel->innerColour));
    g.fillRoundedRectangle(innerPanel, cornerRound);
}

void SKVibes::updateDoublePanels(doublePanelParams* doublePanel)
{
    doublePanelParams params;

    params.outerColour = doublePanel->outerColour;
    params.innerColour = doublePanel->innerColour;

    params.position = doublePanel->position;
    params.width = doublePanel->width;
    params.height = doublePanel->height;
}

void SKVibes::updatePanels(panelParams* panel)
{
    panelParams params;

    params.width = panel->width;
    params.height = panel->height;
    params.colour = panel->colour;
    params.position = panel->position;
}

void SKVibes::drawLabels(juce::Slider* knob, juce::String& text)
{
    juce::Label label;
    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setFont(SKVibes::getPluginFont());
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colour(juce::Colours::black));
    juce::Rectangle<int> knobBounds = knob->getBounds();
    /*juce::Point<int> centre(knobBounds.getCentreX(), knobBounds.getCentreY() + knobBounds.getHeight() / 4);
    auto border = juce::Rectangle<int>(width, height).withCentre(centre);*/
    label.setBounds(knobBounds.getX(), knobBounds.getY(), knobBounds.getWidth(), knobBounds.getHeight());
}

SKVibes::visualParams SKVibes::setVisualParams(int x, int y, int width, int height)
{
    visualParams params;

    params.lineStartX = x;
    params.lineY = y;

    params.visualWidth = width;
    params.lineEndX = params.lineStartX + params.visualWidth; //450
    //CONTROL DOT (central point) defines other points
    params.visualCurveWidth = params.visualWidth / 4.0f; //100
    params.controlMin = params.lineStartX + (params.visualCurveWidth / 2); //50 + 50
    params.controlMax = params.lineEndX - (params.visualCurveWidth / 2); //450 - 50  
    params.controlWidth = params.controlMax - params.controlMin;
    //bounds = 100 -> 400 = 300
    params.maxY = params.lineY - height;
    params.humpHeight = params.lineY - params.maxY;
    
    return params;
}

void SKVibes::drawMidVisual(juce::Graphics& g, visualParams* params, float peakX, float peakY)
{
    float lineThickness = 2.5f;

    auto centre = juce::Point<float>(params->lineStartX + (params->visualWidth / 2), params->lineY);
    auto box = juce::Rectangle<float>(params->visualWidth + (lineThickness * 2), params->humpHeight + (lineThickness * 2)).withCentre(centre);

    params->lineStart.setXY(params->lineStartX, params->lineY);//100
    params->lineEnd.setXY(params->lineEndX, params->lineY);//400
    params->curveStart.setXY(peakX - (params->visualCurveWidth / 2), params->lineY);//200
    params->curveEnd.setXY(peakX + (params->visualCurveWidth / 2), params->lineY);//300
    params->controlDot.setXY(peakX, peakY);

    params->curve.clear();
    params->curve.startNewSubPath(params->lineStart);
    params->curve.lineTo(params->curveStart);
    params->curve.quadraticTo(params->controlDot, params->curveEnd);
    params->curve.lineTo(params->lineEnd);

    g.setColour(juce::Colour(juce::Colours::black));
    g.strokePath(params->curve, juce::PathStrokeType(lineThickness));
    g.drawRoundedRectangle(box, 6.0f, lineThickness);
}

void SKVibes::diodeInit(juce::Button* button, float x, float y, int size, int colour, bool direction, bool state, int index)
{
    juce::Point<float> tmpMidPoint(x + size / 2.0, y + size / 2.0);
    diodes[index]->button = button;
    diodes[index]->position = juce::Point<float>(x, y);
    diodes[index]->bounds = juce::Rectangle<float>(x, y, size, size);
    diodes[index]->midPoint = tmpMidPoint;
    
    diodes[index]->triangle = 
    {
        {juce::Point<float>(tmpMidPoint.x, y + size * 0.25)},
        {juce::Point<float>(tmpMidPoint.x - (size * 0.25), y + size - (size / 3.0))},
        {juce::Point<float>(tmpMidPoint.x + (size * 0.25), y + size - (size / 3.0))}
    };

    diodes[index]->wire =
    {
        {juce::Point<float>(tmpMidPoint.x, y)},
        {juce::Point<float>(tmpMidPoint.x, y + size)}
    };

    diodes[index]->cathode =
    {
        {juce::Point<float>(tmpMidPoint.x - (size * 0.25), y + size * 0.25)},
        {juce::Point<float>(tmpMidPoint.x + (size * 0.25), y + size * 0.25)}
    };

    diodes[index]->softColour = colour;
    diodes[index]->direction = direction;
    diodes[index]->unit = size;
    diodes[index]->thickness = 4.0;
    diodes[index]->state = state;

    if (state == true)
    {
        diodes[index]->hardColour = skColours::skTomato;
    }

    else
    {
        diodes[index]->hardColour = skColours::skCocoa;
    }
}

void SKVibes::drawDiode(juce::Graphics& g, int index, bool state)
{
    juce::Path diode;

    if (state == true)
    {
        g.setColour(juce::Colour(diodes[index]->hardColour));
    }

    else
    {
        g.setColour(juce::Colour(diodes[index]->softColour));
    } 

    diode.addTriangle(diodes[index]->triangle[0], diodes[index]->triangle[1], diodes[index]->triangle[2]);
    diode.startNewSubPath(diodes[index]->wire[0]);
    diode.lineTo(diodes[index]->wire[1]);
    diode.startNewSubPath(diodes[index]->cathode[0]);
    diode.lineTo(diodes[index]->cathode[1]);

    if (diodes[index]->direction == true)
    {
        diode.applyTransform(juce::AffineTransform().rotated(juce::MathConstants<float>::pi, diodes[index]->midPoint.x, diodes[index]->midPoint.y));
    }

    g.strokePath(diode, juce::PathStrokeType(diodes[index]->thickness));
    g.fillPath(diode);

    g.setColour(juce::Colour(skColours::skCocoa));
    g.drawEllipse(diodes[index]->bounds, diodes[index]->thickness);
}

void SKVibes::diodeSort(juce::Button* button, bool state)
{
    for (int i = 0; i < diodes.size(); i++)
    {
        if (button == diodes[i]->button)
        {
            if (state == true)
            {
                diodes[i]->hardColour = skTomato;
                break;
            } 
            
            else if(state == false)
            {
                diodes[i]->hardColour = skCocoa;
                break;
            }          
        }
    }
}