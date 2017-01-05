#ifndef BRIDGESERVERDIALOG_H
#define BRIDGESERVERDIALOG_H

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qtextedit.h>
#include "BridgeServer.h"

class BridgeServerDialog : public QDialog
{
	Q_OBJECT

	public:
		BridgeServerDialog(QWidget *parent = 0, QFlag flags = 0);
		~BridgeServerDialog();

	private slots:
    void changeMessage(const QString &);

	private:
		QTextTableFormat tableFormat;
		QTextEdit *textEdit;
		QPushButton *start, *cancel;
		BridgeServer *server;
};

#endif 
