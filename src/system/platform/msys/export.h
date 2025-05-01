
#ifndef CSYS_EXPORT_H
#define CSYS_EXPORT_H

#ifdef CSYS_STATIC_DEFINE
#  define CSYS_EXPORT
#  define CSYS_NO_EXPORT
#else
#  ifndef CSYS_EXPORT
#    ifdef c_sys_EXPORTS
        /* We are building this library */
#      define CSYS_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define CSYS_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef CSYS_NO_EXPORT
#    define CSYS_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef CSYS_DEPRECATED
#  define CSYS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CSYS_DEPRECATED_EXPORT
#  define CSYS_DEPRECATED_EXPORT CSYS_EXPORT CSYS_DEPRECATED
#endif

#ifndef CSYS_DEPRECATED_NO_EXPORT
#  define CSYS_DEPRECATED_NO_EXPORT CSYS_NO_EXPORT CSYS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CSYS_NO_DEPRECATED
#    define CSYS_NO_DEPRECATED
#  endif
#endif

#endif /* CSYS_EXPORT_H */
