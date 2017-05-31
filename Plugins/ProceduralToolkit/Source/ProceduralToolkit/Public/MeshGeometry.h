// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "UObject/NoExportTypes.h"
#include "SectionGeometry.h"
#include "Math/TransformNonVectorized.h"
#include "ProceduralMeshComponent.h"
#include "SelectionSet.h"
#include "MeshGeometry.generated.h"

/// \todo Select linear - Select based on a position and a linear falloff
/// \todo Select near line segment - Select based on distance to start/end points
/// \todo Select near spline - Select based on distance from a SplineComponent
/// \todo Select by noise function - Select based on the FastNoise library

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

	// Select all vertices
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectAll();

	// Select vertices near a point (Radial selection)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectNear(FVector center =FVector::ZeroVector, float innerRadius=0, float outerRadius=100);

	// Select vertices with a given normal facing
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectFacing(FVector Facing = FVector::UpVector, float InnerRadiusInDegrees = 0, float OuterRadiusInDegrees = 30.0f);


	// Randomly jitter a point.
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Jitter(FRandomStream randomStream, FVector min, FVector max, USelectionSet *selection=nullptr);

	// Translate the selected points.
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Translate(FVector delta, USelectionSet *selection);

	// Rotate the selected points about a set origin
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Rotate(FRotator Rotation = FRotator::ZeroRotator, FVector CenterOfRotation = FVector::ZeroVector, USelectionSet *Selection=nullptr);

	// Scale the selected points about a specified center
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Scale(FVector Scale3d = FVector(1, 1, 1), FVector CenterOfScale = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	// Applies a transform (Translate/Rotate/Scale) as a single operation.
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Transform(FTransform Transform, FVector CenterOfTransform = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	// Makes a mesh more like a sphere by scaling along vectors to an adjustable strength.
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Spherize(float SphereRadius = 100.0f, float FilterStrength = 1.0f, FVector SphereCenter = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	// Moves vertices along their own normals
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Inflate(float Offset = 0.0f, USelectionSet *Selection = nullptr);

	// Scale geometry along a specified axis.
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void ScaleAlongAxis(FVector CenterOfScale = FVector::ZeroVector, FVector Axis = FVector::UpVector, float Scale = 1.0f, USelectionSet *Selection = nullptr);

	// Rotates geometry round a specified axis.
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void RotateAroundAxis(FVector CenterOfRotation = FVector::ZeroVector, FVector Axis = FVector::UpVector, float AngleInDegrees = 0.0f, USelectionSet *Selection = nullptr);
};
