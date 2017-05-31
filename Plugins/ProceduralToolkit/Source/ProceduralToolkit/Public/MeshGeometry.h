// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "UObject/NoExportTypes.h"
#include "SectionGeometry.h"
#include "Math/TransformNonVectorized.h"
#include "ProceduralMeshComponent.h"
#include "SelectionSet.h"
#include "MeshGeometry.generated.h"

/// \todo Check that FRandomStream is correctly passed
/// \todo Select linear - Select based on a position and a linear falloff
/// \todo Select near line segment - Select based on distance to start/end points
/// \todo Select near spline - Select based on distance from a SplineComponent
/// \todo Select by noise function - Select based on the FastNoise library

/// This class stores the geometry for a mesh which can then be mutated by the
/// methods provided to allow a range of topological deformations.
///
/// While it is possible to use this class alone the main intent here is to be the
/// geometry backend for *MeshDeformationComponent*.
///
/// \see MeshDeformationComponent
UCLASS(BlueprintType)
class PROCEDURALTOOLKIT_API UMeshGeometry : public UObject
{
	GENERATED_BODY()

public:
	/// The actual geometry making up this mesh.
	///
	/// This is stored as an array with each element representing the geometry of a single section
	/// of the geometry.
	UPROPERTY(BlueprintReadonly)
		TArray<FSectionGeometry> sections;

	/// Default constructor- creates an empty mesh.
	UMeshGeometry();

	/// Loads the geometry from a static mesh
	/// 
	/// This replaces any geometry currently stored.
	///
	/// \param staticMesh					The mesh to copy the geometry from
	/// \param LOD							A StaticMesh can have multiple meshes for different levels of detail, this specifies which LOD we're taking the information fromaram>
	/// \return *True* if we could read the geometry, *False* if not
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		bool LoadFromStaticMesh(UStaticMesh *staticMesh, int32 LOD = 0);

	/// Write the current geometry to a *ProceduralMeshComponent*.
	/// 
	/// This will rebuild the mesh, completely replacing any geometry it has there.
	///
	/// \param proceduralMeshComponent		The target *ProceduralMeshComponent
	/// \param createCollision				Whether to create a collision shape for it
	/// \return *True* if the update was successful, *False* if not
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		bool UpdateProceduralMeshComponent(UProceduralMeshComponent *proceduralMeshComponent, bool createCollision);

	/// Return the number of total vertices in the geometry.
	///
	/// This is the combined sum of the vertices in each of the sections which make up this mesh.
	///
	/// \return The total vertex count
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		int32 TotalVertexCount() const;

	/// Return the number of total triangles in the geometry.
	///
	/// This is the combined sum of the triangles in each of the sections which make up this mesh.
	///
	/// \return The total triangle count
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		int32 TotalTriangleCount() const;

	/// Get a brief description of this geometry in the form *"4 sections, 1000 vertices, 500 triangles"*
	///
	/// This is mainly for debug purposes and making sure things have not broken.
	///
	/// \return A text summary
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		FString GetSummary() const;

	/// Selects all of the vertices at full strength.
	///
	/// \return A *SelectionSet* with full strength
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectAll();

	/// Selects the vertices near a point in space.
	///
	/// This does a smooth linear radial selection based on distance form the point provided.
	///
	/// \param center		The center of the selection in local space
	/// \param innerRadius	The inner radius, all points inside this will be selected at
	///								maximum strength
	/// \param outerRadius	The outer radius, all points outside this will not be selected
	/// \return A *SelectionSet* for the selected vertices
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectNear(FVector center =FVector::ZeroVector, float innerRadius=0, float outerRadius=100);


