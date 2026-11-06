#include <support/error.hpp>
#include <support/defaults.hpp>

char const* Error::what() const noexcept
{
#	if !COMP3812_CONF_USE_STACKTRACE
	return mMsg.c_str();
#	else // USE_STACKTRACE
	// Formatting via std::to_string avoids depending on __cpp_lib_formatters,
	// support for which has come later in some cases.
	if( mBuffer.empty() )
		mBuffer = std::format( "{}\n-- from --\n{}--\n", mMsg, std::to_string(mTrace) );

	return mBuffer.c_str();
#	endif // ~ USE_STACKTRACE
}


