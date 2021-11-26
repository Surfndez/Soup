<?php
$cmd = "clang -std=c++17 -fno-rtti examples/simple-server.cpp";
/*foreach(scandir("src") as $file)
{
	if(substr($file, -4) == ".cpp")
	{
		$cmd .= " src/".$file;
	}
}*/
$cmd .= " -o simple-server.exe";
if(!defined("PHP_WINDOWS_VERSION_MAJOR"))
{
	$cmd .= " -lstdc++ -pthread";
}
passthru($cmd);
if(defined("PHP_WINDOWS_VERSION_MAJOR"))
{
	passthru("simple-server.exe");
}
else
{
	passthru("./simple-server.exe");
}
