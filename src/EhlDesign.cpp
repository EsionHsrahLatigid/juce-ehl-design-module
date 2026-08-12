#include <ehl/juce_design/EhlDesign.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>

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

float normalized(float value) noexcept
{
    return std::isfinite(value) ? juce::jlimit(0.0f, 1.0f, value) : 0.0f;
}

int quantized(float value, int steps) noexcept
{
    return juce::jlimit(0, steps, juce::roundToInt(normalized(value) * static_cast<float>(steps)));
}

void fillCell(juce::Graphics& g, int x, int y, juce::Colour colour, int size = 4)
{
    g.setColour(colour);
    g.fillRect(x, y, size, size);
}

const char* displayName(DisplayKind kind) noexcept
{
    switch (kind)
    {
        case DisplayKind::delay: return "DUAL HEAD / SPLICE";
        case DisplayKind::reverb: return "FDN / DIFFUSION";
        case DisplayKind::comb: return "COMB TEETH / COUPLE";
        case DisplayKind::distortion: return "FOLD / TRANSFER";
        case DisplayKind::phaser: return "ALLPASS / NOTCHES";
        case DisplayKind::flanger: return "ZERO CROSS / HEADS";
        case DisplayKind::compressor: return "GAIN COMPUTER";
        case DisplayKind::limiter: return "CEILING / RECOVERY";
        case DisplayKind::bitcrusher: return "QUANTIZE / HOLD";
    }
    return "PARAMETER FIELD";
}

void drawSteppedLine(juce::Graphics& g, juce::Rectangle<int> area,
                     const std::function<float(float)>& valueAt)
{
    g.setColour(Palette::paper());
    const int columns = juce::jmax(1, area.getWidth() / 4);
    int previousY = area.getBottom() - 1;
    for (int column = 0; column < columns; ++column)
    {
        const float unitX = columns > 1 ? static_cast<float>(column) / static_cast<float>(columns - 1) : 0.0f;
        const int x = area.getX() + column * 4;
        const int y = area.getBottom() - 1 - juce::roundToInt(normalized(valueAt(unitX)) * static_cast<float>(area.getHeight() - 1));
        g.fillRect(x, y, juce::jmin(4, area.getRight() - x), 2);
        if (column > 0)
            g.fillRect(x, juce::jmin(previousY, y), 2, std::abs(previousY - y) + 2);
        previousY = y;
    }
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

void LookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                   float proportion, float, float, juce::Slider& slider)
{
    static constexpr std::array<std::array<int, 2>, 16> ring {{
        {{ 0, 4 }}, {{ 0, 3 }}, {{ 0, 2 }}, {{ 0, 1 }}, {{ 0, 0 }},
        {{ 1, 0 }}, {{ 2, 0 }}, {{ 3, 0 }}, {{ 4, 0 }}, {{ 4, 1 }},
        {{ 4, 2 }}, {{ 4, 3 }}, {{ 4, 4 }}, {{ 3, 4 }}, {{ 2, 4 }}, {{ 1, 4 }}
    }};

    const int available = juce::jmax(28, juce::jmin(width, height) - 4);
    const int cell = juce::jlimit(4, 12, gridFloor(available / 7));
    const int side = cell * 7;
    const juce::Rectangle<int> frame { x + (width - side) / 2, y + (height - side) / 2, side, side };
    const bool enabled = slider.isEnabled();
    const bool focused = slider.hasKeyboardFocus(false);

    g.setColour(enabled ? Palette::low() : Palette::ink());
    g.fillRect(frame);
    g.setColour(enabled ? (focused ? Palette::paper() : Palette::mid()) : Palette::low());
    g.drawRect(frame, focused ? 2 : 1);

    const int active = quantized(proportion, static_cast<int>(ring.size()));
    const int block = juce::jmax(2, cell - 2);
    const int originX = frame.getX() + cell;
    const int originY = frame.getY() + cell;
    for (int index = 0; index < static_cast<int>(ring.size()); ++index)
    {
        const auto& point = ring[static_cast<std::size_t>(index)];
        fillCell(g, originX + point[0] * cell + 1, originY + point[1] * cell + 1,
                 index < active ? (enabled ? Palette::paper() : Palette::mid()) : Palette::ink(), block);
    }

    fillCell(g, frame.getCentreX() - block / 2, frame.getCentreY() - block / 2,
             enabled ? (slider.isMouseButtonDown() ? Palette::paper() : Palette::mid()) : Palette::low(), block);
}

void LookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                   bool highlighted, bool down)
{
    auto bounds = button.getLocalBounds().withSizeKeepingCentre(button.getWidth(),
                                                                 juce::jmin(Metrics::commandHeight, button.getHeight()));
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
                      .withSizeKeepingCentre(width, juce::jmin(Metrics::commandHeight, height));
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
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, Metrics::valueWidth, Metrics::valueHeight);
    slider.setColour(juce::Slider::backgroundColourId, Palette::low());
    slider.setColour(juce::Slider::trackColourId, Palette::mid());
    slider.setColour(juce::Slider::thumbColourId, Palette::paper());
    slider.setColour(juce::Slider::textBoxTextColourId, Palette::paper());
    slider.setColour(juce::Slider::textBoxBackgroundColourId, Palette::ink());
    slider.setColour(juce::Slider::textBoxOutlineColourId, Palette::mid());
    slider.setWantsKeyboardFocus(true);
    slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
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
    g.fillRect(bounds.getX(), bounds.getY(), bounds.getWidth(), 4);
    g.setColour(Palette::paper());
    g.setFont(juce::FontOptions(18.0f, juce::Font::bold));
    g.drawText(productName, bounds.getX() + Metrics::margin, bounds.getY() + 10,
               bounds.getWidth() - Metrics::margin * 2, 24,
               juce::Justification::centredLeft, true);
    g.setColour(Palette::mid());
    g.setFont(juce::FontOptions(10.0f));
    g.drawText(effectClass, bounds.getX() + Metrics::margin, bounds.getY() + 36,
               bounds.getWidth() - Metrics::margin * 2, 12,
               juce::Justification::centredLeft, true);
    g.drawText("ESIONHSRAHLATIGID", bounds.getX() + Metrics::margin, bounds.getY() + 24,
               bounds.getWidth() - Metrics::margin * 2, 16,
               juce::Justification::centredRight, true);
    g.setColour(Palette::low());
    g.fillRect(bounds.getX() + Metrics::margin, bounds.getY() + Metrics::dividerY,
               juce::jmax(0, bounds.getWidth() - Metrics::margin * 2), 1);
}

ParameterDisplay::ParameterDisplay(DisplayKind displayKind)
    : kind(displayKind)
{
    setInterceptsMouseClicks(false, false);
    setWantsKeyboardFocus(false);
    setName("Parameter field");
    setTitle("Parameter field");
    setDescription("A quantized display of the plug-in's current parameter state.");
}

void ParameterDisplay::setValues(std::array<float, 4> normalizedValues)
{
    for (auto& value : normalizedValues)
        value = normalized(value);
    if (values == normalizedValues)
        return;
    values = normalizedValues;
    repaint();
}

void ParameterDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    g.setColour(Palette::low());
    g.fillRect(bounds);
    g.setColour(Palette::paper());
    g.fillRect(bounds.getX(), bounds.getBottom() - 4, bounds.getWidth(), 4);

    auto labelArea = bounds.removeFromLeft(132).reduced(8, 6);
    g.setColour(Palette::paper());
    g.setFont(juce::FontOptions(10.0f, juce::Font::bold));
    g.drawText(displayName(kind), labelArea.removeFromTop(16), juce::Justification::centredLeft, true);
    g.setColour(Palette::mid());
    g.setFont(juce::FontOptions(9.0f));
    g.drawText("PARAMETER FIELD", labelArea.removeFromTop(14), juce::Justification::centredLeft, true);

    auto graph = bounds.reduced(8, 6);
    graph.removeFromBottom(4);
    g.setColour(Palette::ink());
    g.fillRect(graph);
    g.setColour(Palette::mid());
    g.fillRect(graph.getX(), graph.getCentreY(), graph.getWidth(), 1);
    for (int x = graph.getX(); x < graph.getRight(); x += 16)
        g.fillRect(x, graph.getY(), 1, graph.getHeight());

    const auto a = values[0];
    const auto b = values[1];
    const auto c = values[2];
    const auto d = values[3];
    switch (kind)
    {
        case DisplayKind::delay:
        {
            const int headA = graph.getX() + quantized(a, graph.getWidth() - 4);
            const int headB = graph.getX() + quantized(b, graph.getWidth() - 4);
            g.setColour(Palette::paper());
            g.fillRect(headA, graph.getY(), 4, graph.getHeight());
            g.setColour(Palette::mid());
            g.fillRect(headB, graph.getY() + 6, 4, graph.getHeight() - 12);
            for (int x = graph.getX(); x < graph.getRight(); x += juce::jmax(8, 36 - quantized(c, 28)))
                fillCell(g, x, graph.getBottom() - 6, d > 0.5f ? Palette::paper() : Palette::ink(), 4);
            break;
        }
        case DisplayKind::reverb:
        {
            for (int index = 0; index < 8; ++index)
            {
                const int x = graph.getX() + index * graph.getWidth() / 8 + 2;
                const float shaped = normalized((0.25f + a * 0.75f) * (0.45f + b * 0.55f)
                                                * (1.0f - static_cast<float>(index) * c * 0.055f));
                const int height = 4 + quantized(shaped, graph.getHeight() - 4);
                g.setColour(index % 2 == 0 ? Palette::paper() : Palette::mid());
                g.fillRect(x, graph.getBottom() - height, juce::jmax(4, graph.getWidth() / 12), height);
            }
            if (d > 0.5f)
            {
                g.setColour(Palette::ink());
                g.fillRect(graph.getRight() - 20, graph.getY(), 20, 8);
            }
            break;
        }
        case DisplayKind::comb:
        {
            const int teeth = 4 + quantized(a, 12);
            for (int index = 0; index < teeth; ++index)
            {
                const int x = graph.getX() + index * graph.getWidth() / teeth;
                const int height = 6 + quantized((index % 2 == 0 ? b : c), graph.getHeight() - 6);
                g.setColour(index % 2 == 0 ? Palette::paper() : Palette::mid());
                g.fillRect(x, graph.getCentreY() - height / 2, 2 + quantized(d, 4), height);
            }
            break;
        }
        case DisplayKind::distortion:
            drawSteppedLine(g, graph, [=](float x)
            {
                const float bipolar = x * 2.0f - 1.0f + (c - 0.5f) * 0.6f;
                const float folded = std::sin(bipolar * (1.0f + a * 6.0f) * juce::MathConstants<float>::halfPi);
                return 0.5f + 0.5f * juce::jlimit(-1.0f, 1.0f, folded * (0.3f + b * 0.7f) + (d - 0.5f) * bipolar);
            });
            break;
        case DisplayKind::phaser:
        {
            const int notches = 3 + quantized(a, 9);
            g.setColour(Palette::paper());
            for (int index = 0; index < notches; ++index)
            {
                const float phase = static_cast<float>(index + 1) / static_cast<float>(notches + 1);
                const int x = graph.getX() + juce::roundToInt((phase + (b - 0.5f) * 0.08f * (index % 2 ? 1.0f : -1.0f))
                                                              * static_cast<float>(graph.getWidth()));
                const int depth = 6 + quantized(c * (0.5f + d * 0.5f), graph.getHeight() - 6);
                g.fillRect(x, graph.getCentreY(), 3, juce::jmin(depth, graph.getBottom() - graph.getCentreY()));
            }
            break;
        }
        case DisplayKind::flanger:
        {
            const int zero = graph.getCentreX();
            g.setColour(Palette::ink());
            g.fillRect(zero - 1, graph.getY(), 2, graph.getHeight());
            const int spread = 8 + quantized(b, graph.getWidth() / 3);
            const int centre = graph.getX() + quantized(a, graph.getWidth());
            g.setColour(Palette::paper());
            g.fillRect(juce::jlimit(graph.getX(), graph.getRight() - 3, centre - spread / 2), graph.getY(), 3, graph.getHeight());
            g.setColour(Palette::mid());
            g.fillRect(juce::jlimit(graph.getX(), graph.getRight() - 3, centre + spread / 2), graph.getY() + 4, 3, graph.getHeight() - 8);
            for (int index = 0; index < quantized(c * d, 8); ++index)
                fillCell(g, graph.getRight() - 4 - index * 8, graph.getBottom() - 6, Palette::paper());
            break;
        }
        case DisplayKind::compressor:
            drawSteppedLine(g, graph, [=](float x)
            {
                const float threshold = 0.15f + a * 0.65f;
                if (x <= threshold)
                    return x;
                return threshold + (x - threshold) * (1.0f - b * 0.85f) * (0.7f + c * 0.3f) + d * 0.04f;
            });
            break;
        case DisplayKind::limiter:
        {
            const int ceilingY = graph.getBottom() - 1 - quantized(a, graph.getHeight() - 1);
            g.setColour(Palette::paper());
            g.fillRect(graph.getX(), ceilingY, graph.getWidth(), 3);
            const int blocks = 4 + quantized(b, 20);
            for (int index = 0; index < blocks; ++index)
            {
                const int x = graph.getX() + index * graph.getWidth() / blocks;
                const int height = 4 + quantized((index % 3 == 0 ? c : d), graph.getHeight() - 4);
                g.setColour(index % 3 == 0 ? Palette::paper() : Palette::mid());
                g.fillRect(x, graph.getBottom() - height, juce::jmax(2, graph.getWidth() / blocks - 2),
                           juce::jmax(1, graph.getBottom() - juce::jmax(ceilingY, graph.getBottom() - height)));
            }
            break;
        }
        case DisplayKind::bitcrusher:
        {
            const int levels = 2 + quantized(a, 10);
            const int holds = 1 + quantized(b, 8);
            g.setColour(Palette::mid());
            for (int level = 0; level <= levels; ++level)
            {
                const int y = graph.getY() + level * graph.getHeight() / levels;
                g.fillRect(graph.getX(), y, graph.getWidth(), 1);
            }
            g.setColour(Palette::paper());
            int previousY = graph.getCentreY();
            for (int x = graph.getX(), step = 0; x < graph.getRight(); x += holds * 4, ++step)
            {
                const float wave = 0.5f + 0.45f * std::sin(static_cast<float>(step) * (0.7f + c * 2.0f));
                const int level = quantized(wave, levels);
                const int y = graph.getY() + level * graph.getHeight() / levels;
                g.fillRect(x, y, juce::jmin(holds * 4, graph.getRight() - x), 2);
                g.fillRect(x, juce::jmin(previousY, y), 2, std::abs(previousY - y) + 2);
                previousY = y;
            }
            for (int index = 0; index < quantized(d, 6); ++index)
                fillCell(g, graph.getRight() - 4 - index * 8, graph.getY() + 2, Palette::ink());
            break;
        }
    }
}

