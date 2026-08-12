#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include <array>
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
    static constexpr int dividerY = 60;
    static constexpr int displayHeight = 48;
    static constexpr int controlsTop = 128;
    static constexpr int columns = 6;
    static constexpr int rows = 2;
    static constexpr int columnGap = 8;
    static constexpr int rowGap = 8;
    static constexpr int labelHeight = 16;
    static constexpr int labelGap = 4;
    static constexpr int valueWidth = 72;
    static constexpr int valueHeight = 20;
    static constexpr int commandHeight = 40;
    static constexpr int shortLogoViewBoxWidth = 512;
    static constexpr int shortLogoViewBoxHeight = 192;
    static constexpr int headerLogoX = 528;
    static constexpr int headerLogoY = 12;
    static constexpr int headerLogoWidth = 96;
    static constexpr int headerLogoHeight = 36;
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
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPositionProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider&) override;
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;
    void drawComboBox(juce::Graphics&, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonWidth, int buttonHeight,
                      juce::ComboBox&) override;
    void positionComboBoxText(juce::ComboBox&, juce::Label&) override;
    void drawScrollbar(juce::Graphics&, juce::ScrollBar&, int x, int y,
                       int width, int height, bool isScrollbarVertical,
                       int thumbStartPosition, int thumbSize,
                       bool isMouseOver, bool isMouseDown) override;
    juce::Font getLabelFont(juce::Label&) override;
};

void styleLabel(juce::Label&);
void styleSlider(juce::Slider&);
void styleToggle(juce::ToggleButton&);
void styleComboBox(juce::ComboBox&);

const char* shortLogoPathData() noexcept;
const juce::Path& shortLogoPath();
juce::Rectangle<float> headerLogoBounds(juce::Rectangle<int> editorBounds) noexcept;
juce::AffineTransform shortLogoTransform(juce::Rectangle<float> targetBounds) noexcept;
void paintShortLogo(juce::Graphics&, juce::Rectangle<float> targetBounds,
                    juce::Colour colour = Palette::paper());

void paintEditorChrome(juce::Graphics&, juce::Rectangle<int> editorBounds,
                       const juce::String& productName,
                       const juce::String& effectClass);

enum class DisplayKind
{
    delay,
    reverb,
    comb,
    distortion,
    phaser,
    flanger,
    compressor,
    limiter,
    bitcrusher
};

class ParameterDisplay final : public juce::Component
{
public:
    explicit ParameterDisplay(DisplayKind);

    void setValues(std::array<float, 4> normalizedValues);
    const std::array<float, 4>& getValues() const noexcept { return values; }
    DisplayKind getKind() const noexcept { return kind; }
    void paint(juce::Graphics&) override;

private:
    DisplayKind kind;
    std::array<float, 4> values {};
};

juce::Rectangle<int> parameterDisplayArea(juce::Rectangle<int> editorBounds) noexcept;
juce::Rectangle<int> controlArea(juce::Rectangle<int> editorBounds) noexcept;
juce::Rectangle<int> controlCell(juce::Rectangle<int> editorBounds,
                                 std::size_t index) noexcept;
LabelledControlBounds labelledControlBounds(juce::Rectangle<int> cell) noexcept;
void layoutLabelledControl(juce::Label&, juce::Component&,
                           juce::Rectangle<int> cell) noexcept;
} // namespace ehl::juce_design
