#include <ehl/juce_design/EhlDesign.h>

#include <algorithm>
#include <cmath>

namespace ehl::juce_design
{
namespace
{
constexpr juce::uint32 inkValue = 0xff050505;
constexpr juce::uint32 lowValue = 0xff2a2a2a;
constexpr juce::uint32 midValue = 0xff8a8a86;
constexpr juce::uint32 paperValue = 0xfff2f2f0;

juce::Colour structureColour(const juce::Component& component)
{
    if (! component.isEnabled())
        return Palette::low();
    if (component.hasKeyboardFocus(false))
        return Palette::paper();
    return Palette::mid();
}

int gridFloor(int value) noexcept
{
    return value - value % 4;
}
} // namespace

juce::Colour Palette::ink() noexcept { return juce::Colour(inkValue); }
juce::Colour Palette::low() noexcept { return juce::Colour(lowValue); }
juce::Colour Palette::mid() noexcept { return juce::Colour(midValue); }
juce::Colour Palette::paper() noexcept { return juce::Colour(paperValue); }

LookAndFeel::LookAndFeel()
{
    setColour(juce::Label::textColourId, Palette::paper());
    setColour(juce::Label::backgroundColourId, Palette::ink());
    setColour(juce::Label::outlineColourId, Palette::ink());
    setColour(juce::Slider::backgroundColourId, Palette::low());
    setColour(juce::Slider::trackColourId, Palette::mid());
    setColour(juce::Slider::thumbColourId, Palette::paper());
    setColour(juce::Slider::textBoxTextColourId, Palette::paper());
    setColour(juce::Slider::textBoxBackgroundColourId, Palette::ink());
    setColour(juce::Slider::textBoxOutlineColourId, Palette::mid());
    setColour(juce::ToggleButton::textColourId, Palette::paper());
    setColour(juce::ToggleButton::tickColourId, Palette::paper());
    setColour(juce::ToggleButton::tickDisabledColourId, Palette::mid());
    setColour(juce::ComboBox::backgroundColourId, Palette::ink());
    setColour(juce::ComboBox::textColourId, Palette::paper());
    setColour(juce::ComboBox::outlineColourId, Palette::mid());
    setColour(juce::ComboBox::arrowColourId, Palette::paper());
    setColour(juce::PopupMenu::backgroundColourId, Palette::ink());
    setColour(juce::PopupMenu::textColourId, Palette::paper());
    setColour(juce::PopupMenu::highlightedBackgroundColourId, Palette::paper());
    setColour(juce::PopupMenu::highlightedTextColourId, Palette::ink());
}

void LookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                   float sliderPosition, float minimumSliderPosition,
                                   float maximumSliderPosition,
                                   juce::Slider::SliderStyle style, juce::Slider& slider)
{
    if (style != juce::Slider::LinearHorizontal && style != juce::Slider::LinearBar)
    {
        LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPosition,
                                         minimumSliderPosition, maximumSliderPosition,
                                         style, slider);
        return;
    }

    const int trackY = y + (height - 4) / 2;
    const int sliderX = juce::jlimit(x, x + width, juce::roundToInt(sliderPosition));
    g.setColour(slider.isEnabled() ? Palette::low() : Palette::ink());
    g.fillRect(x, trackY, width, 4);
    g.setColour(slider.isEnabled() ? (slider.hasKeyboardFocus(false) ? Palette::paper() : Palette::mid()) : Palette::low());
    g.fillRect(x, trackY, juce::jmax(0, sliderX - x), 4);

    const int thumbHeight = juce::jmin(Metrics::valueHeight, height);
    const int thumbY = y + (height - thumbHeight) / 2;
    g.setColour(slider.isEnabled() ? Palette::paper() : Palette::mid());
    g.fillRect(sliderX - 4, thumbY, 8, thumbHeight);

    if (slider.hasKeyboardFocus(false))
    {
        g.setColour(Palette::paper());
        g.drawRect(x, y, width, height, 1);
    }
}

void LookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                   bool highlighted, bool down)
{
    auto bounds = button.getLocalBounds().withSizeKeepingCentre(button.getWidth(),
                                                                 juce::jmin(Metrics::valueHeight, button.getHeight()));
    const bool active = button.getToggleState();
    const bool enabled = button.isEnabled();
    const auto fill = ! enabled ? Palette::low() : (active || down ? Palette::paper() : Palette::ink());
    const auto text = ! enabled ? Palette::mid() : (active || down ? Palette::ink() : Palette::paper());

    g.setColour(fill);
    g.fillRect(bounds);
    g.setColour(button.hasKeyboardFocus(false) ? Palette::paper() : (highlighted ? Palette::paper() : Palette::mid()));
    g.drawRect(bounds, 1);
    g.setColour(text);
    g.setFont(juce::FontOptions(11.0f));
    g.drawText(button.getButtonText(), bounds.reduced(8, 0), juce::Justification::centredLeft, true);
}

void LookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                               int, int, int, int, juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<int>(0, 0, width, height)
                      .withSizeKeepingCentre(width, juce::jmin(28, height));
    const bool inverse = isButtonDown && box.isEnabled();
    g.setColour(! box.isEnabled() ? Palette::low() : (inverse ? Palette::paper() : Palette::ink()));
    g.fillRect(bounds);
    g.setColour(structureColour(box));
    g.drawRect(bounds, 1);

    const int arrowX = bounds.getRight() - 16;
    const int arrowY = bounds.getCentreY() - 2;
    g.setColour(! box.isEnabled() ? Palette::mid() : (inverse ? Palette::ink() : Palette::paper()));
    g.fillRect(arrowX, arrowY, 8, 2);
    g.fillRect(arrowX + 2, arrowY + 2, 4, 2);
    g.fillRect(arrowX + 3, arrowY + 4, 2, 2);
}

