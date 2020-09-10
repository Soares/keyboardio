// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details


/* =================================================================================================
 *  This file sets up the following:
 *  - LED mode configuration and ordering
 *  - Macro definitions
 *  - Key mappings
 *  - Main setup and loop functions
 *  Macros must come before key mappings, so we do key mappings late.
 * ============================================================================================== */

// Standard plugins
#include <Kaleidoscope.h>
#include <Kaleidoscope-Macros.h>  // for making shifted numbers be macros
#include <Kaleidoscope-TopsyTurvy.h>  // for inverting ~ and `
#include <Kaleidoscope-LEDControl.h>  // for controlling the LEDs
#include <Kaleidoscope-LEDEffect-BootGreeting.h>  // pulses the LED button for 10s upon power-on
#include <Kaleidoscope-LEDEffect-SolidColor.h>  // LED modes that set all LEDs to a single color
#include <Kaleidoscope-LEDEffect-Breathe.h>  // LED modes that make the keys "breathe"
#include <Kaleidoscope-LEDEffect-Rainbow.h> // LED mode that pulses the keys in a rainbow pattern
#include <Kaleidoscope-LED-Stalker.h>  // "plz reveal my password" mode
#include <Kaleidoscope-LED-AlphaSquare.h>  // "no really make my password super obvious" mode

// 3rd party plugins
#include <Kaleidoscope-LEDEffect-DigitalRain.h>  // matrix style LED mode

// TODOs:
// - Numlock
// - Purple lights

/* ------------------------------------------------------------------------------------------------
 *  LED Mode configuration
 * ---------------------------------------------------------------------------------------------- */

// TODO: Make the boot greeting purple.

// These 'solid' color effect definitions define a rainbow of
// LED color modes calibrated to draw 500mA or less on the
// Keyboardio Model 01.
static kaleidoscope::plugin::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::plugin::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::plugin::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::plugin::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::plugin::LEDSolidColor solidBlue(0, 100, 130);
static kaleidoscope::plugin::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::plugin::LEDSolidColor solidViolet(130, 0, 120);

void config_and_use_LED_modes() {
  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);
  AlphaSquare.color = { 0, 150, 150 };
  StalkerEffect.variant = STALKER(BlazingTrail);
  LEDOff.activate();
}

/* ------------------------------------------------------------------------------------------------
 *  Macro definitions
 * ---------------------------------------------------------------------------------------------- */

enum {
  MACRO_ANY,  // presses a random key when the Any key is pressed
};

static void anyKeyMacro(uint8_t keyState) {
  static Key lastKey;
  bool toggledOn = false;
  if (keyToggledOn(keyState)) {
    lastKey.setKeyCode(Key_A.getKeyCode() + (uint8_t)(millis() % 36));
    toggledOn = true;
  }

  if (keyIsPressed(keyState))
    Kaleidoscope.hid().keyboard().pressKey(lastKey, toggledOn);
}

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {

    case MACRO_ANY:
      anyKeyMacro(keyState);
      break;

  }
  return MACRO_NONE;
}

/* ------------------------------------------------------------------------------------------------
 *  Key mappings
 * ---------------------------------------------------------------------------------------------- */

/*
 * Grid template:
 *
 * ┌───┬───┬───┬───┬───┬───┬───┐                   ┌───┬───┬───┬───┬───┬───┬───┐
 * │   │   │   │   │   │   │   │                   │   │   │   │   │   │   │   │
 * ├───┼───┼───┼───┼───┼───┼───┤                   ├───┼───┼───┼───┼───┼───┼───┤
 * │   │   │   │   │   │   │   │                   │   │   │   │   │   │   │   │
 * ├───┼───┼───┼───┼───┼───┤   │                   │   ├───┼───┼───┼───┼───┼───┤
 * │   │   │   │   │   │   ├───┘                   └───┤   │   │   │   │   │   │
 * ├───┼───┼───┼───┼───┼───┼───┐                   ┌───┼───┼───┼───┼───┼───┼───┤
 * │   │   │   │   │   │   │   │                   │   │   │   │   │   │   │   │
 * └───┴───┴───┴───┴───┼───┼───┼───┬───┐   ┌───┬───┼───┼───┼───┴───┴───┴───┴───┘
 *                     │   │   │   │   │   │   │   │   │   │
 *                     └───┴───┼───┼───┘   └───┼───┼───┴───┘
 *                             │   │           │   │
 *                             └───┘           └───┘
 *
 * Layout template:
 *
 * (___, ___, ___, ___, ___, ___, ___,
 *  ___, ___, ___, ___, ___, ___, ___,
 *  ___, ___, ___, ___, ___, ___,
 *  ___, ___, ___, ___, ___, ___, ___,
 *  ___, ___, ___, ___,
 *  ___,
 *
 *  ___, ___, ___, ___, ___, ___, ___,
 *  ___, ___, ___, ___, ___, ___, ___,
 *       ___, ___, ___, ___, ___, ___,
 *  ___, ___, ___, ___, ___, ___, ___,
 *  ___, ___, ___, ___,
 *  ___)
 */

