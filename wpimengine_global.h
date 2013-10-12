#ifndef WBIMENGINE_GLOBAL_H
#define WBIMENGINE_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <mimenginetypes.h>
#include <mimenginewordsinterface.h>
#include <qstringlist.h>
#include <qstring.h>
#include <QLibrary>
#include <QList>

#if defined(WBIMENGINE_LIBRARY)
#  define WBIMENGINESHARED_EXPORT Q_DECL_EXPORT
#else
#  define WBIMENGINESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WBIMENGINE_GLOBAL_H
