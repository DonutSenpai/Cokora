#include "CharacterRotationComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "../HandlePlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enums/AbilityStateEnum.h"
#include "AbilityStateComponent.h"


//TO DO
//Clean up a lot of stuff
UCharacterRotationComponent::UCharacterRotationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterRotationComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningChar->GetHandleMovementComponent()->OnStartedWalking.AddDynamic( this, &UCharacterRotationComponent::StartedWalking );
}

void UCharacterRotationComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if( bInputDisabled ) return;

	SetLerpState();

	if( bLerpCharacter )
	{
		LerpCharacter( DeltaTime );
	}
}

void UCharacterRotationComponent::LerpCharacter( float DeltaTime )
{
	FRotator NewRotation = FMath::RInterpTo( OwningChar->GetActorRotation(), GetDirectionVector().Rotation(), DeltaTime, CurrentLerpSpeed );

	OwningChar->SetActorRotation( NewRotation );

	if( bAccelerationLerp )
	{
		if( OwningChar->GetActorRotation().Equals( GetDirectionVector().Rotation(), 5.f ) )
		{
			bAccelerationLerp = false;
			OwningChar->GetCharacterMovement()->MaxAcceleration = 1800.f;
		}

	}

}


FVector UCharacterRotationComponent::GetDirectionVector()
{

	FRotator ForwardRotation = FRotator( 0.f, OwningChar->GetControlRotation().Yaw, 0.f );

	FVector ForwardDirection = FVector::ZeroVector;

	FVector RightDirection = FVector::ZeroVector;



	if( !FMath::IsNearlyZero( OwningChar->GetInputAxisValue( "MoveRight" ), KINDA_SMALL_NUMBER ) )
	{
		RightDirection = ForwardRotation.Vector();

		RightDirection = RightDirection.RotateAngleAxis( 90.f, FVector( 0.f, 0.f, 1.f ) );

		RightDirection *= OwningChar->GetInputAxisValue( "MoveRight" );

	}

	if( !FMath::IsNearlyZero( OwningChar->GetInputAxisValue( "MoveForward" ), KINDA_SMALL_NUMBER ) )
	{
		ForwardDirection = ForwardRotation.Vector();

		ForwardDirection *= OwningChar->GetInputAxisValue( "MoveForward" );
	}


	FVector Direction = ForwardDirection + RightDirection;

	if( Direction != FVector::ZeroVector )
	{
		Direction = ( Direction / Direction.Size() );
	}

	return Direction;

}

void UCharacterRotationComponent::SetLerpState()
{
	bRightInput = !FMath::IsNearlyZero( OwningChar->GetInputAxisValue( "MoveRight" ), KINDA_SMALL_NUMBER );
	bForwardInput = !FMath::IsNearlyZero( OwningChar->GetInputAxisValue( "MoveForward" ), KINDA_SMALL_NUMBER );

	bLerpCharacter = ( bRightInput || bForwardInput );

	CurrentLerpSpeed = bAccelerationLerp ? StartWalkingLerpSpeed : LerpSpeed;
}

void UCharacterRotationComponent::StartedWalking()
{
	bAccelerationLerp = true;
	OwningChar->GetCharacterMovement()->MaxAcceleration = 1000.f;
}
