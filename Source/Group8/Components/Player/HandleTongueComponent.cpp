#include "HandleTongueComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

UHandleTongueComponent::UHandleTongueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	SetComponentTickEnabled( true );
	bTickInEditor = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SphereCollider = CreateDefaultSubobject<USphereComponent>("Tongue Collider");
}

void UHandleTongueComponent::BeginPlay()
{

}

void UHandleTongueComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{

}

void UHandleTongueComponent::OnBeginSphereOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult )
{

}

void UHandleTongueComponent::UseTongue()
{
	if (bTongueActive)
	{

	}
}

void UHandleTongueComponent::SetAbilityActive()
{
	bTongueActive = true;
}

