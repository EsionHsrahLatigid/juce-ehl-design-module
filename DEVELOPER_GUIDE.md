# Developer guide

## Local verification

```sh
cmake -S . -B build -G Ninja -DEHL_JUCE_DESIGN_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Consumer update workflow

1. Change this module and pass its contract test.
2. Commit and push the module.
3. Update every plugin's `modules/juce-ehl-design-module` Gitlink to the exact commit.
4. Build and test every consumer.
5. Verify staged Standalone, VST3, and AU products where supported.
6. Commit each consumer update separately.

When the canonical logo changes, update both `assets/logos/white/logo-short.svg` and the path returned by `shortLogoPathData()`. The contract test fails when their path data diverges.

Do not add a local wrapper or copy of `EhlDesign` to a consumer. The Gitlink remains the only source of shared UI implementation.
