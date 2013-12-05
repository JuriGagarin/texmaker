#ifndef TEXMAKERVERSION_H
#define TEXMAKERVERSION_H

#include <QString>
#include <QStringList>
#include <QLatin1String>
#include <QDebug>

class TexmakerVersion
{
public:
    TexmakerVersion();
    TexmakerVersion(int world, int major, int minor = 0);
    TexmakerVersion(const QString& versionString);

    static TexmakerVersion getCurrentVersion();

    void parseString(const QString& versionString);

    bool isValid() const;

    QString toString(bool allwaysPrintMinor = false) const;

    const QVector<int>& getVersionNumber() const;

    int worldVerion() const;
    int majorVersion() const;
    int minorVersion() const;

    friend bool operator> (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator< (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator>= (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator<= (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator== (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator!= (const TexmakerVersion &v1, const TexmakerVersion &v2);

private:
    void invalidate();

    QVector<int> _versionNumbers;
    bool _valid;

};

#endif // TEXMAKERVERSION_H