	/// Selects vertices with a given normal facing
	///
	/// This does a smooth linear selection based on the angle from the specified normal direction.
	/// \param Facing		The facing to select, in world space
	/// \param InnerRadiusInDegrees	The inner radius in degrees, all vertices with a normal within
	///								this deviation from Facing will be selected at full strength.
	/// \param OuterRadiusInDegrees	The outer radius in degrees, all vertices with a normal greater
	///								than this deviation from Facing will not be selected.
	/// \return A *SelectionSet* for the selected vertices
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshGeometry)
		USelectionSet *SelectFacing(FVector Facing = FVector::UpVector, float InnerRadiusInDegrees = 0, float OuterRadiusInDegrees = 30.0f);

	/// Adds random jitter to the position of the points.
	///
	///  The jitter will be a vector randomly selected
	///  (with [continuous uniform distribution]() between *Min* and *Max*, and will
	///  be scaled by each vertex's selection weights if they're provided.
	///
	/// \param randomStream					The random stream to source numbers from
	/// \param min							The minimum jittered offset
	/// \param max							The maximum jittered offset
	/// \param selection					The selection weights, if not specified
	///										then all points will be jittered at
	///										maximum strength
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Jitter(FRandomStream randomStream, FVector min, FVector max, USelectionSet *selection=nullptr);

	/// Move all selected by the provided delta vector.
	///
	///  If a *SelectionSet* is provided the delta will be weighted according to the vertex's
	///   selection weight.
	///
	/// \param delta							The translation delta in local space
	/// \param selection						The selection weights, if not specified
	///											then all points will be moved by the
	///											full delta translation
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Translate(FVector delta, USelectionSet *selection);

	/// Rotates the vertices of the mesh a specified amount round the specified position.
	/// 
	/// If a SelectionSet is provided then the actual rotator will be scaled accordingly allowing
	/// whorls and similar to be easily created.
	///
	/// \param Rotation							The rotation to apply
	/// \param CenterOfRotation					The center of rotation in local space
	/// \param Selection						The selection weights, if not specified
	///											then all points will be rotated by the full rotation
	///											specified
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Rotate(FRotator Rotation = FRotator::ZeroRotator, FVector CenterOfRotation = FVector::ZeroVector, USelectionSet *Selection=nullptr);

	/// Scale the selected points on a per-axis basis about a specified center
	///
	/// \param Scale3d							The per-axis scaling
	/// \param CenterOfScale					The center of the scaling operation in local space
	/// \param Selection						The selection weights, if not specified then all
	///											vertices will be scaled fully by the specified scale
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Scale(FVector Scale3d = FVector(1, 1, 1), FVector CenterOfScale = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	/// Applies Scale/Rotate/Translate as a single operation using a combined transform.
	///
	/// The order of the operations will be Scale/Rotate/Translate as documented
	/// [here](https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Math/FTransform/index.html).
	///
	/// \param Transform					The transformation to apply
	/// \param CenterOfTransform			The center of the transformation, in local space
	/// \param Selection					The SelectionSet, if not specified then all vertices
	///										will be transformed at full strength
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Transform(FTransform Transform, FVector CenterOfTransform = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	/// Morph a mesh into a sphere by moving points along their normal
	///
	/// \param SphereRadius					The radius of the sphere to morph to
	/// \param FilterStrength				The strength of the effect, 0=No effect, 1=Full effect.
	///	\param SphereCenter					The center of the sphere
	/// \param Selection					The SelectionSet, if specified this will be multiplied
	///										by FilterStrength to allow each vertex's morph to be
	///										individually controlled.
	/// \todo Should group the sphere parameters together
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Spherize(float SphereRadius = 100.0f, float FilterStrength = 1.0f, FVector SphereCenter = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	/// Moves vertices a specified offset along their own normals
	///
	/// \param Offset							The distance to offset
	/// \param Selection						The SelectionSet, with the offset being scaled for
	///											each vertex
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void Inflate(float Offset = 0.0f, USelectionSet *Selection = nullptr);

	/// Scale an object along an arbitrary axis
	///
	/// This allows objects to be scaled along any axis, not just the normal XYZ, and so is more
	/// flexible than the standard approach, even thought it is less intuitive.
	///
	/// \param CenterOfScale					The center of the scale operation, in local space
	/// \param Axis								The axis to scale along
	/// \param Scale							The ratio to scale by
	/// \param Selection						The SelectionSet which controls the weighting of the
	///											scale for each vertex.  If not provided then the scale
	///											will apply at full strength to all vertices.
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void ScaleAlongAxis(FVector CenterOfScale = FVector::ZeroVector, FVector Axis = FVector::UpVector, float Scale = 1.0f, USelectionSet *Selection = nullptr);

	/// Rotate vertices about an arbitrary axis
	///
	/// This allows more freedom than the standard 'Rotate around X, Y, and Z' and is more flexible
	/// than the standard approach even if it is less intuitive.
	///
	/// \param CenterOfRotation				The center of the rotation operation in local space
	/// \param Axis							The axis to rotate about
	/// \param AngleInDegrees				The angle to rotate the vertices about
	/// \param Selection					The SelectionSet which controls the amount of rotation
	///										applied to each vertex.
	UFUNCTION(BlueprintCallable, Category = MeshGeometry)
		void RotateAroundAxis(FVector CenterOfRotation = FVector::ZeroVector, FVector Axis = FVector::UpVector, float AngleInDegrees = 0.0f, USelectionSet *Selection = nullptr);
};
