#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#ifdef WIN32
#define NOMINMAX

#define atoll _atoi64
#endif
