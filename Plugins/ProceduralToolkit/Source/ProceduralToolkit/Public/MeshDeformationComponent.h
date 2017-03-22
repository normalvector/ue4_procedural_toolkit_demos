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

	///// Deformation utilities
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Jitter(FRandomStream randomStream, FVector min, FVector max);
};
