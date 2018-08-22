// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "BattleTank.h"
#include "Projectile.h"
#include "TankMovementComponent.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    
    auto TankName = GetName();
    UE_LOG(LogTemp, Warning, TEXT("%s DONKEY: Tank C++ construct"), *TankName)
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay(); //needed for BP begin play to run
	
}

void ATank::AimAt(FVector HitLocation)
{
    if (!TankAimingComponent) { return; }
    TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::Fire()
{
    bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
    
    if (Barrel && isReloaded)
    {
        //spawn a projectile at the socket location on the barrel
        auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
    
        Projectile->LaunchProjectile(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
    }
}


