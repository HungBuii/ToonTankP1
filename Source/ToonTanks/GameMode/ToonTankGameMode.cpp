// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTankGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/Enemy.h"
#include "ToonTanks/Pawns/Tank.h"
#include "ToonTanks/PlayerController/TankController.h"


void AToonTankGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTankGameMode::HandleGameStart()
{
	TargetEnemies = GetTargetEnemyCount();
	
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	TankPlayerController = Cast<ATankController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (TankPlayerController)
	{
		TankPlayerController->SetPlayerEnabledState(false);

		FTimerHandle EnableTimerHandle;
		FTimerDelegate EnableTimerDelegate = FTimerDelegate::CreateUObject(
											TankPlayerController,
											&ATankController::SetPlayerEnabledState, 
											true);

		GetWorldTimerManager().SetTimer(EnableTimerHandle,
										EnableTimerDelegate,
										StartDelay,
										false);
	}
}

int AToonTankGameMode::GetTargetEnemyCount()
{
	TArray<AActor*> Enenmies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), Enenmies);
	return Enenmies.Num();
}

void AToonTankGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (TankPlayerController)
		{
			TankPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else if (AEnemy* DestroyedEnemy = Cast<AEnemy>(DeadActor))
	{
		DestroyedEnemy->HandleDestruction();
		TargetEnemies--;
		UKismetSystemLibrary::PrintString(this, "Tower Tank Alive: " + FString::FromInt(TargetEnemies));
		if (TargetEnemies == 0) GameOver(true);
	}
}
