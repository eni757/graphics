#ifndef SUPPORT_DEFAULTS_HPP_A17C6993_F529_48CA_8AAF_0DDBF5249D7E
#define SUPPORT_DEFAULTS_HPP_A17C6993_F529_48CA_8AAF_0DDBF5249D7E

#include <version>

/* Compile time config: Use <stacktrace>?
 *
 * Enabled by default unless NDEBUG is specified. If your compiler doesn't
 * support it, you can disable it by defining COMP3812_CONF_USE_STACKTRACE=0.
 */
#if !defined(COMP3812_CONF_USE_STACKTRACE)
#	if defined(NDEBUG)
#		define COMP3812_CONF_USE_STACKTRACE 0
#	else
#		if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
#			define COMP3812_CONF_USE_STACKTRACE 1
#		else /* unsupported */
#			define COMP3812_CONF_USE_STACKTRACE 0
#		endif
#	endif
#endif // ~ COMP3812_CONF_USE_STACKTRACE

#endif // SUPPORT_DEFAULTS_HPP_A17C6993_F529_48CA_8AAF_0DDBF5249D7E
