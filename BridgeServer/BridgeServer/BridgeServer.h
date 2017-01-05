#ifndef BRIDGESERVER_H
#define BRIDGESERVER_H

#include <QtNetwork>

class BridgeServer : public QTcpServer
{
  Q_OBJECT

    public:
        BridgeServer(QObject *parent=0);

    private slots:
		void startServer();
        void readyRead();
        void disconnected();
        void sendUserList();
	signals:
		void changeMessage(const QString &);

    protected:
        void incomingConnection(qintptr socketId);
		void deal_Four();
        int judge();
    private:
        QSet<QTcpSocket*> clients;
        QMap<QTcpSocket*,QString> users;
        int card[52],table[4],sleep_time,round;
        enum Stage{waiting,calling,playing};
        Stage now_Stage;
        bool sendUser;
        QTime time;
		int trump,call_or_play_bout,flower;
        int trump_type_who_call_fast[7];
};


#endif

