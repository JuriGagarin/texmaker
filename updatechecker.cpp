#include "updatechecker.h"

UpdateChecker::UpdateChecker(QObject *parent) :
    QObject(parent)
{
    _latestError = QNetworkReply::NoError;
    _timer.setSingleShot(true);
    _currentVersion = TexmakerVersion::getCurrentVersion();
    _timeout = 10000;
    _checked = false;
    connect(&_timer, SIGNAL(timeout()), this, SLOT(stopVersionCheck()));
}

void UpdateChecker::setTimeout(unsigned int timeout)
{
    _timeout = timeout;
}

unsigned int UpdateChecker::getTimeout() const
{
    return _timeout;
}

const TexmakerVersion &UpdateChecker::getCurrentVersion() const
{
    return _currentVersion;
}

const TexmakerVersion &UpdateChecker::getWebVersion() const
{
    return _webVersion;
}

QNetworkReply::NetworkError UpdateChecker::getLatestError() const
{
    return _latestError;
}

bool UpdateChecker::isErrorPresent() const
{
    return _latestError != QNetworkReply::NoError;
}

bool UpdateChecker::hasBeenChecked() const
{
    return _checked;
}

void UpdateChecker::openDownloadPage()
{
    QDesktopServices::openUrl(QUrl("http://www.xm1math.net/texmaker/download.html"));
}

void UpdateChecker::checkForNewVersion()
{
    _checked = true;
    _timer.start(_timeout);
    _reply = _manager.get(QNetworkRequest(QUrl("http://www.xm1math.net/texmaker/version.txt")));
    connect(_reply, SIGNAL (finished()),this, SLOT(processWebResult()));
}

void UpdateChecker::stopVersionCheck()
{
    disconnect(_reply, SIGNAL (finished()),this, SLOT(processWebResult()));
    _reply->abort();
    setLatestError(QNetworkReply::TimeoutError);
}

void UpdateChecker::gotoDownloadPage()
{
    openDownloadPage();
}

void UpdateChecker::processWebResult()
{
    _timer.stop();
    if (QNetworkReply::NoError == _reply->error()) {
        _webVersion.parseString(QString(_reply->readAll()));

//        qDebug() << "WebVersion: " << _webVersion.toString(true);
        if(_webVersion.isValid()) {
            emit availableWebVersion(_webVersion); //fire event because the web version is valid
            setLatestError(QNetworkReply::NoError);

            //check if an update is required
            if(_currentVersion.isValid()) {
                if(_webVersion > _currentVersion){
                    emit newVersionAvailable(_webVersion);
                }else if(_webVersion == _currentVersion) {
                    emit currentVersionIsLatest();
                }
            }
        } else {
            setLatestError(QNetworkReply::UnknownContentError); //because parsing may gone wrong
        }
    } else {
        setLatestError(_reply->error());
    }
}

void UpdateChecker::setLatestError(QNetworkReply::NetworkError networkError, bool autoFireSignal)
{
//        qWarning() << "Unable to check for newer Version. Error: " << _reply->errorString();
    _latestError = networkError;

    if(networkError != QNetworkReply::NoError) {
        _webVersion = TexmakerVersion(); // this will invalidate the web version, which is required  by the getWebVersion function
        if(autoFireSignal) {
            emit error(networkError); //NoError will never be fired :)
        }
    }
}
