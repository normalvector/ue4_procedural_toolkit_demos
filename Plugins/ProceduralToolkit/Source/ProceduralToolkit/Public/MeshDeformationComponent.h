// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "Components/ActorComponent.h"
#include "MeshGeometry.h"
#include "MeshDeformationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDURALTOOLKIT_API UMeshDeformationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeshDeformationComponent();

	// This is the mesh geometry currently stored within the component
	UPROPERTY(BlueprintReadonly, Category = MeshDeformationComponent)
		UMeshGeometry *meshGeometry = nullptr;


	// Pull out the geometry of the mesh so we're able to modify it
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		bool LoadFromStaticMesh(UStaticMesh *staticMesh, int32 LOD = 0);

	// Write out the geometry to a ProceduralMeshComponent.
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		bool UpdateProceduralMeshComponent(UProceduralMeshComponent *proceduralMeshComponent, bool createCollision);

	///// Selection Set utilities

	// Select all vertices
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectAll();

	// Select points near a point in space.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectNear(FVector center = FVector::ZeroVector, float innerRadius = 0, float outerRadius = 100);

	///// Deformation utilities
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Jitter(FRandomStream randomStream, FVector min, FVector max, USelectionSet *selection);

	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Translate(FVector delta, USelectionSet *selection);
};
