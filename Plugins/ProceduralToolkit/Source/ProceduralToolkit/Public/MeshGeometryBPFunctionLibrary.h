// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshGeometry.h"
#include "ProceduralMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Math/RandomStream.h"
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
		static UMeshGeometry *GetMeshGeometryFromStaticMesh(UStaticMesh *staticMesh, int32 LOD=0);

	UFUNCTION(BlueprintCallable, Category = "MeshGeometry")
		static void RebuildProceduralMeshComponentWithMeshGeometry(UProceduralMeshComponent *proceduralMeshComponent, UMeshGeometry *meshGeometry, bool createCollision=false);

	UFUNCTION(BlueprintCallable, Category = "MeshGeometry")
		static UMeshGeometry *JitterMeshGeometry(UMeshGeometry *meshGeometry, FRandomStream randomStream,
			float minX = 0, float minY = 0, float minZ = 0, float maxX = 0, float maxY = 0, float maxZ = 0);
};
