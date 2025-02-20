
#ifndef CVM_EXPORT_H
#define CVM_EXPORT_H

#ifdef CVM_STATIC_DEFINE
#  define CVM_EXPORT
#  define CVM_NO_EXPORT
#else
#  ifndef CVM_EXPORT
#    ifdef cvm_EXPORTS
        /* We are building this library */
#      define CVM_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define CVM_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef CVM_NO_EXPORT
#    define CVM_NO_EXPORT 
#  endif
#endif

#ifndef CVM_DEPRECATED
#  define CVM_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CVM_DEPRECATED_EXPORT
#  define CVM_DEPRECATED_EXPORT CVM_EXPORT CVM_DEPRECATED
#endif

#ifndef CVM_DEPRECATED_NO_EXPORT
#  define CVM_DEPRECATED_NO_EXPORT CVM_NO_EXPORT CVM_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CVM_NO_DEPRECATED
#    define CVM_NO_DEPRECATED
#  endif
#endif

#endif /* CVM_EXPORT_H */
