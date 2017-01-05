# Introduction

以 "Example Qt Chat Program" 為基礎進行延伸

連結：http://thesmithfam.org/blog/2009/07/09/example-qt-chat-program/

花橋規則進行遊戲  代表有Small < Middle < Club < Diamond < Heart < Spades < No Trump


此遊戲分為Server端與Client端 只要有某台固定IP電腦開Server 就可供4個Client端連線。

P.S.  當掉請自行重新連線&&名稱請不要取一樣!!!分不清楚我不負責~~~


# How to build

using Qt with Xcode

1. open terminal:<b>brew install qt</b>
	</br>(path: /usr/local/Cellar/qt5/{version}/)

2. to system <b>"/etc/paths"</b> add qt bin path <b>"/usr/local/Cellar/qt5/5.7.1_1/bin"</b>     

3. open terminal:
	</br><b>cd {BridgeClient or BridgeServer}</b>
	</br><b>sh QOBJECT_rebuild.sh</b> 

4. open <b>BridgeClient.xcodeproj</b> or <b>BridgeServer.xcodeproj</b>

5. (BridgeClient only)
	</br><b>add framework to project     1. CoreFoundation 2. GLUT 3. OpenAL</b>
	</br><b>add new group (name "Resource")</b> and <b>add file(image、sound) to the group</b> 
	</br>(file path: BridgeClient/BridgeClient/Resource/)
