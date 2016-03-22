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
        char_01->InitializeGameValues(TEXT("/Game/Import/Flipbook/Bill_C.Bill_C"),
                                      TEXT("/Game/Import/Flipbook/Dubya.Dubya"));
        
        AHumanCharacter* char_02 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_02->InitializeGameValues(TEXT("/Game/Import/Flipbook/Hill.Hill"),
                                      TEXT("/Game/Import/Flipbook/Jeb.Jeb"));
        
        AHumanCharacter* char_03 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_03->InitializeGameValues(TEXT("/Game/Import/Flipbook/John_K.John_K"),
                                      TEXT("/Game/Import/Flipbook/Nope.Nope"));
        
        AHumanCharacter* char_04 = GetWorld()->SpawnActor<AHumanCharacter>();
        char_04->InitializeGameValues(TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"),
                                      TEXT("/Game/Import/Flipbook/Ted_C.Ted_C"));
        
        
        InActiveCharacters.Add(char_01);
        InActiveCharacters.Add(char_02);
        InActiveCharacters.Add(char_03);
        InActiveCharacters.Add(char_04);
    }
	
}

// Called every frame
void ACharManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
    
    if(bHasGameStarted){
        if(ActiveCharacters.Num() < 2){
            SpawnCharacterToStage();
        }
    }else{
        for (auto Iter(ActiveCharacters.CreateIterator()); Iter; Iter++) {
            if(!(*Iter)->IsValidLowLevel()){
                continue;
            }else{
                //UE_LOG(HarshLog, Warning, TEXT("IMP: Deactivate char with name: %s"), *(*Iter)->GetName());
            }
        }
    }
    /*
    for (auto Iter(characters.CreateIterator()); Iter; Iter++) {
        if(!(*Iter)->IsValidLowLevel()){
            continue;
        }else{
            UE_LOG(HarshLog, Warning, TEXT("1.2. Manager Begin Play %s"), *(*Iter)->GetName());
        }
    }
    */
}

void ACharManager::SpawnCharacterToStage(){
    AHumanCharacter* var_tempChar = GetCharacter();
    if (var_tempChar != nullptr){
        var_tempChar->ActivateAndReInitChar(Bird);
        ActiveCharacters.Add(var_tempChar);
    }
}

void ACharManager::SetBird_01(class UStaticMeshComponent* BirdStaticMeshComponent){
    this->Bird = BirdStaticMeshComponent;
    
}

AHumanCharacter* ACharManager::GetCharacter(){
    
    return InActiveCharacters.Num()>0 ? InActiveCharacters.Pop(): nullptr;
    
}

void ACharManager::StartGame(){
    bHasGameStarted = true;
    
}

void ACharManager::StopGame(){
    bHasGameStarted = false;
    
}