// Fill out your copyright notice in the Description page of Project Settings.

#include "CrappyBird.h"
#include "CharManager.h"


// Sets default values
ACharManager::ACharManager(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
}

// Called when the game starts or when spawned
void ACharManager::BeginPlay()
{
	Super::BeginPlay();

    UE_LOG(HarshLog, Warning, TEXT("1.1. Manager Begin Play"));    
    if(GetWorld()){
        AHumanCharacter* char_01 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_01->InitializeGameValues(TEXT("/Game/Import/Flipbook/Albright.Albright"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_02 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_02->InitializeGameValues(TEXT("/Game/Import/Flipbook/Beck.Beck"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_03 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_03->InitializeGameValues(TEXT("/Game/Import/Flipbook/Bernie.Bernie"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_04 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_04->InitializeGameValues(TEXT("/Game/Import/Flipbook/Bill_C.Bill_C"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_05 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_05->InitializeGameValues(TEXT("/Game/Import/Flipbook/Cheney.Cheney"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_06 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_06->InitializeGameValues(TEXT("/Game/Import/Flipbook/Dubya.Dubya"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_07 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_07->InitializeGameValues(TEXT("/Game/Import/Flipbook/Hill.Hill"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_08 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_08->InitializeGameValues(TEXT("/Game/Import/Flipbook/Jeb.Jeb"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_09 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_09->InitializeGameValues(TEXT("/Game/Import/Flipbook/John_K.John_K"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_10 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_10->InitializeGameValues(TEXT("/Game/Import/Flipbook/O_Reilly.O_Reilly"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        AHumanCharacter* char_11 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_11->InitializeGameValues(TEXT("/Game/Import/Flipbook/Reagan.Reagan"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_12 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_12->InitializeGameValues(TEXT("/Game/Import/Flipbook/Sarah.Sarah"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_13 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_13->InitializeGameValues(TEXT("/Game/Import/Flipbook/Stump.Stump"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        AHumanCharacter* char_14 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_14->InitializeGameValues(TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        

        
        InActiveCharacters.Add(char_01);
        InActiveCharacters.Add(char_02);
        InActiveCharacters.Add(char_03);
        //InActiveCharacters.Add(char_04);
        InActiveCharacters.Add(char_05);
        InActiveCharacters.Add(char_06);
        InActiveCharacters.Add(char_07);
        InActiveCharacters.Add(char_08);
        InActiveCharacters.Add(char_09);
        InActiveCharacters.Add(char_10);
        InActiveCharacters.Add(char_11);
        InActiveCharacters.Add(char_12);
        InActiveCharacters.Add(char_13);
        InActiveCharacters.Add(char_14);
        
        ActiveCharacters.Add(char_04);
        bStopSpawningHumans = true;
    }
	
}

// Called every frame
void ACharManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
    if(!bStopSpawningHumans){
        if(ActiveCharacters.Num() < 14){
            SpawnCharacterToStage();
        }
    }
    if(ActiveCharacters.Num() > 0){
        TArray<AHumanCharacter*> tempArray;
        AHumanCharacter* CurHuman = NULL;
         for(int32 b = 0; b < ActiveCharacters.Num(); b++)
         {
             CurHuman = ActiveCharacters[b];
             if(!CurHuman) continue;
             if(!CurHuman->IsValidLowLevel()) continue;
         
             if(!CurHuman->bIsActive){
                 tempArray.Push(CurHuman);
             }
         }
        
        while(tempArray.Num()>0){
            CurHuman = tempArray.Pop();
            ActiveCharacters.Remove(CurHuman);
            InActiveCharacters.Push(CurHuman);
        }
        
    }
    
}

void ACharManager::SpawnCharacterToStage(){
    AHumanCharacter* var_tempChar = GetCharacter();
    if (var_tempChar != nullptr){
        previousLocation = var_tempChar->ActivateAndReInitChar(BirdComponent, BulletComponent, previousLocation);
        ActiveCharacters.Add(var_tempChar);
    }
}

AHumanCharacter* ACharManager::GetCharacter(){
    return InActiveCharacters.Num()>0 ? InActiveCharacters.Pop(): nullptr;
}

void ACharManager::StartGame(){
    
    bStopSpawningHumans = false;
    previousLocation = FVector(-10.0f,0.0f,440.0f);
    
    AHumanCharacter* CurHuman = NULL;
    for(int32 b = 0; b < ActiveCharacters.Num(); b++)
    {
        CurHuman = ActiveCharacters[b];
        if(!CurHuman) continue;
        if(!CurHuman->IsValidLowLevel()) continue;
        //~~~~~~~~~~~~~~~~~~~~~~
        CurHuman->DeactivateAndHideChar();
        InActiveCharacters.Push(CurHuman);
    }
    
    ActiveCharacters.Empty();
    
    previousLocation = FVector(-10.0f,4000.0f,440.0f);
    //FVector loc(-10.0f,ySpawn,440.0f);
    
    UE_LOG(HarshLog, Warning, TEXT("1.4 Count in ACTIVE Array %d"), ActiveCharacters.Num());
    UE_LOG(HarshLog, Warning, TEXT("1.5 Count in IN-ACTIVE Array %d"), InActiveCharacters.Num());
}

void ACharManager::StopGame(){
    bStopSpawningHumans = true;
}

void ACharManager::SetBulletAndBird(class UPaperSpriteComponent* Bullet, class UStaticMeshComponent* Bird){
    this->BulletComponent = Bullet;
    this->BirdComponent = Bird;
}

void ACharManager::SetBird_01(class UStaticMeshComponent *BirdStaticMeshComponent){
    this->BirdComponent = BirdStaticMeshComponent;
}

void ACharManager::SetBullet(UPaperSpriteComponent *Bullet){
    this->BulletComponent = Bullet;
}



