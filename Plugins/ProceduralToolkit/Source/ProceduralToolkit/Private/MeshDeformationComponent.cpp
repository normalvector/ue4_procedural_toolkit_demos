// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "MeshGeometry.h"
#include "MeshDeformationComponent.h"


// Sets default values for this component's properties
UMeshDeformationComponent::UMeshDeformationComponent()
{
	// This component can never tick, it doesn't update itself.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UMeshDeformationComponent::LoadFromStaticMesh(UStaticMesh *staticMesh, int32 LOD /*= 0*/)
{
	meshGeometry = NewObject<UMeshGeometry>(this);
	bool success = meshGeometry->LoadFromStaticMesh(staticMesh);
	if (!success) {
		meshGeometry = nullptr;
	}
	return success;
}

bool UMeshDeformationComponent::UpdateProceduralMeshComponent(UProceduralMeshComponent *proceduralMeshComponent, bool createCollision)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("UpdateProceduralMeshComponent: No meshGeometry loaded"));
		return false;
	}

	return meshGeometry->UpdateProceduralMeshComponent(proceduralMeshComponent, createCollision);
}

USelectionSet * UMeshDeformationComponent::SelectAll()
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("UMeshDeformationComponent: No meshGeometry loaded"));
		return nullptr;
	}

	return meshGeometry->SelectAll();
}

void UMeshDeformationComponent::Jitter(FRandomStream randomStream, FVector min, FVector max)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Jitter: No meshGeometry loaded"));
		return;
	}
	meshGeometry->Jitter(randomStream, min, max);
}

void UMeshDeformationComponent::Translate(FVector delta)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Translate: No meshGeometry loaded"));
		return;
	}
	meshGeometry->Translate(delta);
}
