// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    ATank* GetControlledTank() const;
    
    //Start the tank moving the barrel so that a shot will hit where the crosshair intersects the world
    void AimTowardsCrosshair();
    
    //Return an OUT parameter, true if hit landscpae
    bool GetSightRayHitLocation(FVector& OutHitLocation) const; //have to pass reference to OUT parameters
};
