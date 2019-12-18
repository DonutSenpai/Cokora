#include "DashComponent.h"
#include "Components/SphereComponent.h"
#include "World/DashInteractables/DashInteractableActor.h"
#include "Player/AbilityStateComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Player/CharacterRotationComponent.h"

UDashComponent::UDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UDashComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningChar->GetAbilityStateComponent()->AbilityStateChanged.AddDynamic( this, &UDashComponent::AbilityStateChanged );
	OnComponentBeginOverlap.AddDynamic( this, &UDashComponent::OnDashBeginOverlap );
	OnComponentEndOverlap.AddDynamic( this, &UDashComponent::OnDashEndOverlap );

	MaxWalkSpeed = OwningChar->GetCharacterMovement()->MaxWalkSpeed;
}

void UDashComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if( !bIsActivated ) return;

	if( bIsDashing && bShortDistanceDash )
	{
		ShortDistanceDashLerp( DeltaTime );
		return;
	}
	else if( bIsDashing && !bShortDistanceDash )
	{
		MovementSpeedLerp( DeltaTime );
	}

}



bool UDashComponent::IsOverlappingDashable()
{
	if( IsOverlappingActor( HitDashable ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UDashComponent::SetAbilityActive()
{
	bIsActivated = true;
}

void UDashComponent::AbilityStateChanged( EAbilityState NewState, EAbilityState PreviousState )
{
	if( !bIsActivated ) return;

	if( NewState == EAbilityState::AS_Dash || NewState == EAbilityState::AS_AirDash )
	{
		Dash();
	}
	else if( NewState == EAbilityState::AS_ShortDash )
	{
		bShortDistanceDash = true;
		Dash();
	}

	if( PreviousState == EAbilityState::AS_AirDash )
	{
		StopDash();
	}

	if( PreviousState == EAbilityState::AS_ShortDash )
	{
		bShortDistanceDash = false;
		StopDash();
	}
}


void UDashComponent::DashHit( class ADashInteractableActor* HitActor, APlayerCharacter* Char )
{
	if( !bIsActivated ) return;

	if( bIsDashing )
	{
		OnDashHit.Broadcast();
		HitActor->DashHit( Char );
		HitDashable = nullptr;
	}
}

void UDashComponent::Dash()
{
	
	bIsDashing = true;

	OnDash.Broadcast();

	SetVariables();
	
	OwningChar->RotationComponent->bInputDisabled = true;

	//UE_LOG( LogTemp, Warning, TEXT( "Ability state is dash" ) );


	TargetVelocity = OwningChar->GetActorForwardVector() * DashSpeed;

	if( HitDashable && OwningChar->GetAbilityStateComponent()->GetAbilityState() == EAbilityState::AS_AirDash )
	{
		DashHit( HitDashable, OwningChar );
	//	StopDash();

	}
	if( bShortDistanceDash )
	{
		InitialLocation = OwningChar->GetActorLocation();
		TargetLocation = OwningChar->GetActorLocation() + OwningChar->GetActorForwardVector() * -50.f;
	}




}

void UDashComponent::StopDash()
{
	if( bShortDistanceDash && HitDashable )
	{
		DashHit( HitDashable, OwningChar );

	}

	ResetVariables();

	OwningChar->GetCharacterMovement()->Velocity = OwningChar->GetCharacterMovement()->Velocity.GetClampedToMaxSize( MaxWalkSpeed );

	//UE_LOG(LogTemp, Warning, TEXT("StopDash is run"));
}

void UDashComponent::OnDashBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult )
{

	if( ADashInteractableActor* Hit = Cast<ADashInteractableActor>( OtherActor ) )
	{
		HitDashable = Cast<ADashInteractableActor>( OtherActor );

		if( OtherActor->IsA( Bush ) )
		{
			if( bIsDashing )
			{
				DashHit( Hit, OwningChar );
				return;
			}

			//UE_LOG( LogTemp, Error, TEXT( "Hit item is a bush" ) );
		}


		if( bIsDashing && !bShortDistanceDash )
		{
			if( HitDashable )
			{
				DashHit( HitDashable, OwningChar );
				StopDash();
			}
		}
		else
		{
			//bShortDistanceDash = true;
		}
	}
}

void UDashComponent::OnDashEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	if( OtherActor == HitDashable && !bIsDashing )
	{
		HitDashable = nullptr;
	}
}

void UDashComponent::MovementSpeedLerp( float DeltaTime )
{
	LerpAlpha += DeltaTime * ( 1.f / DashDuration );

	//UE_LOG( LogTemp, Warning, TEXT( "LerpAlpha: %f" ), LerpAlpha );

	if( LerpAlpha >= 1.f )
	{
		StopDash();
		return;
	}


	float TargetSpeedX = FMath::Lerp( OwningChar->GetVelocity().X, TargetVelocity.X, LerpAlpha );
	float TargetSpeedY = FMath::Lerp( OwningChar->GetVelocity().Y, TargetVelocity.Y, LerpAlpha );


	NewVelocity = FVector( TargetSpeedX, TargetSpeedY, 0.f );

	OwningChar->GetCharacterMovement()->Velocity = NewVelocity;
}

void UDashComponent::ShortDistanceDashLerp( float DeltaTime )
{

	//UE_LOG( LogTemp, Warning, TEXT( "ShortDistanceDashLerp is run" ) );

	OwningChar->PrintEnum( EAbilityState::AS_Dash );
	LerpAlpha += DeltaTime * 5.f;

	if( !bBool && LerpAlpha >= 1.f )
	{
		LerpAlpha = 0.f;
		InitialLocation = OwningChar->GetActorLocation();
		TargetLocation = OwningChar->GetActorLocation() + OwningChar->GetActorForwardVector() * 50.f;
		bBool = true;

	}
	else if( bBool && LerpAlpha >= 0.8f )
	{
		StopDash();
		return;
	}

	float TargetLocationX;
	float TargetLocationY;

	if( !bBool )
	{
		TargetLocationX = FMath::Lerp( InitialLocation.X, TargetLocation.X, LerpAlpha );
		TargetLocationY = FMath::Lerp( InitialLocation.Y, TargetLocation.Y, LerpAlpha );
	}
	else
	{
		TargetLocationX = FMath::Lerp( OwningChar->GetActorLocation().X, TargetLocation.X, LerpAlpha );
		TargetLocationY = FMath::Lerp( OwningChar->GetActorLocation().Y, TargetLocation.Y, LerpAlpha );

	}


	FVector NewLocation = FVector( TargetLocationX, TargetLocationY, OwningChar->GetActorLocation().Z );

	OwningChar->SetActorLocation( NewLocation );

}

void UDashComponent::SetVariables()
{
	OwningChar->GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	OwningChar->GetCharacterMovement()->MaxAcceleration = 10000.f;
	OwningChar->GetCharacterMovement()->GroundFriction = 0.f;

}

void UDashComponent::ResetVariables()
{
	LerpAlpha = 0.f;
	bIsDashing = false;
	bShortDistanceDash = false;
	bBool = false;

	OwningChar->RotationComponent->bInputDisabled = false;



	OwningChar->GetCharacterMovement()->MaxWalkSpeed = 400.f;
	OwningChar->GetCharacterMovement()->MaxAcceleration = 2048.f;
	OwningChar->GetCharacterMovement()->GroundFriction = 8.f;
}


