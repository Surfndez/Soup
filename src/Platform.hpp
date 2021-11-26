#pragma once

#if defined(_WIN32) || defined(WIN32)
#define SOUP_PLATFORM_WINDOWS true
#else
#define SOUP_PLATFORM_WINDOWS false
#endif

namespace Soup
{
	struct Platform
	{
		static constexpr bool windows = SOUP_PLATFORM_WINDOWS;
	};
}
