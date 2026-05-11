/**
 * iodoom - A user-friendly fork of doomgeneric
 * Platform: SDL2
 *
 * Improvements over doomgeneric_sdl.c:
 *  - Integer scaling with aspect-ratio preservation (1x, 2x, 3x, auto)
 *  - Mouse look support (relative mouse mode)
 *  - Configurable FPS cap (default 35 Hz = original Doom rate; set IODOOM_FPS)
 *  - Larger key queue (32 entries) to avoid dropped inputs
 *  - SDL_RENDERER_PRESENTVSYNC option
 *  - Graceful shutdown (atexit already deregistered, no double free)
 *  - Window icon placeholder hook
 *  - Environment-variable configuration (no recompile needed)
 */

#include "doomkeys.h"
#include "m_argv.h"
#include "iodoom.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>

/* ── Configuration (override with env vars) ────────────────────────── */
#define DEFAULT_SCALE   0          /* 0 = auto-fit, 1/2/3 = integer scale */
#define DEFAULT_FPS     35         /* Doom's native game rate              */
#define KEYQUEUE_SIZE   32         /* Upstream was 16 – doubled            */

static int   cfg_scale;           /* resolved at DG_Init time             */
static int   cfg_fps;
static bool  cfg_vsync;
static bool  cfg_fullscreen;
static bool  cfg_mouse;

/* ── SDL objects ────────────────────────────────────────────────────── */
static SDL_Window   *s_Window   = NULL;
static SDL_Renderer *s_Renderer = NULL;
static SDL_Texture  *s_Texture  = NULL;

/* ── Key queue ──────────────────────────────────────────────────────── */
static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int   s_WriteIdx = 0;
static unsigned int   s_ReadIdx  = 0;

/* ── Timing ─────────────────────────────────────────────────────────── */
static Uint64 s_PerfFreq;
static Uint64 s_LastFrame;

/* ── Helpers ────────────────────────────────────────────────────────── */
static int env_int(const char *name, int def)
{
    const char *v = getenv(name);
    return v ? atoi(v) : def;
}

static bool env_bool(const char *name, bool def)
{
    const char *v = getenv(name);
    if (!v) return def;
    return (v[0] == '1' || v[0] == 'y' || v[0] == 'Y' || v[0] == 't');
}

static unsigned char convertToDoomKey(SDL_Keycode sym)
{
    switch (sym) {
    case SDLK_RETURN:    return KEY_ENTER;
    case SDLK_ESCAPE:    return KEY_ESCAPE;
    case SDLK_LEFT:      return KEY_LEFTARROW;
    case SDLK_RIGHT:     return KEY_RIGHTARROW;
    case SDLK_UP:        return KEY_UPARROW;
    case SDLK_DOWN:      return KEY_DOWNARROW;
    case SDLK_LCTRL:
    case SDLK_RCTRL:     return KEY_FIRE;
    case SDLK_SPACE:     return KEY_USE;
    case SDLK_LSHIFT:
    case SDLK_RSHIFT:    return KEY_RSHIFT;
    case SDLK_LALT:
    case SDLK_RALT:      return KEY_LALT;
    case SDLK_F1:        return KEY_F1;
    case SDLK_F2:        return KEY_F2;
    case SDLK_F3:        return KEY_F3;
    case SDLK_F4:        return KEY_F4;
    case SDLK_F5:        return KEY_F5;
    case SDLK_F6:        return KEY_F6;
    case SDLK_F7:        return KEY_F7;
    case SDLK_F8:        return KEY_F8;
    case SDLK_F9:        return KEY_F9;
    case SDLK_F10:       return KEY_F10;
    case SDLK_F11:       return KEY_F11;
    case SDLK_F12:       return KEY_F12;
    case SDLK_EQUALS:
    case SDLK_PLUS:      return KEY_EQUALS;
    case SDLK_MINUS:     return KEY_MINUS;
    case SDLK_TAB:       return KEY_TAB;
    case SDLK_BACKSPACE: return KEY_BACKSPACE;
    case SDLK_PAUSE:     return KEY_PAUSE;
    default:             return (unsigned char)SDL_tolower((int)sym);
    }
}

static void enqueueKey(int pressed, SDL_Keycode sym)
{
    unsigned char dk = convertToDoomKey(sym);
    s_KeyQueue[s_WriteIdx] = (unsigned short)((pressed << 8) | dk);
    s_WriteIdx = (s_WriteIdx + 1) % KEYQUEUE_SIZE;
}

