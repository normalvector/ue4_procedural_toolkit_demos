// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "MeshGeometry.h"
#include "DeformMeshComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDURALTOOLKIT_API UDeformMeshComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// The source mesh that we're deforming.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaticMesh)
		UStaticMesh *StaticMesh = nullptr;

	// The PMC that we're building
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeformMeshComponent)
		UProceduralMeshComponent *ProceduralMeshComponent = nullptr;

	// Sets default values for this component's properties
	UDeformMeshComponent();

	// Pull out the geometry of the mesh so we're able to modify it
	UFUNCTION(BlueprintCallable, Category = DeformMeshComponent)
		UMeshGeometry *GetStaticMeshGeometry(int32 LOD = 0);
	/*
	// Rebuild the PMC
	UFUNCTION(BlueprintCallable, Category = DeformMeshComponent)
		void Rebuild(bool createCollision = false);
	*/
};
