// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "Engine/World.h"
#include "MeshGeometryBPFunctionLibrary.h"


UMeshGeometry *UMeshGeometryBPFunctionLibrary::GetMeshGeometryFromStaticMesh(UStaticMesh *staticMesh, int32 LOD/*=0*/)
{
	// If there's no mesh provided we don't have any work to do.
	if (!staticMesh) {
		return nullptr;
	}

	// Create a MeshGeometry with a transient package as Outer - not providing it will make it instantly GC.
	UMeshGeometry *meshGeometry = NewObject<UMeshGeometry>(staticMesh, UMeshGeometry::StaticClass());
	
	const int32 numSections = staticMesh->GetNumSections(LOD);
	UE_LOG(LogTemp, Log, TEXT("Found %d sections for LOD %d"), numSections, LOD);

	// Iterate over the sections if we have a staticMesh
	if (staticMesh) {
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
			meshGeometry->sections.Emplace(sectionGeometry);
		}
	}

	// Return the MeshGeometry.
	return meshGeometry;
}

void UMeshGeometryBPFunctionLibrary::RebuildProceduralMeshComponentWithMeshGeometry(UProceduralMeshComponent *proceduralMeshComponent, UMeshGeometry *meshGeometry, bool createCollision/*=false*/)
{
	UE_LOG(LogTemp, Log, TEXT("Rebuilding PMC from MeshGeometry"));
	
	// Check we have a PMC
	if (!proceduralMeshComponent) {
		UE_LOG(LogTemp, Warning, TEXT("RebuildProceduralMeshComponentWithMeshGeometry: No ProceduralMeshComponent provided"));
		return;
	}

	// Check we have the mesh geometry too.
	if (!meshGeometry) {
		UE_LOG(LogTemp, Warning, TEXT("RebuildProceduralMeshComponentWithMeshGeometry: No MeshGeometry provided"));
		return;
	}

	// Check both are valid low-level
	// TODO: This should not be needed!
	if (!proceduralMeshComponent->IsValidLowLevel()) {
		UE_LOG(LogTemp, Error, TEXT("RebuildProceduralMeshComponentWithMeshGeometry: Invalid PMC.. Mass panic"));
		return;
	}
	if (!meshGeometry->IsValidLowLevel()) {
		UE_LOG(LogTemp, Error, TEXT("RebuildProceduralMeshComponentWithMeshGeometry: Invalid MG.. Mass panic"));
		return;
	}

	// Clear the geometry
	proceduralMeshComponent->ClearAllMeshSections();

	// Iterate over the mesh sections, creating a PMC MeshSection for each one.
	int32 nextSectionIndex = 0;
	for (auto section : meshGeometry->sections) {
		UE_LOG(LogTemp, Log, TEXT("Rebuilding section.."));
		// Create the PMC section with the StaticMesh's data.
		proceduralMeshComponent->CreateMeshSection_LinearColor(
			nextSectionIndex++, section.vertices, section.triangles, section.normals, section.uvs,
			section.vertexColors, section.tangents, createCollision
		);
	}
}

UMeshGeometry * UMeshGeometryBPFunctionLibrary::JitterMeshGeometry(
	UMeshGeometry *meshGeometry, FRandomStream randomStream,
	float minX /*= 0*/, float minY /*= 0*/, float minZ /*= 0*/, float maxX /*= 0*/, float maxY /*= 0*/, float maxZ /*= 0*/)
{
	// If there's no mesh provided we don't have any work to do.
	if (!meshGeometry) {
		return meshGeometry;
	}

	// Iterate over the sections randomly jittering each vertex in the section
	FVector jitter;
	auto newVerts = TArray<FVector>();
	for (auto section : meshGeometry->sections) {
		UE_LOG(LogTemp, Log, TEXT("Jittering section: %d verts (X SHRINK)"), section.vertices.Num());
		/*
		for (int i = 0; i < section.vertices.Num(); ++i) {
			jitter = FVector(
				randomStream.FRandRange(minX, maxX),
				randomStream.FRandRange(minY, maxY),
				randomStream.FRandRange(minZ, maxZ)
			);
			//section.vertices[i] += jitter;
			newVerts.Emplace(section.vertices[i] + jitter);
			UE_LOG(LogTemp, Log, TEXT("Vertex: %f,%f,%f (%f, %f, %f)"), section.vertices[i].X, section.vertices[i].Y, section.vertices[i].Z, jitter.X, jitter.Y, jitter.Z);
		}
		section.vertices = newVerts;
		*/
		for (FVector &vertex : section.vertices) {
			//vertex.X += randomStream.FRandRange(minX, maxX);
			vertex.X *= 0.25;
			vertex.Y += randomStream.FRandRange(minY, maxY);
			vertex.Z += randomStream.FRandRange(minZ, maxZ);
		}
	}

	// All done
	return meshGeometry;
}
