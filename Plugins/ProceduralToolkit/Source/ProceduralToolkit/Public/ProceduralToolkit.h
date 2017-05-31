// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

/// The interface for loading and unloading the toolkit
/// \todo Think about renaming the whole thing to a smaller-scoped name
class FProceduralToolkitModule : public IModuleInterface
{
public:

	/// Called when the module is loaded into memory
	virtual void StartupModule() override;

	/// Called when the module is removed from memory
	virtual void ShutdownModule() override;
};