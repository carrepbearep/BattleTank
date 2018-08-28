// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;

/**
 * Responsible for helping the player aim
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
    
protected:
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
    void FoundAimingComponent(UTankAimingComponent* AimCompRef);

    UFUNCTION()
    void OnPossessedTankDeath();
    
private:
    void SetPawn(APawn* InPawn);
    
    virtual void BeginPlay() override;
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    //Start the tank moving the barrel so that a shot will hit where the crosshair intersects the world
    void AimTowardsCrosshair();
    
    //Return an OUT parameter, true if hit landscpae
    bool GetSightRayHitLocation(FVector& OutHitLocation) const; //have to pass reference to OUT parameters
    
    UPROPERTY(EditDefaultsOnly)
    float CrossHairXLocation = 0.5;
    
    UPROPERTY(EditDefaultsOnly)
    float CrossHairYLocation = 0.33333;
    
    bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
    bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;
    
    UPROPERTY(EditAnywhere)
    float LineTraceRange = 1000000; //km
};
