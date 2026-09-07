// ==WindhawkMod==
// @id              cursor-motion-blur-cn
// @name            光标运动模糊 - 增强版
// @description     为鼠标指针添加高速运动模糊拖尾，支持延迟渲染、粒子消散、平滑渐变、发光、11种颜色模式和点击特效，Direct2D 硬件加速。
// @version         6.0
// @author          TheatriChris (二改汉化 + 功能增强)
// @github          https://github.com/chrisc44890
// @license         MIT
// @include         windhawk.exe
// @compilerOptions -ld2d1 -lole32 -lgdi32 -lshell32
// ==/WindhawkMod==
// ==WindhawkModReadme==
/*
# 光标运动模糊 - 增强版
高速移动鼠标时产生平滑的运动模糊拖尾，Direct2D 硬件加速渲染。

### 功能特性
* **延迟渲染：** 拖尾头部缓动跟随光标，滞后平滑（0-10 可调）。
* **粒子消散：** 拖尾淡出时释放迷你小球，模拟消散渲染（可开关、密度可调）。
* **点击星爆：** 点击时从光标位置迸发粒子（可开关、数量可调）。
* **平滑渐变：** 拖尾从头到尾透明度渐变淡出（可开关）。
* **微发光效果：** 拖尾外圈柔和发光（可开关、强度可调）。
* **多种形状：** 锥形 / 圆点链 / 等宽带。
* **11 种颜色模式：** 经典黑白 / 单色 / 多色渐变 / 彩虹流动 / 暖色流动 / 冷色流动 / 霓虹脉冲 / 速度变色 / 流动条纹 / 火焰 / 极光。
* **点击波纹：** 按下鼠标左右键时产生扩散波纹（可开关）。
* **游戏检测：** 全屏 DirectX 游戏时自动禁用。
* **动态渲染：** 鼠标静止且无特效时 CPU 占用为 0%。

### 颜色格式
自定义颜色使用十六进制 RGB，例如：`FF0000`=红，`00FF00`=绿，`0000FF`=蓝，`FFD700`=金。
*/
// ==/WindhawkModReadme==
// ==WindhawkModSettings==
/*
- trigger_velocity: 25
  $name: 触发速度
  $description: 鼠标移动多快时触发拖影（像素/帧）。
- stop_velocity: 10
  $name: 停止速度
  $description: 停止拖影的速度阈值（像素/帧）。必须低于触发速度。
- tail_offset_x: 6
  $name: 拖尾 X 偏移
  $description: 拖尾连接到光标的 X 轴偏移量（像素）。
- tail_offset_y: 10
  $name: 拖尾 Y 偏移
  $description: 拖尾连接到光标的 Y 轴偏移量（像素）。
- tail_length: 10
  $name: 拖尾长度
  $description: 拖影跟随的帧数。最低为 2。
- trail_delay: 0
  $name: 拖尾延迟
  $description: 拖尾头部滞后于光标的程度（0-10，0=关闭）。
- enable_smooth_gradient: 1
  $name: 平滑渐变
  $description: 拖尾透明度渐变淡出。1=开，0=关。
- trail_shape: tapered
  $name: 拖尾形状
  $options:
  - tapered: 锥形（经典）
  - dots: 圆点链
  - ribbon: 等宽带
- enable_glow: 1
  $name: 微发光效果
  $description: 拖尾外圈柔和发光。1=开，0=关。
- glow_intensity: 40
  $name: 发光强度
  $description: 发光范围和亮度（0-100）。
- color_mode: classic
  $name: 颜色模式
  $options:
  - classic: 经典黑白
  - single: 单色
  - gradient: 多色渐变
  - rainbow: 彩虹流动
  - warm: 暖色调流动
  - cool: 冷色调流动
  - neon: 霓虹脉冲
  - velocity: 速度变色
  - stripes: 流动条纹
  - fire: 火焰
  - aurora: 极光
- custom_color: "00BFFF"
  $name: 自定义颜色
  $description: 单色/霓虹/条纹模式的主色，十六进制 RGB。
- gradient_head_color: "FF6B35"
  $name: 渐变头部颜色
  $description: 多色渐变/条纹模式的头部或副色，十六进制 RGB。
- gradient_tail_color: "00BFFF"
  $name: 渐变尾部颜色
  $description: 多色渐变模式的尾部颜色，十六进制 RGB。
- enable_particles: 1
  $name: 粒子消散
  $description: 拖尾淡出时释放迷你小球模拟消散。1=开，0=关。
- particle_density: 3
  $name: 粒子密度
  $description: 每帧释放的粒子数量（1-10）。数值越大消散越明显。
- enable_click_starburst: 1
  $name: 点击星爆
  $description: 点击时从光标位置迸发粒子。1=开，0=关。
- starburst_count: 8
  $name: 星爆粒子数
  $description: 每次点击迸发的粒子数量（4-20）。
- enable_click_effect: 1
  $name: 点击波纹
  $description: 点击时产生扩散波纹。1=开，0=关。
- click_max_radius: 40
  $name: 波纹最大半径
  $description: 点击波纹扩散的最大半径（像素）。
- click_duration: 300
  $name: 波纹持续时间
  $description: 点击波纹从出现到消失的时长（毫秒）。
*/
// ==/WindhawkModSettings==
#include <windows.h>
#include <d2d1.h>
#include <math.h>
#include <shellapi.h>
#include <stdlib.h>
#include <deque>
#include <vector>
#include <algorithm>

#define GRAD_STOPS 8