/* Aliases:
 *
 * These aliases serve a few purposes:
 *
 * 1. The OS is in dvorak mode, which means it translates an eg S keypress to an O keypress, etc.
 *    Notably, it also maps, eg, a ] to a =.
 *    When we remap our symbols, we would like to say "put a = here", rather than "put a ] here".
 *    This means we would like the Key_RightBracket name to mean the code associated to Key_Equals.
 * 2. The OS swaps left control and caps lock.
 *    This ensures that when we switch to the laptop keyboard, control & capslock are as desired.
 *    We would like to, in our keymaps, say things like LeftControl where we want LeftControl.
 *    Thus, we would like Key_LeftControl to mean the code associated to Key_CapsLock & vice versa.
 *
 * Rather than trying to change what the Key_* symbols expand to, we simply use the K_* namespace.
 * For uniformity, we also bring a bunch of other codes (such as arrow codes, delete codes, etc.)
 * into the K_* namespace, so that we don't need to worry about which ones have been remapped.
 *
 * TODO: this doesn't play nice w/ things like TOPSY.
 *
 */

#define UNUSED XXX  // marks keys that are "open"

// Dvorak -> Qwerty

// Modifiers
#define K_LeftControl Key_CapsLock
#define K_CapsLock Key_LeftControl

#define K_LeftAlt Key_LeftAlt
#define K_RightAlt Key_RightAlt
#define K_RightControl Key_RightControl
#define K_LeftGui Key_LeftGui
#define K_RightGui Key_RightGui
#define K_LeftShift Key_LeftShift
#define K_RightShift Key_RightShift

// Row 5
#define K_F1 Key_F1
#define K_F2 Key_F2
#define K_F3 Key_F3
#define K_F4 Key_F4
#define K_F5 Key_F5
#define K_F6 Key_F6
#define K_F7 Key_F7
#define K_F8 Key_F8
#define K_F9 Key_F9
#define K_F10 Key_F10
#define K_F11 Key_F11
#define K_F12 Key_F12

// Row 4
#define K_Backtick Key_Backtick
#define K_1 Key_1
#define K_2 Key_2
#define K_3 Key_3
#define K_4 Key_4
#define K_5 Key_5
#define K_6 Key_6
#define K_7 Key_7
#define K_8 Key_8
#define K_9 Key_9
#define K_0 Key_0
#define K_LeftBracket Key_Minus
#define K_RightBracket Key_Equals
#define K_Backspace Key_Backspace

// Row 3
#define K_Tab Key_Tab
#define K_Quote Key_Q
#define K_Comma Key_W
#define K_Period Key_E
#define K_P Key_R
#define K_Y Key_T
#define K_F Key_Y
#define K_G Key_U
#define K_C Key_I
#define K_R Key_O
#define K_L Key_P
#define K_Slash Key_LeftBracket
#define K_Equals Key_RightBracket
#define K_Backslash Key_Backslash

// Row 2
#define K_A Key_A
#define K_O Key_S
#define K_E Key_D
#define K_U Key_F
#define K_I Key_G
#define K_D Key_H
#define K_H Key_J
#define K_T Key_K
#define K_N Key_L
#define K_S Key_Semicolon
#define K_Minus Key_Quote
#define K_Enter Key_Enter

// Row 1
#define K_Semicolon Key_Z
#define K_Q Key_X
#define K_J Key_C
#define K_K Key_V
#define K_X Key_B
#define K_B Key_N
#define K_M Key_M
#define K_W Key_Comma
#define K_V Key_Period
#define K_Z Key_Slash

// Other

#define K_Spacebar Key_Spacebar
#define K_Escape Key_Escape

#define K_NextTrack Consumer_ScanNextTrack
#define K_PrevTrack Consumer_ScanPreviousTrack
#define K_PlayPause Consumer_PlaySlashPause
#define K_Mute Key_Mute
#define K_VolumeUp Key_VolumeUp
#define K_VolumeDown Key_VolumeDown

#define K_LEDEffectNext Key_LEDEffectNext
#define K_LEDEffectPrevious Key_LEDEffectPrevious

#define K_Power Key_Power

