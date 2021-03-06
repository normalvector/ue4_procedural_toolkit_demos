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
	/// \todo Err.. ?  Should this be here?  Have I broken the API?
	MeshDeformationComponent = this;
	MeshGeometry = NewObject<UMeshGeometry>(this);
	bool success = MeshGeometry->LoadFromStaticMesh(staticMesh);
	if (!success) {
		MeshGeometry = nullptr;
	}
	return success;
}


bool UMeshDeformationComponent::UpdateProceduralMeshComponent(UMeshDeformationComponent *&MeshDeformationComponent, UProceduralMeshComponent *proceduralMeshComponent, bool createCollision)
{
	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("UpdateProceduralMeshComponent: No meshGeometry loaded"));
		return false;
	}

	return MeshGeometry->UpdateProceduralMeshComponent(proceduralMeshComponent, createCollision);
}

USelectionSet * UMeshDeformationComponent::SelectAll()
{
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("UMeshDeformationComponent: No meshGeometry loaded"));
		return nullptr;
	}

	return MeshGeometry->SelectAll();
}

USelectionSet * UMeshDeformationComponent::SelectNear(FVector center /*= FVector::ZeroVector*/, float innerRadius /*= 0*/, float outerRadius /*= 100*/)
{
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("SelectNear: No meshGeometry loaded"));
		return nullptr;
	}

	return MeshGeometry->SelectNear(center, innerRadius, outerRadius);
}

USelectionSet * UMeshDeformationComponent::SelectNearSpline(USplineComponent *spline, float innerRadius /*= 0*/, float outerRadius /*= 100*/)
{
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("SelectNearSpline: No meshGeometry loaded"));
		return nullptr;
	}
	// Get the actor's local->world transform- we're going to need it for the spline.
	FTransform actorTransform  = this->GetOwner()->GetTransform();

	return MeshGeometry->SelectNearSpline(spline, actorTransform, innerRadius, outerRadius);
}

USelectionSet * UMeshDeformationComponent::SelectNearLine(FVector lineStart, FVector lineEnd, float innerRadius /*=0*/, float outerRadius/*= 100*/, bool lineIsInfinite/* = false */)
{
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("SelectNearLine: No meshGeometry loaded"));
		return nullptr;
	}

	return MeshGeometry->SelectNearLine(lineStart, lineEnd, innerRadius, outerRadius, lineIsInfinite);
}

USelectionSet * UMeshDeformationComponent::SelectFacing(FVector Facing /*= FVector::UpVector*/, float InnerRadiusInDegrees /*= 0*/, float OuterRadiusInDegrees /*= 30.0f*/)
{
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("SelectFacing: No meshGeometry loaded"));
		return nullptr;
	}
	return MeshGeometry->SelectFacing(Facing, InnerRadiusInDegrees, OuterRadiusInDegrees);
}

USelectionSet * UMeshDeformationComponent::SelectByNoise(
	int32 Seed /*= 1337*/,
	float Frequency /*= 0.01*/,
	ENoiseInterpolation NoiseInterpolation /*= ENoiseInterpolation::Quintic*/,
	ENoiseType NoiseType /*= ENoiseType::Simplex */,
	uint8 FractalOctaves /*= 3*/,
	float FractalLacunarity /*= 2.0*/,
	float FractalGain /*= 0.5*/,
	EFractalType FractalType /*= EFractalType::FBM*/,
	ECellularDistanceFunction CellularDistanceFunction /*= ECellularDistanceFunction::Euclidian*/
) {
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("SelectByNoise: No meshGeometry loaded"));
		return nullptr;
	}
	return MeshGeometry->SelectByNoise(
		Seed, Frequency, NoiseInterpolation, NoiseType,
		FractalOctaves, FractalLacunarity, FractalGain, FractalType,
		CellularDistanceFunction
	);
}

USelectionSet * UMeshDeformationComponent::SelectByTexture(UTexture2D *Texture2D, ETextureChannel TextureChannel /*= ETextureChannel::Red*/)
{
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("SelectByTexture: No meshGeometry loaded"));
		return nullptr;
	}
	return MeshGeometry->SelectByTexture(Texture2D, TextureChannel);
}

USelectionSet * UMeshDeformationComponent::SelectLinear(FVector LineStart, FVector LineEnd, bool Reverse /*= false*/, bool LimitToLine /*= false*/)
{
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Jitter: No meshGeometry loaded"));
		return nullptr;
	}
	return MeshGeometry->SelectLinear(LineStart, LineEnd, Reverse, LimitToLine);
}