// ===================== 颜色工具 =====================
static D2D1_COLOR_F HSVtoRGB(float h, float s, float v) {
    h = fmodf(h, 360.0f);
    if (h < 0) h += 360.0f;
    float c = v * s;
    float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;
    float r, g, b;
    if (h < 60)      { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else              { r = c; g = 0; b = x; }
    return D2D1::ColorF(r + m, g + m, b + m, 1.0f);
}
static D2D1_COLOR_F LerpColor(D2D1_COLOR_F a, D2D1_COLOR_F b, float t) {
    return D2D1::ColorF(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, 1.0f);
}
static D2D1_COLOR_F LighterColor(D2D1_COLOR_F c, float amount = 0.55f) {
    return D2D1::ColorF(c.r + (1.0f - c.r) * amount, c.g + (1.0f - c.g) * amount, c.b + (1.0f - c.b) * amount, 1.0f);
}
static D2D1_COLOR_F ParseHexColor(PCWSTR hex, D2D1_COLOR_F fallback) {
    if (!hex || !*hex) return fallback;
    DWORD val = wcstoul(hex, nullptr, 16);
    return D2D1::ColorF(((val >> 16) & 0xFF) / 255.0f, ((val >> 8) & 0xFF) / 255.0f, (val & 0xFF) / 255.0f, 1.0f);
}

// ===================== 全局状态 =====================
HWND g_overlayHwnd = NULL;
HANDLE g_threadHandle = NULL;
std::deque<POINT> g_history;
POINT g_lastPos = { 0, 0 };

// 延迟渲染
int g_trailDelay = 0;
POINT g_lagPos = { 0, 0 };
bool g_lagInited = false;

// Direct2D
ID2D1Factory* g_pD2DFactory = nullptr;
ID2D1DCRenderTarget* g_pDCRenderTarget = nullptr;
ID2D1SolidColorBrush* g_pSolidOuterBrush = nullptr;
ID2D1SolidColorBrush* g_pSolidInnerBrush = nullptr;
ID2D1LinearGradientBrush* g_pGradOuterBrush = nullptr;
ID2D1LinearGradientBrush* g_pGradInnerBrush = nullptr;
ID2D1GradientStopCollection* g_pGradOuterStops = nullptr;
ID2D1GradientStopCollection* g_pGradInnerStops = nullptr;

HDC g_hdcMem = NULL;
HBITMAP g_hBitmap = NULL;
int g_cachedVW = 0, g_cachedVH = 0;

// ===================== 设置缓存 =====================
float g_triggerVelocity = 25.0f, g_stopVelocity = 10.0f;
int g_tailOffsetX = 6, g_tailOffsetY = 10, g_tailLength = 10;
bool g_enableSmoothGradient = true;
int g_trailShape = 0;
bool g_enableGlow = true;
int g_glowIntensity = 40;
int g_colorMode = 0;
D2D1_COLOR_F g_customColor = { 0.0f, 0.75f, 1.0f, 1.0f };
D2D1_COLOR_F g_gradHeadColor = { 1.0f, 0.42f, 0.21f, 1.0f };
D2D1_COLOR_F g_gradTailColor = { 0.0f, 0.75f, 1.0f, 1.0f };
bool g_enableParticles = true;
int g_particleDensity = 3;
bool g_enableClickStarburst = true;
int g_starburstCount = 8;
bool g_enableClickEffect = true;
int g_clickMaxRadius = 40, g_clickDuration = 300;

// ===================== 粒子系统 =====================
struct Particle {
    float x, y, vx, vy, size;
    DWORD startTime;
    int lifetime;
    D2D1_COLOR_F color;
};
std::vector<Particle> g_particles;

// 波纹
struct Ripple { POINT pos; DWORD startTime; };
std::vector<Ripple> g_ripples;
bool g_prevLButton = false, g_prevRButton = false;

// ===================== 颜色缓存 =====================
struct GradData {
    D2D1_GRADIENT_STOP outer[GRAD_STOPS];
    D2D1_GRADIENT_STOP inner[GRAD_STOPS];
    D2D1_COLOR_F solidOuter, solidInner;
};
static GradData s_cachedGrad = {};
static bool s_gradValid = false;

static bool ColorApproxEq(D2D1_COLOR_F a, D2D1_COLOR_F b) {
    return fabsf(a.r - b.r) < 0.02f && fabsf(a.g - b.g) < 0.02f && fabsf(a.b - b.b) < 0.02f;
}
static bool GradApproxEq(const GradData& a, const GradData& b) {
    for (int i = 0; i < GRAD_STOPS; i++) {
        if (!ColorApproxEq(a.outer[i].color, b.outer[i].color)) return false;
        if (!ColorApproxEq(a.inner[i].color, b.inner[i].color)) return false;
    }
    return true;
}

static void ComputeColors(int mode, DWORD time, float velocity, GradData& out) {
    float t = time / 1000.0f;
    D2D1_COLOR_F headOuter, headInner, tailOuter, tailInner;
    switch (mode) {
        case 0: headOuter = tailOuter = D2D1::ColorF(0,0,0,1); headInner = tailInner = D2D1::ColorF(1,1,1,1); break;
        case 1: headOuter = tailOuter = g_customColor; headInner = tailInner = LighterColor(g_customColor); break;
        case 2: headOuter = g_gradHeadColor; tailOuter = g_gradTailColor; headInner = LighterColor(g_gradHeadColor); tailInner = LighterColor(g_gradTailColor); break;
        case 3: { float h = fmodf(t*80,360); headOuter=HSVtoRGB(h,.9f,1); tailOuter=HSVtoRGB(fmodf(h+140,360),.9f,1); headInner=HSVtoRGB(h,.45f,1); tailInner=HSVtoRGB(fmodf(h+140,360),.45f,1); break; }
        case 4: { float h = fmodf(t*40,60); headOuter=HSVtoRGB(h,.95f,1); tailOuter=HSVtoRGB(fmodf(h+35,60),.95f,1); headInner=HSVtoRGB(h,.5f,1); tailInner=HSVtoRGB(fmodf(h+35,60),.5f,1); break; }
        case 5: { float h = 180+fmodf(t*40,120); headOuter=HSVtoRGB(h,.9f,1); tailOuter=HSVtoRGB(fmodf(h+70,360),.9f,1); headInner=HSVtoRGB(h,.45f,1); tailInner=HSVtoRGB(fmodf(h+70,360),.45f,1); break; }
        case 6: { float p = .6f+.4f*sinf(t*4); D2D1_COLOR_F c=g_customColor; headOuter=D2D1::ColorF(c.r*p,c.g*p,c.b*p,1); tailOuter=D2D1::ColorF(c.r*p*.4f,c.g*p*.4f,c.b*p*.4f,1); headInner=LighterColor(headOuter,.6f); tailInner=LighterColor(tailOuter,.6f); break; }
        case 7: { float sn=min(velocity/60.0f,1.0f); float h=240-sn*240; headOuter=HSVtoRGB(h,.9f,1); tailOuter=HSVtoRGB(fmodf(h+60,360),.7f,.8f); headInner=HSVtoRGB(h,.4f,1); tailInner=HSVtoRGB(fmodf(h+60,360),.3f,.9f); break; }
        case 8: headOuter=g_customColor; tailOuter=g_gradHeadColor; headInner=LighterColor(g_customColor); tailInner=LighterColor(g_gradHeadColor); break;
        case 9: { float f=.85f+.15f*sinf(t*15)*sinf(t*7.3f); headOuter=D2D1::ColorF(1*f,.9f*f,.2f,1); tailOuter=D2D1::ColorF(.7f,.1f,0,1); headInner=D2D1::ColorF(1,1,.7f,1); tailInner=D2D1::ColorF(.9f,.3f,0,1); break; }
        case 10: { float h1=140+30*sinf(t*.8f); float h2=280+40*sinf(t*.6f+1); headOuter=HSVtoRGB(h1,.8f,.9f); tailOuter=HSVtoRGB(h2,.8f,.9f); headInner=HSVtoRGB(190,.5f,1); tailInner=HSVtoRGB(fmodf(h2+30,360),.4f,1); break; }
        default: headOuter=tailOuter=D2D1::ColorF(0,0,0,1); headInner=tailInner=D2D1::ColorF(1,1,1,1); break;
    }
    out.solidOuter = headOuter;
    out.solidInner = headInner;
    for (int i = 0; i < GRAD_STOPS; i++) {
        float ratio = (float)i / (GRAD_STOPS - 1);
        float alpha = 0.86f * (1.0f - ratio);
        if (mode == 8) {
            float phase = fmodf(ratio * 4.0f + t * 2.0f, 1.0f);
            bool stripe = phase < 0.5f;
            D2D1_COLOR_F co = stripe ? headOuter : tailOuter;
            D2D1_COLOR_F ci = stripe ? headInner : tailInner;
            out.outer[i] = { ratio, D2D1::ColorF(co.r, co.g, co.b, alpha) };
            out.inner[i] = { ratio, D2D1::ColorF(ci.r, ci.g, ci.b, alpha) };
        } else if (mode == 9) {
            float fr = ratio * ratio;
            D2D1_COLOR_F co = LerpColor(headOuter, tailOuter, fr);
            D2D1_COLOR_F ci = LerpColor(headInner, tailInner, fr);
            out.outer[i] = { ratio, D2D1::ColorF(co.r, co.g, co.b, alpha) };
            out.inner[i] = { ratio, D2D1::ColorF(ci.r, ci.g, ci.b, alpha) };
        } else {
            D2D1_COLOR_F co = LerpColor(headOuter, tailOuter, ratio);
            D2D1_COLOR_F ci = LerpColor(headInner, tailInner, ratio);
            out.outer[i] = { ratio, D2D1::ColorF(co.r, co.g, co.b, alpha) };
            out.inner[i] = { ratio, D2D1::ColorF(ci.r, ci.g, ci.b, alpha) };
        }
    }
}

static void ReleaseGradientBrushes() {
    if (g_pGradInnerBrush) { g_pGradInnerBrush->Release(); g_pGradInnerBrush = nullptr; }
    if (g_pGradOuterBrush) { g_pGradOuterBrush->Release(); g_pGradOuterBrush = nullptr; }
    if (g_pGradInnerStops) { g_pGradInnerStops->Release(); g_pGradInnerStops = nullptr; }
    if (g_pGradOuterStops) { g_pGradOuterStops->Release(); g_pGradOuterStops = nullptr; }
    s_gradValid = false;
}

static void UpdateColorBrushes(const GradData& data, D2D1_POINT_2F headPt, D2D1_POINT_2F tailPt) {
    if (g_pSolidOuterBrush) g_pSolidOuterBrush->SetColor(data.solidOuter);
    if (g_pSolidInnerBrush) g_pSolidInnerBrush->SetColor(data.solidInner);
    bool needRebuild = !s_gradValid || !GradApproxEq(s_cachedGrad, data);
    if (needRebuild && g_pDCRenderTarget) {
        ReleaseGradientBrushes();
        g_pDCRenderTarget->CreateGradientStopCollection(data.outer, GRAD_STOPS, &g_pGradOuterStops);
        g_pDCRenderTarget->CreateGradientStopCollection(data.inner, GRAD_STOPS, &g_pGradInnerStops);
        g_pDCRenderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(headPt, tailPt), g_pGradOuterStops, &g_pGradOuterBrush);
        g_pDCRenderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(headPt, tailPt), g_pGradInnerStops, &g_pGradInnerBrush);
        s_cachedGrad = data;
        s_gradValid = true;
    }
    if (g_pGradOuterBrush) { g_pGradOuterBrush->SetStartPoint(headPt); g_pGradOuterBrush->SetEndPoint(tailPt); }
    if (g_pGradInnerBrush) { g_pGradInnerBrush->SetStartPoint(headPt); g_pGradInnerBrush->SetEndPoint(tailPt); }
}

