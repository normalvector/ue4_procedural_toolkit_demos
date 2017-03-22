// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshGeometry.h"
#include "ProceduralMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "MeshGeometryBPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALTOOLKIT_API UMeshGeometryBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MeshGeometry")
		static UMeshGeometry *GetMeshGeometryFromStaticMesh(UObject *owner, UStaticMesh *staticMesh, int32 LOD=0);

	UFUNCTION(BlueprintCallable, Category = "MeshGeometry")
		static void RebuildProceduralMeshComponentWithMeshGeometry(UProceduralMeshComponent *proceduralMeshComponent, UMeshGeometry *meshGeometry, bool createCollision=false);
};