void UMeshDeformationComponent::Jitter(UMeshDeformationComponent *&MeshDeformationComponent, FRandomStream &randomStream, FVector min, FVector max, USelectionSet *selection)
{
	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Jitter: No meshGeometry loaded"));
		return;
	}
	MeshGeometry->Jitter(randomStream, min, max, selection);
}

void UMeshDeformationComponent::Translate(UMeshDeformationComponent *&MeshDeformationComponent, FVector delta, USelectionSet *selection)
{
	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Translate: No meshGeometry loaded"));
		return;
	}
	MeshGeometry->Translate(delta, selection);
}

void UMeshDeformationComponent::Rotate(UMeshDeformationComponent *&MeshDeformationComponent, FRotator Rotation/*= FRotator::ZeroRotator*/, FVector CenterOfRotation /*= FVector::ZeroVector*/, USelectionSet *Selection /*=nullptr*/)
{
	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Rotate: No meshGeometry loaded"));
		return;
	}
	MeshGeometry->Rotate(Rotation, CenterOfRotation, Selection);

}

void UMeshDeformationComponent::Scale(UMeshDeformationComponent *&MeshDeformationComponent, FVector Scale3d /*= FVector(1, 1, 1)*/, FVector CenterOfScale /*= FVector::ZeroVector*/, USelectionSet *Selection /*= nullptr*/)
{
	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Scale: No meshGeometry loaded"));
		return;
	}
	MeshGeometry->Scale(Scale3d, CenterOfScale, Selection);
}

void UMeshDeformationComponent::Transform(UMeshDeformationComponent *&MeshDeformationComponent, FTransform Transform, FVector CenterOfTransform /*= FVector::ZeroVector*/, USelectionSet *Selection /*= nullptr*/)
{
	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Transform: No meshGeometry loaded"));
		return;
	}
	MeshGeometry->Transform(Transform, CenterOfTransform, Selection);
}

void UMeshDeformationComponent::Spherize(UMeshDeformationComponent *&MeshDeformationComponent, float SphereRadius /*= 100.0f*/, float FilterStrength /*= 1.0f*/, FVector SphereCenter /*= FVector::ZeroVector*/, USelectionSet *Selection /*= nullptr*/)
{
	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Spherize: No meshGeometry loaded"));
		return;
	}
	MeshGeometry->Spherize(SphereRadius, FilterStrength, SphereCenter, Selection);
}

void UMeshDeformationComponent::Inflate(UMeshDeformationComponent *&MeshDeformationComponent, float Offset /*= 0.0f*/, USelectionSet *Selection /*= nullptr*/)
{
	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Spherize: No meshGeometry loaded"));
		return;
	}
	MeshGeometry->Inflate(Offset, Selection);
}

void UMeshDeformationComponent::ScaleAlongAxis(UMeshDeformationComponent *&MeshDeformationComponent, FVector CenterOfScale /*= FVector::ZeroVector*/, FVector Axis /*= FVector::UpVector*/, float Scale /*= 1.0f*/, USelectionSet *Selection /*= nullptr*/)
{
	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Spherize: No meshGeometry loaded"));
		return;
	}
	MeshGeometry->ScaleAlongAxis(CenterOfScale, Axis, Scale, Selection);
}

void UMeshDeformationComponent::RotateAroundAxis(UMeshDeformationComponent *&MeshDeformationComponent, FVector CenterOfRotation /*= FVector::ZeroVector*/, FVector Axis /*= FVector::UpVector*/, float AngleInDegrees /*= 0.0f*/, USelectionSet *Selection /*= nullptr*/)
{

	MeshDeformationComponent = this;
	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Spherize: No meshGeometry loaded"));
		return;
	}
	MeshGeometry->RotateAroundAxis(CenterOfRotation, Axis, AngleInDegrees, Selection);
}

void UMeshDeformationComponent::Lerp(
	UMeshDeformationComponent *&MeshDeformationComponent, 
	UMeshDeformationComponent *TargetMeshDeformationComponent,
	float Alpha /*= 0.0*/,
	USelectionSet *Selection /*= nullptr*/)
{
	MeshDeformationComponent = this;

	if (!MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Lerp: No meshGeometry loaded"));
		return;
	}

	if (!TargetMeshDeformationComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Lerp: No TargetMeshDeformationComponent"));
		return;
	}

	if (!TargetMeshDeformationComponent->MeshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("Lerp: TargetMeshDeformationComponent has no geometry"));
		return;
	}

	MeshGeometry->Lerp(
		TargetMeshDeformationComponent->MeshGeometry,
		Alpha, Selection
	);
}
