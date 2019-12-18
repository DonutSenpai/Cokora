#include "HandlePlayerCameraComponent.h"
#include "TimerManager.h"
#include "Player/PlayerCharacter.h"
#include "UnrealMathUtility.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "Rotator.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AbilityStateComponent.h"
#include "GameModes/MainGameMode.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Structs/GameModeSettings.h"

//TO DO:
// - Remove unused code
UHandlePlayerCameraComponent::UHandlePlayerCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	SetComponentTickEnabled( true );
	bTickInEditor = true;
}

void UHandlePlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningChar->GetAbilityStateComponent()->AbilityStateChanged.AddDynamic( this, &UHandlePlayerCameraComponent::OnAbilityStateChanged );
	AMainGameMode * GameMode = Cast<AMainGameMode>( GetWorld()->GetAuthGameMode() );
	
	if ( GameMode )
	{
		GameMode->SettingsChanged.AddDynamic(this, &UHandlePlayerCameraComponent::OnGameSettingsChanged);
		LocalLookVerticalSense = GameMode->GetGameSettings().LookVerticalSense;
		LocalLookHorizontalSense = GameMode->GetGameSettings().LookHorizontalSense;

 		bVerticalInputInverted = GameMode->GetGameSettings().bInvertLookVertical;
 		bHorizontalInputInverted = GameMode->GetGameSettings().bInvertLookHorizontal;
	}
}

void UHandlePlayerCameraComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if ( bInputDisabled ) return;

	if ( bLerpCamera || bDashLerp )
	{
		ResetCameraLerp( DeltaTime );
	}
}

void UHandlePlayerCameraComponent::LookVertical( float InputValue )
{
	if ( bInputDisabled ) return;

	if ( bVerticalInputInverted )
		InputValue *= -1;

	if ( InputValue != 0 )
	{
		OwningChar->PC->AddPitchInput( -InputValue * LookSpeed * LocalLookVerticalSense );
	}
}

void UHandlePlayerCameraComponent::LookHorizontal( float InputValue )
{
	if ( bInputDisabled ) return;

	if ( bHorizontalInputInverted )
		InputValue *= -1;

	if ( InputValue != 0.f )
	{
		OwningChar->PC->AddYawInput( InputValue * LookSpeed * LocalLookHorizontalSense );
	}
}

void UHandlePlayerCameraComponent::ResetCameraPosition()
{
	if ( bInputDisabled ) return;

	bLerpCamera = true;
}

void UHandlePlayerCameraComponent::ResetCameraLerp( float DeltaTime )
{
	TargetCameraRotation = OwningChar->GetActorRotation();
	TargetCameraVector = OwningChar->GetActorForwardVector();
	TargetCameraVector.Normalize();

	if ( OwningChar->GetControlRotation().Equals( TargetCameraRotation, KINDA_SMALL_NUMBER ) || ( bDashLerp && LerpAlpha >= 1.f ) )
	{
		bLerpCamera = false;
		CurrentX = 0.f;
		CurrentY = 0.f;
		CurrentZ = 0.f;
		LerpAlpha = 0.f;
		bDashLerp = false;
	}

	LerpAlpha += DeltaTime / 10.f;

	FVector NewVecDir;

	if ( bDashLerp )
	{		
		NewVecDir = FMath::VInterpNormalRotationTo( OwningChar->GetControlRotation().Vector(), OwningChar->GetActorForwardVector(), DeltaTime, DashInterpSpeed );
	}
	else
	{
		NewVecDir = FMath::VInterpNormalRotationTo( OwningChar->GetControlRotation().Vector(), OwningChar->GetActorForwardVector(), DeltaTime, InterpSpeed );
	}

	OwningChar->PC->SetControlRotation( NewVecDir.Rotation() );
}

void UHandlePlayerCameraComponent::FadeScenery()
{
	if ( OwningChar )
	{
		PreviousHitActors = HitActors;
		HitActors.Empty();

		TArray<FHitResult> Hits;
		FVector StartLocation = OwningChar->PC->PlayerCameraManager->GetCameraLocation();

		const FVector EndLocation = OwningChar->GetActorLocation();

		FCollisionQueryParams Params;
	

		FCollisionResponse Response;
		
		FCollisionResponseContainer R;
		R.SetAllChannels(ECR_Ignore);
		R.SetResponse(ECollisionChannel::ECC_GameTraceChannel13, ECR_Overlap);
		Response.SetAllChannels(ECR_Ignore);
		Response.SetResponse(ECollisionChannel::ECC_GameTraceChannel13, ECR_Overlap);
		

		if ( GetWorld()->LineTraceMultiByChannel( Hits, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel13, Params /*,FCollisionResponseParams(Response)*/) )
		{
			for ( int i = 0; i < Hits.Num(); i++ )
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Hits[i].GetActor()->GetName());
				Hits[i].GetActor()->SetActorHiddenInGame( true );
				OnFaidScenery( Hits[i].GetActor() );
				HitActors.AddUnique( Hits[i].GetActor() );
			}
		}
		RestoreScenery();
	}
}

void UHandlePlayerCameraComponent::RestoreScenery()
{
	for ( int i = 0; i < PreviousHitActors.Num(); i++ )
	{
		if ( !HitActors.Contains( PreviousHitActors[i] ) )
		{
			OnRestoreScenery( PreviousHitActors[i] );
			PreviousHitActors[i]->SetActorHiddenInGame( false );
		}
	}
}

void UHandlePlayerCameraComponent::OnAbilityStateChanged( EAbilityState NewState, EAbilityState PreviousState )
{
	if ( NewState == EAbilityState::AS_AirDash || NewState == EAbilityState::AS_Dash )
	{
		if ( FMath::IsNearlyEqual( GetCameraPlayerAngle(), 0.f, RotateAtAngleWhenDashing ) )
		{
			bDashLerp = true;
		}
	}
	if ( PreviousState == EAbilityState::AS_Dash || PreviousState == EAbilityState::AS_AirDash )
	{
		if ( bDashLerp )
		{
			bDashLerp = false;
			LerpAlpha = 0.f;
		}
	}
}

void UHandlePlayerCameraComponent::OnGameSettingsChanged( FGameModeSettings GameSettings )
{
	LocalLookVerticalSense = GameSettings.LookVerticalSense * LookSpeedMultiplier;
	LocalLookHorizontalSense = GameSettings.LookHorizontalSense * LookSpeedMultiplier;

	bVerticalInputInverted = GameSettings.bInvertLookVertical;
	bHorizontalInputInverted = GameSettings.bInvertLookHorizontal;
}


float UHandlePlayerCameraComponent::GetCameraPlayerAngle()
{
	FVector ControlYaw = FRotator( 0.f, OwningChar->GetControlRotation().Yaw, 0.f ).Vector();
	float DotProductYaw = FVector::DotProduct( ControlYaw, OwningChar->GetActorForwardVector() );
	float DotProductYawRight = FVector::DotProduct( ControlYaw, OwningChar->GetActorRightVector() );
	float AngleRight = FMath::RadiansToDegrees( FMath::Acos( DotProductYaw ) ) * ( DotProductYawRight < 0 ? -1 : 1 );

	return AngleRight;
}

