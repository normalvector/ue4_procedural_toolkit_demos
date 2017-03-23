// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "StaticSetBPLibrary.h"




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
