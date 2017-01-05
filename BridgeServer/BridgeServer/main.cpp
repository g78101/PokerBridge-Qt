#include "BridgeServerDialog.h"
#include <QtWidgets/qapplication.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BridgeServerDialog *server = new BridgeServerDialog();
	server->show();
	return a.exec();
}
