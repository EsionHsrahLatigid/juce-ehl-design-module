#include <ehl/juce_design/EhlDesign.h>

#include <juce_events/juce_events.h>

#include <cstdlib>
#include <iostream>

namespace
{
void require(bool condition, const char* message)
{
    if (! condition)
    {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

void checkLayout(juce::Rectangle<int> editor)
{
    for (std::size_t i = 0; i < 12; ++i)
    {
        const auto cell = ehl::juce_design::controlCell(editor, i);
        require(! cell.isEmpty(), "control cell is non-empty");
        require(cell.getX() % 4 == 0 && cell.getY() % 4 == 0, "control cell origin is grid aligned");
        require(cell.getWidth() % 4 == 0 && cell.getHeight() % 4 == 0, "control cell size is grid aligned");
        require(editor.contains(cell), "control cell stays within editor");
        const auto parts = ehl::juce_design::labelledControlBounds(cell);
        require(parts.label.getWidth() == ehl::juce_design::Metrics::labelWidth, "label width is shared");
        require(parts.control.getWidth() >= 140, "control keeps a practical compact width");
    }
}

bool imageContains(const juce::Image& image, juce::Colour colour)
{
    for (int y = 0; y < image.getHeight(); ++y)
        for (int x = 0; x < image.getWidth(); ++x)
            if (image.getPixelAt(x, y) == colour)
                return true;
    return false;
}

void checkDividerPixels(const juce::Image& image, juce::Rectangle<int> bounds)
{
    const int dividerY = bounds.getY() + ehl::juce_design::Metrics::dividerY;
    const int firstX = bounds.getX() + ehl::juce_design::Metrics::margin;
    const int lastX = bounds.getRight() - ehl::juce_design::Metrics::margin - 1;

    require(image.getPixelAt(firstX - 1, dividerY) == ehl::juce_design::Palette::ink(),
            "chrome divider starts at editor margin");
    require(image.getPixelAt(lastX + 1, dividerY) == ehl::juce_design::Palette::ink(),
            "chrome divider ends before trailing editor margin");

    for (int x = firstX; x <= lastX; ++x)
    {
        require(image.getPixelAt(x, dividerY) == ehl::juce_design::Palette::low(),
                "chrome divider row is exact low");
        require(image.getPixelAt(x, dividerY - 1) != ehl::juce_design::Palette::low(),
                "chrome divider does not bleed upward");
        require(image.getPixelAt(x, dividerY + 1) != ehl::juce_design::Palette::low(),
                "chrome divider does not bleed downward");
    }
}

juce::Image renderComponent(juce::Component& component, int width = 200, int height = 32)
{
    component.setBounds(0, 0, width, height);
    juce::Image image(juce::Image::RGB, width, height, true);
    juce::Graphics graphics(image);
    component.paintEntireComponent(graphics, true);
    return image;
}
} // namespace

int main()
{
    juce::ScopedJuceInitialiser_GUI juce;
    using namespace ehl::juce_design;

    require(Palette::ink().getARGB() == 0xff050505, "ink token");
    require(Palette::low().getARGB() == 0xff2a2a2a, "low token");
    require(Palette::mid().getARGB() == 0xff8a8a86, "mid token");
    require(Palette::paper().getARGB() == 0xfff2f2f0, "paper token");

    checkLayout({ 0, 0, Metrics::defaultWidth, Metrics::defaultHeight });
    checkLayout({ 0, 0, Metrics::minimumWidth, Metrics::minimumHeight });

    LookAndFeel lookAndFeel;
    juce::Slider slider;
    slider.setLookAndFeel(&lookAndFeel);
    styleSlider(slider);
    require(slider.getSliderStyle() == juce::Slider::LinearHorizontal, "shared slider style");
    require(slider.getTextBoxPosition() == juce::Slider::TextBoxRight, "shared value placement");
    slider.setRange(0.0, 1.0);
    slider.setValue(0.5);
    auto sliderImage = renderComponent(slider);
    require(imageContains(sliderImage, Palette::low()), "enabled slider renders low track");
    require(imageContains(sliderImage, Palette::mid()), "enabled slider renders mid value");
    require(imageContains(sliderImage, Palette::paper()), "enabled slider renders paper thumb and value");
    slider.setEnabled(false);
    auto disabledSliderImage = renderComponent(slider);
    require(imageContains(disabledSliderImage, Palette::low()), "disabled slider keeps low structure");
    require(imageContains(disabledSliderImage, Palette::mid()), "disabled slider keeps readable mid state");

    juce::ToggleButton toggle("ACTIVE");
    toggle.setLookAndFeel(&lookAndFeel);
    styleToggle(toggle);
    toggle.setToggleState(false, juce::dontSendNotification);
    const auto offToggleImage = renderComponent(toggle, 120, 32);
    require(offToggleImage.getPixelAt(60, 16) == Palette::ink(), "off toggle uses ink fill");
    toggle.setToggleState(true, juce::dontSendNotification);
    const auto onToggleImage = renderComponent(toggle, 120, 32);
    require(onToggleImage.getPixelAt(60, 16) != Palette::ink(), "active toggle inverts away from ink fill");
    require(imageContains(onToggleImage, Palette::paper()), "active toggle uses paper state");
    toggle.setEnabled(false);
    const auto disabledToggleImage = renderComponent(toggle, 120, 32);
    require(imageContains(disabledToggleImage, Palette::low()), "disabled toggle uses low fill");
    require(imageContains(disabledToggleImage, Palette::mid()), "disabled toggle uses mid text");

    juce::ComboBox combo;
    combo.addItem("MODE", 1);
    combo.setSelectedId(1, juce::dontSendNotification);
    combo.setLookAndFeel(&lookAndFeel);
    styleComboBox(combo);
    const auto comboImage = renderComponent(combo, 160, 32);
    require(imageContains(comboImage, Palette::ink()), "combo uses ink fill");
    require(imageContains(comboImage, Palette::mid()), "combo uses mid outline");
    require(imageContains(comboImage, Palette::paper()), "combo uses paper text and arrow");
    combo.setEnabled(false);
    const auto disabledComboImage = renderComponent(combo, 160, 32);
    require(imageContains(disabledComboImage, Palette::low()), "disabled combo uses low fill");
    require(imageContains(disabledComboImage, Palette::mid()), "disabled combo remains readable");

    juce::Image image(juce::Image::RGB, Metrics::defaultWidth, Metrics::defaultHeight, true);
    juce::Graphics graphics(image);
    paintEditorChrome(graphics, image.getBounds(), "Product", "EFFECT");
    require(image.getPixelAt(0, 0) == Palette::ink(), "chrome fills ink background");
    require(image.getPixelAt(Metrics::margin, Metrics::dividerY) == Palette::low(), "chrome draws shared divider");
    checkDividerPixels(image, image.getBounds());
    for (int y = Metrics::headerHeight; y < image.getHeight(); ++y)
        for (int x = 0; x < image.getWidth(); ++x)
            require(image.getPixelAt(x, y) == Palette::ink(), "chrome leaves control field undecorated");

    juce::Image offsetImage(juce::Image::RGB, 384, 224, true);
    juce::Graphics offsetGraphics(offsetImage);
    const auto outside = juce::Colour(0xffff00ff);
    offsetGraphics.fillAll(outside);
    const juce::Rectangle<int> offsetBounds { 24, 12, 320, 180 };
    paintEditorChrome(offsetGraphics, offsetBounds, "Offset", "TEST");
    require(offsetImage.getPixelAt(0, 0) == outside, "offset chrome does not paint outside supplied bounds");
    require(offsetImage.getPixelAt(offsetBounds.getX(), offsetBounds.getY()) == Palette::ink(),
            "offset chrome fills supplied bounds");
    require(offsetImage.getPixelAt(offsetBounds.getX() + Metrics::margin,
                                   offsetBounds.getY() + Metrics::dividerY) == Palette::low(),
            "offset chrome translates the shared divider");
    checkDividerPixels(offsetImage, offsetBounds);

    slider.setLookAndFeel(nullptr);
    toggle.setLookAndFeel(nullptr);
    combo.setLookAndFeel(nullptr);
    std::cout << "ehl_juce_design_tests passed\n";
    return 0;
}