void LookAndFeel::positionComboBoxText(juce::ComboBox& box, juce::Label& label)
{
    label.setBounds(8, 0, box.getWidth() - 32, box.getHeight());
    label.setFont(juce::FontOptions(11.0f));
    label.setJustificationType(juce::Justification::centredLeft);
}

juce::Font LookAndFeel::getLabelFont(juce::Label&)
{
    return juce::Font(juce::FontOptions(11.0f));
}

void styleLabel(juce::Label& label)
{
    label.setColour(juce::Label::textColourId, Palette::paper());
    label.setColour(juce::Label::backgroundColourId, Palette::ink());
    label.setColour(juce::Label::outlineColourId, Palette::ink());
    label.setFont(juce::FontOptions(11.0f));
    label.setJustificationType(juce::Justification::centredLeft);
    label.setMinimumHorizontalScale(0.75f);
}

void styleSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, Metrics::valueWidth, Metrics::valueHeight);
    slider.setColour(juce::Slider::backgroundColourId, Palette::low());
    slider.setColour(juce::Slider::trackColourId, Palette::mid());
    slider.setColour(juce::Slider::thumbColourId, Palette::paper());
    slider.setColour(juce::Slider::textBoxTextColourId, Palette::paper());
    slider.setColour(juce::Slider::textBoxBackgroundColourId, Palette::ink());
    slider.setColour(juce::Slider::textBoxOutlineColourId, Palette::mid());
    slider.setWantsKeyboardFocus(true);
}

void styleToggle(juce::ToggleButton& button)
{
    button.setColour(juce::ToggleButton::textColourId, Palette::paper());
    button.setColour(juce::ToggleButton::tickColourId, Palette::paper());
    button.setColour(juce::ToggleButton::tickDisabledColourId, Palette::mid());
    button.setClickingTogglesState(true);
    button.setWantsKeyboardFocus(true);
}

void styleComboBox(juce::ComboBox& box)
{
    box.setColour(juce::ComboBox::backgroundColourId, Palette::ink());
    box.setColour(juce::ComboBox::textColourId, Palette::paper());
    box.setColour(juce::ComboBox::outlineColourId, Palette::mid());
    box.setColour(juce::ComboBox::arrowColourId, Palette::paper());
    box.setWantsKeyboardFocus(true);
}

void paintEditorChrome(juce::Graphics& g, juce::Rectangle<int> bounds,
                       const juce::String& productName, const juce::String& effectClass)
{
    g.setColour(Palette::ink());
    g.fillRect(bounds);
    g.setColour(Palette::paper());
    g.setFont(juce::FontOptions(18.0f, juce::Font::bold));
    g.drawText(productName, bounds.getX() + Metrics::margin, bounds.getY() + 8,
               bounds.getWidth() - Metrics::margin * 2, 24,
               juce::Justification::centredLeft, true);
    g.setColour(Palette::mid());
    g.setFont(juce::FontOptions(10.0f));
    g.drawText(effectClass, bounds.getX() + Metrics::margin, bounds.getY() + 32,
               bounds.getWidth() - Metrics::margin * 2, 12,
               juce::Justification::centredLeft, true);
    g.setColour(Palette::low());
    g.drawHorizontalLine(bounds.getY() + Metrics::dividerY,
                         static_cast<float>(bounds.getX() + Metrics::margin),
                         static_cast<float>(bounds.getRight() - Metrics::margin));
}

juce::Rectangle<int> controlArea(juce::Rectangle<int> bounds) noexcept
{
    return { bounds.getX() + Metrics::margin,
             bounds.getY() + Metrics::headerHeight,
             juce::jmax(0, bounds.getWidth() - Metrics::margin * 2),
             juce::jmax(0, bounds.getHeight() - Metrics::headerHeight - Metrics::margin) };
}

juce::Rectangle<int> controlCell(juce::Rectangle<int> bounds, std::size_t index) noexcept
{
    auto area = controlArea(bounds);
    const int columnWidth = gridFloor((area.getWidth() - Metrics::columnGap) / Metrics::columns);
    const int availableRowHeight = (area.getHeight() - Metrics::rowGap * (Metrics::rows - 1)) / Metrics::rows;
    const int rowHeight = juce::jlimit(32, 40, gridFloor(availableRowHeight));
    const int column = static_cast<int>(index) / Metrics::rows;
    const int row = static_cast<int>(index) % Metrics::rows;
    if (column < 0 || column >= Metrics::columns)
        return {};
    return { area.getX() + column * (columnWidth + Metrics::columnGap),
             area.getY() + row * (rowHeight + Metrics::rowGap),
             columnWidth,
             rowHeight };
}

LabelledControlBounds labelledControlBounds(juce::Rectangle<int> cell) noexcept
{
    auto label = cell.removeFromLeft(Metrics::labelWidth);
    cell.removeFromLeft(Metrics::labelGap);
    return { label, cell };
}

void layoutLabelledControl(juce::Label& label, juce::Component& control,
                           juce::Rectangle<int> cell) noexcept
{
    const auto bounds = labelledControlBounds(cell);
    label.setBounds(bounds.label);
    control.setBounds(bounds.control);
}
} // namespace ehl::juce_design
