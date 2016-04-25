package com.epicgames.ue4;

import com.epicgames.CrappyBird.OBBDownloaderService;
import com.epicgames.CrappyBird.DownloaderActivity;


public class DownloadShim
{
	public static OBBDownloaderService DownloaderService;
	public static DownloaderActivity DownloadActivity;
	public static Class<DownloaderActivity> GetDownloaderType() { return DownloaderActivity.class; }
}

