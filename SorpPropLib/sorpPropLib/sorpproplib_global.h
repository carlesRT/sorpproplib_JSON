#ifndef SORPPROPLIB_GLOBAL_H
#define SORPPROPLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SORPPROPLIB_LIBRARY)
#  define SORPPROPLIBSHARED_EXPORT Q_DECL_EXPORT
#  define DLL_API_EXPORT __declspec(dllexport)
#else
#  define SORPPROPLIBSHARED_EXPORT Q_DECL_IMPORT
#  define DLL_API_EXPORT DECL_EXPORT __declspec(dllimport)
#endif

#endif // SORPPROPLIB_GLOBAL_H
