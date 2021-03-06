// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "Engine/StaticMesh.h"
#include "KismetProceduralMeshLibrary.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h" // ClosestPointOnLine/ClosestPointOnInfiniteLine
#include "SelectionSet.h"
#include "FastNoise.h"
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

	// Iterate over the sections, and the vertices in each section.
	float distanceFromCenter;
	float distanceBias;
	float selectionRadius = outerRadius - innerRadius;

	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			distanceFromCenter = (vertex - center).Size();
			// Apply bias to map distance to 0-1 based on innerRadius and outerRadius
			distanceBias = 1.0f - FMath::Clamp((distanceFromCenter - innerRadius) / selectionRadius, 0.0f, 1.0f);
			newSelectionSet->weights.Emplace(distanceBias);
		}
	}

	return newSelectionSet;
}

USelectionSet * UMeshGeometry::SelectNearSpline(USplineComponent *spline, FTransform transform, float innerRadius /*= 0*/, float outerRadius /*= 100*/)
{
	USelectionSet *newSelectionSet = NewObject<USelectionSet>(this);

	// Iterate over the sections, and the vertices in each section.
	float distanceFromSpline;
	float distanceBias;
	FVector closestPointOnSpline;
	float selectionRadius = outerRadius - innerRadius;

	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			// Convert the vertex location to local space- and then get the nearest point on the spline in local space.
			closestPointOnSpline = spline->FindLocationClosestToWorldLocation(
				transform.TransformPosition(vertex),
				ESplineCoordinateSpace::Local
			);
			distanceFromSpline = (vertex - closestPointOnSpline).Size();
			// Apply bias to map distance to 0-1 based on innerRadius and outerRadius
			distanceBias = 1.0f - FMath::Clamp((distanceFromSpline - innerRadius) / selectionRadius, 0.0f, 1.0f);
			newSelectionSet->weights.Emplace(distanceBias);
		}
	}

	return newSelectionSet;
}

USelectionSet * UMeshGeometry::SelectNearLine(FVector lineStart, FVector lineEnd, float innerRadius /*=0*/, float outerRadius/*= 100*/, bool lineIsInfinite/* = false */)
{
	USelectionSet *newSelectionSet = NewObject<USelectionSet>(this);

	// Iterate over the sections, and the vertices in each section.
	FVector nearestPointOnLine;
	float distanceToLine;
	float distanceBias;
	float selectionRadius = outerRadius - innerRadius;

	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			// Get the distance from the line based on whether we're looking at an infinite line or not.
			if (lineIsInfinite) {
				nearestPointOnLine = FMath::ClosestPointOnInfiniteLine(lineStart, lineEnd, vertex);
			} else {
				nearestPointOnLine = FMath::ClosestPointOnLine(lineStart, lineEnd, vertex);
			}
			// Apply bias to map distance to 0-1 based on innerRadius and outerRadius
			distanceToLine = (vertex - nearestPointOnLine).Size();
			distanceBias = 1.0f - FMath::Clamp((distanceToLine - innerRadius) / selectionRadius, 0.0f, 1.0f);
			newSelectionSet->weights.Emplace(distanceBias);
		}
	}

	return newSelectionSet;
}

USelectionSet * UMeshGeometry::SelectFacing(FVector Facing /*= FVector::UpVector*/, float InnerRadiusInDegrees /*= 0*/, float OuterRadiusInDegrees /*= 30.0f*/)
{
	// TODO: Check geometry looks valid (normals.Num == vertices.Num)
	USelectionSet *newSelectionSet = NewObject<USelectionSet>(this);
	
	// Normalize the facing vector.
	if (!Facing.Normalize()) {
		// TODO: Better error handling.
		return newSelectionSet;
	}

	// Iterate over the sections, and the the normals in the sections.
	float selectionRadius = OuterRadiusInDegrees - InnerRadiusInDegrees;
	float angleBias;
	float angleToNormal;
	FVector normalizedNormal;
	// As we need normals to we'll use an index-based for loop here for verts.
	for (auto &section : this->sections) {
		for (auto normal : section.normals) {
			normalizedNormal = normal;

			if (normalizedNormal.Normalize()) {
				// Calculate the dot product between the normal and the Facing.
				angleToNormal = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(normal, Facing)));
				angleBias = 1.0f - FMath::Clamp((angleToNormal - InnerRadiusInDegrees) / selectionRadius, 0.0f, 1.0f);
				newSelectionSet->weights.Emplace(angleBias);
			} else {
				newSelectionSet->weights.Emplace(0);
			}
		}
	}

	return newSelectionSet;
}

