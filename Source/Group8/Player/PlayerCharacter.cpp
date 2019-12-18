#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HandlePlayerMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/HandlePlayerCameraComponent.h"
#include "Components/DashComponent.h"
#include "Components/HoverComponent.h"
#include "World/DashInteractables/DashInteractableActor.h"
#include "Components/Player/AbilityStateComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/Player/CharacterRotationComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{

	Body = CreateDefaultSubobject<UStaticMeshComponent>( "Body" );
	Body->SetupAttachment( GetCapsuleComponent() );

	MoveComponent = CreateDefaultSubobject<UHandlePlayerMovementComponent>( "MoveComponent" );
	MoveComponent->OwningChar = this;
	CameraComponent = CreateDefaultSubobject<UHandlePlayerCameraComponent>( "CameraComponent" );
	CameraComponent->OwningChar = this;

	HoverComponent = CreateDefaultSubobject<UHoverComponent>( "HoverComponent" );
	HoverComponent->OwningChar = this;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( "CameraBoom" );
	CameraBoom->SetupAttachment( GetCapsuleComponent() );
	CameraBoom->TargetArmLength = 450.f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>( "Camera" );
	Camera->SetupAttachment( CameraBoom, USpringArmComponent::SocketName );

	DashComponent = CreateDefaultSubobject<UDashComponent>( "DashComponent" );
	DashComponent->SetupAttachment( RootComponent );
	DashComponent->OwningChar = this;

	CameraDefaultDirection = CreateDefaultSubobject<UArrowComponent>( "CameraDefaultDirection" );
	CameraDefaultDirection->SetupAttachment( RootComponent );

	AbilityStateComponent = CreateDefaultSubobject<UAbilityStateComponent>( "AbilityStateComponent" );
	AbilityStateComponent->OwningChar = this;

	RotationComponent = CreateDefaultSubobject<UCharacterRotationComponent>( "RotationComponent" );
	RotationComponent->OwningChar = this;

	ThingPivot = CreateDefaultSubobject<USceneComponent>( "ThingPivot" );
	ThingPivot->SetupAttachment( RootComponent );

	SetActorTickEnabled( true );
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable( true );
	PrimaryActorTick.bStartWithTickEnabled = true;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<APlayerController>( UGameplayStatics::GetPlayerController( GetWorld(), 0 ) );
	AddMovementInput( GetActorForwardVector(), 1000.f );

	AbilityStateComponent->AbilityStateChanged.AddDynamic( this, &APlayerCharacter::AbilityStateChanged );
}

void APlayerCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void APlayerCharacter::SetupPlayerInputComponent( UInputComponent* InputComponent )
{
	Super::SetupPlayerInputComponent( InputComponent );
	InputComponent->BindAxis( "MoveForward", MoveComponent, &UHandlePlayerMovementComponent::MoveForward );
	InputComponent->BindAxis( "MoveRight", MoveComponent, &UHandlePlayerMovementComponent::MoveRight );
	InputComponent->BindAxis( "LookVertical", CameraComponent, &UHandlePlayerCameraComponent::LookVertical );
	InputComponent->BindAxis( "LookHorizontal", CameraComponent, &UHandlePlayerCameraComponent::LookHorizontal );
	InputComponent->BindAction( "ResetCamera", IE_Pressed, CameraComponent, &UHandlePlayerCameraComponent::ResetCameraPosition );


	auto DashStateInput = &InputComponent->BindAction( "Dash", IE_Pressed, AbilityStateComponent, &UAbilityStateComponent::DashInput );
	DashStateInput->bConsumeInput = false;

	auto JumpStateInput = &InputComponent->BindAction( "Jump", IE_Pressed, AbilityStateComponent, &UAbilityStateComponent::JumpInput );
	JumpStateInput->bConsumeInput = false;

}

void APlayerCharacter::OnMovementModeChanged( EMovementMode PrevMovementMode, uint8 PreviousCustomMode /*= 0*/ )
{
	Super::OnMovementModeChanged( PrevMovementMode, PreviousCustomMode );
	//HoverComponent->OnMovementModeChanged( PrevMovementMode, PreviousCustomMode );
}

void APlayerCharacter::Landed( const FHitResult& Hit )
{
	Super::Landed( Hit );
	AbilityStateComponent->OnLanded();
}

void APlayerCharacter::OnWalkingOffLedge_Implementation( const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta )
{
	Super::OnWalkingOffLedge_Implementation( PreviousFloorImpactNormal, PreviousFloorContactNormal, PreviousLocation, TimeDelta );

	AbilityStateComponent->WalkedOffLedge();
}

void APlayerCharacter::DisablePlayerInput()
{
	CameraComponent->bInputDisabled = true;
	MoveComponent->bInputDisabled = true;
	AbilityStateComponent->bInputDisabled = true;
	RotationComponent->bInputDisabled = true;
	AbilityStateComponent->SetStateToNone();
}

void APlayerCharacter::EnablePlayerInput()
{
	CameraComponent->bInputDisabled = false;
	MoveComponent->bInputDisabled = false;
	AbilityStateComponent->bInputDisabled = false;
	RotationComponent->bInputDisabled = false;

	if( MoveComponent->GetHasAnyInput() && !GetCharacterMovement()->IsFalling() )
	{
		AbilityStateComponent->SetStateToWalking();
	}
}

void APlayerCharacter::AbilityStateChanged( EAbilityState NewState, EAbilityState PreviousState )
{
	switch( NewState )
	{
		case EAbilityState::AS_Dash:
		{
			OnDash();
			break;
		}
		case EAbilityState::AS_AirDash:
		{
			OnAirDash();
			break;
		}
		case EAbilityState::AS_Hover:
		{
			OnHover();
			break;
		}

		case EAbilityState::AS_Jump:
		{
			OnJump();
			break;
		}
		case EAbilityState::AS_Falling:
		{
			OnFalling();
			break;
		}
	}
}

