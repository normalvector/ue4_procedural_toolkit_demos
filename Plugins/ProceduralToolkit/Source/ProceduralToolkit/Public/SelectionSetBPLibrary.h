// (c)2017 Paul Golds, released under MIT License.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SelectionSet.h"
#include "SelectionSetBPLibrary.generated.h"


/// This is a *BlueprintFunctionLibrary* implementing the Blueprint nodes which are
/// used to manipulate *SelectionSet* items, including modifying and combining them.
///
/// These methods are designed to return modified values of SelectionSets rather than
/// change the values provided to them.
///
/// \todo RemapToRange - Set an absolute min/max and remap based on them, useful for the noise functions
UCLASS()
class PROCEDURALTOOLKIT_API USelectionSetBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/// Clamp all values in the set to the minimum and maximum provided.
	///
	/// \param Min		The minimum value to clamp to
	/// \param Max		The maximum value to clamp to
	/// \return			The clamped SelectionSet.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Clamp (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Clamp(USelectionSet *Value, float Min=0, float Max=1);

	// Ease (SelectionSet)
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Ease (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Ease(USelectionSet *Value, EEasingFunc::Type easeFunction = EEasingFunc::Linear, int32 steps = 2, float blendExp = 2.0f);
	
	// SelectionSet + SelectionSet
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet + SelectionSet", CompactNodeTitle = "+",
		Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true", Category = "Math|SelectionSet")
	)
		static USelectionSet *Add_SelectionSets(USelectionSet *A, USelectionSet *B);

	// SelectionSet - SelectionSet
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet - SelectionSet", CompactNodeTitle = "-",
			Keywords = "+ add plus", Category = "Math|SelectionSet")
	)
		static USelectionSet *Subtract_SelectionSets(USelectionSet *A, USelectionSet *B);

	// SelectionSet + Float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet + Float", CompactNodeTitle = "+",
			Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true", Category = "Math|SelectionSet")
	)
		static USelectionSet *Add_FloatToSelectionSet(USelectionSet *Value, float Float=0);

	// SelectionSet - float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet - Float", CompactNodeTitle = "-",
			Keywords = "- subtract minus", Category = "Math|SelectionSet")
	)
		static USelectionSet *Subtract_FloatFromSelectionSet(USelectionSet *Value, float Float=0);

	// Float - SelectionSet
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Float - SelectionSet", CompactNodeTitle = "+",
			Keywords = "+ add plus", Category = "Math|SelectionSet")
	)
		static USelectionSet *Subtract_SelectionSetFromFloat(float Float, USelectionSet *Value);

	// SelectionSet * SelectionSet
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet * SelectionSet", CompactNodeTitle = "*",
			Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true", Category = "Math|SelectionSet")
	)
		static USelectionSet *Multiply_SelectionSets(USelectionSet *A, USelectionSet *B);

	// SelectionSet * float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet * Float", CompactNodeTitle = "*",
			Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true", Category = "Math|SelectionSet")
	)
		static USelectionSet *Multiply_SelctionSetByFloat(USelectionSet *Value, float Float=1);

	// SelectionSet / SelectionSet
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet / SelectionSet", CompactNodeTitle = "/",
			Keywords = "/ divide division", Category = "Math|SelectionSet")
	)
		static USelectionSet *Divide_SelectionSets(USelectionSet *A, USelectionSet *B);

	// SelectionSet / float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet / Float", CompactNodeTitle = "/",
			Keywords = "/ divide division", Category = "Math|SelectionSet")
	)
		static USelectionSet *Divide_SelctionSetByFloat(USelectionSet *Value, float Float = 1);

	// OneMinus
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "OneMinus (SelectionSet)", Category = "Math|SelectionSet", Keywords="oneminus minus - negate")
	)
		static USelectionSet *OneMinus(USelectionSet *Value);

	// Set float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Set (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Set(USelectionSet *Value, float Float=0);

	// Randomize
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Randomize (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Randomize(USelectionSet *Value, FRandomStream RandomStream, float Min=0, float Max=1);

	// Max
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Max (SelectionSet, SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Max_SelectionSets(USelectionSet *A, USelectionSet *B);

	// Min
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Min (SelectionSet, SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Min_SelectionSets(USelectionSet *A, USelectionSet *B);

	// Max (Float)
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Max (SelectionSet, Float)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Max_SelectionSetAgainstFloat(USelectionSet *Value, float Float);

	// Min (Float)
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Min (SelectionSet, Float)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Min_SelectionSetAgainstFloat(USelectionSet *Value, float Float);

	// Lerp(SelectionSet)
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Lerp (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Lerp_SelectionSets(USelectionSet *A, USelectionSet *B, float Alpha=0);

	// Lerp(SelectionSet, float)
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Lerp (SelectionSet, Float)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Lerp_SelectionSetWithFloat(USelectionSet *A, float B, float Alpha = 0);
};
