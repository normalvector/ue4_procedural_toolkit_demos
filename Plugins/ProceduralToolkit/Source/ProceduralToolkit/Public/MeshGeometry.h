// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "UObject/NoExportTypes.h"
#include "FSectionGeometry.h"
#include "MeshGeometry.generated.h"

UCLASS(BlueprintType)
class PROCEDURALTOOLKIT_API UMeshGeometry : public UObject
{
	GENERATED_BODY()

	// The actual data making up this mesh
	UPROPERTY()
		TArray<FSectionGeometry> sections;

	// Default constructor- creates an empty mesh.
	UMeshGeometry();
	
};
