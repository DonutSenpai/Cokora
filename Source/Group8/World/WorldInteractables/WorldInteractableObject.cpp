#include "WorldInteractableObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/Player/AbilityStateComponent.h"

AWorldInteractableObject::AWorldInteractableObject()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	BoxCollider->SetupAttachment(RootComponent);

	SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
	SphereCollider->SetupAttachment(RootComponent);

	if ( BoxCollider )
	{
		BoxCollider->OnComponentBeginOverlap.AddDynamic( this, &AWorldInteractableObject::OnBeginBoxOverlap );
		BoxCollider->SetGenerateOverlapEvents( true );
	}
	if (SphereCollider)
	{
		SphereCollider->OnComponentBeginOverlap.AddDynamic( this, &AWorldInteractableObject::OnBeginSphereOverlap );
		SphereCollider->SetGenerateOverlapEvents( true );
	}
}

void AWorldInteractableObject::OnBeginBoxOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult )
{
	if ( OtherActor->ActorHasTag( "WorldCrusher" ) )
	{
		OnOverlap();
	}
	/*else if (OtherActor->IsA<APlayerCharacter>())
	{
		APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
		if (PlayerChar)
		{
			if (PlayerChar->GetAbilityStateComponent()->CurrentAbilityState == EAbilityState::AS_Dash || PlayerChar->GetAbilityStateComponent()->CurrentAbilityState == EAbilityState::AS_AirDash)
			{
				OnOverlap();
			}
		}
	}*/
}

void AWorldInteractableObject::OnBeginSphereOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult )
{
	if ( OtherActor->ActorHasTag( "AboveCrusher" ) )
	{
		OnOverlap();
	}
}
