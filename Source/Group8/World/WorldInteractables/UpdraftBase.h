#pragma once
#include "GameFramework/Actor.h"
#include "UpdraftBase.generated.h"

UCLASS()
class GROUP8_API AUpdraftBase : public AActor
{
	GENERATED_BODY()


		//Inherited functions & constructor
public:
	
	AUpdraftBase();

	virtual void BeginPlay() override;

	//Variables
public:

	UPROPERTY()
	class APlayerCharacter* OverlappingPlayer = nullptr;

	TArray< AActor* > OverlappingActors = {};
	
	UPROPERTY(VisibleAnywhere)
	bool bIsBlocked = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Location")
	FVector TargetLocation = FVector(0.f, 0.f, 300.f);

	FTimerHandle BlockCheckHandle;

	//Components
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Root")
	class USceneComponent* Root = nullptr;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Overlap Sphere")
	class USphereComponent* InteractableChecker;*/

	UPROPERTY( VisibleDefaultsOnly, BlueprintReadWrite )
	class UCapsuleComponent* UpdraftCollision = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class UCapsuleComponent* UpdraftExitCollision = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* TargetPlayerLocation = nullptr;
	
	//Variables
private:
	bool bPlayerIsOverlapping = false;


	//Functions
public:

	UFUNCTION()
		void OnBeginOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult );

	UFUNCTION()
		void OnInnerEndOverlap( class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

	UFUNCTION()
		void OnOuterEndOverlap( class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

	UFUNCTION()
	void ActivateDraft();

	UFUNCTION()
	void DeactivateDraft();

	UFUNCTION(BlueprintImplementableEvent, Category = Events)
	void OnDraftActivated();
	UFUNCTION(BlueprintImplementableEvent, Category = Events)
	void OnDraftDeactivated();


};