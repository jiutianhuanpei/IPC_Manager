#ifndef HBNET_GLOBAL_H
#define HBNET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HBNET_LIBRARY)
#  define HBNET_EXPORT Q_DECL_EXPORT
#else
#  define HBNET_EXPORT Q_DECL_IMPORT
#endif

#define HBClientIdKey QString("HBClientId")

#endif // HBNET_GLOBAL_H
