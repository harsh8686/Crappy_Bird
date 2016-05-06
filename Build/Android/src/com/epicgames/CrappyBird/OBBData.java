package com.epicgames.CrappyBird;

public class OBBData
{
public static class XAPKFile {
public final boolean mIsMain;
public final String mFileVersion;
public final long mFileSize;
XAPKFile(boolean isMain, String fileVersion, long fileSize) {
mIsMain = isMain;
mFileVersion = fileVersion;
mFileSize = fileSize;
}
}

public static final XAPKFile[] xAPKS = {
new XAPKFile(
true, // true signifies a main file
"9", // the version of the APK that the file was uploaded against
52120375L // the length of the file in bytes
)
};
};
