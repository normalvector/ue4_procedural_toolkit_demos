// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "SelectionSetBPLibrary.h"




USelectionSet * USelectionSetBPLibrary::Clamp(USelectionSet *Value, float Min/*=0*/, float Max/*=1*/)
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

USelectionSet * USelectionSetBPLibrary::Ease(USelectionSet *Value, EEasingFunc::Type easeFunction /*= EEasingFunc::Linear*/, int32 steps /*= 2*/, float blendExp /*= 2.0f*/)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);
	
	// TODO: This can be more efficient with lambdas.
	for (int32 i = 0; i < size; i++) {
		switch (easeFunction)
		{
		case EEasingFunc::Step:
			result->weights[i] = FMath::InterpStep<float>(0.f, 1.f, Value->weights[i], steps);
			break;
		case EEasingFunc::SinusoidalIn:
			result->weights[i] = FMath::InterpSinIn<float>(0.f, 1.f, Value->weights[i]);
			break;
		case EEasingFunc::SinusoidalOut:
			result->weights[i] = FMath::InterpSinOut<float>(0.f, 1.f, Value->weights[i]);
			break;
		case EEasingFunc::SinusoidalInOut:
			result->weights[i] = FMath::InterpSinInOut<float>(0.f, 1.f, Value->weights[i]);
			break;
		case EEasingFunc::EaseIn:
			result->weights[i] = FMath::InterpEaseIn<float>(0.f, 1.f, Value->weights[i], blendExp);
			break;
		case EEasingFunc::EaseOut:
			result->weights[i] = FMath::InterpEaseOut<float>(0.f, 1.f, Value->weights[i], blendExp);
			break;
		case EEasingFunc::EaseInOut:
			result->weights[i] = FMath::InterpEaseInOut<float>(0.f, 1.f, Value->weights[i], blendExp);
			break;
		case EEasingFunc::ExpoIn:
			result->weights[i] = FMath::InterpExpoIn<float>(0.f, 1.f, Value->weights[i]);
			break;
		case EEasingFunc::ExpoOut:
			result->weights[i] = FMath::InterpExpoOut<float>(0.f, 1.f, Value->weights[i]);
			break;
		case EEasingFunc::ExpoInOut:
			result->weights[i] = FMath::InterpExpoInOut<float>(0.f, 1.f, Value->weights[i]);
			break;
		case EEasingFunc::CircularIn:
			result->weights[i] = FMath::InterpCircularIn<float>(0.f, 1.f, Value->weights[i]);
			break;
		case EEasingFunc::CircularOut:
			result->weights[i] = FMath::InterpCircularOut<float>(0.f, 1.f, Value->weights[i]);
			break;
		case EEasingFunc::CircularInOut:
			result->weights[i] = FMath::InterpCircularInOut<float>(0.f, 1.f, Value->weights[i]);
			break;
		default:
			// Do nothing: linear.
			result->weights[i] = Value->weights[i];
			break;
		}
	}

	return result;
}

