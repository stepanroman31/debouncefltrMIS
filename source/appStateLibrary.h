/**
 * @file
 * @date 18.08.2025
 * @author Petr Zakopal, Jan Bauer
 * @version 0.0.1
 * @brief Library for managing the application state.
 * @details
 * This library provides an interface for managing input and output values
 * of algorithms or functions. Its primary use in the MIS course is for
 * handling state data, but its applicability is not limited to that context.
 *
 * It can also be used to manage the state of state machines or algorithms
 * whose values must be visible across different parts of the application,
 * rather than being confined to a single module.
 */

#ifndef APP_STATE_LIB
#define APP_STATE_LIB

#include <stdbool.h>

/**
 * @brief Interface for filtering the inputs from
 * the physical interfaces of the board.
 */
typedef struct {
  bool input;  ///< Input value to the filter, generally obtained from the
               ///< `platformMIS` API.
  bool output; ///< Resolved output value after performing the filtering.
} app_state_filter_t;

/**
 * @brief Interface for buttons.
 */
typedef struct {
  app_state_filter_t filter; ///< Filter elements
  bool outputMemory;         ///< Memory output
} app_state_button_t;

/**
 * @brief Interface for the decoder algorithm.
 * @note Additional properties like `outputIn[ModifiedRange]` where
 * `[ModifiedRange]` can be change by the user can be freely added to this
 * struct by the user.
 */
typedef struct {
  bool isMin; ///< Has the value in `outputInBaseRange` reached the minimum
              ///< value available?
  bool isMax; ///< Has the value in `outputInBaseRange` reached the maximum
              ///< value available?
  unsigned char outputInBaseRange; ///< The resolved value of the decoder in the
                                   ///< base range <0,255>.
  bool outAFiltered; ///< Filtered output of A signal
  bool outBFiltered; ///< Filtered output of B signal
} app_state_decoder_t;

/**
 * @brief Interface for the Analog Digital Converter.
 * @note Additional properties like `outputIn[ModifiedRange]` where
 * `[ModifiedRange]` can be change by the user can be freely added to this
 * struct by the user.
 */
typedef struct {
  int inputUnformatted;   ///< Unformatted input value as obtained from the
                          ///< `platformMIS` API using `getPotentiometerValue()`
                          ///< call.
  char outputInBaseRange; ///< The resolved value of the ADC in the base range
                          ///< <0,255>.
  bool isMin; ///< Has the value in `outputInBaseRange` reached the minimum
              ///< value available?
  bool isMax; ///< Has the value in `outputInBaseRange` reached the maximum
              ///< value available?

} app_state_adc_t;


/**
 * @brief Interface for the PWM modulator.
 */
typedef struct {
  unsigned char input; ///< Input values passed to the modulator either from Decoder, ADC or RTM. Must be in range <0,255>.
  unsigned int outputCompare; ///< Value passed to the Output Compare registers.
  unsigned char outputRTMInDegree; ///< RTM converted value of `input` to degrees in the range <0,90>.

} app_state_pwm_t;

/**
 * @brief States of the PLC state machine.
 */
typedef enum {
    PROG, ///< Prog - Init state
    STOP, ///< Stopped
    RESET, ///< Reset
    SET, ///< Set
    TEST, ///< Test
    RUN, ///< Run
} app_state_plc_state_enum_t;


/**
 * @brief String of states of the PLC state machine used for TableTerminal.
 */
typedef struct {
    const char* PROG;
    const char* RESET;
    const char* STOP;
    const char* SET;
    const char* TEST;
    const char* RUN;
} app_state_plc_state_string_t;

/**
 * @brief Returns a string based on the requested application state.
 * @details
 * Can be used to resolve the string value from the enum state
 * for the RTM TableTerminal.
 *
 * @code
 * printf("%s\n", get_app_state_plc_state_string(TEST));
 * @endcode
 */
const char* get_app_state_plc_state_string(app_state_plc_state_enum_t state);

/**
 * @brief Returns the address of the `plc_state_string` struct
 * which holds the human redable string values usable for RTM TableTerminal.
 */
const app_state_plc_state_string_t* get_app_state_plc_state_string_address();

/**
 * @brief Interface for the PLC.
 */
typedef struct {
    app_state_plc_state_enum_t state;
    unsigned char outputCurrentlyUsedForOutputCompare; ///< Output of the PLC for Output Compare - the duty cycle in range <0,255>.
    unsigned char outputRTMInDegree; ///< RTM converted value of `outputCurrentlyUsedForOutputCompare` in the range <0,90>.
} app_state_plc_t;

/**
 * @brief Interface which holds convenient variables which
 * are transitioned between RTM module and the rest of the application.
 */
typedef struct {

} app_state_rtm_t;

/**
 * @brief The Application State.

 */
typedef struct {
  // buttons
  app_state_button_t button_s1; ///< Button S1
  app_state_button_t button_s2; ///< Button S2
  app_state_button_t button_s3; ///< Button S3
  app_state_button_t button_s4; ///< Button S4
  app_state_button_t button_s5; ///< Button S5
  app_state_button_t button_s6; ///< Button S6
  app_state_button_t button_s7; ///< Button S7
  app_state_button_t button_s8; ///< Button S8

  // decoder
  app_state_decoder_t decoder_0; ///< Decoder

  // ADC
  app_state_adc_t adc_0; ///< ADC

  // PWM
  app_state_pwm_t pwm_0; ///< PWM

  // RTM
  app_state_rtm_t rtm; ///< RTM
} app_state_t;

/**
 * @brief Returns the address of the application state variable.
 * @details Is used to obtain the address of the general application state.
 * Because it returns the address of the variable, the values of the variables
 * can be changed.
 */
app_state_t *get_app_state_address();

/**
 * @brief Initializes the app state variable at a run time.
 * @note Runs only once.
 */
void init_app_state();

/**
 * @brief Used for debugging to stop app on it and observe the values.
 * Because PIC cannot display the whole struct when stopping
 * the app somewhere, where the application state struct is not obtained.
 */
void view_app_state();
#endif
