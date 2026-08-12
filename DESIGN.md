# EHL JUCE Design Contract

This module is the single implementation source for the EHL JUCE plug-in family UI.

- Canvas: 640 × 360 default, 512 × 320 minimum, 1280 × 720 maximum.
- Grid: 4 px base, 8 px row spacing, 16 px major margin and column gap.
- Palette: ink `#050505`, low `#2A2A2A`, mid `#8A8A86`, paper `#F2F2F0`.
- Header: a 4 px system rule, product name, effect class, restrained brand text, and one divider.
- Parameter field: one 48 px product-selected, parameter-driven display. It visualizes normalized parameter state only; it is never presented as live audio telemetry.
- Body: six columns by two rows with labels above quantized rotary controls and clean value readouts.
- States: focus uses a paper outline, active toggles invert, disabled controls use low/mid.
- Product display grammars: delay heads, reverb FDN columns, comb teeth, distortion transfer, phaser notches, flanger heads, compressor curve, limiter ceiling, and bitcrusher stair steps.
- Prohibited: chromatic accents, gradients, rounded fake hardware, fake meters, decorative visualizers, ornamental panels, bundled fonts, and product DSP.

Product repositories own parameter identity, automation attachments, accessible names, tooltips, effect-specific behavior, and the normalized values supplied to `ParameterDisplay`. This module owns only visual tokens, component rendering, display grammars, and layout geometry.
