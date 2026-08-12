# JUCE EHL Design Module

Reusable JUCE UI primitives for EsionHsrahLatigid audio plug-ins. The module provides the shared monochrome 8-bit palette, compact editor metrics, quantized control rendering, header paint, six-column parameter layout, and truthful parameter-state displays.

## Acknowledgements

Built with [JUCE](https://github.com/juce-framework/JUCE) and the EHL monochrome 8-bit design system.

## Features

- `EHL::JuceDesign` CMake target.
- Four-level monochrome palette.
- 640 × 360 compact editor contract with a 512 × 320 minimum.
- Quantized rotary sliders, toggles, combo boxes, focus and disabled states.
- 40 px command surfaces for deliberate combo/toggle affordance at the 512 × 320 minimum.
- Shared product header and six-column/two-row labelled-control layout.
- Nine distinct `ParameterDisplay` grammars driven only by normalized values supplied by a product editor.
- No images, bundled fonts, fake audio meters, DSP, or product metadata.

## Requirements

- CMake 3.22 or newer.
- C++17.
- JUCE 8.0.15 or a compatible JUCE target exposing `juce::juce_gui_basics`.

## Installation

Add this repository as a submodule:

```bash
git submodule add https://github.com/EsionHsrahLatigid/juce-ehl-design-module.git modules/juce-ehl-design-module
git submodule update --init --recursive
```

After JUCE is available in the parent CMake project:

```cmake
add_subdirectory(modules/juce-ehl-design-module)
target_link_libraries(MyPlugin PRIVATE EHL::JuceDesign)
```

## Minimal API

```cpp
#include <ehl/juce_design/EhlDesign.h>

ehl::juce_design::LookAndFeel lookAndFeel;
ehl::juce_design::ParameterDisplay parameterDisplay(
    ehl::juce_design::DisplayKind::delay);
setLookAndFeel(&lookAndFeel);
ehl::juce_design::styleSlider(mySlider);
ehl::juce_design::paintEditorChrome(g, getLocalBounds(), "MyPlugin", "EFFECT");
parameterDisplay.setBounds(ehl::juce_design::parameterDisplayArea(getLocalBounds()));
parameterDisplay.setValues({ 0.2f, 0.4f, 0.6f, 0.8f });
ehl::juce_design::layoutLabelledControl(label, mySlider,
    ehl::juce_design::controlCell(getLocalBounds(), 0));
```

Reset the editor LookAndFeel pointer to `nullptr` before the owned LookAndFeel is destroyed.

## Development

```bash
cmake -S . -B build -G Ninja -DEHL_JUCE_DESIGN_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Dependencies and licences

| Dependency | Purpose | Licence |
| --- | --- | --- |
| JUCE | GUI components and rendering | AGPLv3 or commercial JUCE licence |

This repository is licensed under the MIT License. JUCE remains governed by its own licence.
