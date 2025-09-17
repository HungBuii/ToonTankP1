// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this,
									&AEnemy::CheckFireCondition, FireRate, true);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
	
}

bool AEnemy::InFireRange()
{
	// Find the distance to the Tank
	if (Tank)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		
		// Check to see if the Tank is in range
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false;
}

void AEnemy::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}
	if (InFireRange() && Tank->bAlive)
	{
		Fire();
	}
}
