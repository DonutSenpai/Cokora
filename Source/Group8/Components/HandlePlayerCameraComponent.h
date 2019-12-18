#pragma once

#include "Components/ActorComponent.h"
#include "../Enums/AbilityStateEnum.h"
#include "Structs/GameModeSettings.h"
#include "HandlePlayerCameraComponent.generated.h"

UCLASS()
class GROUP8_API UHandlePlayerCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHandlePlayerCameraComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

	void LookVertical( float InputValue );
	void LookHorizontal( float InputValue );

	void ResetCameraPosition();
	void ChangeCameraRotation();
	void ResetCameraLerp( float DeltaTime );

	void ShakeCamera();

	//fade
	UFUNCTION()
	void FadeScenery();

	UFUNCTION()
	void RestoreScenery();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFaidScenery( AActor* ActorToFade );

	UFUNCTION( BlueprintImplementableEvent )
	void OnRestoreScenery( AActor* ActorToRestore );

	UFUNCTION()
	void OnAbilityStateChanged( EAbilityState NewState, EAbilityState PreviousState );

	UFUNCTION()
	void OnGameSettingsChanged( FGameModeSettings GameSettings );

	bool bInputDisabled = false;
	bool bLerpCamera = false;

	class APlayerCharacter* OwningChar = nullptr;

	//The higher the value, the faster the camera rotates
	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( UIMin = 0.1, UIMax = 1.0 ) )
	float LookSpeed = 0.5;

	//At what angle difference will the camera lerp during Dash
	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( UIMin = 0.0f, UIMax = 180.f ) )
	float RotateAtAngleWhenDashing = 90.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Interp" )
	float InterpSpeed = 360.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Interp" )
	float DashInterpSpeed = 360.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Look Speed" )
	float LookSpeedMultiplier = 4;

protected:

	FRotator TargetCameraRotation;
	FVector TargetCameraVector = FVector::ZeroVector;
	FRotator CurrentRotation;

	FTimerHandle SceneryFadeHandle;
	FTimerHandle RestoreSceneryHandle;

	UPROPERTY(VisibleAnywhere, Category = "Scenery Fade")
	TArray<AActor*> IgnoreActors { GetOwner() };

	TArray<FHitResult> PreviousHits;
	TArray<AActor*> HitActors;
	TArray<AActor*> PreviousHitActors;

	//TArray<FHitResult> Hits;

	UPROPERTY(EditDefaultsOnly, Category = "Scenery Fade")
	float SceneryFadeTimer = 0.1f;
	bool bCanFade = true;

	float RestoreSceneryTimer;

	float CurrentRotPitch = 0.f;
	float CurrentRotYaw = 0.f;
	float CurrentRotRoll = 0.f;

	float CurrentX = 0.f;
	float CurrentY = 0.f;
	float CurrentZ = 0.f;

	//Gamemode settings

	float LocalLookVerticalSens = 1.f;
	float LocalLookHorizontalSens = 1.f;

	bool bVerticalInputInverted = false;
	bool bHorizontalInputInverted = false;

private:

	float LerpAlpha = 0.f;

	bool bDashLerp = false;

	//Gets the angle between the camera forward and the player forward
	UFUNCTION( BlueprintCallable, BlueprintPure )
	float GetCameraPlayerAngle();

	

};
