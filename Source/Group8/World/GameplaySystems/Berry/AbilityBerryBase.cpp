#include "AbilityBerryBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/HoverComponent.h"
#include "Components/DashComponent.h"
#include "Components/Player/AbilityStateComponent.h"

AAbilityBerryBase::AAbilityBerryBase()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
	SphereCollider->SetupAttachment(RootComponent);

	if (SphereCollider)
	{
		SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AAbilityBerryBase::OnBeginSphereOverlap);
		SphereCollider->SetGenerateOverlapEvents(true);
	}
}

void AAbilityBerryBase::BeginPlay()
{

}

void AAbilityBerryBase::OnBeginSphereOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult )
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->GetAbilityStateComponent()->SetStateToNone();
		switch(BerryType)
		{
			case EAbilityBerry::AB_Hover:
			{
				PlayerCharacter->GetHoverComponent()->SetAbilityActive();
				OnPickUp(BerryType);
				break;
			}
			case EAbilityBerry::AB_Dash:
			{
				PlayerCharacter->GetDashComponent()->SetAbilityActive();
				OnPickUp(BerryType);
				break;
			}
			case EAbilityBerry::AB_Jump:
			{
				PlayerCharacter->GetAbilityStateComponent()->ActivateJump();
				OnPickUp(BerryType);
				break;
			}
		}
	}
}