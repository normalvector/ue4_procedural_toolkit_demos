// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SelectionSet.h"
#include "StaticSetBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALTOOLKIT_API UStaticSetBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, 
		meta = (DisplayName = "Clamp (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Clamp(USelectionSet *Value, float Min=0, float Max=1);

	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet + SelectionSet", CompactNodeTitle = "+",
		Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true", Category = "Math|SelectionSet")
	)
		static USelectionSet *Add_SelectionSets(USelectionSet *A, USelectionSet *B);
	
	
};
