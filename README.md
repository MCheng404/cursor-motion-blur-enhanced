# Cursor Motion Blur Pro / 光标运动模糊

**EN:** High-performance cursor motion blur with particle effects, 13 color modes, custom function trails, cursor color extraction, and click effects. Direct2D hardware accelerated. A heavily enhanced fork of TheatriChris's Cursor Motion Blur.

**中文：** 高性能鼠标运动模糊拖尾，支持粒子特效、13种颜色模式、自定义函数轨迹、光标取色和点击特效，Direct2D 硬件加速。基于 TheatriChris 的 Cursor Motion Blur 深度二改增强。

![Trail Effect](https://raw.githubusercontent.com/MCheng404/cursor-motion-blur-enhanced/main/assets/screenshot_trail.png)

---

## Installation / 安装

1. Install [Windhawk](https://windhawk.net/)
2. Download `cursor-motion-blur-pro.wh.cpp`
3. In Windhawk, click "Create mod" → paste the code → Save
4. Or wait for the mod to be accepted into the official catalog

**中文：**
1. 安装 [Windhawk](https://windhawk.net/)
2. 下载 `cursor-motion-blur-pro.wh.cpp`
3. 在 Windhawk 中点击"创建 mod"→ 粘贴代码 → 保存
4. 或等待 mod 被官方目录收录

---

## Features / 功能特性

* **Delay Rendering / 延迟渲染：** Trail head eases toward the cursor (0-10 adjustable). / 拖尾头部缓动跟随光标，滞后平滑（0-10 可调）。
* **Fadeout Modes / 淡出模式：** Hard cut / Accelerated shrink / Soft fade (alpha + length synchronized, no hard cutoff). / 硬截断 / 加速收缩 / 软截断（透明度渐变+拖尾收缩同步，末端加速淡出杜绝硬切）。
* **Dynamic Width / 动态宽度：** Trail widens with speed and acceleration. / 移动越快、急转时拖尾越宽，增强运动感。
* **Enhanced Glow / 增强发光：** Dual-layer halo (outer glow + inner bloom). / 双层光晕（外晕+内辉），更柔和自然。
* **Head Highlight / 头部高光：** Bright center dot at the trail head. / 拖尾头部明亮中心点，提升质感。
* **Trail Shadow / 拖尾阴影：** Dark underlay adds depth. / 底层暗色阴影层，增加立体感。
* **Dot Chain Links / 圆链连接线：** Auto-connecting dots for continuous trail. / 圆点之间自动连接，拖尾更连续。
* **Exponential Taper / 指数收尖：** Fuller head, sharper tail than linear taper. / 头部饱满尾部尖细，比线性收尖更自然。
* **Particle Dissipation / 粒子消散：** Mini particles released from the trail, attracted back to cursor with configurable origin (head/middle/tail/custom), attraction strength, and cursor repulsion force. Shapes: circle / star / hexagram / random mix. Colors fade from bright to dark over lifetime. / 拖尾释放迷你粒子，全程缓慢吸附回光标位置。释放位置可选（开头/中间/结尾/自定义），吸附强度可调。光标周围有排斥力，粒子飞到附近被弹开并随机扰乱轨迹。粒子支持圆形/五角星/六芒星/随机混合形状，颜色随生命周期从亮到暗渐变。
* **Click Starburst / 点击星爆：** Particle burst on click (toggleable, count adjustable). / 点击时从光标位置迸发粒子（可开关、数量可调）。
* **Cursor Color Extraction / 光标取色：** Real-time pixel color sampling under the cursor (2 modes), with auto complementary-color shift for visibility. / 实时提取光标下方像素颜色，拖尾自动融入背景（2 种模式）。支持自动互补色偏移，确保拖尾在任何背景上都醒目可见。
* **Function Trails / 函数轨迹：** Custom math expressions generate trail curves, 4 built-in presets. / 支持自定义数学公式生成轨迹曲线，内置 4 组预设函数。
* **Wave Trails / 波浪轨迹：** Sine-wave trail shape with flowing animation. / 拖尾形状增加正弦波浪曲线，轨迹呈流动波浪状。
* **Tapered Dot Chain / 类锥形圆链：** Dense dot-based tapered trail, big head small tail. / 由密集圆点组成的锥形拖尾，头部大尾部小。
* **Smooth Gradient / 平滑渐变：** Head-to-tail opacity gradient (toggleable). / 拖尾从头到尾透明度渐变淡出（可开关）。
* **Micro Glow / 微发光效果：** Soft outer glow (toggleable, intensity adjustable). / 拖尾外圈柔和发光（可开关、强度可调）。
* **13 Color Modes / 13 种颜色模式：** Classic / Single / Gradient (3-color) / Rainbow / Warm / Cool / Neon / Velocity / Stripes / Fire / Aurora / Cursor Extract / Cursor Mix.
* **Click Ripple / 点击波纹：** Expanding ripple on left/right click (toggleable). / 按下鼠标左右键时产生扩散波纹（可开关）。
* **Game Detection / 游戏检测：** Auto-disable in fullscreen DirectX games. / 全屏 DirectX 游戏时自动禁用。
* **Idle at 0% CPU / 动态渲染：** Zero CPU when cursor is stationary and no effects active. / 鼠标静止且无特效时 CPU 占用为 0%。

---

## Function Trail Variables / 函数轨迹变量

**EN:** Available variables: `t` (normalized 0=head 1=tail), `d` (distance from head in px), `time` (seconds). Functions: sin cos tan exp sqrt abs log. Operators: + - * / ^. Constants: pi e.

**中文：** 自定义公式中可使用：`t`（归一化位置 0=头 1=尾）、`d`（距头部像素距离）、`time`（秒）。支持函数：sin cos tan exp sqrt abs log，运算符：+ - * / ^，常量：pi e。

**Examples / 示例：** `sin(d * 0.15) * 8`, `sin(d * 0.25) * exp(0 - t * 2.5) * 10`

---

## Color Format / 颜色格式

**EN:** Hex RGB, e.g. `FF0000`=red, `00FF00`=green, `0000FF`=blue, `FFD700`=gold.

**中文：** 自定义颜色使用十六进制 RGB，例如：`FF0000`=红，`00FF00`=绿，`0000FF`=蓝，`FFD700`=金。

---

## Architecture / 架构

- **Mod type:** Windhawk Tool Mod (`@include windhawk.exe`), runs in a dedicated process
- **Rendering:** Direct2D hardware accelerated layered window (WS_EX_LAYERED)
- **Compiler options:** `-ld2d1 -lole32 -lgdi32 -lshell32`
- **Zero CPU when idle:** Window is hidden when no drawing is needed

---

## Version History / 版本历史

| Version | Changes |
|---------|---------|
| v9.2 | AI review fixes: single author, en-first localization, boolean settings, unload hang fix, verbatim boilerplate, thread-safe settings reload, clamps, screenshot |
| v9.1 | 3-color gradient (comma-separated), @author length fix |
| v9.0 | Rename to cursor-motion-blur-pro, performance optimizations, non-linear attraction, flicker fix |
| v8.9 | Multi-shape particles (star/hexagram), lifetime color fading (inspired by Mouse-Trail) |
| v8.8 | Cursor color auto complementary shift |
| v8.7 | 3 fadeout modes (hard/accelerate/soft) |
| v8.6 | Particle cursor repulsion force |
| v8.5 | Particle system rewrite: cursor attraction, configurable origin |
| v8.4 | Render quality: dot chain links, exponential taper, 12-stop gradient, trail shadow |
| v8.3 | Smooth fadeout, speed response, enhanced glow, head highlight |
| v8.2 | Particle modes, interval, acceleration effect |
| v8.1 | Dot chain density multiplier, per-dot variation |
| v8.0 | Custom function expression parser, tapered dot chain |
| v7.0 | Cursor color extraction (2 modes), wave trail shape |
| v6.0 | Particle dissipation, click starburst |
| v5.0 | Delay rendering, 5 new color modes |
| v4.0 | Flicker fix, micro glow, 8 color modes |
| v3.0 | Smooth gradient, 3 trail shapes, click ripple |
| v2.0 | Original by TheatriChris |

---

## Credits / 致谢

Original mod by [TheatriChris](https://github.com/chrisc44890) — [Cursor Motion Blur](https://github.com/chrisc44890) (MIT License)

Enhanced fork by [MCheng404](https://github.com/MCheng404)

Inspired by [Mouse-Trail](https://github.com/wanchen-wang/Mouse-Trail) by wanchen-wang

## License / 许可证

MIT License — inherited from original mod.
