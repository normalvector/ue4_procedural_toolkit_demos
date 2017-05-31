// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "Components/ActorComponent.h"
#include "MeshGeometry.h"
#include "MeshDeformationComponent.generated.h"


/// \todo Select linear - Select based on a position and a linear falloff
/// \todo Select by noise function - Select based on the FastNoise library
/// \todo Select by vertex color - Both baked into the mesh and painted onto a StaticMeshActor.  Choose channel.  This may need extra work since VertexColor isn't there when we get the data from the mesh.
/// \todo Select by Texture - Similar to vertex color but using a texture map.

/// \todo Check that FRandomStream is correctly passed

/// *ActorComponent* for easy geometry deformation.
///
/// This is the main class for the *Mesh Deformation Component*, and
/// provides an *ActorComponent* which can be attached to a UE4 Actor to
/// give access to the feature.
///
/// This includes the basic functionality to:
/// * Load geometry data and store it in the component
/// * Select vertices
/// * Apply transformations (Much of this is a delegate
///   to *MeshGeometry* and just repeated here so that we
///   can call the function on the component)
/// * Output the result to a *ProceduralMeshComponent*.
///
/// ## Use of SelectionSet to control Weighting
///
/// All of the transformation functions can take an optional *SelectionSet*
/// which allows the strength of the effect to be controlled on a per-vertex
/// basis, with 0=No effect, 1=Full effect, and values in between meaning partial
/// effect.  Values less than <0 or >1 can be provided but the effects are not guaranteed
/// to be sensible.
///
/// Each transformation function tries to handle the SelectionSet as a linear interpolation
/// in a sensible manner, but this can change depending on the nature of the function.
/// For example *Translate* can treat the weighting as a lerp factor between the points original
/// position and the translation target, but *Rotate* needs to apply their blending to the actual
/// angle of rotation.  If in doubt look at the actual implementation for the function in
/// *MeshGeometry*.

