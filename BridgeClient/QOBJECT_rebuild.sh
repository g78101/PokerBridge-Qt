#!/bin/sh
ProjectName=BridgeClient.pro

if [ $# == 1 ]; then
	ProjectName=$1
	echo  "$ProjectName"
fi

qmake -project

echo "QT += widgets network opengl" >> $ProjectName

qmake -spec macx-xcode $ProjectName