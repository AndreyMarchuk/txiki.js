
#ifndef TJS_EXPORT_H
#define TJS_EXPORT_H

/*
 * For use in Windows DLLs
 */

#if defined( TJS_STATIC_DEFINE ) || !defined( _WIN32 )
#  define TJS_EXPORT
#  define TJS_NO_EXPORT
#else
#  ifndef TJS_EXPORT
#    ifdef tjs_EXPORTS
        /* We are building this library */
#      define TJS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define TJS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef TJS_NO_EXPORT
#    define TJS_NO_EXPORT 
#  endif
#endif

#ifndef TJS_DEPRECATED
#  if defined( _WIN32 )
#    define TJS_DEPRECATED __declspec(deprecated)
#  else
#    define TJS_DEPRECATED
#  endif
#endif

#ifndef TJS_DEPRECATED_EXPORT
#  define TJS_DEPRECATED_EXPORT TJS_EXPORT TJS_DEPRECATED
#endif

#ifndef TJS_DEPRECATED_NO_EXPORT
#  define TJS_DEPRECATED_NO_EXPORT TJS_NO_EXPORT TJS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TJS_NO_DEPRECATED
#    define TJS_NO_DEPRECATED
#  endif
#endif

#endif /* TJS_EXPORT_H */
