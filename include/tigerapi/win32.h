//
// Created by sukai on 2022/12/22.
//
#ifndef OPEN_API_SDK_WIN32_HEADER_
#define OPEN_API_SDK_WIN32_HEADER_

#if defined (OPEN_API_BUILD_DLL)
#  if defined (INSIDE_OPENAPI)
#    define OPENAPI_EXPORT __declspec(dllexport)
#  else
#    define OPENAPI_EXPORT __declspec(dllimport)
#  endif
#else
#  define OPENAPI_EXPORT
#endif

#endif // OPEN_API_SDK_WIN32_HEADER_
