#pragma once

#include "Logger.h"

class Filesystem;

class ACNodeCore
{
public:
	Logger* logger;	
	Filesystem* filesystem;
};

// defined in Startup.cpp
extern ACNodeCore core;
