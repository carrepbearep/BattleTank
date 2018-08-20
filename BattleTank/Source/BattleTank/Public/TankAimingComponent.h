// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

//forward declaration
class UTankBarrel;

//holds barrel's properties and elevates method
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
    UTankBarrel* Barrel = nullptr;
    
    void MoveBarrelTowards(FVector AimDirection);
    
public:
	// Sets default values for this component's properties
	UTankAimingComponent();
    
    void SetBarrelReference(UTankBarrel* BarrelToSet);
    
    // TODO add SetTurretReference
    
    void AimAt(FVector HitLocation, float LaunchSpeed);
	
};