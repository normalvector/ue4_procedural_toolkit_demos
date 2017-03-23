// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "Engine/StaticMesh.h"
#include "KismetProceduralMeshLibrary.h"
#include "SelectionSet.h"
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

int32 UMeshGeometry::TotalVertexCount() const
{
	int32 totalVertexCount = 0;
	for (auto section : this->sections) {
		totalVertexCount += section.vertices.Num();
	}
	return totalVertexCount;
}

int32 UMeshGeometry::TotalTriangleCount() const
{
	int32 totalTriangleCount = 0;
	for (auto section : this->sections) {
		totalTriangleCount += section.triangles.Num();
	}
	return totalTriangleCount / 3; // 3pts per triangle
}

FString UMeshGeometry::GetSummary() const
{
	return FString::Printf(TEXT("%d sections, %d vertices, %d triangles"), this->sections.Num(), this->TotalVertexCount(), this->TotalTriangleCount());
}

USelectionSet *UMeshGeometry::SelectAll()
{
	USelectionSet *newSelectionSet = NewObject<USelectionSet>(this);
	newSelectionSet->CreateSelectionSet(this->TotalVertexCount());
	newSelectionSet->SetAllWeights(1.0f);
	return newSelectionSet;
}

USelectionSet * UMeshGeometry::SelectNear(FVector center /*=FVector::ZeroVector*/, float innerRadius/*=0*/, float outerRadius/*=100*/)
{
	USelectionSet *newSelectionSet = NewObject<USelectionSet>(this);
	//newSelectionSet->CreateSelectionSet(this->TotalVertexCount());

	int32 nextVertexIndex = 0;
	// Iterate over the sections, and the vertices in each section.
	float distanceFromCenter;
	float distanceBias;
	float selectionRadius = outerRadius - innerRadius;

	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			distanceFromCenter = (vertex - center).Size();
			// Apply bias to map distance to 0-1 based on innerRadius and outerRadius
			distanceBias = FMath::Clamp((distanceFromCenter - innerRadius) / selectionRadius, 0.0f, 1.0f);
			newSelectionSet->weights.Emplace(distanceBias);
		}
	}

	return newSelectionSet;
}

void UMeshGeometry::Jitter(FRandomStream randomStream, FVector min, FVector max)
{
	// Iterate over the sections, and the vertices in each section.
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			vertex.X += randomStream.FRandRange(min.X, max.X);
			vertex.Y += randomStream.FRandRange(min.Y, max.Y);
			vertex.Z += randomStream.FRandRange(min.Z, max.Z);
		}
	}
}

void UMeshGeometry::Translate(FVector delta, USelectionSet *selection)
{
	// TODO: Check selectionSet size.

	// Check if there's any need to apply weights
	if (selection) {
		// Iterate over the sections, and the the vertices in the sections.
		int32 nextSelectionIndex = 0;
		for (auto &section : this->sections) {
			for (auto &vertex : section.vertices) {
				vertex = FMath::Lerp(vertex, vertex + delta, selection->weights[nextSelectionIndex++]);
			}
		}
	}
	else {
		// Iterate over the sections, and the the vertices in the sections.
		for (auto &section : this->sections) {
			for (auto &vertex : section.vertices) {
				vertex += delta;
			}
		}
	
	}
}
