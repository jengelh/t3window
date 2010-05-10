#ifndef T3_TERMINAL_H
#define T3_TERMINAL_H

/** @file */

/** @defgroup t3window_term Terminal manipulation functions. */
/** @defgroup t3window_other Contants, data types and miscellaneous functions. */

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>
#include "window_api.h"

/** @addtogroup t3window_other */
/** @{ */
/** @name Version information */
/*@{*/
/** The version of libt3window encoded as a single integer.

    The least significant 8 bits represent the patch level.
    The second 8 bits represent the minor version.
    The third 8 bits represent the major version.

	At runtime, the value of T3_WINDOW_VERSION can be retrieved by calling
	::t3_window_get_version.

    @internal
    The value 0 is an invalid value which should be replaced by the script
    that builds the release package.
*/
#define T3_WINDOW_VERSION 0

/* Although it doesn't make a lot of sense to put this function in either this
   file or in window.h, there is a good reason to put it in here: because
   window.h includes terminal.h, this function (and the macro) will always
   be available, regardless of which files the user includes. */
T3_WINDOW_API long t3_window_get_version(void);
/*@}*/

/** Boolean type that does not clash with C++ or C99 bool. */
typedef enum {t3_false, t3_true} t3_bool;

/** @typedef t3_attr_t
    @brief Type to hold attributes used for terminal display.

    The value of a ::t3_attr_t should be a bitwise or of T3_ATTR_* attribute values. When the
    terminal only supports setting colors by color pair, the ::T3_ATTR_FG macro can be used to
    specify the color pair to activate.
*/
#if INT_MAX < 2147483647L
typedef long t3_attr_t;
#else
typedef int t3_attr_t;
#endif

/** Data structure to store information about the capabilities of the terminal. */
typedef struct {
	t3_attr_t highlights; /**< The supported attributes other then color attributes. This is a bitmask of T3_ATTR_* flags. */
	int colors; /**< The maximum number of supported colors, or 0 if color is not supported. */
	int pairs; /**< The maximum number of color pairs that are supported by this terminal, or 0 if color is not supported. */
	int cap_flags; /**< A bitmask of T3_TERM_CAP flags indicating capabilities of the terminal. */
} t3_term_caps_t;

/** Terminal capability flag: terminal can set foreground. */
#define T3_TERM_CAP_FG (1 << 0)
/** Terminal capability flag: terminal can set foreground. */
#define T3_TERM_CAP_BG (1 << 1)
/** Terminal capability flag: terminal uses color pairs for setting color. */
#define T3_TERM_CAP_CP (1 << 2)

/** User callback type.
    The user callback is passed a pointer to the characters that are is marked with
    ::T3_ATTR_USER, the length of the string, the width of the string in display cells
    and the attributes they are drawn with.
*/
typedef void (*t3_attr_user_callback_t)(const char *str, int length, int width, t3_attr_t attr);

/** @name Attributes */
/*@{*/
/** Use callback for drawing the characters.

    When T3_ATTR_USER is set all other attribute bits are ignored. These can be used by
    the callback to determine the drawing style. The callback is set with ::t3_term_set_callback.
	Note that the callback is responsible for outputing the characters as well (using ::t3_term_putc).
*/
#define T3_ATTR_USER ((t3_attr_t) (1L << 0))
/** Draw characters with underlining. */
#define T3_ATTR_UNDERLINE ((t3_attr_t) (1L << 1))
/** Draw characters with bold face/bright appearance. */
#define T3_ATTR_BOLD ((t3_attr_t) (1L << 2))
/** Draw characters with reverse video. */
#define T3_ATTR_REVERSE ((t3_attr_t) (1L << 3))
/** Draw characters blinking. */
#define T3_ATTR_BLINK ((t3_attr_t) (1L << 4))
/** Draw characters with dim appearance. */
#define T3_ATTR_DIM ((t3_attr_t) (1L << 5))
/** Draw characters with alternate character set (for line drawing etc). */
#define T3_ATTR_ACS ((t3_attr_t) (1L << 6))

