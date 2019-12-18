#include "MainGameMode.h"

AMainGameMode::AMainGameMode()
{

}

void AMainGameMode::OnSettingsChanged()
{
	
}

void AMainGameMode::ChangeGameSettings( FGameModeSettings NewSettings )
{
	GameSettings = NewSettings;
	SettingsChanged.Broadcast(GameSettings);
}


