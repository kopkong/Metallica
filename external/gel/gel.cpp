#include "stxutif.h"

#ifdef __MINGW32_VERSION
#ifndef _GLIBCXX_USE_WCHAR_T

namespace std
{
	//instantiate the codecvt facet id for the required types
	locale::id codecvt<wchar_t,char,mbstate_t>::id;
}

#endif //_GLIBCXX_USE_WCHAR_T
#endif //__MINGW32_VERSION


namespace gel
{namespace stdx
{
	//instantiate the global locale
	std::locale utf8_locale(std::locale(), new utf8cvt<true>);

}}