USelectionSet *USelectionSetBPLibrary::Add_SelectionSets(USelectionSet *A, USelectionSet *B)
{
	// Need both provided
	if (!A || !B) {
		return nullptr;
	}

	auto result = NewObject<USelectionSet>(A->GetOuter());
	int32 smallestSize = A->weights.Num() < B->weights.Num() ? A->weights.Num() : B->weights.Num();
	result->weights.SetNumZeroed(smallestSize);

	for (int32 i = 0; i < smallestSize; i++) {
		result->weights[i] = A->weights[i] + B->weights[i];
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Subtract_SelectionSets(USelectionSet *A, USelectionSet *B)
{
	// Need both provided
	if (!A || !B) {
		return nullptr;
	}

	auto result = NewObject<USelectionSet>(A->GetOuter());
	int32 smallestSize = A->weights.Num() < B->weights.Num() ? A->weights.Num() : B->weights.Num();
	result->weights.SetNumZeroed(smallestSize);

	for (int32 i = 0; i < smallestSize; i++) {
		result->weights[i] = A->weights[i] - B->weights[i];
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Add_FloatToSelectionSet(USelectionSet *Value, float Float/*=0*/)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = Value->weights[i] + Float;
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Subtract_FloatFromSelectionSet(USelectionSet *Value, float Float/*=0*/)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = Value->weights[i] - Float;
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Subtract_SelectionSetFromFloat(float Float, USelectionSet *Value)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = Float - Value->weights[i];
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Multiply_SelectionSets(USelectionSet *A, USelectionSet *B)
{
	// Need both provided
	if (!A || !B) {
		return nullptr;
	}

	auto result = NewObject<USelectionSet>(A->GetOuter());
	int32 smallestSize = A->weights.Num() < B->weights.Num() ? A->weights.Num() : B->weights.Num();
	result->weights.SetNumZeroed(smallestSize);
	for (int32 i = 0; i < smallestSize; i++) {
		result->weights[i] = A->weights[i] * B->weights[i];
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Multiply_SelctionSetByFloat(USelectionSet *Value, float Float/*=1*/)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = Value->weights[i] * Float;
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Divide_SelectionSets(USelectionSet *A, USelectionSet *B)
{
	// Need both provided
	if (!A || !B) {
		return nullptr;
	}

	auto result = NewObject<USelectionSet>(A->GetOuter());
	int32 smallestSize = A->weights.Num() < B->weights.Num() ? A->weights.Num() : B->weights.Num();
	result->weights.SetNumZeroed(smallestSize);

	for (int32 i = 0; i < smallestSize; i++) {
		result->weights[i] = A->weights[i] / B->weights[i];
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Divide_SelctionSetByFloat(USelectionSet *Value, float Float /*= 1*/)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = Value->weights[i] / Float;
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::OneMinus(USelectionSet *Value)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = 1.0f - Value->weights[i];
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Set(USelectionSet *Value, float Float/*=0*/)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = Float;
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Randomize(USelectionSet *Value, FRandomStream RandomStream, float Min/*=0*/, float Max/*=1*/)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = RandomStream.FRandRange(Min, Max);
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Max_SelectionSets(USelectionSet *A, USelectionSet *B)
{
	// Need both provided
	if (!A || !B) {
		return nullptr;
	}

	auto result = NewObject<USelectionSet>(A->GetOuter());
	int32 smallestSize = A->weights.Num() < B->weights.Num() ? A->weights.Num() : B->weights.Num();
	result->weights.SetNumZeroed(smallestSize);

	for (int32 i = 0; i < smallestSize; i++) {
		//result->weights[i] = A->weights[i] > B->weights[i] ? A->weights[i] : B->weights[i];
		result->weights[i] = FMath::Max(A->weights[i], B->weights[i]);
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Min_SelectionSets(USelectionSet *A, USelectionSet *B)
{
	// Need both provided
	if (!A || !B) {
		return nullptr;
	}

	auto result = NewObject<USelectionSet>(A->GetOuter());
	int32 smallestSize = A->weights.Num() < B->weights.Num() ? A->weights.Num() : B->weights.Num();
	result->weights.SetNumZeroed(smallestSize);

	for (int32 i = 0; i < smallestSize; i++) {
		//result->weights[i] = A->weights[i] < B->weights[i] ? A->weights[i] : B->weights[i];
		result->weights[i] = FMath::Min(A->weights[i], B->weights[i]);
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Max_SelectionSetAgainstFloat(USelectionSet *Value, float Float)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		//result->weights[i] = Value->weights[i] > Float ? Value->weights[i] : Float;
		result->weights[i] = FMath::Max(Value->weights[i], Float);
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Min_SelectionSetAgainstFloat(USelectionSet *Value, float Float)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		//result->weights[i] = Value->weights[i] < Float ? Value->weights[i] : Float;
		result->weights[i] = FMath::Min(Value->weights[i], Float);
	}

	return result;
}

USelectionSet * USelectionSetBPLibrary::Lerp_SelectionSets(USelectionSet *A, USelectionSet *B, float Alpha/*=0*/)
{
	// Need both provided
	if (!A || !B) {
		return nullptr;
	}

	auto result = NewObject<USelectionSet>(A->GetOuter());
	int32 smallestSize = A->weights.Num() < B->weights.Num() ? A->weights.Num() : B->weights.Num();
	result->weights.SetNumZeroed(smallestSize);

	for (int32 i = 0; i < smallestSize; i++) {
		result->weights[i] = FMath::Lerp(A->weights[i], B->weights[i], Alpha);
	}

	return result;

}

USelectionSet * USelectionSetBPLibrary::Lerp_SelectionSetWithFloat(USelectionSet *Value, float Float, float Alpha /*= 0*/)
{
	// Need a SelectionSet
	if (!Value) {
		return nullptr;
	}

	USelectionSet *result = NewObject<USelectionSet>(Value->GetOuter());
	auto size = Value->weights.Num();
	result->weights.SetNumZeroed(size);

	for (int32 i = 0; i < size; i++) {
		result->weights[i] = FMath::Lerp(Value->weights[i], Float, Alpha);
	}

	return result;
}