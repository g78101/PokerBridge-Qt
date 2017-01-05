//
//  main.cpp
//  BridgeClient
//
//  Created by Talka_Ying on 13/1/30.
//  Copyright (c) 2013年 Talka_Ying. All rights reserved.
//

#include <QtWidgets/qapplication.h>
#include "MainWindow.h"

/*
 * This is your main() function. Very simple.
 */
int main(int argc, char **argv)
{
    // Every Qt GUI needs a QApplication.
    QApplication app(argc, argv);
    
    // This is your MainWindow that you created with Designer
    // Declare it and show it.
    MainWindow mainWindow;
    mainWindow.show();
    
    // This will not return until the last window is closed.
    // This runs the GUI's event loop handling painting and
    // user input and stuff like that.
    return app.exec();
}