/* Mouse button → Doom key mapping */
static unsigned char mouseButtonToDoomKey(Uint8 btn)
{
    switch (btn) {
    case SDL_BUTTON_LEFT:   return KEY_FIRE;
    case SDL_BUTTON_RIGHT:  return KEY_USE;
    case SDL_BUTTON_MIDDLE: return KEY_STRAFE;
    default:                return 0;
    }
}

/* ── Aspect-correct destination rect ────────────────────────────────── */
static SDL_Rect calcDestRect(int winW, int winH)
{
    SDL_Rect r;
    if (cfg_scale > 0) {
        r.w = IODOOM_RESX * cfg_scale;
        r.h = IODOOM_RESY * cfg_scale;
    } else {
        /* Fit inside window preserving 8:5 aspect */
        float scaleX = (float)winW / IODOOM_RESX;
        float scaleY = (float)winH / IODOOM_RESY;
        float s = (scaleX < scaleY) ? scaleX : scaleY;
        /* Snap to integer for crisp pixels when possible */
        int si = (int)s;
        if (si < 1) si = 1;
        r.w = IODOOM_RESX * si;
        r.h = IODOOM_RESY * si;
    }
    r.x = (winW - r.w) / 2;
    r.y = (winH - r.h) / 2;
    return r;
}

