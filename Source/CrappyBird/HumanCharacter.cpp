// Fill out your copyright notice in the Description page of Project Settings.

#include "CrappyBird.h"
#include "HumanCharacter.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"

AHumanCharacter::AHumanCharacter(){
    /*
     struct FConstructorStatics{
     ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> MovingAnimationAsset;
     ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BulletHitAnimationAsset;
     
     //FConstructorStatics(const TCHAR* aa, const TCHAR* bb): MovingAnimationAsset(aa), BulletHitAnimationAsset(bb) { }
     FConstructorStatics(): MovingAnimationAsset(TEXT("/Game/Import/Flipbook/FatBoy.FatBoy")), BulletHitAnimationAsset(TEXT("/Game/Import/Flipbook/FatMan.FatMan")){ }
     
     };
     static FConstructorStatics ConstructorStatics;
     
     MovingAnimation = ConstructorStatics.MovingAnimationAsset.Get();
     BulletHitAnimation = ConstructorStatics.BulletHitAnimationAsset.Get();
     */
}

void AHumanCharacter::InitializeGameValues(const TCHAR* refMovingAni, const TCHAR* refBulletHitAni){
    
    UPaperFlipbook *MovingAnimationAsset_01 = LoadObjFromPath<UPaperFlipbook>(refMovingAni);
    UPaperFlipbook *BulletHitAnimationAsset_01 = LoadObjFromPath<UPaperFlipbook>(refBulletHitAni);
    
   
    MovingAnimation = MovingAnimationAsset_01;
    BulletHitAnimation = BulletHitAnimationAsset_01;
    
    //set sprite
    GetSprite()->SetFlipbook(MovingAnimation);
    
    //enabling collision
    SetActorEnableCollision(true);
    
    //set collision capsule
    GetCapsuleComponent()->SetCapsuleHalfHeight(250);
    GetCapsuleComponent()->SetCapsuleRadius(175);
    
    //set custom collision values
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    
    
    //define overlap
    GetCapsuleComponent()->bGenerateOverlapEvents = true;
    //register overlap function
    TScriptDelegate<FWeakObjectPtr> delegateFunction;
    delegateFunction.BindUFunction(this, "OnBeginOverlap");
    GetCapsuleComponent()->OnComponentBeginOverlap.Add(delegateFunction);
    
    //set physics info
    GetCharacterMovement()->GravityScale = 0.0f;

    //set player yawn, pitch and roll
    SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
    
    DeactivateAndHideChar();
        
}

void AHumanCharacter::DeactivateAndHideChar(){
    bMoveTowardsHell = false;
    bIsActive = false;
    SetActorHiddenInGame(true);

}

void AHumanCharacter::ActivateAndReInitChar(class UStaticMeshComponent* BirdStaticMeshComponent){
    SetActorHiddenInGame(false);
    bIsActive = true;
    bIsConcious = false;
    
    //SET LOCATION
    int32 maxIncrement = 2000;
    int32 minIncrement = 800;
    float rand  = FMath::RandRange(minIncrement, maxIncrement);
    float ySpawn = 4000.0f + rand;
    SetActorLocation(FVector(-10.0f,ySpawn,440.0f));
    
    //SET SPEED
    maxIncrement = -2000;
    minIncrement = -450;
    rand  = FMath::RandRange(minIncrement, maxIncrement);
    speedMultiplyer = rand;
    
    this->Bird = BirdStaticMeshComponent;
    
    
    UE_LOG(HarshLog, Warning, TEXT("BIRD SPAWN: rand num: %f"), rand);
    UE_LOG(HarshLog, Warning, TEXT("BIRD SPAWN: ySpawn num: %f"), ySpawn);
    UE_LOG(HarshLog, Warning, TEXT("BIRD SPAWN: location: %s"), *GetActorLocation().ToString());
}

void AHumanCharacter::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    
    //check is active
    if(bIsActive && Bird){
        
        FVector ActorLocation = GetActorLocation();
        //1. check if bird is near
        if(!bIsConcious){
            float bird_y = Bird->GetComponentLocation().Y;
            float char_y = ActorLocation.Y;
            float dy = char_y - bird_y;
            
            if(dy < 500){
                bIsConcious = true;
                
                //change speed
                int32 maxIncrement = 1000;
                int32 minIncrement = -1000;
                float rand  = FMath::RandRange(minIncrement, maxIncrement);
                float var_newSpeed = speedMultiplyer + rand;
                if(FMath::Abs(var_newSpeed) <= 250.0f){
                    var_newSpeed = var_newSpeed < 0 ? -410.0f:250.0f;
                }
                previousSpeedMultiplyer = speedMultiplyer;
                speedMultiplyer = var_newSpeed;
                if(speedMultiplyer>0){
                    this->SetActorRotation(FRotator(0.0f,270.0f,0.0f));
                }
            }
        }else{  //2. if concious check whether out of bound
            
            if(ActorLocation.Y < -1728 || ActorLocation.Y > 4800){
                this->DeactivateAndHideChar();
            }
            
        }
        
        //3. if active update location
        float currentTerrainSpeedY = DeltaSeconds * speedMultiplyer;
        ActorLocation.Y += currentTerrainSpeedY;
        SetActorLocation(ActorLocation);
        
    }
    
   

}

void AHumanCharacter::OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                     bool bFromSweep, const FHitResult& SweepResult){
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("In Beginoverlap function. CODE"));
    if(Bird){
        TArray<UStaticMeshComponent*> Components;
        OtherActor->GetComponents<UStaticMeshComponent>(Components);
        for (int32 j=0; j<Components.Num(); j++){
            UStaticMeshComponent* var_01 = Components[j];
            if(var_01 == Bird)
                OnHitByBird();
        }
    }
    
}

void AHumanCharacter::OnHitByBird(){
    GetSprite()->SetFlipbook(BulletHitAnimation);
}

void AHumanCharacter::OnHitByBullet() {
    
}

void AHumanCharacter::SetBird(class UStaticMeshComponent* BirdStaticMeshComponent){
    this->Bird = BirdStaticMeshComponent;
    
}