/** Bit number of the least significant color attribute bit. */
#define T3_ATTR_COLOR_SHIFT 7
/** Convert a color number to a foreground color attribute. */
#define T3_ATTR_FG(x) (((((t3_attr_t) (x)) & 0xff) + 1) << T3_ATTR_COLOR_SHIFT)
/** Convert a color number to a background color attribute. */
#define T3_ATTR_BG(x) (((((t3_attr_t) (x)) & 0xff) + 1) << (T3_ATTR_COLOR_SHIFT + 9))
/** Bitmask to leave only the foreground color in a ::t3_chardata_t value. */
#define T3_ATTR_FG_MASK (0x1ff << T3_ATTR_COLOR_SHIFT)
/** Bitmask to leave only the background color in a ::t3_chardata_t value. */
#define T3_ATTR_BG_MASK (0x1ff << (T3_ATTR_COLOR_SHIFT + 9))

/** Foreground color unspecified. */
#define T3_ATTR_FG_UNSPEC ((t3_attr_t) 0L)
/** Foreground color default. */
#define T3_ATTR_FG_DEFAULT (((t3_attr_t) 256) << (T3_ATTR_COLOR_SHIFT))
/** Foreground color black. */
#define T3_ATTR_FG_BLACK T3_ATTR_FG(0)
/** Foreground color red. */
#define T3_ATTR_FG_RED T3_ATTR_FG(1)
/** Foreground color green. */
#define T3_ATTR_FG_GREEN T3_ATTR_FG(2)
/** Foreground color yellow. */
#define T3_ATTR_FG_YELLOW T3_ATTR_FG(3)
/** Foreground color blue. */
#define T3_ATTR_FG_BLUE T3_ATTR_FG(4)
/** Foreground color magenta. */
#define T3_ATTR_FG_MAGENTA T3_ATTR_FG(5)
/** Foreground color cyan. */
#define T3_ATTR_FG_CYAN T3_ATTR_FG(6)
/** Foreground color white. */
#define T3_ATTR_FG_WHITE T3_ATTR_FG(7)

/** Background color unspecified. */
#define T3_ATTR_BG_UNSPEC ((t3_attr_t) 0L)
/** Background color default. */
#define T3_ATTR_BG_DEFAULT (((t3_attr_t) 256) << (T3_ATTR_COLOR_SHIFT + 9))
/** Background color black. */
#define T3_ATTR_BG_BLACK T3_ATTR_BG(0)
/** Background color red. */
#define T3_ATTR_BG_RED T3_ATTR_BG(1)
/** Background color green. */
#define T3_ATTR_BG_GREEN T3_ATTR_BG(2)
/** Background color yellow. */
#define T3_ATTR_BG_YELLOW T3_ATTR_BG(3)
/** Background color blue. */
#define T3_ATTR_BG_BLUE T3_ATTR_BG(4)
/** Background color magenta. */
#define T3_ATTR_BG_MAGENTA T3_ATTR_BG(5)
/** Background color cyan. */
#define T3_ATTR_BG_CYAN T3_ATTR_BG(6)
/** Background color white. */
#define T3_ATTR_BG_WHITE T3_ATTR_BG(7)
/*@}*/

/** Alternate character set symbolic constants. */
enum {
	T3_ACS_TTEE = 'w', /**< Tee pointing down. */
	T3_ACS_RTEE = 'u', /**< Tee pointing left. */
	T3_ACS_LTEE = 't', /**< Tee pointing right. */
	T3_ACS_BTEE = 'v', /**< Tee pointing up. */
	T3_ACS_ULCORNER = 'l', /**< Upper left corner. */
	T3_ACS_URCORNER = 'k', /**< Upper right corner. */
	T3_ACS_LLCORNER = 'm', /**< Lower left corner. */
	T3_ACS_LRCORNER = 'j', /**< Lower right corner. */
	T3_ACS_HLINE = 'q', /**< Horizontal line. */
	T3_ACS_VLINE = 'x', /**< Vertical line. */
	T3_ACS_UARROW = '-', /**< Arrow pointing up. */
	T3_ACS_DARROW = '.', /**< Arrow pointing down. */
	T3_ACS_LARROW = ',', /**< Arrow pointing left. */
	T3_ACS_RARROW = '+', /**< Arrow pointing right. */
	T3_ACS_BOARD = 'h', /**< Board of squares. */
	T3_ACS_CKBOARD = 'a' /**< Checker board pattern (stipple). */
};

