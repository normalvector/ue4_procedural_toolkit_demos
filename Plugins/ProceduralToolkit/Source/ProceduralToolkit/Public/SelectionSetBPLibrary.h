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
	/// **Math|SelectionSet|Clamp (SelectionSet)**: Clamp all values i7n the set to the minimum and maximum provided.
	///
	/// \param Value	The SelectionSet to clamp
	/// \param Min		The minimum value to clamp to
	/// \param Max		The maximum value to clamp to
	/// \return			The clamped SelectionSet.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Clamp (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Clamp(USelectionSet *Value, float Min=0, float Max=1);

	/// **Math|SelectionSet|Ease (SelectionSet)**: Apply an easing function to all values in SelectionSet
	///
	/// \param Value		The SelectionSet to apply the easing function to
	/// \param EaseFunction	The [Easing Function](https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Kismet/EEasingFunc__Type/index.html) to apply
	/// \param Steps		The number of steps to apply (Only for *Step* easing)
	/// \param BlendExp		Controls the blending of the ease function, only applies for *Ease*, *EaseIn* and *EaseInOut* types
	/// \return				The eased SelectionSet
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Ease (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Ease(USelectionSet *Value, EEasingFunc::Type EaseFunction = EEasingFunc::Linear, int32 Steps = 2, float BlendExp = 2.0f);
	
	/// **Math|SelectionSet|SelectionSet + SelectionSet**: Add two SelectionSets together
	///
	/// \param A			The first SelectionSet to add
	/// \param B			The second SelectionSet to add
	/// \return				The sum of the two SelectionSets
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet + SelectionSet", CompactNodeTitle = "+",
		Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true", Category = "Math|SelectionSet")
	)
		static USelectionSet *Add_SelectionSets(USelectionSet *A, USelectionSet *B);

	/// **Math|SelectionSet|SelectionSet - SelectionSet**: Subtract one SelectionSet from another
	///
	/// \param A			The SelectionSet to subtract from (*A*-B)
	/// \param B			The SelectionSet to subtract (A-*B*)
	/// \return				The result of SelectionSet A - SelectionSet B
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet - SelectionSet", CompactNodeTitle = "-",
			Keywords = "+ add plus", Category = "Math|SelectionSet")
	)
		static USelectionSet *Subtract_SelectionSets(USelectionSet *A, USelectionSet *B);

	/// **Math|SelectionSet|SelectionSet + Float**: Add a constant Float to all values of a SelectionSet
	///
	/// \param Value		The SelectionSet to add the constant to (*Value* + Float)
	/// \param Float		The Float to add to the SelectioNSet (Value + *Float*)
	/// \return The result of the SelectionSet + Float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet + Float", CompactNodeTitle = "+",
			Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true", Category = "Math|SelectionSet")
	)
		static USelectionSet *Add_FloatToSelectionSet(USelectionSet *Value, float Float=0);

	/// **Math|SelectionSet|SelectionSet - float**: Subtract a constant Float from all values of a SelectionSet
	///
	/// \param Value		The SelectionSet to subtract the constant from (*Value* - Float)
	/// \param Float		The Float to subtract from the SelectioNSet (Value - *Float*)
	/// \return The result of the SelectionSet - Float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet - Float", CompactNodeTitle = "-",
			Keywords = "- subtract minus", Category = "Math|SelectionSet")
	)
		static USelectionSet *Subtract_FloatFromSelectionSet(USelectionSet *Value, float Float=0);

	/// **Math|SelectionSet|Float - SelectionSet**: Subtract the values of a SelectionSet from a constant Float
	///
	/// \param Float		The Float to subtract the SelectionSet from (*Float* - Value)
	/// \param Value		The SelectionSet to subtract from the Float (Float - *Value*)
	/// \return The result of Float-Value
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Float - SelectionSet", CompactNodeTitle = "+",
			Keywords = "+ add plus", Category = "Math|SelectionSet")
	)
		static USelectionSet *Subtract_SelectionSetFromFloat(float Float, USelectionSet *Value);

	/// **Math|SelectionSet|SelectionSet \* SelectionSet**: Multiplies the values of two SelectionSets
	///
	/// \param A			The first SelectionSet to multiply (*A*\*B)
	/// \param B			The second SelectionSet to multiply (A\**B*)
	/// \return				The result of the two SelectionSets multiplied by each other
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet * SelectionSet", CompactNodeTitle = "*",
			Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true", Category = "Math|SelectionSet")
	)
		static USelectionSet *Multiply_SelectionSets(USelectionSet *A, USelectionSet *B);

	/// **Math|SelectionSet|SelectionSet * Float**: Multiplies the values of a SelectionSet by a Float
	///
	/// \param Value		The SelectionSet to multiply by the float (*Value* \* Float)
	/// \param Float		The Float to multiply the SelectionSet by (Value \* *Float*)
	/// \return The result of Value \* Float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet * Float", CompactNodeTitle = "*",
			Keywords = "* multiply", CommutativeAssociativeBinaryOperator = "true", Category = "Math|SelectionSet")
	)
		static USelectionSet *Multiply_SelctionSetByFloat(USelectionSet *Value, float Float=1);

	/// **Math|SelectionSet|SelectionSet / SelectionSet**: Divides the values from one SelectionSet by those of another
	///
	/// \param A			The SelectionSet to divide
	/// \param B			The SelectionSet to divide by
	/// \return				A SelectionSet with the values of A/B
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet / SelectionSet", CompactNodeTitle = "/",
			Keywords = "/ divide division", Category = "Math|SelectionSet")
	)
		static USelectionSet *Divide_SelectionSets(USelectionSet *A, USelectionSet *B);

	/// **Math|SelectionSet|SelectionSet / Float**: Divides the values from a SelectionSet by a Float
	///
	/// \param Value			The SelectionSet to divide (*A*/B)
	/// \param Float			The Float to divide by (A/*B*)
	/// \return				A SelectionSet with the values of Value/Float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "SelectionSet / Float", CompactNodeTitle = "/",
			Keywords = "/ divide division", Category = "Math|SelectionSet")
	)
		static USelectionSet *Divide_SelctionSetByFloat(USelectionSet *Value, float Float = 1);


	/// **Math|SelectionSet|OneMinus**: Returns a SelectionSet with values 1- those of another SelectionSet
	///
	/// If a SelectionSet is normalized to the range 0-1 then this will reverse it.
	///
	/// \param Value		The SelectionSet to apply OneMinus to
	/// \return				The result of 1-Value
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "OneMinus (SelectionSet)", Category = "Math|SelectionSet", Keywords="oneminus minus - negate")
	)
		static USelectionSet *OneMinus(USelectionSet *Value);

	/// **Math|SelectionSet|Set**: Set all values of a SelectionSet to the same Float
	///
	/// This can be used to create new SelectionSets to be combined with the original, and will
	/// return a SelectionSet with the same number of weightings and type fas Value but where each
	/// value is set to Float.
	///
	/// \param Value		The source SelectionSet
	/// \param Float		The Float which all weights will be set to
	/// \return				The new SelectionSet
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Set (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Set(USelectionSet *Value, float Float=0);

	/// **Math|SelectionSet|Randomize**: Randomizes a SelectionSet's values between two limits
	///
	/// \param Value		The source SelectionSet
	/// \param RandomStream	The source for random numbers
	/// \param Min			The minimum limit for the random weights
	/// \param Max			The maximum limit for the random weights
	/// \return				The new random SelectionSet
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Randomize (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Randomize(USelectionSet *Value, FRandomStream RandomStream, float Min=0, float Max=1);

	/// **Math|SelectionSet|Max (SelectionSet, SelectionSet)**: Return the maximum value from two SelectionSets
	///
	/// This can be used to combine two SelectionSets with the 'highest' value for height map manipulation and so on.
	///
	/// \param A			The first SelectionSet to obtain maximum values from
	/// \param B			The second SelectionSet to obtain maximum values from
	/// \return The SelectionSet with the maximum values from A and B
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Max (SelectionSet, SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Max_SelectionSets(USelectionSet *A, USelectionSet *B);

	/// **Math|SelectionSet|Min (SelectionSet, SelectionSet)**: Return the minimum value from two SelectionSets
	///
	/// This can be used to combine two SelectionSets with the 'lowest' value for height map manipulation and so on.
	///
	/// \param A			The first SelectionSet to obtain minimum values from
	/// \param B			The second SelectionSet to obtain minumum values from
	/// \return The SelectionSet with the minimum values from A and B
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Min (SelectionSet, SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Min_SelectionSets(USelectionSet *A, USelectionSet *B);

	/// **Math|SelectionSet|Max (Float)**: Return the maximum of a SelectionSet and a Float
	///
	/// This can be viewed as the 'bottom half' of a clamp, making sure all values of a SelectionSet
	/// are at least equal to the Float provided.
	///
	/// \param Value		The SelectionSet to apply the filter to
	/// \param Float		The Float to compare the SelectionSet values with
	/// \return The result of the maximum of the SelectionSet and the Float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Max (SelectionSet, Float)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Max_SelectionSetAgainstFloat(USelectionSet *Value, float Float);


	/// **Math|SelectionSet|Min (Float)**: Return the minimum of a SelectionSet and a Float
	///
	/// This can be viewed as the 'top half' of a clamp, making sure all values of a SelectionSet
	/// are capped by the Float provided.
	///
	/// \param Value		The SelectionSet to apply the filter to
	/// \param Float		The Float to compare the SelectionSet values with
	/// \return The result of the minimum of the SelectionSet and the Float
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Min (SelectionSet, Float)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Min_SelectionSetAgainstFloat(USelectionSet *Value, float Float);

	/// **Math|SelectionSet|Lerp(SelectionSet)**: Apply a lerp to blend two SelectionSets together
	///
	/// \param A		The first SelectionSet to apply the lerp to
	/// \param B		The second SelectionSet to apply the lerp to
	/// \param Alpha	The blend factor between the two SelectionSets, 0=A, 1=B
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Lerp (SelectionSet)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Lerp_SelectionSets(USelectionSet *A, USelectionSet *B, float Alpha=0);

	/// **Math|SelectionSet|Lerp(SelectionSet, Float)**: Apply a lerp to blend a SelectionSet against a Float
	///
	/// \param Value	The first SelectionSet to apply the lerp to
	/// \param Float	The second SelectionSet to apply the lerp to
	/// \param Alpha	The blend factor between the two SelectionSets, 0=Original Value, 1=Replaced with Float
	/// \return The lerped SelectionSet
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Lerp (SelectionSet, Float)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Lerp_SelectionSetWithFloat(USelectionSet *Value, float Float, float Alpha = 0);

	/// **Math|SelectionSet|RemapToCurve(SelectionSet, Curve)**: Remap the values of a SelectionSet to a CurveFloat
	///
	/// The remap will return the T=0 value of the Curve for Weight=0, and the T=Max value of the Curve for Weight=1,
	/// and values in between will be suitably scaled.
	///
	/// \param Value	The SelectionSet to apply the remap to
	/// \param Curve	The CurveFloat to shape the remap
	/// \return The result of the SelectionSet remapped to the Curve
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "RemapToCurve (SelectionSet, Curve)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Remap_SelectionSetToCurve(USelectionSet *Value, UCurveFloat *Curve);

	/// **Math|SelectionSetRemapToRange(SelectionSet, float, float)**: Remaps all of the values between a new min/max
	///
	/// This goes through all of the weightings in the SelectionSet and remaps the lowest one to Min, the highest one
	/// to Max, and all others to a scaled value in between.
	///
	/// This can be useful when dealing with SelectionSets from noise functions and other such effects where there's
	/// no guarantee to the range, and remap it to 0-1 ready for use.
	///
	/// \param Value	The SelectionSet to remap
	///	\param Min		The minimum value to remap to
	/// \param Max		The maximum value to remap to
	/// \return The SelectionSet remapped to Min-Max
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "RemapToRange (SelectionSet, float, float)", Category = "Math|SelectionSet")
	)
		static USelectionSet *Remap_Range(USelectionSet *Value, float Min = 0.0f, float Max = 1.0f);
};
