// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "MeshGeometry.h"




UMeshGeometry::UMeshGeometry()
{
	// Create empty data sets.
	sections = TArray<FSectionGeometry>();
}

bool UMeshGeometry::LoadFromStaticMesh(UStaticMesh *staticMesh, int32 LOD /*= 0*/)
{
	// If there's no mesh provided we don't have any work to do.
	if (!staticMesh) {
		return false;
	}

	// Clear any existing geometry.
	this->sections.Empty();

	const int32 numSections = staticMesh->GetNumSections(LOD);
	UE_LOG(LogTemp, Log, TEXT("Found %d sections for LOD %d"), numSections, LOD);

	// Iterate over the sections
	for (int meshSectionIndex = 0; meshSectionIndex < numSections; ++meshSectionIndex) {
		// Create the geometry for the section
		FSectionGeometry sectionGeometry;

		// Copy the static mesh's geometry for the section to the struct.
		UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(
			staticMesh, LOD, meshSectionIndex,
			sectionGeometry.vertices, sectionGeometry.triangles,
			sectionGeometry.normals, sectionGeometry.uvs, sectionGeometry.tangents
		);
		UE_LOG(LogTemp, Log, TEXT("Section %d: Found %d verts and %d triangles"), meshSectionIndex, sectionGeometry.vertices.Num(), sectionGeometry.triangles.Num() / 3);

		// Load vertex colors with default values for as many vertices as needed
		sectionGeometry.vertexColors.InsertDefaulted(0, sectionGeometry.vertices.Num());

		// Add the finished struct to the mesh's section list
		this->sections.Emplace(sectionGeometry);
	}

	// All done
	return true;
}