USelectionSet * UMeshGeometry::SelectByNoise(
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
	USelectionSet *newSelectionSet = NewObject<USelectionSet>(this);

	// TODO: Lots of work here!
	FastNoise noise;

	// Set up all of the noise details from the parameters provided
	noise.SetSeed(Seed);
	noise.SetFrequency(Frequency);
	noise.SetInterp((FastNoise::Interp)NoiseInterpolation);
	noise.SetNoiseType((FastNoise::NoiseType)NoiseType);
	noise.SetFractalOctaves(FractalOctaves);
	noise.SetFractalLacunarity(FractalLacunarity);
	noise.SetFractalGain(FractalGain);
	noise.SetFractalType((FastNoise::FractalType) FractalType);
	noise.SetCellularDistanceFunction((FastNoise::CellularDistanceFunction) CellularDistanceFunction);
	/// \todo Is this needed.. ?  FastNoise doesn't seem to have a SetPositionWarpAmp param
	///noise.SetPositionWarpAmp(PositionWarpAmp);

	// Iterate over the sections, and the vertices in each section.
	float NoiseValue;
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			NoiseValue = noise.GetNoise(vertex.X, vertex.Y, vertex.Z);
			newSelectionSet->weights.Emplace(NoiseValue);
		}
	}

	return newSelectionSet;
}

USelectionSet * UMeshGeometry::SelectByTexture(UTexture2D *Texture2D, ETextureChannel TextureChannel /*=ETextureChannel::Red*/)
{
	USelectionSet *newSelectionSet = NewObject<USelectionSet>(this);

	// Check we have a texture and that it's in the right format
	if (!Texture2D) {
		return nullptr;
	}

	/// \todo Log a message when we do this...
	Texture2D->SRGB = false;
	Texture2D->CompressionSettings = TC_VectorDisplacementmap;
	Texture2D->UpdateResource();

	// Get the raw color data from the texture
	FTexture2DMipMap *MipMap0 = &Texture2D->PlatformData->Mips[0];
	int32 textureWidth = MipMap0->SizeX;
	int32 textureHeight = MipMap0->SizeY;
	FByteBulkData *BulkData = &MipMap0->BulkData;
	UE_LOG(LogTemp, Log, TEXT("Texture res: %d x %d"), textureWidth, textureHeight);

	// Check we got the data and lock it
	if (!BulkData) {
		/// \todo Log an error..
		return nullptr;
	}
	FColor *colorArray = static_cast<FColor*>(BulkData->Lock(LOCK_READ_ONLY));

	// Iterate over the sections, and the vertices in each section.
	for (auto &section : this->sections) {
		for (auto &uv : section.uvs) {
			// Convert our UV to a texture index.
			int32 textureX = (int32)FMath::RoundHalfFromZero(uv.X * textureWidth);
			int32 textureY = (int32)FMath::RoundHalfFromZero(uv.Y * textureHeight);

			/// \todo Wrap/Clamp
			textureX = FMath::Clamp(textureX, 0, textureWidth-1);
			textureY = FMath::Clamp(textureY, 0, textureHeight-1);

			// Get the color and access the correct channel.
			int32 index = (textureY * textureWidth) + textureX;
			//UE_LOG(LogTemp, Log, TEXT("UV %f,%f = %d,%d (%d)"), uv.X, uv.Y, textureX, textureY, index);
			
			FLinearColor color = colorArray[index];
			//UE_LOG(LogTemp, Log, TEXT("Color %f,%f,%f / %f"), color.R, color.G, color.B, color.A);
			
			switch (TextureChannel) {
			case ETextureChannel::Red:
				newSelectionSet->weights.Emplace(color.R);
				break;
			case ETextureChannel::Green:
				newSelectionSet->weights.Emplace(color.G);
				break;
			case ETextureChannel::Blue:
				newSelectionSet->weights.Emplace(color.B);
				break;
			case ETextureChannel::Alpha:
				newSelectionSet->weights.Emplace(color.A);
				break;
			}
			//newSelectionSet->weights.Emplace(0);
		}
	}

	// Unlock the texture data
	BulkData->Unlock();

	return newSelectionSet;
}

