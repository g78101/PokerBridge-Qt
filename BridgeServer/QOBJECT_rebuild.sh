#!/bin/sh
ProjectName=BridgeServer.pro

if [ $# == 1 ]; then
	ProjectName=$1
	echo  "$ProjectName"
fi

qmake -project

echo "QT += widgets network" >> $ProjectName

qmake -spec macx-xcode $ProjectName