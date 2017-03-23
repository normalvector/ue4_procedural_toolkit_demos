// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "StaticSetBPLibrary.h"




USelectionSet * UStaticSetBPLibrary::Clamp(USelectionSet *Value, float Min/*=0*/, float Max/*=1*/)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = FMath::Clamp(Value->weights[i], Min, Max);
	}

	return result;
}

USelectionSet *UStaticSetBPLibrary::Add_SelectionSets(USelectionSet *A, USelectionSet *B)
{
	// Need both provided
	if (!A || !B) {
		return nullptr;
	}

	auto result = NewObject<USelectionSet>(A->GetOuter());
	int32 smallestSize = A->weights.Num() < B->weights.Num() ? A->weights.Num() : B->weights.Num();
	for (int32 i = 0; i < smallestSize; i++) {
		result->weights.Emplace(A->weights[i] + B->weights[i]);
	}

	return result;
}
