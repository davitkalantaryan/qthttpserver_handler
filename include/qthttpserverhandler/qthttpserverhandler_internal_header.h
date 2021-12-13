//
// file:			qthttpserverhandler_internal_header.h
// path:			${repositoryRoot}/include/qthttpserverhandler/qthttpserverhandler_internal_header.h
// created on:		2021 Dec 13
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#ifndef INCLUDE_QTHTTPSERVERHANDLER_QTHTTPSERVERHANDLER_INTERNAL_HEADER_H
#define INCLUDE_QTHTTPSERVERHANDLER_QTHTTPSERVERHANDLER_INTERNAL_HEADER_H

#ifdef QTHTTPSERVERHANDLER_USE_CPPUTILS_INTERNAL_HEADER
#include <cpputils_internal_header.h>
#endif

#if defined(__EMSCRIPTEN__) || defined(EMSCRIPTEN) || defined(QTHTTPSERVERHANDLER_WASM)
#define QTHTTPSERVERHANDLER_EMSCRIPTEN_IS_USED
#endif

#ifdef SYSTEM_EXE_START_IS_POSSIBLE
#define QTHTTPSERVERHANDLER_EXE_START_IS_POSSIBLE
#endif

#if !defined(QTHTTPSERVERHANDLER_EMSCRIPTEN_IS_USED) // todo: continue list
#define QTHTTPSERVERHANDLER_DESKTOP_IS_USED
#endif

#if !defined(QTHTTPSERVERHANDLER_EMSCRIPTEN_IS_USED) || defined(USE_DLOPEN_FROM_WASM)
#define QTHTTPSERVERHANDLER_DLOPEN_IS_POSSIBLE
#endif

#ifdef _MSC_VER
	#pragma warning (disable:4355)
    #define QTHTTPSERVERHANDLER_BEFORE_CPP_17_FALL_THR
	#if defined(_MSVC_LANG) && (_MSVC_LANG>=201100L)
        #define QTHTTPSERVERHANDLER_CPP_11_DEFINED		1
	#endif
	#if defined(_MSVC_LANG) && (_MSVC_LANG>=201400L)
        #define QTHTTPSERVERHANDLER_CPP_14_DEFINED		1
	#endif
	#if defined(_MSVC_LANG) && (_MSVC_LANG>=201700L)
        #define QTHTTPSERVERHANDLER_CPP_17_DEFINED		1
	#endif
	#define QTHTTPSERVERHANDLER_THISCALL	__thiscall
    #define QTHTTPSERVERHANDLER_DLL_PUBLIC		__declspec(dllexport)
    #define QTHTTPSERVERHANDLER_DLL_PRIVATE
    #define QTHTTPSERVERHANDLER_IMPORT_FROM_DLL	__declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
    #define QTHTTPSERVERHANDLER_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
	#if defined(__cplusplus) && (__cplusplus>=201100L)
        #define QTHTTPSERVERHANDLER_CPP_11_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201400L)
        #define QTHTTPSERVERHANDLER_CPP_14_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201700L)
        #define QTHTTPSERVERHANDLER_CPP_17_DEFINED		1
	#endif
	#define QTHTTPSERVERHANDLER_THISCALL
    //#define QTHTTPSERVERHANDLER_DLL_PUBLIC		__attribute__((visibility("default")))
    #define QTHTTPSERVERHANDLER_DLL_PUBLIC
    #define QTHTTPSERVERHANDLER_DLL_PRIVATE		__attribute__((visibility("hidden")))
    #define QTHTTPSERVERHANDLER_IMPORT_FROM_DLL
#elif defined(__CYGWIN__)
    #define QTHTTPSERVERHANDLER_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
	#if defined(__cplusplus) && (__cplusplus>=201100L)
        #define QTHTTPSERVERHANDLER_CPP_11_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201400L)
        #define QTHTTPSERVERHANDLER_CPP_14_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201700L)
        #define QTHTTPSERVERHANDLER_CPP_17_DEFINED		1
	#endif
	#define QTHTTPSERVERHANDLER_THISCALL
    #define QTHTTPSERVERHANDLER_DLL_PUBLIC		__attribute__((dllexport))
    #define QTHTTPSERVERHANDLER_DLL_PRIVATE
    #define QTHTTPSERVERHANDLER_IMPORT_FROM_DLL	__attribute__((dllimport))
#elif defined(__MINGW64__) || defined(__MINGW32__)
    #define QTHTTPSERVERHANDLER_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
	#if defined(__cplusplus) && (__cplusplus>=201100L)
        #define QTHTTPSERVERHANDLER_CPP_11_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201400L)
        #define QTHTTPSERVERHANDLER_CPP_14_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201700L)
        #define QTHTTPSERVERHANDLER_CPP_17_DEFINED		1
	#endif
	#define QTHTTPSERVERHANDLER_THISCALL
    #define QTHTTPSERVERHANDLER_DLL_PUBLIC		_declspec(dllexport)
    #define QTHTTPSERVERHANDLER_DLL_PRIVATE
    #define QTHTTPSERVERHANDLER_IMPORT_FROM_DLL	_declspec(dllimport)
