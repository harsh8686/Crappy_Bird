// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "HumanCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent), ClassGroup="Human")
class CRAPPYBIRD_API AHumanCharacter : public APaperCharacter
{
	GENERATED_BODY()
    
protected:
    class UPaperFlipbook* MovingAnimation;
    class UPaperFlipbook* BulletHitAnimation;
    class UStaticMeshComponent *Bird;
    
    void ChangeDirection();
    void ChangeSpeed();
    void OnHitByBullet();
    void OnHitByBird();
    void ChangeAnimation();
    Boolean IsCloserToBird();
    int32 i;
    
    virtual void Tick(float DeltaSeconds) override;

public:
    
    Boolean BIsActive;
    AHumanCharacter();
    void InitializeGameValues();
    
    UFUNCTION(BlueprintCallable, Category = "Human")
    void SetBird(class UStaticMeshComponent *Bird);
    //class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
    UFUNCTION()
    void OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult& SweepResult);
    
};
