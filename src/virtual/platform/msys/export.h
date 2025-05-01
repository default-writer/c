
#ifndef CVM_EXPORT_H
#define CVM_EXPORT_H

#ifdef CVM_STATIC_DEFINE
#  define CVM_EXPORT
#  define CVM_NO_EXPORT
#else
#  ifndef CVM_EXPORT
#    ifdef c_vm_EXPORTS
        /* We are building this library */
#      define CVM_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define CVM_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef CVM_NO_EXPORT
#    define CVM_NO_EXPORT __attribute__((visibility("hidden")))
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

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CVM_NO_DEPRECATED
#    define CVM_NO_DEPRECATED
#  endif
#endif

#endif /* CVM_EXPORT_H */