// ===================== 粒子生成辅助 =====================
static inline float Rand01() { return rand() / (float)RAND_MAX; }

static void SpawnParticles(float x, float y, int count, float speedMin, float speedMax,
                           float sizeMin, float sizeMax, int lifeMin, int lifeMax,
                           D2D1_COLOR_F color, DWORD time, bool radial = false) {
    for (int i = 0; i < count; i++) {
        float angle = radial ? (i / (float)count * 6.28318f) : (Rand01() * 6.28318f);
        float speed = speedMin + Rand01() * (speedMax - speedMin);
        g_particles.push_back({
            x, y,
            cosf(angle) * speed, sinf(angle) * speed,
            sizeMin + Rand01() * (sizeMax - sizeMin),
            time, lifeMin + (int)(Rand01() * (lifeMax - lifeMin)),
            color
        });
    }
}

// ===================== 设置加载 =====================
void LoadSettings() {
    g_triggerVelocity = (float)Wh_GetIntSetting(L"trigger_velocity");
    g_stopVelocity = (float)Wh_GetIntSetting(L"stop_velocity");
    g_tailOffsetX = Wh_GetIntSetting(L"tail_offset_x");
    g_tailOffsetY = Wh_GetIntSetting(L"tail_offset_y");
    g_tailLength = Wh_GetIntSetting(L"tail_length");
    g_trailDelay = Wh_GetIntSetting(L"trail_delay");
    g_enableSmoothGradient = Wh_GetIntSetting(L"enable_smooth_gradient") != 0;
    g_enableGlow = Wh_GetIntSetting(L"enable_glow") != 0;
    g_glowIntensity = Wh_GetIntSetting(L"glow_intensity");
    g_enableParticles = Wh_GetIntSetting(L"enable_particles") != 0;
    g_particleDensity = Wh_GetIntSetting(L"particle_density");
    g_enableClickStarburst = Wh_GetIntSetting(L"enable_click_starburst") != 0;
    g_starburstCount = Wh_GetIntSetting(L"starburst_count");
    g_enableClickEffect = Wh_GetIntSetting(L"enable_click_effect") != 0;
    g_clickMaxRadius = Wh_GetIntSetting(L"click_max_radius");
    g_clickDuration = Wh_GetIntSetting(L"click_duration");

    PCWSTR str = Wh_GetStringSetting(L"trail_shape");
    if (str) { if (wcscmp(str,L"dots")==0) g_trailShape=1; else if (wcscmp(str,L"ribbon")==0) g_trailShape=2; else g_trailShape=0; Wh_FreeStringSetting(str); }
    str = Wh_GetStringSetting(L"color_mode");
    if (str) {
        if (wcscmp(str,L"single")==0) g_colorMode=1;
        else if (wcscmp(str,L"gradient")==0) g_colorMode=2;
        else if (wcscmp(str,L"rainbow")==0) g_colorMode=3;
        else if (wcscmp(str,L"warm")==0) g_colorMode=4;
        else if (wcscmp(str,L"cool")==0) g_colorMode=5;
        else if (wcscmp(str,L"neon")==0) g_colorMode=6;
        else if (wcscmp(str,L"velocity")==0) g_colorMode=7;
        else if (wcscmp(str,L"stripes")==0) g_colorMode=8;
        else if (wcscmp(str,L"fire")==0) g_colorMode=9;
        else if (wcscmp(str,L"aurora")==0) g_colorMode=10;
        else g_colorMode=0;
        Wh_FreeStringSetting(str);
    }
    str = Wh_GetStringSetting(L"custom_color");
    if (str) { g_customColor = ParseHexColor(str, D2D1::ColorF(0,.75f,1)); Wh_FreeStringSetting(str); }
    str = Wh_GetStringSetting(L"gradient_head_color");
    if (str) { g_gradHeadColor = ParseHexColor(str, D2D1::ColorF(1,.42f,.21f)); Wh_FreeStringSetting(str); }
    str = Wh_GetStringSetting(L"gradient_tail_color");
    if (str) { g_gradTailColor = ParseHexColor(str, D2D1::ColorF(0,.75f,1)); Wh_FreeStringSetting(str); }

    if (g_triggerVelocity<=0) g_triggerVelocity=25;
    if (g_stopVelocity<=0) g_stopVelocity=10;
    if (g_tailLength<2) g_tailLength=10;
    if (g_trailDelay<0) g_trailDelay=0; if (g_trailDelay>10) g_trailDelay=10;
    if (g_glowIntensity<0) g_glowIntensity=0; if (g_glowIntensity>100) g_glowIntensity=10;
    if (g_particleDensity<1) g_particleDensity=1; if (g_particleDensity>10) g_particleDensity=10;
    if (g_starburstCount<4) g_starburstCount=4; if (g_starburstCount>20) g_starburstCount=20;
    if (g_clickMaxRadius<=0) g_clickMaxRadius=40;
    if (g_clickDuration<=0) g_clickDuration=300;
    s_gradValid = false;
    g_lagInited = false;
}