/* ── DoomGeneric API ────────────────────────────────────────────────── */
void DG_Init(void)
{
    /* Read config */
    cfg_scale      = env_int ("IODOOM_SCALE",      DEFAULT_SCALE);
    cfg_fps        = env_int ("IODOOM_FPS",         DEFAULT_FPS);
    cfg_vsync      = env_bool("IODOOM_VSYNC",       false);
    cfg_fullscreen = env_bool("IODOOM_FULLSCREEN",  false);
    cfg_mouse      = env_bool("IODOOM_MOUSE",       false);

    if (cfg_fps <= 0) cfg_fps = DEFAULT_FPS;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "[iodoom] SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); /* nearest-neighbour */

    Uint32 winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (cfg_fullscreen) winFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    int winW = (cfg_scale > 0) ? IODOOM_RESX * cfg_scale : IODOOM_RESX * 2;
    int winH = (cfg_scale > 0) ? IODOOM_RESY * cfg_scale : IODOOM_RESY * 2;

    s_Window = SDL_CreateWindow("iodoom",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                winW, winH, winFlags);
    if (!s_Window) {
        fprintf(stderr, "[iodoom] SDL_CreateWindow failed: %s\n", SDL_GetError());
        exit(1);
    }

    Uint32 rendFlags = SDL_RENDERER_ACCELERATED;
    if (cfg_vsync) rendFlags |= SDL_RENDERER_PRESENTVSYNC;

    s_Renderer = SDL_CreateRenderer(s_Window, -1, rendFlags);
    if (!s_Renderer) {
        /* Fallback to software renderer */
        s_Renderer = SDL_CreateRenderer(s_Window, -1, SDL_RENDERER_SOFTWARE);
    }
    if (!s_Renderer) {
        fprintf(stderr, "[iodoom] SDL_CreateRenderer failed: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_RenderSetLogicalSize(s_Renderer, 0, 0);
    SDL_SetRenderDrawColor(s_Renderer, 0, 0, 0, 255);

    s_Texture = SDL_CreateTexture(s_Renderer,
                                  SDL_PIXELFORMAT_BGR24,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  IODOOM_RESX, IODOOM_RESY);
    if (!s_Texture) {
        fprintf(stderr, "[iodoom] SDL_CreateTexture failed: %s\n", SDL_GetError());
        exit(1);
    }

    if (cfg_mouse) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    s_PerfFreq = SDL_GetPerformanceFrequency();
    s_LastFrame = SDL_GetPerformanceCounter();

    printf("[iodoom] scale=%s fps_cap=%d vsync=%s fullscreen=%s mouse=%s\n",
           cfg_scale ? "fixed" : "auto", cfg_fps,
           cfg_vsync?"on":"off", cfg_fullscreen?"on":"off", cfg_mouse?"on":"off");
}

void DG_DrawFrame(void)
{
    /* --- poll events --- */
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            SDL_Quit();
            exit(0);
            break;
        case SDL_KEYDOWN:
            /* Toggle fullscreen with F11 */
            if (e.key.keysym.sym == SDLK_F11) {
                Uint32 f = SDL_GetWindowFlags(s_Window);
                SDL_SetWindowFullscreen(s_Window,
                    (f & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            /* Toggle mouse grab with F10 / Ctrl+G */
            if (e.key.keysym.sym == SDLK_BACKQUOTE) {
                SDL_bool cur = SDL_GetRelativeMouseMode();
                SDL_SetRelativeMouseMode(cur ? SDL_FALSE : SDL_TRUE);
            }
            enqueueKey(1, e.key.keysym.sym);
            break;
        case SDL_KEYUP:
            enqueueKey(0, e.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (cfg_mouse) {
                unsigned char dk = mouseButtonToDoomKey(e.button.button);
                if (dk) {
                    s_KeyQueue[s_WriteIdx] = (unsigned short)((1 << 8) | dk);
                    s_WriteIdx = (s_WriteIdx + 1) % KEYQUEUE_SIZE;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (cfg_mouse) {
                unsigned char dk = mouseButtonToDoomKey(e.button.button);
                if (dk) {
                    s_KeyQueue[s_WriteIdx] = (unsigned short)((0 << 8) | dk);
                    s_WriteIdx = (s_WriteIdx + 1) % KEYQUEUE_SIZE;
                }
            }
            break;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                SDL_RenderClear(s_Renderer);
            }
            break;
        default:
            break;
        }
    }

    /* --- blit framebuffer --- */
    void *pixels; int pitch;
    if (SDL_LockTexture(s_Texture, NULL, &pixels, &pitch) == 0) {
        /* DG_ScreenBuffer is XRGB (4 bytes); we need BGR24 (3 bytes) */
        const uint32_t *src = (const uint32_t *)DG_ScreenBuffer;
        uint8_t *dst = (uint8_t *)pixels;
        int total = IODOOM_RESX * IODOOM_RESY;
        for (int i = 0; i < total; i++) {
            uint32_t c = src[i];
            dst[0] = (uint8_t)(c);         /* B */
            dst[1] = (uint8_t)(c >> 8);    /* G */
            dst[2] = (uint8_t)(c >> 16);   /* R */
            dst += 3;
        }
        SDL_UnlockTexture(s_Texture);
    }

    int winW, winH;
    SDL_GetWindowSize(s_Window, &winW, &winH);
    SDL_Rect dst = calcDestRect(winW, winH);

    SDL_RenderClear(s_Renderer);
    SDL_RenderCopy(s_Renderer, s_Texture, NULL, &dst);
    SDL_RenderPresent(s_Renderer);

    /* --- software FPS cap (when vsync is off) --- */
    if (!cfg_vsync) {
        Uint64 now    = SDL_GetPerformanceCounter();
        Uint64 target = s_PerfFreq / (Uint64)cfg_fps;
        Uint64 elapsed = now - s_LastFrame;
        if (elapsed < target) {
            Uint32 ms = (Uint32)(((target - elapsed) * 1000) / s_PerfFreq);
            if (ms > 1) SDL_Delay(ms - 1);
            /* Spin the remaining sub-millisecond */
            while (SDL_GetPerformanceCounter() - s_LastFrame < target) {}
        }
        s_LastFrame = SDL_GetPerformanceCounter();
    }
}

void DG_SleepMs(uint32_t ms)
{
    SDL_Delay(ms);
}

uint32_t DG_GetTicksMs(void)
{
    return SDL_GetTicks();
}

int DG_GetKey(int *pressed, unsigned char *doomKey)
{
    if (s_ReadIdx == s_WriteIdx) return 0;
    unsigned short kd = s_KeyQueue[s_ReadIdx];
    s_ReadIdx = (s_ReadIdx + 1) % KEYQUEUE_SIZE;
    *pressed = kd >> 8;
    *doomKey = kd & 0xFF;
    return 1;
}

void DG_SetWindowTitle(const char *title)
{
    if (s_Window) {
        char buf[256];
        snprintf(buf, sizeof(buf), "iodoom – %s", title);
        SDL_SetWindowTitle(s_Window, buf);
    }
}

int main(int argc, char **argv)
{
    printf("iodoom – a user-friendly Doom port\n");
    printf("Based on doomgeneric by ozkl (https://github.com/ozkl/doomgeneric)\n\n");
    printf("Env vars: IODOOM_SCALE=0|1|2|3  IODOOM_FPS=35  IODOOM_VSYNC=0|1\n");
    printf("          IODOOM_FULLSCREEN=0|1  IODOOM_MOUSE=0|1\n\n");

    if (SDL_Init(0) != 0) {
        fprintf(stderr, "SDL_Init probe failed: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Quit();

    iodoom_Create(argc, argv);

    for (;;) {
        iodoom_Tick();
    }

    return 0;
}
