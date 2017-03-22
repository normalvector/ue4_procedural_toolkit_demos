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
	// If there's no static mesh we have nothing to do..
	if (!staticMesh) {
		UE_LOG(LogTemp, Warning, TEXT("LoadFromStaticMesh: No StaticMesh provided"));
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Reading mesh geometry from static mesh '%s'"), *staticMesh->GetName());

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

bool UMeshGeometry::UpdateProceduralMeshComponent(UProceduralMeshComponent *proceduralMeshComponent, bool createCollision)
{
	// If there's no PMC we have nothing to do..
	if (!proceduralMeshComponent) {
		UE_LOG(LogTemp, Warning, TEXT("UpdateProceduralMeshComponent: No proceduralMeshComponent provided"));
		return false;
	}

	// Clear the geometry
	proceduralMeshComponent->ClearAllMeshSections();

	// Iterate over the mesh sections, creating a PMC MeshSection for each one.
	int32 nextSectionIndex = 0;
	for (auto section : this->sections) {
		UE_LOG(LogTemp, Log, TEXT("Rebuilding section.."));
		// Create the PMC section with the StaticMesh's data.
		proceduralMeshComponent->CreateMeshSection_LinearColor(
			nextSectionIndex++, section.vertices, section.triangles, section.normals, section.uvs,
			section.vertexColors, section.tangents, createCollision
		);
	}
	return true;
}

void UMeshGeometry::Jitter(FRandomStream randomStream, float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	// Iterate over the sections, and the the vertices in the sections.
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			vertex.X += randomStream.FRandRange(minX, maxX);
			vertex.Y += randomStream.FRandRange(minY, maxY);
			vertex.Z += randomStream.FRandRange(minZ, maxZ);
		}
	}
}
