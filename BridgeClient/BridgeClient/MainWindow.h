//
//  MainWindow.h
//  BridgeClient
//
//  Created by Talka_Ying on 13/1/30.
//  Copyright (c) 2013年 Talka_Ying. All rights reserved.
//

#ifndef MainWindow_h
#define MainWindow_h

#include "GLCallBridge.h"
#include "GLPlayBridge.h"

#include <QtGui>
#include <QtWidgets/qmainwindow.h>
#include <QtNetwork/qtcpsocket.h>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "header/alut.h"

/*
 * This is the MainWindow class that we have told to inherit from
 * our Designer MainWindow (ui::MainWindow)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    // Every QWidget needs a constructor, and they should allow
    // the user to pass a parent QWidget (or not).
    MainWindow(QWidget *parent=0);
    ~MainWindow();
    ALboolean LoadALData();
	void SetListenerValues();
    void KillALData();
    
    private slots:
    
    // This function gets called when a user clicks on the
    // loginButton on the front page (which you placed there
    // with Designer)
    void on_loginButton_clicked();
    
    // This gets called when you click the sayButton on
    // the chat page.
    void on_sayButton_clicked();
    
    // This is a function we'll connect to a socket's readyRead()
    // signal, which tells us there's text to be read from the chat
    // server.
    void readyRead();
    
    // This function gets called when the socket tells us it's connected.
    void connected();
    
    void callingsend_slot(int);
  
    void callButtonClick();
    void button_pass();
	void showAllPoker();
	void catchInformation(int);
signals:
    void callingsend_signal(int);
	void setOurScore(int);
    void setEnemyScore(int);

private:
    void GUI_initial();
    // This is the socket that will let us communitate with the server.
    QTcpSocket *socket;
    
    GLCallBridge *callingBridge;
	GLPlayBridge *playingBridge;
    QLineEdit *serverLineEdit;
    QLineEdit *sayLineEdit;
    QLineEdit *userLineEdit;
    QListWidget *userListWidget;
    QTextEdit *roomTextEdit;
	QTextEdit *systemEdit;
    
    QTextEdit *userEdit;
    QTextEdit *SNPcall_userEdit; // SNP = Show Now Pokers
    QTextEdit *SNPplay_userEdit;
    
    QLabel *user0,*user1,*user2,*user3;
    QLabel *SNP_user0,*SNP_user1,*SNP_user2,*SNP_user3;
    QLabel *ourTeam,*enemyTeam,*trumpLabel;
    QLCDNumber *ourScore,*goal_ourScore,*enemyScore,*goal_enemyScore;

    QStackedWidget *stackedWidget;
    QStackedWidget *stackedWidget2;
    QWidget *waitingPage;
    QWidget *loginPage;
	QWidget *callingPage;
	QWidget *playingPage;
    QWidget *centralwidget;
    QWidget *chatwidget;        
    QFrame *secondFrame;
	QPushButton **callbutton;
	QPushButton *showbutton;
    
    enum Stage{waiting,calling,playing};
    enum GameStatus{
        PLAY_CARD=0,
        PLAY_LAST_CARD,
        PLAY_WAITING_JUDGE,
        PLAY_GAME_OVER
    };

    Stage now_Stage;
    int callpoker,call_or_play_turn,myturnid,trump,now_flower,table_time;
    int ourScore_number,enemyScore_number,win_number;
    int card[13],table[4];
    QStringList userName;
    QStringList flower;
    char *resourcesPath;
    
    // Buffers to hold sound data.
	ALuint Buffers[1];
    
	// Sources are points of emitting sound.
	ALuint Sources[1];
   
};


#endif
