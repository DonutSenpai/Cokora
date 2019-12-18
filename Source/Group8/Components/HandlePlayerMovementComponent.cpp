#include "HandlePlayerMovementComponent.h"
#include "Player/PlayerCharacter.h"
#include "HoverComponent.h"

#include "Player/AbilityStateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UHandlePlayerMovementComponent::UHandlePlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHandlePlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningChar = Cast<APlayerCharacter>( GetOwner() );

	OwningChar->GetAbilityStateComponent()->AbilityStateChanged.AddDynamic( this, &UHandlePlayerMovementComponent::OnAbilityStateChanged );

}

void UHandlePlayerMovementComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void UHandlePlayerMovementComponent::MoveForward( float InputValue )
{
	if( bInputDisabled ) return;

	PreviousInputValForward = InputValueForward;
	InputValueForward = InputValue;

	bForwardInput = InputValue != 0.f;

	if( !FMath::IsNearlyZero( InputValueForward, KINDA_SMALL_NUMBER ) && FMath::IsNearlyZero( PreviousInputValForward, KINDA_SMALL_NUMBER ) &&
		FMath::IsNearlyZero(PreviousInputValRight, KINDA_SMALL_NUMBER))
	{
		//UE_LOG( LogTemp, Warning, TEXT("On started walking happened") );
			OnStartedWalking.Broadcast();
	}
	else if( FMath::IsNearlyZero( InputValueForward, KINDA_SMALL_NUMBER ) && FMath::IsNearlyZero( InputValueRight, KINDA_SMALL_NUMBER ) )
	{
		OnStoppedWalking.Broadcast();
	}

	if( InputValue != 0.f )
	{
		if( OwningChar->GetAbilityStateComponent()->GetAbilityState()== EAbilityState::AS_Dash ) return;
		bool bIsStatePush = OwningChar->GetAbilityStateComponent()->GetAbilityState() == EAbilityState::AS_Push;
		if( bIsStatePush && InputValue <= 0 )
		{
			return;
		}
		FRotator ControlForward = FRotator( 0.f, OwningChar->GetControlRotation().Yaw, 0.f );
		FVector TargetDirection = bIsStatePush ? OwningChar->GetActorForwardVector() : ControlForward.Vector();
		OwningChar->AddMovementInput( TargetDirection, InputValue );

	}
}

void UHandlePlayerMovementComponent::MoveRight( float InputValue )
{
	if( bInputDisabled ) return;

	PreviousInputValRight = InputValueRight;
	InputValueRight = InputValue;

	bRightInput = InputValue != 0.f;

	if( !FMath::IsNearlyZero( InputValueRight, KINDA_SMALL_NUMBER ) && FMath::IsNearlyZero( PreviousInputValRight, KINDA_SMALL_NUMBER ) &&
		FMath::IsNearlyZero(PreviousInputValForward, KINDA_SMALL_NUMBER))
	{
		//UE_LOG( LogTemp, Warning, TEXT( "On started walking happened" ) );
		OnStartedWalking.Broadcast();
	}
	else if( FMath::IsNearlyZero( InputValueForward, KINDA_SMALL_NUMBER ) && FMath::IsNearlyZero( InputValueRight, KINDA_SMALL_NUMBER ) )
	{
		OnStoppedWalking.Broadcast();
	}

	if( InputValue != 0 )
	{


		if( OwningChar->GetAbilityStateComponent()->GetAbilityState() == EAbilityState::AS_Dash ) return;
		if( OwningChar->GetAbilityStateComponent()->GetAbilityState() == EAbilityState::AS_Push ) return;
		// find out which way is right
		FRotator Rotation = OwningChar->GetControlRotation();
		FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// get right vector 
		FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

		OwningChar->AddMovementInput( Direction, InputValue );

	}


}

void UHandlePlayerMovementComponent::Jump()
{

}

void UHandlePlayerMovementComponent::OnAbilityStateChanged( EAbilityState NewState, EAbilityState PreviousState )
{
	if( NewState == EAbilityState::AS_Jump )
	{
		EnterJumpState();
	}

	if( PreviousState == EAbilityState::AS_Jump )
	{
		ExitJumpState();
	}

	if( NewState == EAbilityState::AS_Dash || NewState == EAbilityState::AS_AirDash || NewState == EAbilityState::AS_ShortDash )
	{
		bInputDisabled = true;
	}

	if( PreviousState == EAbilityState::AS_Dash || PreviousState == EAbilityState::AS_AirDash || PreviousState == EAbilityState::AS_ShortDash )
	{
		bInputDisabled = false;
	}

}

void UHandlePlayerMovementComponent::EnterJumpState()
{
	OwningChar->GetCharacterMovement()->Velocity.Y = OwningChar->GetCharacterMovement()->Velocity.Y * 0.1f;
	OwningChar->GetCharacterMovement()->Velocity.X = OwningChar->GetCharacterMovement()->Velocity.X * 0.1f;

	OwningChar->GetCharacterMovement()->AirControl = JumpAirControl;

	OwningChar->Jump();
}

void UHandlePlayerMovementComponent::ExitJumpState()
{
	OwningChar->GetCharacterMovement()->AirControl = NormalAirControl;
}



