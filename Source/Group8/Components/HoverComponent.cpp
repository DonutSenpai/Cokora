#include "HoverComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/MovementComponent.h"
#include "Player/AbilityStateComponent.h"


UHoverComponent::UHoverComponent()
{
/*
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	SetComponentTickEnabled( true );
	bTickInEditor = true;*/
}

void UHoverComponent::BeginPlay()
{
	Super::BeginPlay();
	//OwningChar->GetAbilityStateComponent()->AbilityStateChanged.AddDynamic( this, &UHoverComponent::AbilityStateChanged );
}

void UHoverComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	//UE_LOG(LogTemp, Warning, TEXT("Tick tock"));
}

void UHoverComponent::Hover()
{
	
}

void UHoverComponent::StopHover()
{
	
}

void UHoverComponent::SetAbilityActive()
{
	bAbilityActivated = true;
}

void UHoverComponent::AbilityStateChanged( EAbilityState NewState )
{
	if ( NewState == EAbilityState::AS_Hover )
	{
		Hover();
	}
	else if ( OwningChar->GetAbilityStateComponent()->GetAbilityState() == EAbilityState::AS_Hover )
	{
		StopHover();
	}
}