// ===================== 游戏检测 =====================
bool IsGameRunning() {
    HWND hwnd = GetForegroundWindow();
    if (!hwnd || hwnd == GetDesktopWindow()) return false;
    static HWND s_pm = FindWindowW(L"Progman", NULL), s_ww = FindWindowW(L"WorkerW", NULL);
    if (hwnd == s_pm || hwnd == s_ww) return false;
    QUERY_USER_NOTIFICATION_STATE state;
    if (SUCCEEDED(SHQueryUserNotificationState(&state)) && state == QUNS_RUNNING_D3D_FULL_SCREEN) return true;
    RECT rcApp; GetWindowRect(hwnd, &rcApp);
    HMONITOR hMon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { sizeof(mi) };
    if (GetMonitorInfo(hMon, &mi)) {
        bool fs = rcApp.left<=mi.rcMonitor.left && rcApp.top<=mi.rcMonitor.top && rcApp.right>=mi.rcMonitor.right && rcApp.bottom>=mi.rcMonitor.bottom;
        if (fs) {
            RECT rcClip;
            if (GetClipCursor(&rcClip)) {
                int vW=GetSystemMetrics(SM_CXVIRTUALSCREEN), vH=GetSystemMetrics(SM_CYVIRTUALSCREEN);
                if ((rcClip.right-rcClip.left)<vW || (rcClip.bottom-rcClip.top)<vH) return true;
            }
            CURSORINFO ci={sizeof(CURSORINFO)};
            if (GetCursorInfo(&ci) && ci.flags==0) return true;
        }
    }
    return false;
}

