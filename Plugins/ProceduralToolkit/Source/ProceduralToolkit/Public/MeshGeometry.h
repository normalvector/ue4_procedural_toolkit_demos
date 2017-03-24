// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "UObject/NoExportTypes.h"
#include "SectionGeometry.h"
#include "ProceduralMeshComponent.h"
#include "SelectionSet.h"
#include "MeshGeometry.generated.h"

UCLASS(BlueprintType)
class PROCEDURALTOOLKIT_API UMeshGeometry : public UObject
{
	GENERATED_BODY()

public:
	// The actual data making up this mesh
	UPROPERTY(BlueprintReadonly)
		TArray<FSectionGeometry> sections;

	// Default constructor- creates an empty mesh.
	UMeshGeometry();

	// Read data from a StaticMesh
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		bool LoadFromStaticMesh(UStaticMesh *staticMesh, int32 LOD = 0);

	// Write to a PMC
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		bool UpdateProceduralMeshComponent(UProceduralMeshComponent *proceduralMeshComponent, bool createCollision);

	// Return the number of total vertices in the geometry.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		int32 TotalVertexCount() const;

	// Return the number of total triangles in the geometry.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		int32 TotalTriangleCount() const;

	// Get a textual representation of this geometry (Summarises vert count and so on)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		FString GetSummary() const;

	///// Selection Set utilities

	// Select all vertices
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectAll();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectNear(FVector center =FVector::ZeroVector, float innerRadius=0, float outerRadius=100);

	///// Deformation utilities
	
	// Randomly jitter a point.
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Jitter(FRandomStream randomStream, FVector min, FVector max, USelectionSet *selection=nullptr);


	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Translate(FVector delta, USelectionSet *selection);
};
