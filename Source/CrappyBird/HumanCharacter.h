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
    /*
     *VARIABLES
     */
    
    bool bIsActive;
    bool bMoveTowardsHell;
    bool bIsConcious;
    
    float speedMultiplyer;
    float previousSpeedMultiplyer;
    
    class UPaperFlipbook* MovingAnimation;
    class UPaperFlipbook* BulletHitAnimation;
    class UStaticMeshComponent *Bird;
    
    /*
     *FUNCTIONS
     */
    void OnHitByBullet();
    void OnHitByBird();
    bool IsCloserToBird();
    virtual void Tick(float DeltaSeconds) override;
    void DeactivateAndHideChar();
    
public:
    UFUNCTION()
    virtual void OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                bool bFromSweep, const FHitResult& SweepResult);
    AHumanCharacter();
    void InitializeGameValues(const TCHAR* refMovingAni, const TCHAR* refBulletHitAni);
    UFUNCTION(BlueprintCallable, Category = "Human")
    void SetBird(class UStaticMeshComponent *Bird);
    void ActivateAndReInitChar(class UStaticMeshComponent* BirdStaticMeshComponent);
    
    template <typename ObjClass>
    FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
    {
        if(Path == NAME_None) return NULL;
        //~
        
        return Cast<ObjClass>(StaticLoadObject( ObjClass::StaticClass(), NULL,*Path.ToString()));
    }
    
};