#define K_Delete Key_Delete
#define K_Home Key_Home
#define K_End Key_End
#define K_Insert Key_Insert
#define K_PageUp Key_PageUp
#define K_PageDown Key_PageDown
#define K_PrintScreen Key_PrintScreen
#define K_NumLock Key_NumLock

#define K_UpArrow Key_UpArrow
#define K_DownArrow Key_DownArrow
#define K_LeftArrow Key_LeftArrow
#define K_RightArrow Key_RightArrow

#define K_LeftClick Key_mouseBtnL
#define K_RightClick Key_mouseBtnR
#define K_MiddleClick Key_mouseBtnM

#define K_Brighter CONSUMER_KEY(0x6f, HID_TYPE_OSC)
#define K_Darker CONSUMER_KEY(0x70, HID_TYPE_OSC)


enum { DVORAK, FUNCTION };  // layers

// TODO:
// Some sort of numlock?
//
// Can compose key be one-shotted, and alt when held down? (Useful for eg Alt+# patterns)
//
// Shifted '
// Shifted ,
// Shifted .

// *INDENT-OFF*
KEYMAPS(
  /* Dvorak base layer:
   *
   * ┌───┬───┬───┬───┬───┬───┬───┐                   ┌───┬───┬───┬───┬───┬───┬───┐
   * │Prg│Bri│PgU│Hom│Mut│Ins│LED│                   │Any│Prn│PrT│End│PgD│Dar│Pow│
   * ├───┼───┼───┼───┼───┼───┼───┤                   ├───┼───┼───┼───┼───┼───┼───┤
   * │ ! │ ' │ , │ . │ p │ y │Ply│                   │Vol│ f │ g │ c │ r │ l │ ? │
   * ├───┼───┼───┼───┼───┼───┤Pau│                   │Up ├───┼───┼───┼───┼───┼───┤
   * │ " │ a │ o │ e │ u │ i ├───┘                   └───┤ d │ h │ t │ n │ s │ - │
   * ├───┼───┼───┼───┼───┼───┼───┐                   ┌───┼───┼───┼───┼───┼───┼───┤
   * │Alt│ ; │ q │ j │ k │ x │ ↓ │                   │ ↑ │ b │ m │ w │ v │ z │Clk│
   * └───┴───┴───┴───┴───┼───┼───┼───┬───┐   ┌───┬───┼───┼───┼───┴───┴───┴───┴───┘
   *                     │Cmp│Ctl│Shf│Cmd│   │Tab│Bsp│Spc│Esc│
   *                     └───┴───┼───┼───┘   └───┼───┼───┴───┘
   *                             │Fnc│           │Ent│
   *                             └───┘           └───┘
   *
   * ┌───┬───┬───┬───┬───┬───┬───┐                   ┌───┬───┬───┬───┬───┬───┬───┐
   * │Prg│Ins│PgU│Hom│Mut│Bri│LED│                   │Any│Dar│Prv│End│PgD│Prn│Pow│
   * ├───┼───┼───┼───┼───┼───┼───┤                   ├───┼───┼───┼───┼───┼───┼───┤
   * │ ! │ ' │ , │ . │ P │ Y │Ply│                   │Vol│ F │ G │ C │ R │ L │ ? │
   * ├───┼───┼───┼───┼───┼───┤Pau│                   │Up ├───┼───┼───┼───┼───┼───┤
   * │ " │ A │ O │ E │ U │ I ├───┘                   └───┤ D │ H │ T │ N │ S │ _ │
   * ├───┼───┼───┼───┼───┼───┼───┐                   ┌───┼───┼───┼───┼───┼───┼───┤
   * │Alt│ : │ Q │ J │ K │ X │ ↓ │                   │ ↑ │ B │ M │ W │ V │ Z │Clk│
   * └───┴───┴───┴───┴───┼───┼───┼───┬───┐   ┌───┬───┼───┼───┼───┴───┴───┴───┴───┘
   *                     │Cmp│Ctl│Shf│Cmd│   │Tab│Bsp│Spc│Esc│
   *                     └───┴───┼───┼───┘   └───┼───┼───┴───┘
   *                             │Fnc│           │Ent│
   *                             └───┘           └───┘
   */
  [DVORAK] = KEYMAP_STACKED
  // Note that remapping PRG to Power a bad idea, b/c it makes flashing real annoying.
  (XXX,              K_Brighter,  K_PageUp, K_Home,   K_Mute, K_Insert, K_LEDEffectNext,
   LSHIFT(K_1),      K_Quote,     K_Comma,  K_Period, K_P,    K_Y,      K_PlayPause,
   LSHIFT(K_Quote),  K_A,         K_O,      K_E,      K_U,    K_I,
   K_LeftAlt,        K_Semicolon, K_Q,      K_J,      K_K,    K_X,      K_DownArrow,

   K_RightAlt, K_LeftControl, K_LeftShift, K_LeftGui,
   ShiftToLayer(FUNCTION),

   M(MACRO_ANY), K_PrintScreen, K_PrevTrack, K_End, K_PageDown, K_Darker, K_Power,
   K_VolumeUp,   K_F,           K_G,         K_C,   K_R,        K_L,      LSHIFT(K_Slash),
                 K_D,           K_H,         K_T,   K_N,        K_S,      K_Minus,
   K_UpArrow,    K_B,           K_M,         K_W,   K_V,        K_Z,      K_CapsLock,

   K_Tab, K_Backspace, K_Spacebar, K_Escape,
   K_Enter),

  /* Function layer:
   *
   * ┌───┬───┬───┬───┬───┬───┬───┐                   ┌───┬───┬───┬───┬───┬───┬───┐
   * │F1 │F2 │F3 │F4 │F5 │F6 │LBk│                   │Any│F7 │F8 │F9 │F10│F11│F12│
   * ├───┼───┼───┼───┼───┼───┼───┤                   ├───┼───┼───┼───┼───┼───┼───┤
   * │ ` │ & │ < │ / │ [ │ { │Nxt│                   │Vol│ } │ ] │ \ │ > │ | │ @ │
   * ├───┼───┼───┼───┼───┼───┤Trk│                   │Dn ├───┼───┼───┼───┼───┼───┤
   * │ ~ │ ^ │ # │ % │ : │ ( ├───┘                   └───┤ ) │ = │ + │ * │ $ │ - │
   * ├───┼───┼───┼───┼───┼───┼───┐                   ┌───┼───┼───┼───┼───┼───┼───┤
   * │Alt│ 1 │ 2 │ 3 │ 4 │ 5 │ ← │                   │ → │ 6 │ 7 │ 8 │ 9 │ 0 │Clk│
   * └───┴───┴───┴───┴───┼───┼───┼───┬───┐   ┌───┬───┼───┼───┼───┴───┴───┴───┴───┘
   *                     │Cmp│Ctl│Shf│Cmd│   │Tab│Del│Spc│Esc│
   *                     └───┴───┼───┼───┘   └───┼───┼───┴───┘
   *                             │Fnc│           │Ent│
   *                             └───┘           └───┘
   */
  [FUNCTION] =  KEYMAP_STACKED
  (K_F1,               K_F2,        K_F3,            K_F4,        K_F5,                K_F6,                  K_LEDEffectNext,
   K_Backtick,         LSHIFT(K_7), LSHIFT(K_Comma), K_Slash,     K_LeftBracket,       LSHIFT(K_LeftBracket), K_NextTrack,
   LSHIFT(K_Backtick), LSHIFT(K_6), LSHIFT(K_3),     LSHIFT(K_5), LSHIFT(K_Semicolon), LSHIFT(K_9),
   ___,                K_1,         K_2,             K_3,         K_4,                 K_5,                   K_LeftArrow,

   ___, ___, ___, ___,
   ShiftToLayer(FUNCTION),

   ___,           K_F7,                   K_F8,           K_F9,             K_F10,            K_F11,               K_F12,
   K_VolumeDown,  LSHIFT(K_RightBracket), K_RightBracket, K_Backslash,      LSHIFT(K_Period), LSHIFT(K_Backslash), LSHIFT(K_2),
                  LSHIFT(K_0),            K_Equals,       LSHIFT(K_Equals), LSHIFT(K_8),      LSHIFT(K_4),         K_Minus,
   K_RightArrow, K_6,                    K_7,            K_8,              K_9,              K_0,                 ___,

   ___, K_Delete, ___, ___,
   ___)
 
);
// *INDENT-ON*

/* ------------------------------------------------------------------------------------------------
 *  Main setup & loop functions
 * ---------------------------------------------------------------------------------------------- */

KALEIDOSCOPE_INIT_PLUGINS(
  Macros,
  TopsyTurvy,
  BootGreetingEffect,
  LEDControl,
  LEDOff,
  LEDRainbowWaveEffect,
  LEDDigitalRainEffect,
  solidRed, solidOrange, solidYellow, solidGreen, solidBlue, solidIndigo, solidViolet,
  StalkerEffect,
  AlphaSquareEffect,
  LEDRainbowEffect,
  LEDBreatheEffect
);

void setup() {
  Kaleidoscope.setup();
  config_and_use_LED_modes();
}

void loop() {
  Kaleidoscope.loop();
}