#include "window_errors.h"

/** @name Error codes (libt3window specific) */
/*@{*/
/** Error code: no information found for the terminal in the terminfo database. */
#define T3_ERR_TERMINFODB_NOT_FOUND (-64)
/** Error code: the file descriptor is a hard-copy terminal. */
#define T3_ERR_HARDCOPY_TERMINAL (-63)
/** Error code: the file descriptor is not a terminal. */
#define T3_ERR_NOT_A_TTY (-62)
/** Error code: a timeout occured. */
#define T3_ERR_TIMEOUT (-61)
/** Error code: terminal provides too limited possibilities for the library to function. */
#define T3_ERR_TERMINAL_TOO_LIMITED (-60)
/** Error code: could not retrieve information about the size of the terminal window. */
#define T3_ERR_NO_SIZE_INFO (-59)
/** Error code: input contains non-printable characters. */
#define T3_ERR_NONPRINT (-58)

T3_WINDOW_API const char *t3_window_strerror(int error);
/*@}*/

/** @} */

T3_WINDOW_API int t3_term_init(int fd);
T3_WINDOW_API void t3_term_restore(void);
T3_WINDOW_API int t3_term_get_keychar(int msec);
T3_WINDOW_API void t3_term_set_cursor(int y, int x);
T3_WINDOW_API void t3_term_hide_cursor(void);
T3_WINDOW_API void t3_term_show_cursor(void);
T3_WINDOW_API void t3_term_get_size(int *height, int *width);
T3_WINDOW_API t3_bool t3_term_resize(void);
T3_WINDOW_API void t3_term_update(void);
T3_WINDOW_API void t3_term_redraw(void);
T3_WINDOW_API void t3_term_set_attrs(t3_attr_t new_attrs);
T3_WINDOW_API void t3_term_set_user_callback(t3_attr_user_callback_t callback);
T3_WINDOW_API int t3_term_get_keychar(int msec);
T3_WINDOW_API int t3_term_unget_keychar(int c);
T3_WINDOW_API void t3_term_putp(const char *str);
T3_WINDOW_API t3_bool t3_term_acs_available(int idx);

T3_WINDOW_API int t3_term_strwidth(const char *str);

T3_WINDOW_API t3_attr_t t3_term_combine_attrs(t3_attr_t a, t3_attr_t b);
T3_WINDOW_API t3_attr_t t3_term_get_ncv(void);
T3_WINDOW_API t3_attr_t t3_term_get_supported_attrs(void);

/** These are implemented in convert_output.c */
T3_WINDOW_API t3_bool t3_term_can_draw(const char *str, size_t str_len);
T3_WINDOW_API void t3_term_set_replacement_char(char c);
T3_WINDOW_API t3_bool t3_term_putc(char c);
T3_WINDOW_API void t3_term_disable_ansi_optimization(void);

/** Get terminal capabilities.
    @param caps The location to store the capabilites.
    @ingroup t3window_term

    This function can be used to obtain the supported video attributes and other information about
    the capabilities of the terminal. This define calls ::t3_term_get_caps_internal with the
    correct version argument.
*/
#define t3_term_get_caps(caps) t3_term_get_caps_internal((caps), T3_WINDOW_VERSION)

T3_WINDOW_API void t3_term_get_caps_internal(t3_term_caps_t *caps, size_t caps_size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
