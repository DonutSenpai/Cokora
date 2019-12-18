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
	
	

	//GetWorld()->GetTimerManager().SetTimer( SceneryFadeHandle, this, &UHandlePlayerCameraComponent::FadeScenery, SceneryFadeTimer, true );

	if ( GameMode )
	{
		GameMode->SettingsChanged.AddDynamic(this, &UHandlePlayerCameraComponent::OnGameSettingsChanged);
		LocalLookVerticalSens = GameMode->GetGameSettings().LookVerticalSens;
		LocalLookHorizontalSens = GameMode->GetGameSettings().LookHorizontalSens;

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
		OwningChar->PC->AddPitchInput( -InputValue * LookSpeed * LocalLookVerticalSens );
	}
}

void UHandlePlayerCameraComponent::LookHorizontal( float InputValue )
{
	if ( bInputDisabled ) return;

	if ( bHorizontalInputInverted )
		InputValue *= -1;

	if ( InputValue != 0.f )
	{
		OwningChar->PC->AddYawInput( InputValue * LookSpeed * LocalLookHorizontalSens );
	}
}

void UHandlePlayerCameraComponent::ResetCameraPosition()
{
	if ( bInputDisabled ) return;

	//if ( ( OwningChar->InputComponent->GetAxisValue( "MoveForward" ) != 0 ||
	//	OwningChar->InputComponent->GetAxisValue( "MoveRight" ) != 0 ||
	//	OwningChar->InputComponent->GetAxisValue( "LookVertical" ) != 0 ||
	//	OwningChar->InputComponent->GetAxisValue( "LookHorizontal" ) != 0 ) )
	//{
	//	return;
	//}
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
		/*  CurrentX = FMath::FInterpConstantTo( OwningChar->GetControlRotation().Vector().X, TargetCameraVector.X, DeltaTime, DashInterpSpeed );
			CurrentY = FMath::FInterpConstantTo( OwningChar->GetControlRotation().Vector().Y, TargetCameraVector.Y, DeltaTime, DashInterpSpeed );
			CurrentZ = FMath::FInterpConstantTo( OwningChar->GetControlRotation().Vector().Z, TargetCameraVector.Z, DeltaTime, DashInterpSpeed );

			NewVecDir = FVector( CurrentX, CurrentY, CurrentZ );*/

		NewVecDir = FMath::VInterpNormalRotationTo( OwningChar->GetControlRotation().Vector(), OwningChar->GetActorForwardVector(), DeltaTime, DashInterpSpeed );
	}
	else
	{
		NewVecDir = FMath::VInterpNormalRotationTo( OwningChar->GetControlRotation().Vector(), OwningChar->GetActorForwardVector(), DeltaTime, InterpSpeed );
	}

	OwningChar->PC->SetControlRotation( NewVecDir.Rotation() );
}

void UHandlePlayerCameraComponent::ShakeCamera()
{

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
		//UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2);

		FCollisionQueryParams Params;
		//Params.bIgnoreBlocks = true;

		FCollisionResponse Response;

		//FCollisionResponseParams ResponseParams = Response;
		
		FCollisionResponseContainer R;
		R.SetAllChannels(ECR_Ignore);
		R.SetResponse(ECollisionChannel::ECC_GameTraceChannel13, ECR_Overlap);

		Response.SetAllChannels(ECR_Ignore);

		Response.SetResponse(ECollisionChannel::ECC_GameTraceChannel13, ECR_Overlap);

		//FCollisionResponseContainer Re = FCollisionResponseContainer(Response);

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
	LocalLookVerticalSens = GameSettings.LookVerticalSens * LookSpeedMultiplier;
	LocalLookHorizontalSens = GameSettings.LookHorizontalSens * LookSpeedMultiplier;

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