// ===================== 主绘制循环 =====================
VOID CALLBACK SmearTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    POINT pt; GetCursorPos(&pt);
    int dx = pt.x - g_lastPos.x, dy = pt.y - g_lastPos.y;
    float velocity = sqrtf((float)(dx*dx + dy*dy));
    g_lastPos = pt;

    // 延迟渲染
    POINT renderPos = pt;
    if (g_trailDelay > 0) {
        if (!g_lagInited) { g_lagPos = pt; g_lagInited = true; }
        float ease = 1.0f - (g_trailDelay / 14.0f);
        if (ease < 0.12f) ease = 0.12f;
        g_lagPos.x += (int)((pt.x - g_lagPos.x) * ease);
        g_lagPos.y += (int)((pt.y - g_lagPos.y) * ease);
        renderPos = g_lagPos;
    } else g_lagInited = false;

    // 预计算颜色（粒子和绘制都要用）
    GradData cols;
    ComputeColors(g_colorMode, dwTime, velocity, cols);

    int vX = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int vY = GetSystemMetrics(SM_YVIRTUALSCREEN);

    // 点击检测
    bool lDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    bool rDown = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    if (g_enableClickEffect) {
        if (lDown && !g_prevLButton) g_ripples.push_back({ pt, dwTime });
        if (rDown && !g_prevRButton) g_ripples.push_back({ pt, dwTime });
    }
    if (g_enableClickStarburst) {
        if ((lDown && !g_prevLButton) || (rDown && !g_prevRButton)) {
            SpawnParticles((float)(pt.x - vX), (float)(pt.y - vY), g_starburstCount,
                           2.5f, 5.5f, 1.5f, 3.0f, 250, 450, cols.solidOuter, dwTime, true);
        }
    }
    g_prevLButton = lDown; g_prevRButton = rDown;

    // 清理过期波纹和粒子
    if (!g_ripples.empty())
        g_ripples.erase(std::remove_if(g_ripples.begin(), g_ripples.end(),
            [&](const Ripple& r){return dwTime-r.startTime>(DWORD)g_clickDuration;}), g_ripples.end());

    // 游戏检测缓存
    static DWORD lastFsCheck = 0;
    static bool isGameCached = false, isSmearing = false;
    static int lowVelFrames = 0, needsClear = false;
    if (dwTime - lastFsCheck > 500) { isGameCached = IsGameRunning(); lastFsCheck = dwTime; }

    int vW = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int vH = GetSystemMetrics(SM_CYVIRTUALSCREEN) - 1;

    if (isGameCached) {
        if (isSmearing || !g_history.empty() || !g_ripples.empty() || !g_particles.empty() || needsClear) {
            isSmearing = false; g_history.clear(); g_ripples.clear(); g_particles.clear();
        } else return;
    } else {
        if (velocity > g_triggerVelocity && !isSmearing) { isSmearing = true; lowVelFrames = 0; }
        else if (velocity < g_stopVelocity && isSmearing) { lowVelFrames++; if (lowVelFrames > 2) isSmearing = false; }
        else if (velocity >= g_stopVelocity && isSmearing) lowVelFrames = 0;

        if (isSmearing) {
            POINT np = { renderPos.x - vX, renderPos.y - vY };
            g_history.push_front(np);
            while (g_history.size() > (size_t)g_tailLength) g_history.pop_back();
        } else {
            // 淡出时在尾部释放粒子
            if (g_enableParticles && !g_history.empty()) {
                POINT tail = g_history.back();
                SpawnParticles((float)tail.x + g_tailOffsetX, (float)tail.y + g_tailOffsetY,
                               g_particleDensity, 0.3f, 2.0f, 0.8f, 2.2f, 300, 700, cols.solidOuter, dwTime);
            }
            if (!g_history.empty()) { g_history.pop_back(); if (!g_history.empty()) g_history.pop_back(); }
            if (g_history.size() == 1) g_history.clear();
        }
    }

    // 更新粒子物理
    for (auto& p : g_particles) { p.x += p.vx; p.y += p.vy; p.vx *= 0.95f; p.vy *= 0.95f; }
    if (!g_particles.empty())
        g_particles.erase(std::remove_if(g_particles.begin(), g_particles.end(),
            [&](const Particle& p){return dwTime - p.startTime > (DWORD)p.lifetime;}), g_particles.end());

    bool tailVisible = isSmearing || g_history.size() >= 2;
    bool isDrawing = tailVisible || !g_ripples.empty() || !g_particles.empty();

    static bool isWindowVisible = true;
    if (isDrawing && !isWindowVisible) { ShowWindow(hwnd, SW_SHOWNA); isWindowVisible = true; }
    else if (!isDrawing && !needsClear && isWindowVisible) { ShowWindow(hwnd, SW_HIDE); isWindowVisible = false; }
    if (!isDrawing && !needsClear) return;

    // 后台缓冲
    HDC hdcScreen = GetDC(NULL);
    if (!g_hBitmap || g_cachedVW != vW || g_cachedVH != vH) {
        if (g_hBitmap) DeleteObject(g_hBitmap);
        if (g_hdcMem) DeleteDC(g_hdcMem);
        g_hdcMem = CreateCompatibleDC(hdcScreen);
        g_hBitmap = CreateCompatibleBitmap(hdcScreen, vW, vH);
        SelectObject(g_hdcMem, g_hBitmap);
        g_cachedVW = vW; g_cachedVH = vH;
        if (g_pDCRenderTarget) { g_pDCRenderTarget->Release(); g_pDCRenderTarget = nullptr; ReleaseGradientBrushes(); }
    }
    if (!g_pDCRenderTarget && g_pD2DFactory) {
        if (g_pSolidOuterBrush) { g_pSolidOuterBrush->Release(); g_pSolidOuterBrush = nullptr; }
        if (g_pSolidInnerBrush) { g_pSolidInnerBrush->Release(); g_pSolidInnerBrush = nullptr; }
        ReleaseGradientBrushes();
        D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            0, 0, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT);
        g_pD2DFactory->CreateDCRenderTarget(&props, &g_pDCRenderTarget);
        if (g_pDCRenderTarget) {
            g_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0,0,0,1), &g_pSolidOuterBrush);
            g_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1,1,1,1), &g_pSolidInnerBrush);
        }
    }
    if (!g_pDCRenderTarget) { ReleaseDC(NULL, hdcScreen); return; }

    RECT rc = { 0, 0, vW, vH };
    g_pDCRenderTarget->BindDC(g_hdcMem, &rc);
    g_pDCRenderTarget->BeginDraw();
    g_pDCRenderTarget->Clear(D2D1::ColorF(0, 0, 0, 0));

    // ===================== 粒子绘制（最底层） =====================
    if (!g_particles.empty()) {
        for (auto& p : g_particles) {
            float progress = (float)(dwTime - p.startTime) / p.lifetime;
            if (progress < 0 || progress >= 1) continue;
            float alpha = (1.0f - progress) * 0.65f;
            g_pSolidOuterBrush->SetColor(p.color);
            g_pSolidOuterBrush->SetOpacity(alpha);
            g_pDCRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(p.x, p.y), p.size, p.size), g_pSolidOuterBrush);
        }
        g_pSolidOuterBrush->SetOpacity(1.0f);
        needsClear = true;
    }

    // ===================== 拖尾绘制 =====================
    if (tailVisible && g_history.size() >= 2) {
        std::vector<D2D1_POINT_2F> smoothed;
        for (auto& p : g_history) smoothed.push_back(D2D1::Point2F((float)p.x + g_tailOffsetX, (float)p.y + g_tailOffsetY));
        for (int iter = 0; iter < 2; ++iter) {
            if (smoothed.size() < 3) break;
            std::vector<D2D1_POINT_2F> ns;
            ns.push_back(smoothed.front());
            for (size_t i = 0; i < smoothed.size() - 1; ++i) {
                D2D1_POINT_2F p0=smoothed[i], p1=smoothed[i+1];
                ns.push_back(D2D1::Point2F(.75f*p0.x+.25f*p1.x, .75f*p0.y+.25f*p1.y));
                ns.push_back(D2D1::Point2F(.25f*p0.x+.75f*p1.x, .25f*p0.y+.75f*p1.y));
            }
            ns.push_back(smoothed.back());
            smoothed = ns;
        }
        UpdateColorBrushes(cols, smoothed[0], smoothed.back());
        float glowR = g_enableGlow ? (g_glowIntensity/100.0f)*7.0f : 0;
        float glowO = g_enableGlow ? (g_glowIntensity/100.0f)*0.28f : 0;

        if (g_trailShape == 1) {
            // 圆点链
            for (size_t i = 0; i < smoothed.size(); ++i) {
                float ratio = (float)i/(smoothed.size()-1);
                float dr = 8.0f*(1-ratio); if (dr < 0.5f) continue;
                int idx = (int)(ratio*(GRAD_STOPS-1)+0.5f); if (idx>=GRAD_STOPS) idx=GRAD_STOPS-1;
                D2D1_COLOR_F dco = cols.outer[idx].color, dci = cols.inner[idx].color;
                float da = 0.86f*(1-ratio); if (g_enableSmoothGradient) da *= (1-ratio*0.4f);
                if (glowR > 0.1f) {
                    g_pSolidOuterBrush->SetColor(dco); g_pSolidOuterBrush->SetOpacity(glowO*(1-ratio*0.5f));
                    g_pDCRenderTarget->FillEllipse(D2D1::Ellipse(smoothed[i], dr+glowR, dr+glowR), g_pSolidOuterBrush);
                }
                g_pSolidOuterBrush->SetColor(dco); g_pSolidOuterBrush->SetOpacity(da);
                g_pDCRenderTarget->FillEllipse(D2D1::Ellipse(smoothed[i], dr, dr), g_pSolidOuterBrush);
                g_pSolidInnerBrush->SetColor(dci); g_pSolidInnerBrush->SetOpacity(da);
                g_pDCRenderTarget->FillEllipse(D2D1::Ellipse(smoothed[i], dr*0.6f, dr*0.6f), g_pSolidInnerBrush);
            }
            g_pSolidOuterBrush->SetOpacity(1); g_pSolidInnerBrush->SetOpacity(1);
            needsClear = true;
        } else {
            // 多边形带状
            size_t sl = smoothed.size();
            std::vector<D2D1_POINT_2F> lo, ro, lc, rc, gl, gr;
            for (size_t i = 0; i < sl; ++i) {
                float ddx, ddy;
                if (i==0) { ddx=smoothed[0].x-smoothed[1].x; ddy=smoothed[0].y-smoothed[1].y; }
                else if (i==sl-1) { ddx=smoothed[i-1].x-smoothed[i].x; ddy=smoothed[i-1].y-smoothed[i].y; }
                else { ddx=smoothed[i-1].x-smoothed[i+1].x; ddy=smoothed[i-1].y-smoothed[i+1].y; }
                float ln=sqrtf(ddx*ddx+ddy*ddy); if (ln>0){ddx/=ln;ddy/=ln;}else{ddx=1;ddy=0;}
                float nx=-ddy, ny=ddx, ratio=(float)i/(sl-1);
                float ow, cw;
                if (g_trailShape==2) { float et=(ratio>.8f)?(1-(ratio-.8f)/.2f):1; ow=10*et; cw=6*et; }
                else { ow=10-10*ratio; cw=6-6*ratio; }
                if (i==sl-1){ow=0;cw=0;}
                lo.push_back(D2D1::Point2F(smoothed[i].x+nx*ow, smoothed[i].y+ny*ow));
                ro.push_back(D2D1::Point2F(smoothed[i].x-nx*ow, smoothed[i].y-ny*ow));
                lc.push_back(D2D1::Point2F(smoothed[i].x+nx*cw, smoothed[i].y+ny*cw));
                rc.push_back(D2D1::Point2F(smoothed[i].x-nx*cw, smoothed[i].y-ny*cw));
                if (glowR>.1f) { float gw=ow+glowR*(1-ratio*.3f); gl.push_back(D2D1::Point2F(smoothed[i].x+nx*gw,smoothed[i].y+ny*gw)); gr.push_back(D2D1::Point2F(smoothed[i].x-nx*gw,smoothed[i].y-ny*gw)); }
            }
            // 发光层
            if (glowR>.1f && gl.size()>=2) {
                ID2D1PathGeometry* pg=nullptr; ID2D1GeometrySink* ps=nullptr;
                g_pD2DFactory->CreatePathGeometry(&pg); pg->Open(&ps);
                ps->SetFillMode(D2D1_FILL_MODE_WINDING); ps->BeginFigure(gl[0],D2D1_FIGURE_BEGIN_FILLED);
                for (size_t i=1;i<gl.size();++i) ps->AddLine(gl[i]);
                for (int i=(int)gr.size()-1;i>=0;--i) ps->AddLine(gr[i]);
                ps->EndFigure(D2D1_FIGURE_END_CLOSED); ps->Close(); ps->Release();
                g_pSolidOuterBrush->SetColor(cols.solidOuter); g_pSolidOuterBrush->SetOpacity(glowO);
                g_pDCRenderTarget->FillGeometry(pg, g_pSolidOuterBrush); pg->Release();
            }
            // 外层
            ID2D1PathGeometry *pog=nullptr, *pcg=nullptr;
            ID2D1GeometrySink* ps=nullptr;
            g_pD2DFactory->CreatePathGeometry(&pog); pog->Open(&ps);
            ps->SetFillMode(D2D1_FILL_MODE_WINDING); ps->BeginFigure(lo[0],D2D1_FIGURE_BEGIN_FILLED);
            for (size_t i=1;i<lo.size();++i) ps->AddLine(lo[i]);
            for (int i=(int)ro.size()-1;i>=0;--i) ps->AddLine(ro[i]);
            ps->EndFigure(D2D1_FIGURE_END_CLOSED); ps->Close(); ps->Release();
            // 内层
            g_pD2DFactory->CreatePathGeometry(&pcg); pcg->Open(&ps);
            ps->SetFillMode(D2D1_FILL_MODE_WINDING); ps->BeginFigure(lc[0],D2D1_FIGURE_BEGIN_FILLED);
            for (size_t i=1;i<lc.size();++i) ps->AddLine(lc[i]);
            for (int i=(int)rc.size()-1;i>=0;--i) ps->AddLine(rc[i]);
            ps->EndFigure(D2D1_FIGURE_END_CLOSED); ps->Close(); ps->Release();
            // 头部帽
            ID2D1EllipseGeometry *poe=nullptr, *pie=nullptr;
            g_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(smoothed[0],10,10),&poe);
            g_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(smoothed[0],6,6),&pie);
            ID2D1Brush *ob=g_pSolidOuterBrush, *ib=g_pSolidInnerBrush;
            if (g_enableSmoothGradient && g_pGradOuterBrush && g_pGradInnerBrush) { ob=g_pGradOuterBrush; ib=g_pGradInnerBrush; }
            else { g_pSolidOuterBrush->SetOpacity(.86f); g_pSolidInnerBrush->SetOpacity(.86f); }
            ID2D1Geometry* og[2]={pog,poe}, *ig[2]={pcg,pie};
            ID2D1GeometryGroup *pog_g=nullptr, *pig_g=nullptr;
            g_pD2DFactory->CreateGeometryGroup(D2D1_FILL_MODE_WINDING,og,2,&pog_g);
            g_pD2DFactory->CreateGeometryGroup(D2D1_FILL_MODE_WINDING,ig,2,&pig_g);
            g_pDCRenderTarget->FillGeometry(pog_g, ob);
            g_pDCRenderTarget->FillGeometry(pig_g, ib);
            g_pSolidOuterBrush->SetOpacity(1); g_pSolidInnerBrush->SetOpacity(1);
            pog_g->Release(); pig_g->Release(); poe->Release(); pie->Release(); pog->Release(); pcg->Release();
            needsClear = true;
        }
    }

    // ===================== 点击波纹 =====================
    if (g_enableClickEffect && !g_ripples.empty()) {
        for (auto& ripple : g_ripples) {
            float elapsed=(float)(dwTime-ripple.startTime), progress=elapsed/g_clickDuration;
            if (progress<0||progress>=1) continue;
            float radius=progress*g_clickMaxRadius, alpha=(1-progress)*.7f;
            D2D1_POINT_2F c=D2D1::Point2F((float)(ripple.pos.x-vX),(float)(ripple.pos.y-vY));
            g_pSolidOuterBrush->SetColor(cols.solidOuter); g_pSolidOuterBrush->SetOpacity(alpha);
            g_pDCRenderTarget->DrawEllipse(D2D1::Ellipse(c,radius,radius),g_pSolidOuterBrush,2.5f);
            if (radius>4) { g_pSolidInnerBrush->SetColor(cols.solidInner); g_pSolidInnerBrush->SetOpacity(alpha*.8f);
                g_pDCRenderTarget->DrawEllipse(D2D1::Ellipse(c,radius*.7f,radius*.7f),g_pSolidInnerBrush,1.5f); }
            needsClear = true;
        }
        g_pSolidOuterBrush->SetOpacity(1); g_pSolidInnerBrush->SetOpacity(1);
    }

    HRESULT hr = g_pDCRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET) { g_pDCRenderTarget->Release(); g_pDCRenderTarget=nullptr; ReleaseGradientBrushes(); }

    BLENDFUNCTION blend={0}; blend.BlendOp=AC_SRC_OVER; blend.SourceConstantAlpha=255; blend.AlphaFormat=AC_SRC_ALPHA;
    POINT ptPos={vX,vY}; SIZE sz={vW,vH}; POINT ptSrc={0,0};
    UpdateLayeredWindow(hwnd,hdcScreen,&ptPos,&sz,g_hdcMem,&ptSrc,0,&blend,ULW_ALPHA);
    ReleaseDC(NULL,hdcScreen);
    if (!isSmearing && g_history.empty() && g_ripples.empty() && g_particles.empty()) needsClear=false;
}

