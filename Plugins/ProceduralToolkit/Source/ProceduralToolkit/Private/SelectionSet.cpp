// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "SelectionSet.h"


void USelectionSet::CreateSelectionSet(int32 size)
{
	weights.Empty();
	weights.AddZeroed(size);
}

void USelectionSet::SetAllWeights(float weight)
{
	for (auto &weightItr : weights) {
		weightItr = weight;
	}
}

void USelectionSet::RandomizeWeights(FRandomStream randomStream, float min /*= 0*/, float max /*= 0*/)
{
	for (auto &weight : weights) {
		weight = randomStream.FRandRange(min, max);
	}
}

void USelectionSet::Clamp(float min /*= 0*/, float max /*= 1*/)
{
	for (auto &weight : weights) {
		weight = FMath::Clamp(weight, min, max);
	}
}