/// \see UActorComponent
/// \see MeshGeometry
/// \see SelectionSet
UCLASS(ClassGroup=(Custom),meta=(BlueprintSpawnableComponent))
class PROCEDURALTOOLKIT_API UMeshDeformationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/// Sets default values for this component's properties
	UMeshDeformationComponent();

	/// This is the mesh geometry currently stored within the component
	UPROPERTY(BlueprintReadonly, Category = MeshDeformationComponent)
		UMeshGeometry *meshGeometry = nullptr;
	
	/// Loads the geometry from a static mesh
	/// 
	/// This stores the geometryin *meshGeometry* and replaces any existing
	/// geometry.
	///
	/// \param MeshDeformationComponent		This component
	/// \param staticMesh					The mesh to copy the geometry from
	/// \param LOD							A StaticMesh can have multiple meshes for different levels of detail, this specifies which LOD we're taking the information fromaram>
	/// \return *True* if we could read the geometry, *False* if not
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		bool LoadFromStaticMesh(UMeshDeformationComponent *&MeshDeformationComponent, UStaticMesh *staticMesh, int32 LOD = 0);

	/// Write the current geometry to a *ProceduralMeshComponent*.
	/// 
	/// This will rebuild the mesh, completely replacing any geometry it has there.
	///
	/// \param MeshDeformationComponent		This component
	/// \param proceduralMeshComponent		The target *ProceduralMeshComponent
	/// \param createCollision				Whether to create a collision shape for it
	/// \return *True* if the update was successful, *False* if not
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		bool UpdateProceduralMeshComponent(UMeshDeformationComponent *&MeshDeformationComponent, UProceduralMeshComponent *proceduralMeshComponent, bool createCollision);

	/// Selects all of the vertices at full strength.
	///
	/// /return A *SelectionSet* with full strength
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshDeformationComponent)
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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshDeformationComponent)
		USelectionSet *SelectNear(FVector center = FVector::ZeroVector, float innerRadius = 0, float outerRadius = 100);

	/// Selects the vertices near a Spline, allowing curves to easily guide deformation.
	///
	/// This does a smooth linear radial selection based on distance from the spline provided.
	///
	/// \param spline		The spline to be used for the selection
	/// \param innerRadius	The inner radius, all points closer to the spline than this distance
	///						will be selected at maximum strength.
	/// \param outerRadius	The outer radius, all points further from the spline than this distance
	///						will not be selected
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshDeformationComponent)
		USelectionSet *SelectNearSpline(USplineComponent *spline, float innerRadius = 0, float outerRadius = 100);

	/// Selects vertices near a line segment with the provided start/end points.
	///
	/// This does a smooth linear selection based on the distance from the line points provided.
	///
	/// \param lineStart		The position of the start of the line in local space
	/// \param lineEnd			The position of the end of the line in local spac3e
	/// \param innerRadius		The inner radius, all points closer to the line segment than this distance
	///							will be selected at maximum strength
	/// \param outerRadius		The outer radius, all points further from the line segment than this distance
	///							will not be selected
	/// \param lineIsInfinite	If this is checked then lineStart/lineEnd will treated as two points on an
	///							infinite line instead of being the start/end of a line segment
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshDeformationComponent)
		USelectionSet *SelectNearLine(FVector lineStart, FVector lineEnd, float innerRadius = 0, float outerRadius = 100, bool lineIsInfinite = false);

	/// Selects vertices with a given normal facing
	///
	/// This does a smooth linear selection based on the angle from the specified normal direction.
	/// \param Facing		The facing to select, in world space
	/// \param InnerRadiusInDegrees	The inner radius in degrees, all vertices with a normal within
	///								this deviation from Facing will be selected at full strength.
	/// \param OuterRadiusInDegrees	The outer radius in degrees, all vertices with a normal greater
	///								than this deviation from Facing will not be selected.
	/// \return A *SelectionSet* for the selected vertices
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshDeformationComponent)
		USelectionSet *SelectFacing(FVector Facing = FVector::UpVector, float InnerRadiusInDegrees = 0, float OuterRadiusInDegrees = 30.0f);

	/// Selects vertices based on a noise function.
	///
	/// This uses the [FastNoise](https://github.com/Auburns/FastNoise) noise library by Jordan Pack and released under the MIT license.
	/// Not all of these settings are used by each noise type, details on their application is in the
	/// [FastNoise docs](https://github.com/Auburns/FastNoise/wiki/Noise-Settings).
	///
	/// \param Seed							The seed for the random number generator
	/// \param Frequency					The frequency of the noise, the higher the value the more detail
	/// \param NoiseInterpolation			The interpolation used to smooth between noise values
	/// \param NoiseType					The type of noise we're using
	/// \param FractalOctaves				The number of fractal octaves to apply
	/// \param FractalLacunarity			Set the fractal lacunarity, the higher the value the more space the
	///										the fractal will fill up
	/// \param FractalGain					The strength of the fractal
	/// \param FractalType					The type of fractal being used
	/// \param CellularDistanceFunction		The function used to calculate the value for a given point.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MeshDeformationComponent)
		USelectionSet *SelectByNoise(
			int32 Seed = 1337,
			float Frequency = 0.01,
			ENoiseInterpolation NoiseInterpolation = ENoiseInterpolation::Quintic,
			ENoiseType NoiseType = ENoiseType::Simplex,
			uint8 FractalOctaves = 3,
			float FractalLacunarity = 2.0,
			float FractalGain = 0.5,
			EFractalType FractalType = EFractalType::FBM,
			ECellularDistanceFunction CellularDistanceFunction = ECellularDistanceFunction::Euclidian
		);

	/// Adds random jitter to the position of the points.
	///
	///  The jitter will be a vector randomly selected
	///  (with [continuous uniform distribution]() between *Min* and *Max*, and will
	///  be scaled by each vertex's selection weights if they're provided.
	///
	/// \param MeshDeformationComponent		This component.
	/// \param randomStream					The random stream to source numbers from
	/// \param min							The minimum jittered offset
	/// \param max							The maximum jittered offset
	/// \param selection					The selection weights, if not specified
	///										then all points will be jittered at
	///										maximum strength
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Jitter(UMeshDeformationComponent *&MeshDeformationComponent, FRandomStream randomStream, FVector min, FVector max, USelectionSet *selection);

	/// Move all selected by the provided delta vector.
	///
	///  If a *SelectionSet* is provided the delta will be weighted according to the vertex's
	///   selection weight.
	///
	/// \param MeshDeformationComponent			This component
	/// \param delta							The translation delta in local space
	/// \param selection						The selection weights, if not specified
	///											then all points will be moved by the
	///											full delta translation
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Translate(UMeshDeformationComponent *&MeshDeformationComponent, FVector delta, USelectionSet *selection);

	/// Rotates the vertices of the mesh a specified amount round the specified position.
	/// 
	/// If a SelectionSet is provided then the actual rotator will be scaled accordingly allowing
	/// whorls and similar to be easily created.
	///
	/// \param MeshDeformationComponent			This component
	/// \param Rotation							The rotation to apply
	/// \param CenterOfRotation					The center of rotation in local space
	/// \param Selection						The selection weights, if not specified
	///											then all points will be rotated by the full rotation
	///											specified
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Rotate(UMeshDeformationComponent *&MeshDeformationComponent, FRotator Rotation = FRotator::ZeroRotator, FVector CenterOfRotation = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	/// Scale the selected points on a per-axis basis about a specified center
	///
	/// \param MeshDeformationComponent			This component
	/// \param Scale3d							The per-axis scaling
	/// \param CenterOfScale					The center of the scaling operation in local space
	/// \param Selection						The selection weights, if not specified then all
	///											vertices will be scaled fully by the specified scale
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Scale(UMeshDeformationComponent *&MeshDeformationComponent, FVector Scale3d = FVector(1, 1, 1), FVector CenterOfScale = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	/// Applies Scale/Rotate/Translate as a single operation using a combined transform.
	///
	/// The order of the operations will be Scale/Rotate/Translate as documented
	/// [here](https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Math/FTransform/index.html).
	///
	/// \param MeshDeformationComponent		This component
	/// \param Transform					The transformation to apply
	/// \param CenterOfTransform			The center of the transformation, in local space
	/// \param Selection					The SelectionSet, if not specified then all vertices
	///										will be transformed at full strength
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Transform(UMeshDeformationComponent *&MeshDeformationComponent, FTransform Transform, FVector CenterOfTransform = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	/// Morph a mesh into a sphere by moving points along their normal
	///
	/// \param MeshDeformationComponent		This component
	/// \param SphereRadius					The radius of the sphere to morph to
	/// \param FilterStrength				The strength of the effect, 0=No effect, 1=Full effect.
	///	\param SphereCenter					The center of the sphere
	/// \param Selection					The SelectionSet, if specified this will be multiplied
	///										by FilterStrength to allow each vertex's morph to be
	///										individually controlled.
	/// \todo Should group the sphere parameters together
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Spherize(UMeshDeformationComponent *&MeshDeformationComponent, float SphereRadius = 100.0f, float FilterStrength = 1.0f, FVector SphereCenter = FVector::ZeroVector, USelectionSet *Selection = nullptr);

	/// Moves vertices a specified offset along their own normals
	///
	/// \param MeshDeformationComponent			This component
	/// \param Offset							The distance to offset
	/// \param Selection						The SelectionSet, with the offset being scaled for
	///											each vertex
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void Inflate(UMeshDeformationComponent *&MeshDeformationComponent, float Offset = 0.0f, USelectionSet *Selection = nullptr);

	/// Scale an object along an arbitrary axis
	///
	/// This allows objects to be scaled along any axis, not just the normal XYZ, and so is more
	/// flexible than the standard approach, even thought it is less intuitive.
	///
	/// \param MeshDeformationComponent			This component
	/// \param CenterOfScale					The center of the scale operation, in local space
	/// \param Axis								The axis to scale along
	/// \param Scale							The ratio to scale by
	/// \param Selection						The SelectionSet which controls the weighting of the
	///											scale for each vertex.  If not provided then the scale
	///											will apply at full strength to all vertices.
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void ScaleAlongAxis(UMeshDeformationComponent *&MeshDeformationComponent, FVector CenterOfScale = FVector::ZeroVector, FVector Axis = FVector::UpVector, float Scale = 1.0f, USelectionSet *Selection = nullptr);

	/// Rotate vertices about an arbitrary axis
	///
	/// This allows more freedom than the standard 'Rotate around X, Y, and Z' and is more flexible
	/// than the standard approach even if it is less intuitive.
	///
	/// \param MeshDeformationComponent		This component
	/// \param CenterOfRotation				The center of the rotation operation in local space
	/// \param Axis							The axis to rotate about
	/// \param AngleInDegrees				The angle to rotate the vertices about
	/// \param Selection					The SelectionSet which controls the amount of rotation
	///										applied to each vertex.
	UFUNCTION(BlueprintCallable, Category = MeshDeformationComponent)
		void RotateAroundAxis(UMeshDeformationComponent *&MeshDeformationComponent, FVector CenterOfRotation = FVector::ZeroVector, FVector Axis = FVector::UpVector, float AngleInDegrees = 0.0f, USelectionSet *Selection = nullptr);
};