USelectionSet * UMeshGeometry::SelectLinear(FVector LineStart, FVector LineEnd, bool Reverse /*= false*/, bool LimitToLine /*= false*/)
{

	USelectionSet *newSelectionSet = NewObject<USelectionSet>(this);

	// Do the reverse if needed..
	if (Reverse) {
		FVector TmpVector = LineStart;
		LineStart = LineEnd;
		LineEnd = TmpVector;
	}

	// Calculate the length of the line.
	float LineLength = (LineEnd - LineStart).Size();
	if (LineLength < 0.1) {
		// Lines too close..
		/// \todo Log error message..
		return nullptr;
	}

	// Iterate over the sections, and the vertices in each section
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			// Get the nearest point on the line
			FVector NearestPointOnLine = FMath::ClosestPointOnLine(LineStart, LineEnd, vertex);

			// If we've hit one of the end points then return the limits
			if (NearestPointOnLine == LineEnd) {
				newSelectionSet->weights.Emplace(LimitToLine ? 0.0f : 1.0f);
			}
			else if (NearestPointOnLine == LineStart) {
				newSelectionSet->weights.Emplace(0.0f);
			}
			else {
				// Get the distance to the two start point- it's the ratio we're after.
				float DistanceToLineStart = (NearestPointOnLine - LineStart).Size();
				newSelectionSet->weights.Emplace(DistanceToLineStart / LineLength);
			}
		}
	}

	return newSelectionSet;
}

void UMeshGeometry::Jitter(FRandomStream &randomStream, FVector min, FVector max, USelectionSet *selection /*=nullptr*/)
{
	// TODO: Check selectionSet size.

	// Iterate over the sections, and the the vertices in the sections.
	int32 nextSelectionIndex = 0;
	FVector randomJitter;
	// Iterate over the sections, and the vertices in each section.
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			randomJitter = FVector(
				randomStream.FRandRange(min.X, max.X),
				randomStream.FRandRange(min.Y, max.Y),
				randomStream.FRandRange(min.Z, max.Z)
			);
			vertex = FMath::Lerp(
				vertex,
				vertex + randomJitter,
				selection ? selection->weights[nextSelectionIndex++] : 1.0f
			);
		}
	}
}

void UMeshGeometry::Translate(FVector delta, USelectionSet *selection)
{
	// TODO: Check selectionSet size.

	// Iterate over the sections, and the the vertices in the sections.
	int32 nextSelectionIndex = 0;
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			vertex = FMath::Lerp(
				vertex,
				vertex + delta,
				selection ? selection->weights[nextSelectionIndex++] : 1.0f
			);
		}
	}
}

void UMeshGeometry::Rotate(FRotator Rotation /*= FRotator::ZeroRotator*/, FVector CenterOfRotation /*= FVector::ZeroVector*/, USelectionSet *Selection)
{
	// TODO: Check SelectionSet size.

	// Iterate over the sections, and the the vertices in the sections.
	int32 nextSelectionIndex = 0;
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			vertex = FMath::Lerp(
				vertex,
				CenterOfRotation + Rotation.RotateVector(vertex - CenterOfRotation),
				Selection ? Selection->weights[nextSelectionIndex++] : 1.0f
			);
		}
	}
}

