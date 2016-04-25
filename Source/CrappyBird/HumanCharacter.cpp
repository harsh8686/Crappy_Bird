// Fill out your copyright notice in the Description page of Project Settings.

#include "CrappyBird.h"
#include "HumanCharacter.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
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

    
    
    this->DeactivateAndHideChar();
        
}

void AHumanCharacter::DeactivateAndHideChar(){
    bMoveTowardsHell = false;
    bIsActive = false;
    SetActorHiddenInGame(true);
    if(MeshComp)
        MeshComp->DestroyComponent();
}

FVector AHumanCharacter::ActivateAndReInitChar(class UStaticMeshComponent* Bird, class UPaperSpriteComponent* Bullet, FVector& previousLoc){
    
    this->BirdComponent = Bird;
    this->BulletComponent = Bullet;
    
    SetActorHiddenInGame(false);
    bIsActive = true;
    bIsConcious = false;
    
    UE_LOG(HarshLog, Warning, TEXT("Previous loc: %s"), *previousLoc.ToString());
    
    //SET LOCATION
    int32 maxIncrement = 2000;
    int32 minIncrement = 1000;
    float rand  = FMath::RandRange(minIncrement, maxIncrement);
    float ySpawn = previousLoc.Y + rand;
    FVector loc(previousLoc.X, ySpawn, previousLoc.Z);
    //FVector loc(-10.0f,ySpawn,440.0f);
    SetActorLocation(loc);
    
    //SET SPEED
    maxIncrement = -2000;
    minIncrement = -450;
    rand  = FMath::RandRange(minIncrement, maxIncrement);
    speedMultiplyer = rand;
    
    UE_LOG(HarshLog, Warning, TEXT("New loc: %s"), *loc.ToString());
    UE_LOG(HarshLog, Warning, TEXT("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    
    //SET ROTATION
    SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
    OnHitByBullet();
    
    return loc;
}

void AHumanCharacter::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    
    //check is active
    if(bIsActive && BirdComponent){
        
        FVector ActorLocation = GetActorLocation();
        //1. check if bird is near
        if(!bIsConcious){
            float bird_y = BirdComponent->GetComponentLocation().Y;
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
    
    if(OtherComp == BulletComponent){
         OnHitByBullet();
    }
    
    if(OtherComp == BirdComponent){
        OnHitByBird();
    }
    
}

void AHumanCharacter::OnHitByBird(){
    //GetSprite()->SetFlipbook(BulletHitAnimation);
}

void AHumanCharacter::OnHitByBullet() {
    
    //trying with static mesh
    if(!MeshComp){
        //UStaticMesh *MeshShape = LoadObjFromPath<UStaticMesh>(TEXT("/Game/Meshes/SM_Btn_Achievements.SM_Btn_Achievements"));
        UStaticMesh *MeshShape = LoadObjFromPath<UStaticMesh>(TEXT("/Game/Import/Meshes/Shiittteeee/SM_Shitteeee.SM_Shitteeee"));
    
        //meshcomp declaration
        MeshComp = NewObject<UStaticMeshComponent>(this);
        MeshComp->SetStaticMesh(MeshShape);
    
        //Setup material
        //const TCHAR* refMaterial = TEXT("/Game/Meshes/SM_Player_01.SM_Player_01");
        const TCHAR* refMaterial = TEXT("/Game/Import/Materials/Shiittteeee/Shitteeee.Shitteeee");
        UMaterial * mat_01 =LoadObjFromPath<UMaterial>(refMaterial);
        UMaterialInstanceDynamic* myDynamicMaterial = UMaterialInstanceDynamic::Create(mat_01, this);
        //MeshComp->SetMaterial(0, myDynamicMaterial);
    
    
        //setup collision
        MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
        MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    
        //Setup location
        FTransform refTrans = MeshComp->GetRelativeTransform();
        //refTrans.SetLocation(FVector(0.0f,0.0f,400.0f));
        //refTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
        MeshComp->SetRelativeTransform(refTrans);
    
        //Attach to HumanCharacter Component
        MeshComp->AttachTo(this->GetRootComponent(),FName(TEXT("WeaponPoint")), EAttachLocation::KeepRelativeOffset);
    
        //Register component
        MeshComp->RegisterComponent();
        FVector socketLoaction = GetRootComponent()->GetSocketLocation("WeaponPoint");
        UE_LOG(HarshLog, Warning, TEXT("Socket Location [Weapon Point]: %s"), *socketLoaction.ToString());
    }
    
}


//x: -700.0 cm
//y: 768.0;
//z: 1024 //1536.0


