#ifndef LOGLOADER_H
#define LOGLOADER_H

#include <QThread>
#include "datapacketdecoder.h"

class LogLoader : public QThread
{
	Q_OBJECT
public:
	explicit LogLoader(QObject *parent = 0);
	void loadFile(QString file);
	class Packet
	{
	public:
		Packet(bool valid = true) { isValid = valid; }
		bool isNAK;
		bool isValid;
		QByteArray header;
		QByteArray payload;
		unsigned short payloadid;
		unsigned short length;
		bool haslength;
		bool hasseq;
		unsigned short sequencenum;
	};
private:
	Packet parseBuffer(QByteArray buffer);
	void loadDataFieldsFromValues();
	QString m_fileName;
	int m_badChecksums;
	int m_falseStarts;
	int m_outOfPacketBytes;
	int m_lengthMismatch;
	int m_badLengthHeader;
	int m_badEscapeChar;
	int m_locationIdInfoReq;
	int m_locationIdInfoReply;
	FEDataPacketDecoder *decoder;
protected:
	void run();
signals:
	void payloadDecoded(QVariantMap map);
	void decoderFailure(QByteArray buffer);
	void incomingDatalogPacket(QByteArray data);
	void done();
public slots:
	
};

#endif // LOGLOADER_H