juce::Rectangle<int> parameterDisplayArea(juce::Rectangle<int> bounds) noexcept
{
    return { bounds.getX() + Metrics::margin,
             bounds.getY() + Metrics::headerHeight,
             juce::jmax(0, bounds.getWidth() - Metrics::margin * 2),
             Metrics::displayHeight };
}

juce::Rectangle<int> controlArea(juce::Rectangle<int> bounds) noexcept
{
    return { bounds.getX() + Metrics::margin,
             bounds.getY() + Metrics::controlsTop,
             juce::jmax(0, bounds.getWidth() - Metrics::margin * 2),
             juce::jmax(0, bounds.getHeight() - Metrics::controlsTop - Metrics::margin) };
}

juce::Rectangle<int> controlCell(juce::Rectangle<int> bounds, std::size_t index) noexcept
{
    auto area = controlArea(bounds);
    const int columnWidth = gridFloor((area.getWidth() - Metrics::columnGap * (Metrics::columns - 1)) / Metrics::columns);
    const int availableRowHeight = (area.getHeight() - Metrics::rowGap * (Metrics::rows - 1)) / Metrics::rows;
    const int rowHeight = juce::jmax(0, gridFloor(availableRowHeight));
    const int rowWidth = columnWidth * Metrics::columns + Metrics::columnGap * (Metrics::columns - 1);
    const int rowOffset = gridFloor(juce::jmax(0, area.getWidth() - rowWidth) / 2);
    const int column = static_cast<int>(index) % Metrics::columns;
    const int row = static_cast<int>(index) / Metrics::columns;
    if (column < 0 || column >= Metrics::columns)
        return {};
    return { area.getX() + rowOffset + column * (columnWidth + Metrics::columnGap),
             area.getY() + row * (rowHeight + Metrics::rowGap),
             columnWidth,
             rowHeight };
}

LabelledControlBounds labelledControlBounds(juce::Rectangle<int> cell) noexcept
{
    auto label = cell.removeFromTop(Metrics::labelHeight);
    cell.removeFromTop(Metrics::labelGap);
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
