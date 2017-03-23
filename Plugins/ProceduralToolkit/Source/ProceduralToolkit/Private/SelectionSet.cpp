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

USelectionSet *USelectionSet::Clamp(float min /*= 0*/, float max /*= 1*/)
{
	for (auto &weight : weights) {
		weight = FMath::Clamp(weight, min, max);
	}
	return this;
}

USelectionSet *USelectionSet::Ease(EEasingFunc::Type easeFunction /*=Linear*/, int32 steps /*=2*/, float blendExp /*=2.0*/)
{
	// TODO: This can be more efficient using lambdas.
	for (auto &weight : weights) {
		switch (easeFunction)
		{
		case EEasingFunc::Step:
			weight = FMath::InterpStep<float>(0.f, 1.f, weight, steps);
			break;
		case EEasingFunc::SinusoidalIn:
			weight = FMath::InterpSinIn<float>(0.f, 1.f, weight);
			break;
		case EEasingFunc::SinusoidalOut:
			weight = FMath::InterpSinOut<float>(0.f, 1.f, weight);
			break;
		case EEasingFunc::SinusoidalInOut:
			weight = FMath::InterpSinInOut<float>(0.f, 1.f, weight);
			break;
		case EEasingFunc::EaseIn:
			weight = FMath::InterpEaseIn<float>(0.f, 1.f, weight, blendExp);
			break;
		case EEasingFunc::EaseOut:
			weight = FMath::InterpEaseOut<float>(0.f, 1.f, weight, blendExp);
			break;
		case EEasingFunc::EaseInOut:
			weight = FMath::InterpEaseInOut<float>(0.f, 1.f, weight, blendExp);
			break;
		case EEasingFunc::ExpoIn:
			weight = FMath::InterpExpoIn<float>(0.f, 1.f, weight);
			break;
		case EEasingFunc::ExpoOut:
			weight = FMath::InterpExpoOut<float>(0.f, 1.f, weight);
			break;
		case EEasingFunc::ExpoInOut:
			weight = FMath::InterpExpoInOut<float>(0.f, 1.f, weight);
			break;
		case EEasingFunc::CircularIn:
			weight = FMath::InterpCircularIn<float>(0.f, 1.f, weight);
			break;
		case EEasingFunc::CircularOut:
			weight = FMath::InterpCircularOut<float>(0.f, 1.f, weight);
			break;
		case EEasingFunc::CircularInOut:
			weight = FMath::InterpCircularInOut<float>(0.f, 1.f, weight);
			break;
		default:
			break;
		}

	}
	return this;
}