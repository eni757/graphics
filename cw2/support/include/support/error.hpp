#ifndef SUPPORT_ERROR_HPP_6E0C75E3_1CD1_4840_9DFC_D32F86BCD697
#define SUPPORT_ERROR_HPP_6E0C75E3_1CD1_4840_9DFC_D32F86BCD697

#include <string>
#include <format>
#include <utility>
#include <exception>

#include "defaults.hpp"

#if COMP3812_CONF_USE_STACKTRACE
#	include <stacktrace>
#endif // ~ USE_STACKTRACE

// Class used for exceptions. Unlike e.g. std::runtime_error, which only
// accepts a "fixed" string, Error provides std::print()-like formatting.
//
// Example:
//
//	throw Error( "glGetError() returned {}", glerr );
//
class Error : public std::exception
{
	public:
		template< typename... tArgs >
		explicit Error( std::format_string<tArgs...>, tArgs&&... );

	public:
		char const* what() const noexcept override;

	private:
		std::string mMsg;

#		if COMP3812_CONF_USE_STACKTRACE
		std::stacktrace mTrace = std::stacktrace::current(1);
		mutable std::string mBuffer;
#		endif // ~ USE_STACKTRACE
};

template< typename... tArgs > inline
Error::Error( std::format_string<tArgs...> aFmt, tArgs&&... aArgs )
	: mMsg( std::format( aFmt, std::forward<tArgs>(aArgs)... ) )
{}

#endif // SUPPORT_ERROR_HPP_6E0C75E3_1CD1_4840_9DFC_D32F86BCD697
