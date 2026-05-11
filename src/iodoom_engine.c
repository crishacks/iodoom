/**
 * iodoom_engine.c – Core engine lifecycle
 *
 * iodoom_Create  – initialise engine + platform, then hand off to D_DoomMain
 * iodoom_Tick    – run one game tick (defined in d_main.c)
 */

#include <stdio.h>
#include <stdlib.h>

#include "m_argv.h"
#include "iodoom.h"

pixel_t *DG_ScreenBuffer = NULL;

void M_FindResponseFile(void);
void D_DoomMain(void);

void iodoom_Create(int argc, char **argv)
{
    myargc = argc;
    myargv = argv;

    M_FindResponseFile();

    DG_ScreenBuffer = malloc(IODOOM_RESX * IODOOM_RESY * sizeof(pixel_t));
    if (!DG_ScreenBuffer) {
        fprintf(stderr, "[iodoom] fatal: failed to allocate framebuffer\n");
        exit(1);
    }

    DG_Init();

    D_DoomMain();
}
