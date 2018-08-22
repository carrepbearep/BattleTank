// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

//Forward declarations
class UTankBarrel;
class UTankAimingComponent;
class AProjectile;
class UTankMovementComponent;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
    void AimAt(FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "Firing")
    void Fire();
    
protected:
    UPROPERTY(BlueprintReadOnly)
    UTankAimingComponent* TankAimingComponent = nullptr;
    
    UPROPERTY(BlueprintReadOnly)
    UTankMovementComponent* TankMovementComponent = nullptr;
    
private:
	// Sets default values for this pawn's properties
	ATank();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    TSubclassOf<AProjectile> ProjectileBlueprint;
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float LaunchSpeed = 4000;
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float ReloadTimeInSeconds = 3;
    
    //Local barrel reference for spawning projectile
    UTankBarrel* Barrel = nullptr;  //TODO remove
    
    double LastFireTime = 0;
	
};
