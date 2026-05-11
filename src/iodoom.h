/**
 * iodoom.h – Public API for the iodoom engine
 *
 * To port iodoom to a new platform, implement all functions marked
 * "IMPLEMENT FOR YOUR PLATFORM" below.
 */
#ifndef IODOOM_H
#define IODOOM_H

#include <stdlib.h>
#include <stdint.h>

/* ── Resolution ──────────────────────────────────────────────────── */
#ifndef IODOOM_RESX
#define IODOOM_RESX 640
#endif

#ifndef IODOOM_RESY
#define IODOOM_RESY 400
#endif

/* Backwards-compat aliases so legacy platform files still compile */
#define DOOMGENERIC_RESX IODOOM_RESX
#define DOOMGENERIC_RESY IODOOM_RESY

/* ── Pixel type ──────────────────────────────────────────────────── */
#ifdef CMAP256
typedef uint8_t  pixel_t;
#else
typedef uint32_t pixel_t;
#endif

/* Shared framebuffer – written by the engine, read by DG_DrawFrame */
extern pixel_t *DG_ScreenBuffer;

#ifdef __cplusplus
extern "C" {
#endif

/* ── Engine lifecycle ─────────────────────────────────────────────── */
void iodoom_Create(int argc, char **argv);
void iodoom_Tick(void);

/* Backwards-compat aliases */
#define doomgeneric_Create iodoom_Create
#define doomgeneric_Tick   iodoom_Tick

/* ── Platform callbacks (IMPLEMENT FOR YOUR PLATFORM) ────────────── */

/** Initialise display, audio, input. Called once at startup. */
void DG_Init(void);

/** Blit DG_ScreenBuffer to screen and poll input events. */
void DG_DrawFrame(void);

/** Sleep for at least ms milliseconds. */
void DG_SleepMs(uint32_t ms);

/** Return milliseconds elapsed since program start. */
uint32_t DG_GetTicksMs(void);

/**
 * Dequeue one keyboard event.
 * Returns 1 if an event was available, 0 if the queue is empty.
 * *pressed = 1 for key-down, 0 for key-up.
 * *key     = iodoom key code (see doomkeys.h).
 */
int DG_GetKey(int *pressed, unsigned char *key);

/** Optional – set the OS window title string. */
void DG_SetWindowTitle(const char *title);

#ifdef __cplusplus
}
#endif

#endif /* IODOOM_H */
