################################################################
# iodoom – Makefile
# Targets: sdl (default), xlib, clean, info
################################################################

# ── Toolchain ───────────────────────────────────────────────
CC      ?= gcc
PKG     ?= pkg-config

# ── Output ──────────────────────────────────────────────────
OUTPUT   = iodoom
OBJDIR   = build

# ── Optimization / Debug ────────────────────────────────────
# Release build: make RELEASE=1
ifdef RELEASE
  OPT = -O3 -march=native -fomit-frame-pointer -flto
  DBG =
else
  OPT = -O2 -ggdb3
  DBG =
endif

# ── Common flags ────────────────────────────────────────────
CFLAGS  += $(OPT) $(DBG)
CFLAGS  += -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
CFLAGS  += -DNORMALUNIX -DLINUX -DSNDSERV -D_DEFAULT_SOURCE
CFLAGS  += -std=c99
LDFLAGS += -lm -lc

# Silence upstream doom warnings we can't easily fix
CFLAGS  += -Wno-sign-compare -Wno-implicit-fallthrough -Wno-char-subscripts

# ── Verbose control (make V=1 for full commands) ─────────────
ifeq ($(V),1)
Q =
else
Q = @
endif

# ── Source files ─────────────────────────────────────────────
SRC_DOOM = \
  dummy.o am_map.o doomdef.o doomstat.o dstrings.o \
  d_event.o d_items.o d_iwad.o d_loop.o d_main.o d_mode.o d_net.o \
  f_finale.o f_wipe.o g_game.o \
  hu_lib.o hu_stuff.o \
  info.o \
  i_cdmus.o i_endoom.o i_joystick.o i_scale.o i_sound.o i_system.o i_timer.o \
  memio.o \
  m_argv.o m_bbox.o m_cheat.o m_config.o m_controls.o m_fixed.o \
  m_menu.o m_misc.o m_random.o \
  p_ceilng.o p_doors.o p_enemy.o p_floor.o p_inter.o p_lights.o \
  p_map.o p_maputl.o p_mobj.o p_plats.o p_pspr.o p_saveg.o p_setup.o \
  p_sight.o p_spec.o p_switch.o p_telept.o p_tick.o p_user.o \
  r_bsp.o r_data.o r_draw.o r_main.o r_plane.o r_segs.o r_sky.o r_things.o \
  sha1.o sounds.o statdump.o \
  st_lib.o st_stuff.o s_sound.o \
  tables.o \
  v_video.o \
  wi_stuff.o \
  w_checksum.o w_file.o w_main.o w_wad.o w_file_stdc.o \
  i_input.o i_video.o z_zone.o \
  iodoom_engine.o

# ── SDL2 target (default) ────────────────────────────────────
SDL_CFLAGS  := $(shell $(PKG) --cflags sdl2 2>/dev/null || echo "-I/usr/include/SDL2 -D_REENTRANT")
SDL_LIBS    := $(shell $(PKG) --libs   sdl2 2>/dev/null || echo "-lSDL2")

SRC_SDL  = $(SRC_DOOM) iodoom_sdl.o
OBJS_SDL = $(addprefix $(OBJDIR)/, $(SRC_SDL))

# ── Xlib target ──────────────────────────────────────────────
SRC_XLIB  = $(SRC_DOOM) iodoom_xlib.o
OBJS_XLIB = $(addprefix $(OBJDIR)/, $(SRC_XLIB))

.DEFAULT_GOAL := sdl

sdl: EXTRA_CFLAGS  = $(SDL_CFLAGS)
sdl: EXTRA_LIBS    = $(SDL_LIBS)
sdl: $(OBJDIR) $(OBJS_SDL)
	@echo "  LD    $(OUTPUT)"
	$(Q)$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(OBJS_SDL) -o $(OUTPUT) $(LDFLAGS) $(EXTRA_LIBS)
	@echo "  SIZE"
	$(Q)size $(OUTPUT)
	@echo ""
	@echo "Built: ./$(OUTPUT)"
	@echo "Usage: ./$(OUTPUT) -iwad /path/to/doom.wad"

xlib: EXTRA_LIBS = -lX11
xlib: $(OBJDIR) $(OBJS_XLIB)
	@echo "  LD    $(OUTPUT)-xlib"
	$(Q)$(CC) $(CFLAGS) $(OBJS_XLIB) -o $(OUTPUT)-xlib $(LDFLAGS) $(EXTRA_LIBS)

$(OBJDIR)/iodoom_sdl.o: iodoom_sdl.c
	@echo "  CC    $<"
	$(Q)$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@echo "  CC    $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	$(Q)mkdir -p $(OBJDIR)

clean:
	$(Q)rm -rf $(OBJDIR) $(OUTPUT) $(OUTPUT)-xlib *.map

info:
	@echo "CC      = $(CC)"
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "SDL2    = $(SDL_CFLAGS) $(SDL_LIBS)"

.PHONY: sdl xlib clean info

$(OBJDIR)/iodoom_engine.o: iodoom_engine.c iodoom.h
	@echo "  CC    $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/iodoom_xlib.o: iodoom_xlib.c iodoom.h
	@echo "  CC    $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@
