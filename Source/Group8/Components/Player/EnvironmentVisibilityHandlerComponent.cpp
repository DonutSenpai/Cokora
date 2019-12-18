#include "EnvironmentVisibilityHandlerComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UEnvironmentVisibilityHandlerComponent::UEnvironmentVisibilityHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnvironmentVisibilityHandlerComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	SetOverlappingActors();
	SetOverlappingComponents();
}

void UEnvironmentVisibilityHandlerComponent::SetOverlappingActors()
{

	//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;

	//UKismetSystemLibrary::ComponentOverlapActors(this, GetComponentTransform(), OverlappingActors  )


}

void UEnvironmentVisibilityHandlerComponent::SetOverlappingComponents()
{

}
