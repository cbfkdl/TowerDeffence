// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/PortCollision.h"
#include "MyPrimaryDataAsset.h"
#include "Animation/AnimMontage.h"
#include "Animation/MyAnimInstance.h"
#include "Item/MyItemDataAsset.h"
#include "Item/MyPotionItemData.h"
#include "UI/MyWidgetComponent.h"
#include "UI/MyHpBarWidget.h"
#include "UI/MyHUDUserWidget.h"
#include "CharacaterStat/MyCharacterStatComponent.h"

DEFINE_LOG_CATEGORY(LogMyCharacter2);

// Sets default values
AMyCharacterBase::AMyCharacterBase()
{
    // Pawn 	
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    // Capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_CAPSULE);

    // Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // Mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    //static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Animation/MY_Character.MY_Character_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    static ConstructorHelpers::FObjectFinder<UMyPrimaryDataAsset> ShoulderDataRef(TEXT("/Script/Port.MyPrimaryDataAsset'/Game/ThirdPerson/CharacterControl/MY_Shoulder.MY_Shoulder'"));
    if (ShoulderDataRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
    }

    // Stat Component
    Stat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("Stat"));

    // Widget Component
    HpBar = CreateDefaultSubobject<UMyWidgetComponent>(TEXT("Widget"));
    HpBar->SetupAttachment(GetMesh());
    HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
    static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ThirdPerson/UI/WBP_HpBar.WBP_HpBar_C"));
    if (HpBarWidgetRef.Class)
    {
        HpBar->SetWidgetClass(HpBarWidgetRef.Class);
        HpBar->SetWidgetSpace(EWidgetSpace::Screen);
        HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
        HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // Item Actions
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AMyCharacterBase::DrinkPotion)));
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AMyCharacterBase::ReadScroll)));

    //Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
    //Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AMyCharacterBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    
    //Stat->OnEnergyZero.AddUObject(this, &AMyCharacterBase::~~~energyZero Acting)
}

void AMyCharacterBase::SetCharacterControlData(const UMyPrimaryDataAsset* ControlData)
{
    // Pawn
    bUseControllerRotationYaw = ControlData->bUseControllerRotationYaw;

    // CharacterMovement
    GetCharacterMovement()->bOrientRotationToMovement = ControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = ControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = ControlData->RotationRate;
}

void AMyCharacterBase::ProcessRollCommand()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(RollActionMontage, 1.0f);
}

void AMyCharacterBase::SetupCharacterWidget(UMyUserWidget* InUserWidget)
{
    UMyHpBarWidget* HpBarWidget = Cast<UMyHpBarWidget>(InUserWidget);
    if (HpBarWidget)
    {
        HpBarWidget->SetMaxHp(Stat->GetMaxHp());
        HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
        Stat->OnEnergyChanged.AddUObject(HpBarWidget, &UMyHpBarWidget::UpdateHpBar);
    }
}

void AMyCharacterBase::SetupHUDWIdget(UMyHUDUserWidget* HUDWidget)
{
    if (HUDWidget)
    {
        HUDWidget->UpdateStat(Stat->GetBaseStat()/*, Modifier*/);
        HUDWidget->UpdatePowerToWin();
        Stat->OnBatteryChanged.AddUObject(HUDWidget, &UMyHUDUserWidget::UpdateStat);
    }
}

void AMyCharacterBase::TakeItem(UMyItemDataAsset* InItemData)
{
    if (InItemData)
    {
        TakeItemActions[(uint8)InItemData->Type].ItemDeletgate.ExecuteIfBound(InItemData);
    }
}

void AMyCharacterBase::DrinkPotion(UMyItemDataAsset* InItemData)
{
    UE_LOG(LogMyCharacter2, Log, TEXT("Drink Potion Hp Restore"));
    Stat->UpdateHp(10.0f);
}

void AMyCharacterBase::ReadScroll(UMyItemDataAsset* InItemData)
{

    UMyPotionItemData* PotionItem = Cast<UMyPotionItemData>(InItemData);

    // 아이템의 InitialPower를 통해서 값을 증가 시킴
    Stat->UpdatePower(PotionItem->BaseStat.InitialPower);
    UE_LOG(LogMyCharacter2, Log, TEXT("Read Scroll %.2f"), PotionItem->BaseStat.InitialPower);

    float CollectedPower = 0;

}

void AMyCharacterBase::UpdatePower(float InPower)
{
    Stat->UpdatePower(InPower);
    // 파워에 따라 속도를 변경
    GetCharacterMovement()->MaxWalkSpeed = Stat->GetCalculateWalkSpeed();
    float DivinePower = GetCharacterPower() / GetInitialPower();
    PowerChangeEffect(DivinePower);
}

float AMyCharacterBase::GetInitialPower()
{
    return Stat->GetInitialPower();
}

float AMyCharacterBase::GetCharacterPower()
{
    return Stat->GetCharacterPower();
}


