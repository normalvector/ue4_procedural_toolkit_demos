// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "Kismet/KismetMathLibrary.h"
#include "SelectionSet.h"


void USelectionSet::CreateSelectionSet(int32 size)
{
	this->Empty();
	weights.AddZeroed(size);
}

void USelectionSet::Empty()
{
	weights.Empty();
}

USelectionSet *USelectionSet::SetAllWeights(float weight)
{
	for (auto &weightItr : weights) {
		weightItr = weight;
	}
	return this;
}

USelectionSet *USelectionSet::RandomizeWeights(FRandomStream randomStream, float min /*= 0*/, float max /*= 1*/)
{
	for (auto &weight : weights) {
		weight = randomStream.FRandRange(min, max);
	}
	return this;
}
