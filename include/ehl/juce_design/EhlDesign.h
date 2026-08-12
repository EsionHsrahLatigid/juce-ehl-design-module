#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include <cstddef>

namespace ehl::juce_design
{
struct Palette final
{
    static juce::Colour ink() noexcept;
    static juce::Colour low() noexcept;
    static juce::Colour mid() noexcept;
    static juce::Colour paper() noexcept;
};

struct Metrics final
{
    static constexpr int defaultWidth = 640;
    static constexpr int defaultHeight = 360;
    static constexpr int minimumWidth = 512;
    static constexpr int minimumHeight = 320;
    static constexpr int maximumWidth = 1280;
    static constexpr int maximumHeight = 720;
    static constexpr int margin = 16;
    static constexpr int headerHeight = 64;
    static constexpr int dividerY = 56;
    static constexpr int columns = 2;
    static constexpr int rows = 6;
    static constexpr int columnGap = 16;
    static constexpr int rowGap = 8;
    static constexpr int labelWidth = 88;
    static constexpr int labelGap = 4;
    static constexpr int valueWidth = 72;
    static constexpr int valueHeight = 24;
};

struct LabelledControlBounds final
{
    juce::Rectangle<int> label;
    juce::Rectangle<int> control;
};

class LookAndFeel final : public juce::LookAndFeel_V4
{
public:
    LookAndFeel();

    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPosition, float minimumSliderPosition,
                          float maximumSliderPosition, juce::Slider::SliderStyle,
                          juce::Slider&) override;
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;
    void drawComboBox(juce::Graphics&, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonWidth, int buttonHeight,
                      juce::ComboBox&) override;
    void positionComboBoxText(juce::ComboBox&, juce::Label&) override;
    juce::Font getLabelFont(juce::Label&) override;
};

void styleLabel(juce::Label&);
void styleSlider(juce::Slider&);
void styleToggle(juce::ToggleButton&);
void styleComboBox(juce::ComboBox&);

void paintEditorChrome(juce::Graphics&, juce::Rectangle<int> editorBounds,
                       const juce::String& productName,
                       const juce::String& effectClass);

juce::Rectangle<int> controlArea(juce::Rectangle<int> editorBounds) noexcept;
juce::Rectangle<int> controlCell(juce::Rectangle<int> editorBounds,
                                 std::size_t index) noexcept;
LabelledControlBounds labelledControlBounds(juce::Rectangle<int> cell) noexcept;
void layoutLabelledControl(juce::Label&, juce::Component&,
                           juce::Rectangle<int> cell) noexcept;
} // namespace ehl::juce_design
