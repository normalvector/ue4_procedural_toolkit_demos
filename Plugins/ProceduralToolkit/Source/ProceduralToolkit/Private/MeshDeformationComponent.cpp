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
	// If there's no static mesh we have nothing to do..
	if (!staticMesh) {
		UE_LOG(LogTemp, Warning, TEXT("LoadFromStaticMesh: No StaticMesh provided"));
		meshGeometry = nullptr;
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Reading mesh geometry from static mesh '%s'"), *staticMesh->GetName());
	meshGeometry = NewObject<UMeshGeometry>(this);
	meshGeometry->LoadFromStaticMesh(staticMesh);

	return true;
}
