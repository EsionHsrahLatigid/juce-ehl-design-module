# JUCE EHL Design Module

EsionHsrahLatigidオーディオプラグイン用の再利用可能なJUCE UIプリミティブです。共通モノクロ8-bitパレット、正規`ehl`ヘッダーマーク、コンパクトなエディタ寸法、量子化コントロール描画、6列×2行パラメータ配置を提供します。

## 謝辞

[JUCE](https://github.com/juce-framework/JUCE)とEHLモノクロ8-bitデザインシステムを利用しています。

## 機能

- `EHL::JuceDesign` CMake target。
- 4段階モノクロパレット。
- 640 × 360、最小512 × 320のコンパクトなエディタ契約。
- 量子化ロータリースライダー、トグル、コンボボックス、フォーカス／無効状態。
- 正規SVGと同じパスで描く`ehl` short markを含む共通製品ヘッダー。
- 9種類のエフェクト固有`ParameterDisplay`と6列×2行のラベル付きコントロール配置。
- ラスター画像、同梱フォント、偽メーター、DSP、製品メタデータを含みません。

## 必要環境

- CMake 3.22以上。
- C++17。
- JUCE 8.0.15、または`juce::juce_gui_basics` targetを公開する互換JUCE。

## インストール

submoduleとして追加します。

```bash
git submodule add https://github.com/EsionHsrahLatigid/juce-ehl-design-module.git modules/juce-ehl-design-module
git submodule update --init --recursive
```

親CMake projectでJUCEを利用可能にした後に追加します。

```cmake
add_subdirectory(modules/juce-ehl-design-module)
target_link_libraries(MyPlugin PRIVATE EHL::JuceDesign)
```

## 最小API

```cpp
#include <ehl/juce_design/EhlDesign.h>

ehl::juce_design::LookAndFeel lookAndFeel;
setLookAndFeel(&lookAndFeel);
ehl::juce_design::styleSlider(mySlider);
ehl::juce_design::paintEditorChrome(g, getLocalBounds(), "MyPlugin", "EFFECT");
ehl::juce_design::layoutLabelledControl(label, mySlider,
    ehl::juce_design::controlCell(getLocalBounds(), 0));
```

所有するLookAndFeelが破棄される前に、エディタのLookAndFeelポインタを`nullptr`へ戻してください。

## 開発

```bash
cmake -S . -B build -G Ninja -DEHL_JUCE_DESIGN_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## 依存関係とライセンス

| 依存関係 | 用途 | ライセンス |
| --- | --- | --- |
| JUCE | GUIコンポーネントと描画 | AGPLv3または商用JUCEライセンス |

本リポジトリはMIT Licenseです。JUCEにはJUCE自身のライセンスが適用されます。