// ===================== 覆盖层线程 =====================
DWORD WINAPI OverlayThreadProc(LPVOID) {
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    srand((unsigned)GetTickCount());
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
    HINSTANCE hi = GetModuleHandle(NULL);
    const wchar_t CN[] = L"CursorMotionBlurCnClass";
    WNDCLASS wc={}; wc.lpfnWndProc=DefWindowProc; wc.hInstance=hi; wc.lpszClassName=CN; RegisterClass(&wc);
    int sx=GetSystemMetrics(SM_XVIRTUALSCREEN), sy=GetSystemMetrics(SM_YVIRTUALSCREEN);
    int sw=GetSystemMetrics(SM_CXVIRTUALSCREEN), sh=GetSystemMetrics(SM_CYVIRTUALSCREEN)-1;
    g_overlayHwnd = CreateWindowEx(
        WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_TOPMOST|WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE,
        CN, L"CursorMotionBlurCnOverlay", WS_POPUP, sx, sy, sw, sh, NULL, NULL, hi, NULL);
    if (!g_overlayHwnd) return 0;
    ShowWindow(g_overlayHwnd, SW_SHOWNA);
    GetCursorPos(&g_lastPos);
    SetTimer(g_overlayHwnd, 1, USER_TIMER_MINIMUM, SmearTimerProc);
    MSG msg;
    while (GetMessage(&msg,NULL,0,0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    ReleaseGradientBrushes();
    if (g_pSolidInnerBrush){g_pSolidInnerBrush->Release();g_pSolidInnerBrush=nullptr;}
    if (g_pSolidOuterBrush){g_pSolidOuterBrush->Release();g_pSolidOuterBrush=nullptr;}
    if (g_pDCRenderTarget){g_pDCRenderTarget->Release();g_pDCRenderTarget=nullptr;}
    if (g_pD2DFactory){g_pD2DFactory->Release();g_pD2DFactory=nullptr;}
    if (g_hBitmap) DeleteObject(g_hBitmap);
    if (g_hdcMem) DeleteDC(g_hdcMem);
    DestroyWindow(g_overlayHwnd); UnregisterClass(CN, hi); CoUninitialize();
    return 0;
}

// ===================== Tool Mod 回调 =====================
BOOL WhTool_ModInit() { LoadSettings(); g_threadHandle=CreateThread(NULL,0,OverlayThreadProc,NULL,0,NULL); return TRUE; }
void WhTool_ModUninit() { if(g_overlayHwnd) PostMessage(g_overlayHwnd,WM_QUIT,0,0); if(g_threadHandle){WaitForSingleObject(g_threadHandle,INFINITE);CloseHandle(g_threadHandle);} }
void WhTool_ModSettingsChanged() { LoadSettings(); }

////////////////////////////////////////////////////////////////////////////////
bool g_isToolModProcessLauncher;
HANDLE g_toolModProcessMutex;
void WINAPI EntryPoint_Hook() { Wh_Log(L">"); ExitThread(0); }
BOOL Wh_ModInit() {
    DWORD sid;
    if (ProcessIdToSessionId(GetCurrentProcessId(),&sid)&&sid==0) return FALSE;
    bool ie=false,itp=false,ictp=false; int argc; LPWSTR* argv=CommandLineToArgvW(GetCommandLine(),&argc);
    if(!argv){Wh_Log(L"CommandLineToArgvW failed");return FALSE;}
    for(int i=1;i<argc;i++){if(wcscmp(argv[i],L"-service")==0||wcscmp(argv[i],L"-service-start")==0||wcscmp(argv[i],L"-service-stop")==0){ie=true;break;}}
    for(int i=1;i<argc-1;i++){if(wcscmp(argv[i],L"-tool-mod")==0){itp=true;if(wcscmp(argv[i+1],WH_MOD_ID)==0)ictp=true;break;}}
    LocalFree(argv); if(ie)return FALSE;
    if(ictp){
        g_toolModProcessMutex=CreateMutex(nullptr,TRUE,L"windhawk-tool-mod_" WH_MOD_ID);
        if(!g_toolModProcessMutex){Wh_Log(L"CreateMutex failed");ExitProcess(1);}
        if(GetLastError()==ERROR_ALREADY_EXISTS){Wh_Log(L"Tool mod already running (%s)",WH_MOD_ID);ExitProcess(1);}
        if(!WhTool_ModInit())ExitProcess(1);
        IMAGE_DOS_HEADER* dh=(IMAGE_DOS_HEADER*)GetModuleHandle(nullptr);
        IMAGE_NT_HEADERS* nh=(IMAGE_NT_HEADERS*)((BYTE*)dh+dh->e_lfanew);
        void* ep=(BYTE*)dh+nh->OptionalHeader.AddressOfEntryPoint;
        Wh_SetFunctionHook(ep,(void*)EntryPoint_Hook,nullptr); return TRUE;
    }
    if(itp)return FALSE; g_isToolModProcessLauncher=true; return TRUE;
}
void Wh_ModAfterInit() {
    if(!g_isToolModProcessLauncher)return;
    WCHAR cpp[MAX_PATH];
    switch(GetModuleFileName(nullptr,cpp,ARRAYSIZE(cpp))){case 0:case ARRAYSIZE(cpp):Wh_Log(L"GetModuleFileName failed");return;}
    WCHAR cl[MAX_PATH+2+(sizeof(L" -tool-mod " WH_MOD_ID "")/sizeof(WCHAR))-1];
    swprintf_s(cl,L"\"%s\" -tool-mod \"%s\"",cpp,WH_MOD_ID);
    HMODULE km=GetModuleHandle(L"kernelbase.dll"); if(!km)km=GetModuleHandle(L"kernel32.dll"); if(!km){Wh_Log(L"No kernelbase/kernel32");return;}
    using CPIW_t=BOOL(WINAPI*)(HANDLE,LPCWSTR,LPWSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,WINBOOL,DWORD,LPVOID,LPCWSTR,LPSTARTUPINFOW,LPPROCESS_INFORMATION,PHANDLE);
    CPIW_t p=(CPIW_t)GetProcAddress(km,"CreateProcessInternalW"); if(!p){Wh_Log(L"No CreateProcessInternalW");return;}
    STARTUPINFO si{.cb=sizeof(STARTUPINFO),.dwFlags=STARTF_FORCEOFFFEEDBACK}; PROCESS_INFORMATION pi;
    if(!p(nullptr,cpp,cl,nullptr,nullptr,FALSE,NORMAL_PRIORITY_CLASS,nullptr,nullptr,&si,&pi,nullptr)){Wh_Log(L"CreateProcess failed");return;}
    CloseHandle(pi.hProcess);CloseHandle(pi.hThread);
}
void Wh_ModSettingsChanged(){if(g_isToolModProcessLauncher)return;WhTool_ModSettingsChanged();}
void Wh_ModUninit(){if(g_isToolModProcessLauncher)return;WhTool_ModUninit();ExitProcess(0);}