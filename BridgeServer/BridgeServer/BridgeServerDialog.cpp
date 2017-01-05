#include "BridgeServerDialog.h"

#include<QtWidgets/qboxlayout.h>
#include<QtWidgets/qpushbutton.h>

BridgeServerDialog::BridgeServerDialog(QWidget *parent, QFlag flags)
: QDialog(parent, flags)
{
	QHBoxLayout *hboxLayout;
	QVBoxLayout	 *vboxLayout;
	QPushButton *start, *cancel;
	server = new BridgeServer();
	this->setWindowTitle("Bridge Server"); 
	this->resize(513, 349);
	
	vboxLayout = new QVBoxLayout();
	hboxLayout = new QHBoxLayout();
	start = new QPushButton("Start");
	cancel = new QPushButton("Cancel");

    textEdit = new QTextEdit(this);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setReadOnly(true);
    
    vboxLayout->addWidget(textEdit);
    hboxLayout->addWidget(start);
    hboxLayout->addWidget(cancel);
    vboxLayout->addLayout(hboxLayout);
    
    setLayout(vboxLayout);
    tableFormat.setBorder(0);
    
    connect(start,SIGNAL(clicked()),server,SLOT(startServer()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(close()));
    //傳字串的形態很重要!!!  可是我忘記怎麼解決了 XD
    connect(server,SIGNAL(changeMessage(const QString&)),this,SLOT(changeMessage(const QString&)));
}

BridgeServerDialog::~BridgeServerDialog()
{
}

void BridgeServerDialog::changeMessage(const QString &message)
{
    if ( message.isEmpty())
        return;
	textEdit->append(tr("< %1 >").arg(message));
	
}
