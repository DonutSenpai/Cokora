#pragma once

#include "CoreMinimal.h"
#include "GameModeSettings.generated.h"

USTRUCT( BlueprintType )
struct FGameModeSettings
{
	GENERATED_BODY()

		//Game Settings
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	float LookVerticalSens = 1.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	float LookHorizontalSens = 1.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	bool bInvertLookVertical = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	bool bInvertLookHorizontal = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings" )
	float VibrationAmount = 1;
};