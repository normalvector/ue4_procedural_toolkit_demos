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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshDeformationComponent)
		USelectionSet *SelectAll();

	// Select points near a point in space.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshDeformationComponent)
		USelectionSet *SelectNear(FVector center = FVector::ZeroVector, float innerRadius = 0, float outerRadius = 100);

	// Select vertices with a given normal facing
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshDeformationComponent)
		USelectionSet *SelectFacing(FVector Facing = FVector::UpVector, float InnerRadiusInDegrees = 0, float OuterRadiusInDegrees = 30.0f);

	///// Deformation utilities
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Jitter(FRandomStream randomStream, FVector min, FVector max, USelectionSet *selection);

	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Translate(FVector delta, USelectionSet *selection);

	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Rotate(FRotator Rotation = FRotator::ZeroRotator, FVector CenterOfRotation = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	// Scale the selected points about a specified center
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Scale(FVector Scale3d = FVector(1, 1, 1), FVector CenterOfScale = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	// Applies a transform (Translate/Rotate/Scale) as a single operation.
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Transform(FTransform Transform, FVector CenterOfTransform = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	// Makes a mesh more like a sphere by scaling along vectors to an adjustable strength.
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Spherize(float SphereRadius = 100.0f, float FilterStrength = 1.0f, FVector SphereCenter = FVector::ZeroVector, USelectionSet *Selection = nullptr);


	// Moves vertices along their own normals
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Inflate(float Offset = 0.0f, USelectionSet *Selection = nullptr);
};
