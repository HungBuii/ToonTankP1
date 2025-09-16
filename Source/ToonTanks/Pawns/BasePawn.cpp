// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Projectile/Projectile.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	RootComponent = CapsuleComponent;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	
	ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn"));
	ProjectileSpawn->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	// All vectors using are "World Space" not "Local Space"
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	
	// FRotator LookAtRotation = ToTarget.Rotation(); // Not use it because we will take care only "Yaw" rotation
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	// SetWorldRotation(): Put this component at the specified rotation in world space.
	TurretMesh->SetWorldRotation // Not use "AddActorLocalRotation()"
	(
		FMath::RInterpTo(TurretMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			25.f) // Interpolation speed, if the speed given is 0, then jump to the target. Speed higher -> spin faster 
			); // Use "Interpolation" help tries to reach Target rotation based on Current rotation,
	// giving a nice smooth feeling when rotating to Target rotation. (Target is Kismet Math Library)

	/* or
	TurretMesh->SetWorldRotation(LookAtRotation); // it's ok but not giving a nice smooth feeling when rotating to Target rotation
	 */
	
}

void ABasePawn::Fire()
{
	// FVector ProjectileSpawnPointLocation = ProjectileSpawn->GetComponentLocation();
	// DrawDebugSphere(
	// 	GetWorld(),
	// 	ProjectileSpawnPointLocation,
	// 	25.f,
	// 	12,
	// 	FColor::Green,
	// 	false,
	// 	3.f);

	FVector Location = ProjectileSpawn->GetComponentLocation();
	FRotator Rotation = ProjectileSpawn->GetComponentRotation();
	
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	Projectile->SetOwner(this);
}

void ABasePawn::HandleDestruction()
{
	
}
