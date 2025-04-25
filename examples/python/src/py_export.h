#ifndef PY_EXPORT_H
#define PY_EXPORT_H

#ifdef PY_STATIC_DEFINE
#define PY_EXPORT
#define PY_NO_EXPORT
#else
#ifndef PY_EXPORT
#ifdef c_vm_EXPORTS
/* We are building this library */
#define PY_EXPORT __attribute__((visibility("default")))
#else
/* We are using this library */
#define PY_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifndef PY_NO_EXPORT
#define PY_NO_EXPORT __attribute__((visibility("hidden")))
#endif
#endif

#ifndef PY_DEPRECATED
#define PY_DEPRECATED __attribute__((__deprecated__))
#endif

#ifndef PY_DEPRECATED_EXPORT
#define PY_DEPRECATED_EXPORT PY_EXPORT PY_DEPRECATED
#endif

#ifndef PY_DEPRECATED_NO_EXPORT
#define PY_DEPRECATED_NO_EXPORT PY_NO_EXPORT PY_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#ifndef PY_NO_DEPRECATED
#define PY_NO_DEPRECATED
#endif
#endif

#endif /* PY_EXPORT_H */
