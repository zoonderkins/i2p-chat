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
#ifndef FILETRANSFERSEND_H
#define FILETRANSFERSEND_H
#include "ConnectionManager.h"
#include "I2PStream.h"
#include <QElapsedTimer>
#include <QTimer>
#include <QtGlobal>
#include <QtGui>

/*
      Filetransferprotocol 0.3: (sender Receive)

      tags:
      0:\t{StartPos}\n  	accpted start from StartPos // at the moment
   only from 0 1:\t\n			not accepted 2:\t{remotePos}\n for
   progress
*/

namespace FileTransferProtocol {
const QString MINPROTOCOLVERSION = "0.3";
const double MINPROTOCOLVERSION_D = 0.3;
const QString MAXPROTOCOLVERSION = "0.3";
const double MAXPROTOCOLVERSION_D = 0.3;
// const QString FIRSTPACKET ="CHATSYSTEMFILETRANSFER\t"+PROTOCOLVERSION+"\n";
//+sizeinbit\nFileName
}; // namespace FileTransferProtocol

#define NORMPACKETSIZE 1024
#define MAXPACKETSIZE 30720
#define AVERAGETRANSFERSPEEDPERIOD 1000 // 1 sec

class CCore;
class CFileTransferSend : public QObject {
  Q_OBJECT

public:
  CFileTransferSend(CCore &Core, CConnectionManager &ConnectionManager,
                    QString FilePath, QString Destination,
                    QString Protocolversion, double ProtocolversionD);
  ~CFileTransferSend();

  // forbid some operators
  CFileTransferSend(const CFileTransferSend &) = delete;
  CFileTransferSend &operator=(const CFileTransferSend &) = delete;

  quint64 getFileSize() { return mFileSize; };
  qint32 getStreamID() { return mStreamID; };
  QString getDestination() { return mDestination; };
  QString getFileName() { return mFileName; };
  QString getUsingProtocolVersion() { return mUsingProtocolVersion; };
  quint64 getAlreadySentSize() { return mAlreadySentSize; };
  bool getAlreadyTransferAccepted() { return mFileTransferAccepted; };
  bool getIsTransferring();
  bool getIsTransferComplete() { return mAlreadyFinished; };
  void doConvertNumberToTransferSize(quint64 inNumber, QString &outNumber,
                                     QString &outType,
                                     bool addStoOutType = true);
public slots:
  void slotAbbortFileSend();

private slots:
  void slotStreamStatus(const SAM_Message_Types::RESULT result, const qint32 ID,
                        QString Message);
  void slotDataReceived(const qint32 ID, QByteArray t);
  void slotCalcAverageTransferSpeed();

signals:
  void signAlreadySentSizeChanged(quint64 Size);
  void signFileTransferAccepted(bool t);
  void signFileTransferFinishedOK();
  void signFileTransferError();
  void signFileTransferAborted(); // recipient cancelled
  void signAverageTransferSpeed(QString SNumber, QString Type);
  void signETA(QString Value);

private:
  void StartFileTransfer(qint64 mFromPos = 0);
  void SendFile_v0dot1();
  void SendFile_v0dot2();
  void SendFile_v0dot3();
  void CalcETA(int speed);

  CCore &mCore;
  CConnectionManager &mConnectionManager;
  const QString mFilePath;
  const QString mDestination;
  const QString mUsingProtocolVersion;
  const double mUsingProtocolVersionD;
  CI2PStream *mStream;
  qint64 mFileSize;
  qint64 mAlreadySentSize;
  qint64 mRemoteReceivedSize;
  qint32 mStreamID;
  QFile mFileForSend;
  bool mSendFirstPacket;
  bool mFileTransferAccepted;
  bool mAlreadyFinished;
  QString mFileName;

  QTimer mTimerForActAverageTransferSpeed;
  QElapsedTimer mTimer;
  int mCurrentPacketSize;
  QByteArray mRemoteDataReceiveBuffer;
};
#endif