void UMeshGeometry::Scale(FVector Scale3d /*= FVector(1, 1, 1)*/, FVector CenterOfScale /*= FVector::ZeroVector*/, USelectionSet *Selection /*= nullptr*/)
{
	// TODO: Check selectionSet size.

	// Iterate over the sections, and the the vertices in the sections.
	int32 nextSelectionIndex = 0;
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			vertex = FMath::Lerp(
				vertex,
				CenterOfScale + (vertex - CenterOfScale) * Scale3d,
				Selection ? Selection->weights[nextSelectionIndex++] : 1.0f
			);
		}
	}
}

void UMeshGeometry::Transform(FTransform Transform /*= FTransform::Identity*/, FVector CenterOfTransform /*= FVector::ZeroVector*/, USelectionSet *Selection /*= nullptr*/)
{
	// TODO: Check selectionSet size.

	// Iterate over the sections, and the the vertices in the sections.
	int32 nextSelectionIndex = 0;
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			vertex = FMath::Lerp(
				vertex,
				CenterOfTransform + Transform.TransformPosition(vertex - CenterOfTransform),
				Selection ? Selection->weights[nextSelectionIndex++] : 1.0f
			);
		}
	}
}

void UMeshGeometry::Spherize(float SphereRadius /*= 100.0f*/, float FilterStrength /*= 1.0f*/, FVector SphereCenter /*= FVector::ZeroVector*/, USelectionSet *Selection)
{
	// TODO: Check selectionSet size.

	// Iterate over the sections, and the the vertices in the sections.
	int32 nextSelectionIndex = 0;
	FVector vertexRelativeToCenter;
	float targetVectorLength;
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			vertexRelativeToCenter = vertex - SphereCenter;
			// Calculate the required length- incorporating both the SphereRadius and Selection.
			targetVectorLength = FMath::Lerp(vertexRelativeToCenter.Size(), SphereRadius, FilterStrength * (Selection ? Selection->weights[nextSelectionIndex++] : 1.0f));
			// TODO: Think what happens when this fails?
			if (vertexRelativeToCenter.Normalize()) {
				vertex = SphereCenter + (vertexRelativeToCenter * targetVectorLength);
			}
		}
	}
}

void UMeshGeometry::Inflate(float Offset /*= 0.0f*/, USelectionSet *Selection /*= nullptr*/)
{
	// TODO: Check selectionSet size.
	// TODO: Check normals size.

	// Iterate over the sections, and the the vertices in the sections.
	// As we need normals to we'll use an index-based for loop here for verts.
	for (auto &section : this->sections) {
		for (int32 vertexIndex = 0; vertexIndex < section.vertices.Num(); ++vertexIndex) {
			section.vertices[vertexIndex] = FMath::Lerp(
				section.vertices[vertexIndex],
				section.vertices[vertexIndex] + (section.normals[vertexIndex] * Offset),
				Selection ? Selection->weights[vertexIndex] : 1.0f
			);
		}
	}
}

void UMeshGeometry::ScaleAlongAxis(FVector CenterOfScale /*= FVector::ZeroVector*/, FVector Axis /*= FVector::UpVector*/, float Scale /*= 1.0f*/, USelectionSet *Selection /*= nullptr*/)
{
	// TODO Check SelectionSet size
	// TODO: Check non-zero vectors.

	// Iterate over the sections, and the the vertices in the sections.
	int32 nextSelectionIndex = 0;
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			FVector closestPointOnLine = FMath::ClosestPointOnInfiniteLine(CenterOfScale, CenterOfScale + Axis, vertex);
			FVector offsetFromClosestPoint = vertex - closestPointOnLine;
			FVector scaledPointOnLine = Scale * (closestPointOnLine - CenterOfScale) + CenterOfScale;
			vertex = FMath::Lerp(vertex, scaledPointOnLine + offsetFromClosestPoint, Selection ? Selection->weights[nextSelectionIndex++] : 1.0f);
		}
	}
}

