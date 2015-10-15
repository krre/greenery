#pragma once
#include <QtCore>
#include "osgbridge/osg/positionattitudetransform.h"

class Utils : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString buildDate READ buildDate CONSTANT)
    Q_PROPERTY(QString homePath READ homePath CONSTANT)

public:
    QString buildDate() { return QString(__DATE__); }
    Q_INVOKABLE QString urlToPath(QUrl url);
    Q_INVOKABLE QString urlToFileName(QUrl url);
    QString homePath() { return QStandardPaths::writableLocation(QStandardPaths::HomeLocation); }
    Q_INVOKABLE static bool isFileExists(const QString& filePath);
    Q_INVOKABLE static void removeFile(const QString& path);
    Q_INVOKABLE PositionAttitudeTransform* findUnit(const PositionAttitudeTransform* parentUnit, const QString& nameUnit) const;
};
