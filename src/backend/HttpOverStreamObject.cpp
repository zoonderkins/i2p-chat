/***************************************************************************
 *   Copyright (C) 2008 by I2P-Messenger                                   *
 *   Messenger-Dev@I2P-Messenger                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "HttpOverStreamObject.h"

CHttpOverStreamObject::CHttpOverStreamObject(
    CConnectionManager &ConnectionManager)
    : mConnectionManager(ConnectionManager) {
  mStream = NULL;
  mFirstStreamStatus = true;
  mIsTimeOutCantReachPeerCLosed = false;
  mMode = HTTPMODE::GET;
}

CHttpOverStreamObject::~CHttpOverStreamObject() {
  if (mStream != NULL) {
    mConnectionManager.doDestroyStreamObjectByID(mStream->getID());
  }
}

void CHttpOverStreamObject::slotStreamStatus(
    const SAM_Message_Types::RESULT result, const qint32 ID, QString Message) {
  if (mStreamID != ID) {
    qCritical() << "File\t" << __FILE__ << Qt::endl
                << "Line:\t" << __LINE__ << Qt::endl
                << "Function:\t"
                << "CHttpOverStreamObject::slotStreamStatus" << Qt::endl
                << "Stream-Message:\t" << Message << Qt::endl
                << "Message:\t"
                << "mStreamID!=ID WTF" << Qt::endl
                << "mStreamID:\t" << mStreamID << Qt::endl
                << "ID:\t" << ID << Qt::endl;
  }

  switch (result) {
  case (SAM_Message_Types::OK): {
    if (mFirstStreamStatus == true) {
      sendRequest();
      mFirstStreamStatus = false;
    }
    break;
  }
  case (SAM_Message_Types::INVALID_KEY):
  case (SAM_Message_Types::CANT_REACH_PEER):
  case (SAM_Message_Types::TIMEOUT): {
    mConnectionManager.doDestroyStreamObjectByID(mStreamID);
    mStream = NULL;
    mDataReceived.clear();
    mIsTimeOutCantReachPeerCLosed = true;
    emit signDoneSuccessfully(false, mDataReceived);
    break;
  }
  case (SAM_Message_Types::CLOSED): {

    mConnectionManager.doDestroyStreamObjectByID(mStreamID);
    mStream = NULL;

    if (mIsTimeOutCantReachPeerCLosed == false) {
      emit signDoneSuccessfully(true, mDataReceived);
    }

    mDataReceived.clear();
    break;
  }
  default: {
    mConnectionManager.doDestroyStreamObjectByID(mStreamID);
    mStream = NULL;
    mDataReceived.clear();
    break;
  }
  }
}

void CHttpOverStreamObject::slotDataReceived(const qint32 ID, QByteArray t) {
  if (mStreamID != ID) {
    qCritical() << "File\t" << __FILE__ << Qt::endl
                << "Line:\t" << __LINE__ << Qt::endl
                << "Function:\t"
                << "CHttpOverStreamObject::slotDataReceived" << Qt::endl
                << "Message:\t"
                << "mStreamID!=ID WTF" << Qt::endl
                << "mStreamID:\t" << mStreamID << Qt::endl
                << "ID:\t" << ID << Qt::endl;
  }

  mDataReceived.append(t);
}

void CHttpOverStreamObject::doHttpRequest(HTTPMODE mode, QString Destination,
                                          QStringList HttpHeader) {
  mStream = mConnectionManager.doCreateNewStreamObject(CONNECT, false, true);
  mStream->setUsedFor("HttpOverStreamObject");
  mStreamID = mStream->getID();

  connect(mStream,
          SIGNAL(signStreamStatusReceived(const SAM_Message_Types::RESULT,
                                          const qint32, const QString)),
          this,
          SLOT(slotStreamStatus(const SAM_Message_Types::RESULT, const qint32,
                                QString)));

  connect(mStream, SIGNAL(signDataReceived(const qint32, const QByteArray)),
          this, SLOT(slotDataReceived(const qint32, QByteArray)));

  mMode = mode;
  mDestination = Destination;
  mHttpHeader = HttpHeader;
  mFirstStreamStatus = true;
  mDataReceived.clear();
  mIsTimeOutCantReachPeerCLosed = false;

  mStream->doConnect(Destination);
}

void CHttpOverStreamObject::sendRequest() {
  QByteArray PacketForSending;

  if (mMode == GET) {
    PacketForSending.append("GET ");
  } else {
    PacketForSending.append("POST ");
  }

  for (int i = 0; i < mHttpHeader.count(); i++) {
    PacketForSending.append(mHttpHeader.at(i).toUtf8());
  }

  *(mStream) << PacketForSending;
}
