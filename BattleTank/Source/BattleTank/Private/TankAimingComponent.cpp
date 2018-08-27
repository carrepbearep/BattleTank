// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
    Super::BeginPlay();
    
    //So that first fire is after initial reload
    LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
    Barrel = BarrelToSet;
    Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (RoundsLeft <= 0) {
        FiringState = EFiringState::OutOfAmmo;
    }
    else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) {
        FiringState = EFiringState::Reloading;
    }
    else if (IsBarrelMoving()) {
        FiringState = EFiringState::Aiming;
    }
    else {
        FiringState = EFiringState::Locked;
    }
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
    return RoundsLeft;
}

EFiringState UTankAimingComponent::GetFiringState() const
{
    return FiringState;
}

bool UTankAimingComponent::IsBarrelMoving()
{
    if (!ensure(Barrel)) { return false; }
    auto BarrelForward = Barrel->GetForwardVector();
    return !BarrelForward.Equals(AimDirection, 0.01); //vectors are equal
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
    if (!ensure(Barrel)) { return; }
    FVector OutLaunchVelocity;
    FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

    //parameter must be present to prevent bug (visibility)
    bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, StartLocation, HitLocation, LaunchSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace);
    
    if (bHaveAimSolution) {
        AimDirection = OutLaunchVelocity.GetSafeNormal();
        MoveBarrelTowards(AimDirection);
    }

    //If no solution found do nothing
}

void UTankAimingComponent::Fire()
{
    if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
    {
        //spawn a projectile at the socket location on the barrel
        if (!ensure(Barrel)) { return; }
        if (!ensure(ProjectileBlueprint)) { return; }
        
        auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
        
        Projectile->LaunchProjectile(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
        RoundsLeft--;
    }
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimingDirection)
{
    if (!ensure(Barrel) || !ensure(Turret)) { return; }   //same as !ensure(Barrel && Turret)
    
    //Work out difference between current barrel rotation and aim direction
    auto BarrelRotator = Barrel->GetForwardVector().Rotation();
    auto AimAsRotator = AimingDirection.Rotation();
    auto DeltaRotator = AimAsRotator - BarrelRotator;
    
    //Always yaw the shortest way
    Barrel->Elevate(DeltaRotator.Pitch);
    if (FMath::Abs(DeltaRotator.Yaw) < 180) {
        Turret->Rotate(DeltaRotator.Yaw);
    }
    else {  //avoid going the long way round
        Turret->Rotate(-DeltaRotator.Yaw);
    }
}

