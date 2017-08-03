# Introduction

以 "Example Qt Chat Program" 為基礎進行延伸

連結：http://thesmithfam.org/blog/2009/07/09/example-qt-chat-program/

花橋規則進行遊戲  代表有Small < Middle < Club < Diamond < Heart < Spades < No Trump


此遊戲分為Server端與Client端 只要有某台固定IP電腦開Server 就可供4個Client端連線。

P.S.  當掉請自行重新連線&&名稱請不要取一樣!!!分不清楚我不負責~~~


# How to build

using Qt with Xcode

1. in terminal: **brew install qt**
2. in terminal: **brew link qt --force**   
3. in terminal:
**cd {BridgeClient or BridgeServer}**
**sh QOBJECT_rebuild.sh**
4. open **BridgeClient.xcodeproj** or **BridgeServer.xcodeproj**
5. *(BridgeClient only)* </br>
**add framework to project 1. CoreFoundation 2. GLUT 3. OpenAL** </br>
**add new group (name "Resource")** and **add file(image、sound) to the group**
(file path: BridgeClient/BridgeClient/Resource/)

# Game Screenshots

the Screenshots 1
![alt text](https://raw.githubusercontent.com/g78101/PokerBridge-Qt/master/GameScreenshots/1.jpg)

the Screenshots 2
![alt text](https://raw.githubusercontent.com/g78101/PokerBridge-Qt/master/GameScreenshots/2.jpg)
