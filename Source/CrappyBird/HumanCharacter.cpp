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
    
    bMoveTowardsHell = false;
    bIsActive = false;
    SetActorHiddenInGame(true);
    
}

void AHumanCharacter::ActivateAndReInitChar(class UStaticMeshComponent* BirdStaticMeshComponent){
    SetActorHiddenInGame(false);
    speedMultiplyer = (0.1f) * (4000) * (-1);
    bIsActive = true;
    bIsConcious = false;
    
    int32 maxIncrement = 200;
    int32 minIncrement = -200;
    float rand  = FMath::RandRange(minIncrement, maxIncrement);
    SetActorLocation(FVector(-10.0f,780.0f,590.0f));
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
            
            if(dy < 30){
                bIsConcious = true;
                
                //change speed
                int32 maxIncrement = 500;
                int32 minIncrement = -500;
                float rand  = FMath::RandRange(minIncrement, maxIncrement);
                float var_newSpeed = speedMultiplyer + rand;
                if(FMath::Abs(var_newSpeed) <= 75.0f){
                    var_newSpeed = var_newSpeed < 0 ? -75.0f:75.0f;
                }
                UE_LOG(HarshLog, Warning, TEXT("variable speed: %f"), rand);
                previousSpeedMultiplyer = speedMultiplyer;
                speedMultiplyer = var_newSpeed;
                if(speedMultiplyer>0){
                    this->SetActorRotation(FRotator(0.0f,270.0f,0.0f));
                }
                
                UE_LOG(HarshLog, Warning, TEXT("previousSpeedMultiplyer speed: %f"), previousSpeedMultiplyer);
                UE_LOG(HarshLog, Warning, TEXT("speedMultiplyer speed: %f"), speedMultiplyer);
            }
        }
        
        //2. if concious check whether out of bound
        if(bIsConcious){
            
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
    UE_LOG(HarshLog, Warning, TEXT("1.In begin overlap function %s"), *OtherActor->GetName());

    if(Bird){
        UE_LOG(HarshLog, Warning, TEXT("2.Bird Found"));
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
    UE_LOG(HarshLog, Warning, TEXT("3.On Hitting Bird!"));
    GetSprite()->SetFlipbook(BulletHitAnimation);
    /*
     char -> move to exit gate
     char -> change sprite
    */
}

void AHumanCharacter::OnHitByBullet() {
    /*
     char -> change vertical position
     char -> change sprite
     
     */
}

void AHumanCharacter::SetBird(class UStaticMeshComponent* BirdStaticMeshComponent){
    this->Bird = BirdStaticMeshComponent;
    
}

