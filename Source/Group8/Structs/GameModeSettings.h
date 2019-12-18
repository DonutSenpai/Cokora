#pragma once

#include "CoreMinimal.h"
#include "GameModeSettings.generated.h"

//TO DO
//fixed wrong spellings
USTRUCT( BlueprintType )
struct FGameModeSettings
{
	GENERATED_BODY()

		//Game Settings
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	float LookVerticalSense = 1.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	float LookHorizontalSense = 1.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	bool bInvertLookVertical = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	bool bInvertLookHorizontal = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	float VibrationAmount = 1;
};