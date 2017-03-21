// (c)2017 Paul Golds, released under MIT License.

#include "ProceduralToolkit.h"
#include "DeformMeshComponent.h"


// Sets default values for this component's properties
UDeformMeshComponent::UDeformMeshComponent()
{
	// This component can't ever tick.  It's possible another component's tick will update us, but we don't tick.
	PrimaryComponentTick.bCanEverTick = false;
}
