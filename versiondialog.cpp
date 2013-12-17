/***************************************************************************
 *   copyright       : (C) 2003-2013 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "versiondialog.h"
#include "texmakerversion.h"
#include "updatechecker.h"

#include <QtCore/QUrl>
#include <QDesktopServices>
#include <QDebug>



VersionDialog::VersionDialog(UpdateChecker& checker, QWidget *parent)
    :QDialog( parent), _updateChecker(checker)
{
    ui.setupUi(this);

    displayVersionString(*ui.lineEditCurrent, _updateChecker.getCurrentVersion());
    setAvailableVersionIsCurrent(false);

    connect(&_updateChecker, SIGNAL(error(QNetworkReply::NetworkError,QString)), this, SLOT(errorOccured(QNetworkReply::NetworkError,QString)));
    connect(&_updateChecker, SIGNAL(availableWebVersion(TexmakerVersion)), this, SLOT(gotWebVersion(TexmakerVersion)));
    connect(&_updateChecker, SIGNAL(newVersionAvailable()), this, SLOT(onNewVersionAvailable()));

    QString errorString = "";

    //maybe this is the first check or an error occured, so check again
    if(!_updateChecker.hasBeenChecked() || (_updateChecker.hasBeenChecked() && _updateChecker.isErrorPresent())) {
        errorString = QString::fromUtf8("?.?.?");
        checkForNewVersion();
    }
    displayVersionString(*ui.lineEditAvailable, _updateChecker.getWebVersion(), errorString);
    setAvailableVersionIsCurrent(_updateChecker.getWebVersion() == _updateChecker.getCurrentVersion());
    if(_updateChecker.getWebVersion() > _updateChecker.getCurrentVersion()) {
        onNewVersionAvailable();
    }


    connect(ui.pushButtonDownload, SIGNAL(clicked()), &_updateChecker, SLOT(gotoDownloadPage()));
    connect(ui.pushButtonCheck, SIGNAL(clicked()), this, SLOT(checkForNewVersion()));
}

VersionDialog::~VersionDialog(){
}

void VersionDialog::errorOccured(QNetworkReply::NetworkError error, QString errorDescription)
{
    ui.pushButtonCheck->setEnabled(true);
    displayVersionString(*ui.lineEditAvailable, _updateChecker.getWebVersion());
    ui.lineEditAvailable->setToolTip(errorDescription);
    setAvailableVersionIsCurrent(false);
}

void VersionDialog::gotWebVersion(TexmakerVersion version)
{
    ui.pushButtonCheck->setEnabled(true);
    setAvailableVersionIsCurrent(version == _updateChecker.getCurrentVersion());
    displayVersionString(*ui.lineEditAvailable, version);
}

void VersionDialog::checkForNewVersion()
{
    ui.lineEditAvailable->setToolTip("");
    ui.pushButtonCheck->setEnabled(false);
    _updateChecker.checkForNewVersion();
}

void VersionDialog::onNewVersionAvailable()
{
    ui.labelUpdateStatus->setText(tr("There is a new version available."));
}

void VersionDialog::displayVersionString(QLineEdit &lineEdit, const TexmakerVersion &version, QString errorString)
{
    if(version.isValid()) {
        lineEdit.setText(version.toString());
    } else {
        lineEdit.setText(errorString.isEmpty() ? tr("Error") : errorString);
    }
}

void VersionDialog::setAvailableVersionIsCurrent(bool isCurrent)
{
    ui.labelAvailableIsUpToDate->setVisible(isCurrent);
    if(isCurrent) {
        ui.labelUpdateStatus->setText(tr("Your version is up to date."));
    } else {
        ui.labelUpdateStatus->setText("");
    }
}
