# EHL JUCE Plugin UI Design Contract

This repository is the single source of truth for reusable EsionHsrahLatigid JUCE plugin UI primitives.

## Locked profile

- Logical editor canvas: `640x360`, minimum `512x320`.
- Base grid: 4 logical pixels; major spacing: 8 logical pixels.
- Palette: `ink #050505`, `low #2A2A2A`, `mid #8A8A86`, `paper #F2F2F0`.
- Controls use square, integer-aligned, quantized geometry.
- Operational text stays clean and legible.
- No chromatic accents, gradients, glow, RGB split, fake hardware, rounded panels, or decorative noise over operational content.

## Logo contract

- The compact header uses the canonical one-colour `ehl` short mark, never replacement text or a waveform motif.
- `assets/logos/white/logo-short.svg` is the source asset; the identical outlined path is embedded for resource-independent Standalone, VST3, and AU rendering.
- At `640x360`, the mark occupies `96x36` logical pixels at `(528,12)`, matching the YUP design module.
- Below 640 logical pixels wide, the mark and its inset scale with editor width. At larger widths it remains `96x36` so it cannot overrun the 64-pixel header.
- The mark is `paper` on `ink`. Product titles and operational labels remain separate and undamaged.

## Public compatibility surface

Palette, `Metrics`, `LookAndFeel`, `shortLogoPathData`, `shortLogoPath`, `headerLogoBounds`, `shortLogoTransform`, `paintShortLogo`, `paintEditorChrome`, `ParameterDisplay`, and layout helpers are the supported API.

Changing palette values, logo geometry or placement, editor metrics, indicator direction, namespace, CMake target, or public API requires a coordinated consumer migration.

## Product boundary

Product identity, parameter semantics, DSP behavior, warning copy, and product-specific editor composition stay in consumer repositories. Consumers use the exact Gitlink and do not copy or wrap the shared implementation.