void UMeshGeometry::RotateAroundAxis(FVector CenterOfRotation /*= FVector::ZeroVector*/, FVector Axis /*= FVector::UpVector*/, float AngleInDegrees /*= 0.0f*/, USelectionSet *Selection /*= nullptr*/)
{
	// TODO Check SelectionSet size
	// TODO: Check non-zero vectors.

	// Normalize the axis direction.
	auto normalizedAxis = Axis.GetSafeNormal();
	if (normalizedAxis.IsNearlyZero(0.1f)) {
		UE_LOG(LogTemp, Error, TEXT("RotateAroundAxis: Could not normalize Axis, zero vector?"));
		return;
	}

	// Iterate over the sections, and the vertices in the sections.
	int32 nextSelectionIndex = 0;
	for (auto &section : this->sections) {
		for (auto &vertex : section.vertices) {
			FVector closestPointOnLine = FMath::ClosestPointOnInfiniteLine(CenterOfRotation, CenterOfRotation + Axis, vertex);
			FVector offsetFromClosestPoint = vertex - closestPointOnLine;
			float scaledRotation = FMath::Lerp(
				0.0f, AngleInDegrees,
				Selection ? Selection->weights[nextSelectionIndex++] : 1.0f
			);
			FVector rotatedOffset = offsetFromClosestPoint.RotateAngleAxis(scaledRotation, normalizedAxis);
			vertex = closestPointOnLine + rotatedOffset;
		}
	}
}

void UMeshGeometry::Lerp(UMeshGeometry *TargetMeshGeometry, float Alpha /*= 0.0f*/, USelectionSet *Selection /*= nullptr*/) {
	// TODO: Check SelectionSet size

	// Iterate over the sections, and the vertices in the sections.  Do it by index so we
	// can access the same data from TargetMeshGeometry
	int32 nextSelectionIndex = 0;

	if (!TargetMeshGeometry) {
		UE_LOG(LogTemp, Error, TEXT("Lerp: No TargetMeshGeometry"));
		return;
	}
	if (this->sections.Num() != TargetMeshGeometry->sections.Num()) {
		UE_LOG(
			LogTemp, Error, TEXT("Lerp: Cannot lerp geometries with different numbers of sections, %d compared to %d"),
			this->sections.Num(), TargetMeshGeometry->sections.Num()
		);
		return;
	}

	for (int32 sectionIndex = 0; sectionIndex < this->sections.Num(); sectionIndex++) {
		UE_LOG(
			LogTemp, Error, TEXT("Lerp: SECTION INDEX %d"),
			sectionIndex
		);

		if (this->sections[sectionIndex].vertices.Num() != TargetMeshGeometry->sections[sectionIndex].vertices.Num()) {
			UE_LOG(
				LogTemp, Error, TEXT("Lerp: Cannot lerp geometries with different numbers of vertices, %d compared to %d for section %d"),
				this->sections[sectionIndex].vertices.Num(), TargetMeshGeometry->sections[sectionIndex].vertices.Num(), sectionIndex
			);
			return;
		}

		for (int32 vertexIndex = 0; vertexIndex < this->sections[sectionIndex].vertices.Num(); ++vertexIndex) {
			// Get the existing data from the two components.
			FVector vertexFromThis = this->sections[sectionIndex].vertices[vertexIndex];
			FVector vertexFromTarget = TargetMeshGeometry->sections[sectionIndex].vertices[vertexIndex];

			UE_LOG(
				LogTemp, Error, TEXT("Lerp: Vertex index %d"),
				vertexIndex
			);
			// TODO: World/local logic should live here.
			this->sections[sectionIndex].vertices[vertexIndex] = FMath::Lerp(
				vertexFromThis, vertexFromTarget,
				Alpha * (Selection ? Selection->weights[nextSelectionIndex++] : 1.0f)
			);
		}
	}
}