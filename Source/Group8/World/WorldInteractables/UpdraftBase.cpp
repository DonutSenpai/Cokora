#include "UpdraftBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerCharacter.h"
#include "../../Components/HoverComponent.h"
#include "Components/Player/AbilityStateComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../DashInteractables/DashPushInteractableActor.h"

AUpdraftBase::AUpdraftBase()
{
	Root = CreateDefaultSubobject<USceneComponent>( "Root" );
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>( "Mesh" );
	Mesh->SetupAttachment( RootComponent );

	UpdraftCollision = CreateDefaultSubobject<UCapsuleComponent>( "UpdraftCollision" );
	UpdraftCollision->SetupAttachment( RootComponent );
	UpdraftCollision->SetGenerateOverlapEvents( true );

	UpdraftExitCollision = CreateDefaultSubobject<UCapsuleComponent>( "UpdraftExitCollision" );
	UpdraftExitCollision->SetupAttachment( RootComponent );
	UpdraftExitCollision->SetGenerateOverlapEvents( true );

	//UpdraftCollision->OnComponentBeginOverlap.AddDynamic( this, &AUpdraftBase::OnPlayerBeginOverlap );

	TargetPlayerLocation = CreateDefaultSubobject<UStaticMeshComponent>( "TargetPlayerLocation" );
	TargetPlayerLocation->SetupAttachment( RootComponent );

}

void AUpdraftBase::BeginPlay()
{
	UpdraftCollision->OnComponentBeginOverlap.AddDynamic( this, &AUpdraftBase::OnBeginOverlap );
	UpdraftCollision->OnComponentEndOverlap.AddDynamic( this, &AUpdraftBase::OnInnerEndOverlap );

	UpdraftExitCollision->OnComponentEndOverlap.AddDynamic( this, &AUpdraftBase::OnOuterEndOverlap );

	UpdraftCollision->GetOverlappingActors( OverlappingActors );

	for( int i = 0; i < OverlappingActors.Num(); i++ )
	{
		if( ADashInteractableActor* OverlappingObject = Cast<ADashInteractableActor>( OverlappingActors[i] ) )
		{
			if( !bIsBlocked )
			{
				DeactivateDraft();
			}

			OverlappingObject->OnDestroyedByPlayer.AddDynamic( this, &AUpdraftBase::ActivateDraft );
		}
	}

}

void AUpdraftBase::OnBeginOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult )
{
	if( ADashInteractableActor* DashInteractable = Cast<ADashInteractableActor>( OtherActor ) )
	{
		if( !bIsBlocked )
		{
			DeactivateDraft();
		}
		return;
	}
	else if( APlayerCharacter* Player = Cast<APlayerCharacter>( OtherActor ) )
	{
		if( bPlayerIsOverlapping )
		{
			return;
		}
		else
		{
			bPlayerIsOverlapping = true;
		}
		UE_LOG( LogTemp, Warning, TEXT( "Player Begin overlap" ) );

		OverlappingPlayer = Player;

		if( !bIsBlocked )
		{
			if( OverlappingPlayer->GetAbilityStateComponent()->GetAbilityState() != EAbilityState::AS_Hover )
			{
				OverlappingPlayer->AbilityStateComponent->HoverStart( this );
			}
		}
	}

}

void AUpdraftBase::OnInnerEndOverlap( class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	ADashInteractableActor* DashInteractable = Cast<ADashInteractableActor>( OtherActor );

	if( DashInteractable )
	{
		//UE_LOG( LogTemp, Warning, TEXT( "Interactable END overlap" ) );
		if( bIsBlocked )
		{
			ActivateDraft();
		}
	}
}

void AUpdraftBase::OnOuterEndOverlap( class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	if( OtherActor == OverlappingPlayer )
	{
		bPlayerIsOverlapping = false;

		UE_LOG( LogTemp, Warning, TEXT( "Player END overlap" ) );
		if( OverlappingPlayer->GetAbilityStateComponent()->GetAbilityState() == EAbilityState::AS_Hover )
		{
			OverlappingPlayer->AbilityStateComponent->HoverEnd( this );
		}

		OverlappingPlayer = nullptr;
	}
}

void AUpdraftBase::ActivateDraft()
{
	bIsBlocked = false;

	OnDraftActivated();

	if( OverlappingPlayer )
	{
		if( OverlappingPlayer->GetAbilityStateComponent()->GetAbilityState() != EAbilityState::AS_Hover )
		{
			OverlappingPlayer->AbilityStateComponent->HoverStart( this );
		}
	}
}

void AUpdraftBase::DeactivateDraft()
{
	bIsBlocked = true;

	OnDraftDeactivated();
	if( OverlappingPlayer && OverlappingPlayer->GetAbilityStateComponent()->GetAbilityState() == EAbilityState::AS_Hover )
	{
		OverlappingPlayer->AbilityStateComponent->HoverEnd( this );
	}
}
