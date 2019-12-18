#include "MainGameMode.h"

//TO DO
//Removed unused stuff

void AMainGameMode::ChangeGameSettings( FGameModeSettings NewSettings )
{
	GameSettings = NewSettings;
	SettingsChanged.Broadcast(GameSettings);
}


