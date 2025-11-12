/**
 * @file
 * @brief Implementation of the application state management library.
 * @details
 * Provides the function definitions for managing input/output values and
 * state machines, as declared in @ref appStateLibrary.h.
 */

#include "appStateLibrary.h"

static const app_state_plc_state_string_t plc_state_string = {
    .PROG = "Prog",
    .STOP = "Stop",
    .RESET = "Reset",
    .SET = "Set",
    .TEST = "Test",
    .RUN = "Run",
};

const app_state_plc_state_string_t* get_app_state_plc_state_string_address(){
    return &plc_state_string;
}

const char *get_app_state_plc_state_string(app_state_plc_state_enum_t state) {

  switch (state) {
  case PROG: {
    return plc_state_string.PROG;
    break;
  }
  case STOP: {
    return plc_state_string.STOP;
    break;
  }
  case RESET: {
    return plc_state_string.RESET;
    break;
  }
  case SET: {
    return plc_state_string.SET;
    break;
  }
  case TEST: {
    return plc_state_string.TEST;
    break;
  }
  case RUN: {
    return plc_state_string.RUN;
    break;
  }
  }
}

// values can be initialized at a compile time
app_state_t appState = {
    .button_s1.filter.input = false,
    .button_s1.filter.output = false,
};

app_state_t *get_app_state_address() { return &appState; }

void init_app_state() {
  appState.button_s1.filter.input = false;
  appState.button_s1.filter.output = false;
}

void view_app_state(){
    app_state_t *app_state = get_app_state_address();
}
