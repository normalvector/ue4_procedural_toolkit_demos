// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "Engine/StaticMesh.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "DeformMeshComponent.h"


// Sets default values for this component's properties
UDeformMeshComponent::UDeformMeshComponent()
{
	// This component can't ever tick.  It's possible another component's tick will update us, but we don't tick.
	PrimaryComponentTick.bCanEverTick = false;
}

UMeshGeometry *UDeformMeshComponent::GetStaticMeshGeometry(int32 LOD /*= 0*/)
{
	// Create a MeshGeometry with this component as it's outer (owner).
	auto meshGeometry = NewObject<UMeshGeometry>(this);

	const int32 numSections = StaticMesh->GetNumSections(LOD);
	UE_LOG(LogTemp, Log, TEXT("Found %d sections for LOD %d"), numSections, LOD);

	// Iterate over the sections.
	for (int meshSectionIndex = 0; meshSectionIndex < numSections; ++meshSectionIndex) {
		// Create the geometry for the section
		FSectionGeometry sectionGeometry;

		// Copy the static mesh's geometry for the section to the struct.
		UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(
			StaticMesh, LOD, meshSectionIndex,
			sectionGeometry.vertices, sectionGeometry.triangles,
			sectionGeometry.normals, sectionGeometry.uvs, sectionGeometry.tangents
		);
		UE_LOG(LogTemp, Log, TEXT("Section %d: Found %d verts and %d triangles"), meshSectionIndex, sectionGeometry.vertices.Num(), sectionGeometry.triangles.Num() / 3);

		// Load vertex colors with default values for as many vertices as needed
		sectionGeometry.vertexColors.InsertDefaulted(0, sectionGeometry.vertices.Num());

		// Add the finished struct to the mesh's section list
		meshGeometry->sections.Emplace(sectionGeometry);
	}
	// Return the MeshGeometry.
	return meshGeometry;
}
