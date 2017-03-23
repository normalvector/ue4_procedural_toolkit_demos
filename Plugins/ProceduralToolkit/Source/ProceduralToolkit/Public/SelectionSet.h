// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "SelectionSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROCEDURALTOOLKIT_API USelectionSet : public UObject
{
	GENERATED_BODY()
	
public:
	// The set of weights this set has.
	UPROPERTY(BlueprintReadWrite, Category = SelectionSet)
		TArray<float> weights;

	// Populates the set with the supplied number weights 
	void CreateSelectionSet(int32 size);

	// Empties the set
	void Empty();

	// Set all values to same value
	UFUNCTION(BlueprintCallable, Category = SelectionSet)
		USelectionSet *SetAllWeights(float weight);

	// Randomize the weights of the selection set
	UFUNCTION(BlueprintCallable, Category = SelectionSet)
		USelectionSet *RandomizeWeights(FRandomStream randomStream, float minWeight = 0, float maxWeight = 1);

	// Apply an easing function
	UFUNCTION(BlueprintCallable, Category = SelectionSet)
		USelectionSet *Ease(EEasingFunc::Type easeFunction = EEasingFunc::Linear, int32 steps = 2, float blendExp = 2.0f);
};
