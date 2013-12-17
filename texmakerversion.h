#ifndef TEXMAKERVERSION_H
#define TEXMAKERVERSION_H

#include <QString>
#include <QStringList>
#include <QLatin1String>
#include <QDebug>

class TexmakerVersion
{
public:
    /**
     * @brief TexmakerVersion this creates an invalid TexmakerVersion instance
     * @warning
     */
    TexmakerVersion();

    /**
     * @brief TexmakerVersion creates a valid TexmakerVersion instance which world, major and minor version number corresponds the given input numbers
     * @param world sets the world version number
     * @param major sets the major version number
     * @param minor sets the minor version number
     */
    TexmakerVersion(int world, int major, int minor = 0);

    /**
     * @brief TexmakerVersion constructs a Texmakerversion insance
     * @see parseString(const QString& versionString)
     * @param versionString represents a QString in Form of "4.0.3" or "4.1.0", make sure that the seperator symbol is a ".". If you pass "4.1" as exmaple, world=4, mahor=1 and minor=0
     */
    TexmakerVersion(const QString& versionString);

    /**
     * @brief getCurrentVersion returns the current running version
     * @warning the returned version may not be valid
     * @return the current version
     */
    static TexmakerVersion getCurrentVersion();

    /**
     * @brief parseString set the version number corresponding to the given input string.
     * @see TexmakerVersion(const QString& versionString)
     * @param versionString Version numbers should be seperated by "." and consits of three or two numbers. E.g. "4.0.3", "4.1.0" or "4.1" (minor will be set to zero).
     */
    void parseString(const QString& versionString);

    /**
     * @brief isValid checks if this instance is valid. Invalid may be uninitialized or parsing may gone wrong.
     * @return valid or not
     */
    bool isValid() const;

    /**
     * @brief toString a simple pretty print function
     * @param allwaysPrintMinor if the version is "4.1" the returned string, if true will be "4.1.0" otherwise "4.1"
     * @return a string representing the version numbers, if invalid will return "0.0.0"
     */
    QString toString(bool allwaysPrintMinor = false) const;

    /**
     * @brief getVersionNumbers gives acsess to the version numbers
     * @return a const reference to a vector or numbers. Size should be 3. Position 0 represents world version, position 2 the minor number.
     */
    const QVector<int>& getVersionNumbers() const;


    /**
     * @brief worldVerion
     * @return the world version
     */
    int worldVerion() const;

    /**
     * @brief majorVersion
     * @return the major version
     */
    int majorVersion() const;

    /**
     * @brief minorVersion
     * @return minor version
     */
    int minorVersion() const;

    friend bool operator> (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator< (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator>= (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator<= (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator== (const TexmakerVersion &v1, const TexmakerVersion &v2);
    friend bool operator!= (const TexmakerVersion &v1, const TexmakerVersion &v2);

private:

    /**
     * @brief invalidate sets _versionNumbers to 0 and turns _valid to false
     */
    void invalidate();

    /**
     * @brief _versionNumbers representing world (at position 0), major (1) and minor (2) version numbers
     */
    QVector<int> _versionNumbers;

    /**
     * @brief _valid describes if the current instance is valid, may be ivalid because of invalidate() call, a non sucessfull call from parseString or calling the default constructor
     */
    bool _valid;

};

#endif // TEXMAKERVERSION_H
