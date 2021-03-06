// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "HumanCharacter.h"
#include "CharManager.generated.h"


UCLASS()
class CRAPPYBIRD_API ACharManager : public AActor
{
	GENERATED_BODY()
	
    //Private variable
    TArray<AHumanCharacter*> InActiveCharacters;
    TArray<AHumanCharacter*> ActiveCharacters;
    
    bool bStopSpawningHumans;
    
    class UStaticMeshComponent *BirdComponent;
    class UPaperSpriteComponent* BulletComponent;
    
    FVector previousLocation;
    
public:	
	// Sets default values for this actor's properties
    ACharManager(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
    // Called every frame
	virtual void Tick( float DeltaSeconds ) override;
    
    //function to create char
    void SpawnCharacterToStage();
	
    AHumanCharacter* GetCharacter();

    UFUNCTION(BlueprintCallable, Category = "CharManager")
    void SetBird_01(class UStaticMeshComponent *Bird);
    
    
    UFUNCTION(BlueprintCallable, Category = "CharManager")
    void SetBullet(UPaperSpriteComponent* Bullet);
    
    UFUNCTION(BlueprintCallable, Category = "CharManager")
    void SetBulletAndBird(class UPaperSpriteComponent* Bullet, class UStaticMeshComponent *Bird);
    
    
    UFUNCTION(BlueprintCallable, Category = "CharManager")
    void StartGame();
    
    UFUNCTION(BlueprintCallable, Category = "CharManager")
    void StopGame();
	
};
