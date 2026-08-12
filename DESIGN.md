# EHL JUCE Design Contract

This module is the single implementation source for the EHL JUCE plug-in family UI.

- Canvas: 640 × 360 default, 512 × 320 minimum, 1280 × 720 maximum.
- Grid: 4 px base, 8 px row spacing, 16 px major margin and column gap.
- Palette: ink `#050505`, low `#2A2A2A`, mid `#8A8A86`, paper `#F2F2F0`.
- Header: product name, effect class, and one divider only.
- Body: two columns by six rows, labels 88 px wide, continuous controls horizontal.
- States: focus uses a paper outline, active toggles invert, disabled controls use low/mid.
- Prohibited: chromatic accents, gradients, rounded fake hardware, decorative visualizers, ornamental panels, bundled fonts, and product DSP.

Product repositories own parameter identity, automation attachments, accessible names, tooltips, and effect-specific behavior. This module owns only visual tokens, component rendering, and layout geometry.
