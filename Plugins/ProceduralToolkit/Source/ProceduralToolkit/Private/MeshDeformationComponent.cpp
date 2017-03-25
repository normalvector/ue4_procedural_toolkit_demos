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

bool UMeshDeformationComponent::LoadFromStaticMesh(UMeshDeformationComponent *&MeshDeformationComponent, UStaticMesh *staticMesh, int32 LOD /*= 0*/)
{
	MeshDeformationComponent = this;
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

USelectionSet * UMeshDeformationComponent::SelectNear(FVector center /*= FVector::ZeroVector*/, float innerRadius /*= 0*/, float outerRadius /*= 100*/)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("UMeshDeformationComponent: No meshGeometry loaded"));
		return nullptr;
	}

	return meshGeometry->SelectNear(center, innerRadius, outerRadius);
}

USelectionSet * UMeshDeformationComponent::SelectFacing(FVector Facing /*= FVector::UpVector*/, float InnerRadiusInDegrees /*= 0*/, float OuterRadiusInDegrees /*= 30.0f*/)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Jitter: No meshGeometry loaded"));
		return nullptr;
	}
	return meshGeometry->SelectFacing(Facing, InnerRadiusInDegrees, OuterRadiusInDegrees);
}

void UMeshDeformationComponent::Jitter(FRandomStream randomStream, FVector min, FVector max, USelectionSet *selection)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Jitter: No meshGeometry loaded"));
		return;
	}
	meshGeometry->Jitter(randomStream, min, max, selection);
}

void UMeshDeformationComponent::Translate(FVector delta, USelectionSet *selection)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Translate: No meshGeometry loaded"));
		return;
	}
	meshGeometry->Translate(delta, selection);
}

void UMeshDeformationComponent::Rotate(FRotator Rotation/*= FRotator::ZeroRotator*/, FVector CenterOfRotation /*= FVector::ZeroVector*/, USelectionSet *Selection /*=nullptr*/)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Rotate: No meshGeometry loaded"));
		return;
	}
	meshGeometry->Rotate(Rotation, CenterOfRotation, Selection);

}

void UMeshDeformationComponent::Scale(FVector Scale3d /*= FVector(1, 1, 1)*/, FVector CenterOfScale /*= FVector::ZeroVector*/, USelectionSet *Selection /*= nullptr*/)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Scale: No meshGeometry loaded"));
		return;
	}
	meshGeometry->Scale(Scale3d, CenterOfScale, Selection);
}

void UMeshDeformationComponent::Transform(FTransform Transform, FVector CenterOfTransform /*= FVector::ZeroVector*/, USelectionSet *Selection /*= nullptr*/)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Transform: No meshGeometry loaded"));
		return;
	}
	meshGeometry->Transform(Transform, CenterOfTransform, Selection);
}

void UMeshDeformationComponent::Spherize(float SphereRadius /*= 100.0f*/, float FilterStrength /*= 1.0f*/, FVector SphereCenter /*= FVector::ZeroVector*/, USelectionSet *Selection /*= nullptr*/)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Spherize: No meshGeometry loaded"));
		return;
	}
	meshGeometry->Spherize(SphereRadius, FilterStrength, SphereCenter, Selection);
}

void UMeshDeformationComponent::Inflate(float Offset /*= 0.0f*/, USelectionSet *Selection /*= nullptr*/)
{
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Spherize: No meshGeometry loaded"));
		return;
	}
	meshGeometry->Inflate(Offset, Selection);
}
