#pragma once

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Structs/GameModeSettings.h"
#include "MainGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSettingsChanged , FGameModeSettings, GameSettings );

UCLASS()
class GROUP8_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMainGameMode();

	//UFUNCTION(BlueprintCallable, Category = "Game Settings")
	void OnSettingsChanged();

	FORCEINLINE FGameModeSettings GetGameSettings() const { return GameSettings; }

	UFUNCTION(BlueprintCallable, Category = "Game Settings")
	void ChangeGameSettings(FGameModeSettings NewSettings);

public:
	FSettingsChanged SettingsChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	FGameModeSettings GameSettings;

protected:
	
};


