#include "AbilityStateComponent.h"
#include "../HoverComponent.h"
#include "../DashComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "World/WorldInteractables/UpdraftBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../HandlePlayerMovementComponent.h"


UAbilityStateComponent::UAbilityStateComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityStateComponent::BeginPlay()
{
	Super::BeginPlay();

	DashEndTimerDelegate.BindUFunction( this, FName( "DashEnd" ) );

	DashCooldown = OwningChar->GetDashComponent()->GetDashCooldown();

	//OwningChar->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic( this, &UAbilityStateComponent::OnCharacterBeginOverlap );
	//OwningChar->GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic( this, &UAbilityStateComponent::OnEndOverlap );

	OwningChar->GetHandleMovementComponent()->OnStartedWalking.AddDynamic( this, &UAbilityStateComponent::OnStartedWalkingInternal );
	OwningChar->GetHandleMovementComponent()->OnStoppedWalking.AddDynamic( this, &UAbilityStateComponent::OnStoppedWalkingInternal );
	OwningChar->GetDashComponent()->OnDashHit.AddDynamic( this, &UAbilityStateComponent::OnDashHitInternal );

}

void UAbilityStateComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if( DashCooldownTimer > 0.f )
	{
		DashCooldownTimer -= DeltaTime;
		//UE_LOG( LogTemp, Warning, TEXT( "%f" ), DashCooldownTimer );
	}
}

//void UAbilityStateComponent::OnCharacterBeginOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult )
//{
//	Updraft = Cast<AUpdraftBase>( OtherActor );
//
//	if( Updraft )
//	{
//		HoverStart( Updraft );
//	}
//}

//void UAbilityStateComponent::OnEndOverlap( class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
//{
//	if( Updraft == OtherActor )
//	{
//		HoverEnd( Updraft );
//	}
//}

void UAbilityStateComponent::ActivateJump()
{
	bJumpIsActivated = true;
}

void UAbilityStateComponent::JumpInput()
{
	if( CurrentAbilityState == EAbilityState::AS_None || CurrentAbilityState == EAbilityState::AS_Walking )
	{
		if( bJumpIsActivated )
		{
			ChangeAbilityState( EAbilityState::AS_Jump );
		}
	}
}

void UAbilityStateComponent::DashInput()
{

	if( CurrentAbilityState == EAbilityState::AS_Dash || CurrentAbilityState == EAbilityState::AS_AirDash || CurrentAbilityState == EAbilityState::AS_ShortDash )
		return;

	if( bHasDashed && CurrentAbilityState != EAbilityState::AS_Hover )
		return;

	if( DashCooldownTimer > 0.f )
		return;

	if( OwningChar->GetDashComponent()->bIsActivated == false )
		return;

	bIsDashing = true;
	DashCooldownTimer = DashCooldown;
	bHasDashed = true;
	OwningChar->MoveComponent->bInputDisabled = true;

	if( CurrentAbilityState == EAbilityState::AS_Hover && UpdraftReference )
	{
		HoverEnd( UpdraftReference );
	}

	if( CurrentAbilityState == EAbilityState::AS_None || CurrentAbilityState == EAbilityState::AS_Walking )
	{
		if( OwningChar->GetDashComponent()->IsOverlappingDashable() )
		{
			ChangeAbilityState( EAbilityState::AS_ShortDash );
		}
		else
		{
			ChangeAbilityState( EAbilityState::AS_Dash );
		}
	}
	else
	{
		ChangeAbilityState( EAbilityState::AS_AirDash );
	}

	float StopDashTime = CurrentAbilityState == EAbilityState::AS_ShortDash ? 1.f : OwningChar->GetDashComponent()->DashDuration;

	GetWorld()->GetTimerManager().SetTimer( DashEndHandle, DashEndTimerDelegate, 0.1f, false, StopDashTime );

}




void UAbilityStateComponent::OnLanded()
{
	if( OwningChar->GetHandleMovementComponent()->GetHasForwardInput() || OwningChar->GetHandleMovementComponent()->GetHasRightInput() )
	{
		ChangeAbilityState( EAbilityState::AS_Walking );
	}
	else
	{
		ChangeAbilityState( EAbilityState::AS_None );
	}

	bHasDashed = false;
	bHasHovered = false;

}

void UAbilityStateComponent::WalkedOffLedge()
{
	if( CurrentAbilityState == EAbilityState::AS_None || CurrentAbilityState == EAbilityState::AS_Walking )
	{
		ChangeAbilityState( EAbilityState::AS_Falling );
	}
}

void UAbilityStateComponent::OnStartedWalkingInternal()
{
	if( CurrentAbilityState == EAbilityState::AS_None )
	{
		ChangeAbilityState( EAbilityState::AS_Walking );
	}
}

void UAbilityStateComponent::OnStoppedWalkingInternal()
{
	if( CurrentAbilityState == EAbilityState::AS_Walking )
	{
		ChangeAbilityState( EAbilityState::AS_None );
	}
}

