// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperFlipbook.h"
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
    
    bool bMoveTowardsHell;
    bool bIsConcious;
    
    float speedMultiplyer;
    float previousSpeedMultiplyer;
    
    class UPaperFlipbook* MovingAnimation;
    class UPaperFlipbook* BulletHitAnimation;
    
    class UPaperSpriteComponent* BulletComponent;
    
    class UStaticMeshComponent* BirdComponent;
    class UStaticMeshComponent* MeshComp;
    
    
    /*
     *FUNCTIONS
     */
    void OnHitByBullet();
    void OnHitByBird();
    bool IsCloserToBird();
    virtual void Tick(float DeltaSeconds) override;
    
    
public:
    bool bIsActive;
    
    //FUNCTION
    AHumanCharacter();
    
    UFUNCTION()
    virtual void OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                bool bFromSweep, const FHitResult& SweepResult);
    
    void InitializeGameValues(const TCHAR* refMovingAni, const TCHAR* refBulletHitAni);
    
    FVector ActivateAndReInitChar(class UStaticMeshComponent* Bird, class UPaperSpriteComponent* Bullet, FVector& previousLoc);
    void DeactivateAndHideChar();
    
    template <typename ObjClass>
    FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
    {
        if(Path == NAME_None) return NULL;
        return Cast<ObjClass>(StaticLoadObject( ObjClass::StaticClass(), this,*Path.ToString()));
    }
    
};
