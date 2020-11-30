#pragma once

#include "Logger.h"

class ACNodeCore
{
public:
	Logger* logger;	
};

// defined in Startup.cpp
extern ACNodeCore core;