void UAbilityStateComponent::DashEnd()
{
	bIsDashing = false;
	OwningChar->MoveComponent->bInputDisabled = false;

	if( CurrentAbilityState == EAbilityState::AS_Dash || CurrentAbilityState == EAbilityState::AS_ShortDash )
	{
		if( OwningChar->GetHandleMovementComponent()->GetHasAnyInput() )
		{
			ChangeAbilityState( EAbilityState::AS_Walking );
		}
		else
		{
			ChangeAbilityState( EAbilityState::AS_None );
		}

		bHasDashed = false;
	}
	else if( CurrentAbilityState == EAbilityState::AS_AirDash && OwningChar->GetCharacterMovement()->IsFalling() )
	{
		ChangeAbilityState( EAbilityState::AS_Falling );
	}
	else if( CurrentAbilityState == EAbilityState::AS_AirDash && !OwningChar->GetCharacterMovement()->IsFalling() )
	{
		if( OwningChar->GetHandleMovementComponent()->GetHasAnyInput() )
		{
			ChangeAbilityState( EAbilityState::AS_Walking );
		}
		else
		{
			ChangeAbilityState( EAbilityState::AS_None );
		}
	}
}

bool UAbilityStateComponent::HoverStart( AUpdraftBase* Updraft )
{

	UpdraftReference = Updraft;
	DashCooldownTimer = 1.f;

	if( CurrentAbilityState == EAbilityState::AS_Dash || CurrentAbilityState == EAbilityState::AS_AirDash || CurrentAbilityState == EAbilityState::AS_ShortDash )
	{
		if( GetWorld()->GetTimerManager().IsTimerActive( DashEndHandle ) )
		{
			GetWorld()->GetTimerManager().PauseTimer( DashEndHandle );
		}

		DashEnd();
	}

	ChangeAbilityState( EAbilityState::AS_Hover );
	OnHoverStart.Broadcast( Updraft );
	return true;
}


void UAbilityStateComponent::HoverEnd( AUpdraftBase* Updraft )
{
	if( CurrentAbilityState == EAbilityState::AS_Hover )
	{
		OnHoverEnd.Broadcast( Updraft );
		UpdraftReference = nullptr;

		if( bIsDashing )
		{
			return;
		}

		if( OwningChar->GetCharacterMovement()->IsFalling() )
		{
			ChangeAbilityState( EAbilityState::AS_Falling );
		}
		else
		{
			ChangeAbilityState( EAbilityState::AS_None );
		}


	}
}


void UAbilityStateComponent::SetStateToNone()
{
	ChangeAbilityState( EAbilityState::AS_None );
}

void UAbilityStateComponent::SetStateToWalking()
{
	ChangeAbilityState( EAbilityState::AS_Walking );
}

void UAbilityStateComponent::ChangeAbilityState( EAbilityState NewState )
{
	if( bInputDisabled ) return;

	PreviousAbilityState = CurrentAbilityState;
	CurrentAbilityState = NewState;

	AbilityStateChanged.Broadcast( CurrentAbilityState, PreviousAbilityState );

	OwningChar->PrintEnum( CurrentAbilityState );

	FString StringToPrint;

	const UEnum* EnumPtr = FindObject<UEnum>( ANY_PACKAGE, TEXT( "EAbilityState" ), true );
	if( !EnumPtr ) StringToPrint = FString( "Invalid" );

	StringToPrint = EnumPtr->GetEnumName( ( int32 )NewState );

	UE_LOG( LogTemp, Warning, TEXT( "Ability State: %s" ), *StringToPrint );


	//Down below are checks for current ability state and previous ability state
		//This is for calling the proper events for blueprint uses

	switch( PreviousAbilityState )
	{
	case EAbilityState::AS_Dash:
	{
		AnimationEventOnDashEnd.Broadcast( PreviousAbilityState );
		break;
	}
	case EAbilityState::AS_AirDash:
	{
		AnimationEventOnDashEnd.Broadcast( PreviousAbilityState );
		break;
	}
	case EAbilityState::AS_ShortDash:
	{
		AnimationEventOnDashEnd.Broadcast( PreviousAbilityState );
		break;
	}
	case EAbilityState::AS_Walking:
	{
		AnimationEventOnStoppedWalking.Broadcast( OwningChar->GetHandleMovementComponent()->GetHasAnyInput() );
		break;
	}
	case EAbilityState::AS_Hover:
	{
		AnimationEventOnEndHover.Broadcast();

	}

	}

	switch( CurrentAbilityState )
	{

	case EAbilityState::AS_None:
	{
		AnimationEventOnIdle.Broadcast();
		break;
	}
	case EAbilityState::AS_Dash:
	{
		AnimationEventOnDash.Broadcast( CurrentAbilityState );
		break;
	}
	case EAbilityState::AS_AirDash:
	{
		AnimationEventOnDash.Broadcast( CurrentAbilityState );
		break;
	}
	case EAbilityState::AS_ShortDash:
	{
		AnimationEventOnDash.Broadcast( CurrentAbilityState );
		break;
	}
	case  EAbilityState::AS_Hover:
	{
		AnimationEventOnStartHover.Broadcast();
		break;
	}
	case EAbilityState::AS_Jump:
	{
		AnimationEventOnJump.Broadcast();
		break;
	}
	case EAbilityState::AS_Walking:
	{
		AnimationEventOnStartedWalking.Broadcast();
		break;
	}

	}



}

void UAbilityStateComponent::OnDashHitInternal()
{
	if( GetWorld()->GetTimerManager().IsTimerActive( DashEndHandle ) )
	{
		GetWorld()->GetTimerManager().PauseTimer( DashEndHandle );
	}

	DashEnd();

	AnimationEventOnDashHit.Broadcast();

}


