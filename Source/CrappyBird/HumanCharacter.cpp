// Fill out your copyright notice in the Description page of Project Settings.

#include "CrappyBird.h"
#include "HumanCharacter.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"

AHumanCharacter::AHumanCharacter(){
    //setup assests
    struct FConstructorStatics{
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> MovingAnimationAsset;
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BulletHitAnimationAsset;
        FConstructorStatics()
                    : MovingAnimationAsset(TEXT("/Game/Import/Flipbook/FatBoy.FatBoy"))
                    , BulletHitAnimationAsset(TEXT("/Game/Import/Flipbook/FatMan.FatMan"))
        {
        
        }
    };
    
    static FConstructorStatics ConstructorStatics;
    
    MovingAnimation = ConstructorStatics.MovingAnimationAsset.Get();
    BulletHitAnimation = ConstructorStatics.BulletHitAnimationAsset.Get();
    

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
    
    
    //set player yawn, pitch and roll
    SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
    SetActorLocation(FVector(-10.0f,780.0f,150.0f));
    
    //set physics info
    GetCharacterMovement()->GravityScale = 0.0f;
    
    i = 0;
    BIsActive = false;
}

void AHumanCharacter::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    
    i++;
    if(i>100){
        BIsActive= !BIsActive;
        i=0;
    }
    
    //check is active
    if(BIsActive){
        //1. if active update location
        FVector ActorLocation = GetActorLocation();
        float currentTerrainSpeedY = DeltaSeconds * (0.1f) * (4000) * (-1);
        ActorLocation.Y += currentTerrainSpeedY;
        SetActorLocation(ActorLocation);

        //check is bird is near
        
        //if yes then change location or speed
        //if hit by bullet then change animation
        //if out of bound then set it to inactive
    }

}

void AHumanCharacter::SetBird(class UStaticMeshComponent* BirdStaticMeshComponent){
    /*
    UE_LOG(HarshLog, Warning, TEXT("Bird saved and bird location: %s"),
           *BirdStaticMeshComponent->GetComponentLocation().ToString());
     */
}

void AHumanCharacter::OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                     bool bFromSweep, const FHitResult& SweepResult){
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("In OnOverlap!"));
    }
}

void AHumanCharacter::InitializeGameValues(){
    
    
}

void AHumanCharacter::OnHitByBird(){
    
}

void AHumanCharacter::OnHitByBullet() {
    
}

void AHumanCharacter::ChangeAnimation(){
    
}

void AHumanCharacter::ChangeDirection(){
    
    
}

void AHumanCharacter::ChangeSpeed(){
    
}