#elif defined(__SUNPRO_CC)
    // #define QTHTTPSERVERHANDLER_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ; // ???
    #define QTHTTPSERVERHANDLER_BEFORE_CPP_17_FALL_THR
	#if defined(__cplusplus) && (__cplusplus>=201100L)
        #define QTHTTPSERVERHANDLER_CPP_11_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201400L)
        #define QTHTTPSERVERHANDLER_CPP_14_DEFINED		1
	#endif
	#if defined(__cplusplus) && (__cplusplus>=201700L)
        #define QTHTTPSERVERHANDLER_CPP_17_DEFINED		1
	#endif
	#define QTHTTPSERVERHANDLER_THISCALL
    #define QTHTTPSERVERHANDLER_DLL_PUBLIC
    #define QTHTTPSERVERHANDLER_DLL_PRIVATE		__hidden
    #define QTHTTPSERVERHANDLER_IMPORT_FROM_DLL
#endif  // #ifdef _MSC_VER

#if defined(QTHTTPSERVERHANDLER_COMPILING_SHARED_LIB)
    #define QTHTTPSERVERHANDLER_EXPORT QTHTTPSERVERHANDLER_DLL_PUBLIC
#elif defined(QTHTTPSERVERHANDLER_USING_STATIC_LIB_OR_OBJECTS)
    #define QTHTTPSERVERHANDLER_EXPORT
#else
    #define QTHTTPSERVERHANDLER_EXPORT QTHTTPSERVERHANDLER_IMPORT_FROM_DLL
#endif

#ifdef __cplusplus
#define QTHTTPSERVERHANDLER_BEGIN_C   extern "C" {
#define QTHTTPSERVERHANDLER_END_C     }
#define QTHTTPSERVERHANDLER_EXTERN_C  extern "C"
#else
#define QTHTTPSERVERHANDLER_BEGIN_C
#define QTHTTPSERVERHANDLER_END_C
#define QTHTTPSERVERHANDLER_EXTERN_C
#endif


#ifndef QTHTTPSERVERHANDLER_FALLTHROUGH
#ifdef QTHTTPSERVERHANDLER_CPP_17_DEFINED
#define QTHTTPSERVERHANDLER_FALLTHROUGH	[[fallthrough]] ;
#else
#define QTHTTPSERVERHANDLER_FALLTHROUGH	QTHTTPSERVERHANDLER_BEFORE_CPP_17_FALL_THR
//#define QTHTTPSERVERHANDLER_FALLTHROUGH
#endif
#endif

#ifndef QTHTTPSERVERHANDLER_NULL
#ifdef QTHTTPSERVERHANDLER_CPP_11_DEFINED
#define QTHTTPSERVERHANDLER_NULL	nullptr
#else
#define QTHTTPSERVERHANDLER_NULL	NULL
#endif
#endif

#ifndef QTHTTPSERVERHANDLER_REGISTER
#ifdef __cplusplus
#define QTHTTPSERVERHANDLER_REGISTER
#else
#define QTHTTPSERVERHANDLER_REGISTER	register
#endif
#endif

//#define QTHTTPSERVERHANDLER_IS_LITTLE_ENDIAN (((union { unsigned x; unsigned char c; }){1}).c)

#if defined(_DEBUG) || defined(CPPUTILS_DEBUG) || defined(QTHTTPSERVERHANDLER_DEBUG)
// we have debug compilation
#else
// we have release
#ifndef NDEBUG
// let's define NDEBUG (No DEBUG)
#define NDEBUG
#endif
#endif

#ifdef NDEBUG
//#define MAKE_VOID(_val)                 static_cast<void>(_val)
#define DO_DEBUG_EXP(_exp)
#define QTHTTPSERVERHANDLER_SAFE_CAST(_type,_val)	static_cast<_type>(_val)
#else
//#define MAKE_VOID(_var)                 do{}while(0)
//#define MAKE_VOID(_var)
#define DO_DEBUG_EXP(_exp)              _exp ;
#define QTHTTPSERVERHANDLER_SAFE_CAST(_type,_val)	dynamic_cast<_type>(_val)
#endif


#ifdef __cplusplus
#define QTHTTPSERVERHANDLER_STATIC_CAST(_type,_val)    static_cast<_type>(_val)
#else
#define QTHTTPSERVERHANDLER_STATIC_CAST(_type,_val)    ( (_type)(_val) )
#endif


#define QTHTTPSERVERHANDLER_STRINGIFY(_x)			QTHTTPSERVERHANDLER_STRINGIFY_PRIVATE(_x)
#define QTHTTPSERVERHANDLER_STRINGIFY_PRIVATE(_x)	#_x


#define QTHTTPSERVERHANDLER_QT_NSP  ::

#define QTHTTPSERVERHANDLER_NO_NULL


#endif  // #ifndef INCLUDE_QTHTTPSERVERHANDLER_QTHTTPSERVERHANDLER_INTERNAL_HEADER_H
