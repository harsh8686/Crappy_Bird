#!/bin/sh
cd "`dirname "$0"`"
ADB=
if [ "$ANDROID_HOME" != "" ]; then ADB=$ANDROID_HOME/platform-tools/adb; else ADB=/Users/Harshsingh/NVPACK/android-sdk-macosx; fi
DEVICE=
if [ "$1" != "" ]; then DEVICE="-s $1"; fi
echo
echo Uninstalling existing application. Failures here can almost always be ignored.
$ADB $DEVICE uninstall com.epicgames.CrappyBird
echo
echo Installing existing application. Failures here indicate a problem with the device \(connection or storage permissions\) and are fatal.
$ADB $DEVICE install CrappyBird-armv7-es2.apk
if [ $? -eq 0 ]; then
	echo
	echo Removing old data. Failures here are usually fine - indicating the files were not on the device.
	$ADB $DEVICE shell 'rm -r $EXTERNAL_STORAGE/UE4Game/CrappyBird'
	$ADB $DEVICE shell 'rm -r $EXTERNAL_STORAGE/UE4Game/UE4CommandLine.txt'
	$ADB $DEVICE shell 'rm -r $EXTERNAL_STORAGE/obb/com.epicgames.CrappyBird'




if [ 1 ]; then
		echo
		echo Installation successful
		exit 0
	fi
fi
echo
echo There was an error installing the game or the obb file. Look above for more info.
echo
echo Things to try:
echo Check that the device (and only the device) is listed with \"$ADB devices\" from a command prompt.
echo Make sure all Developer options look normal on the device
echo Check that the device has an SD card.
exit 1
