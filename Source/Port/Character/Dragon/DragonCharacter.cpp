// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Dragon/DragonCharacter.h"
#include "Character/MyPrimaryDataAsset.h"
#include "UI/MyWidgetComponent.h"
#include "UI/MyHpBarWidget.h"
#include "Components/CapsuleComponent.h"
#include "Physics/PortCollision.h"
#include "Animation/AnimMontage.h"
#include "Animation/MyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacaterStat/MyTowerStatComponent.h"
#include "Engine/DamageEvents.h"
#include "AI/MyTowerAIController.h"
#include "CharacaterStat/MyTowerStatComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADragonCharacter::ADragonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMyTowerAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SetActorScale3D(FVector(0.1f, 0.1f, 0.1f));

	//GetCharacterMovement()->GravityScale = 0.0f;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_TOWER);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	Effect->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/CubeCollect/DesertDragon/Meshes/SK_DesertDragon.SK_DesertDragon'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/CubeCollect/Animation/DragonAnimation.DragonAnimation_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/CubeCollect/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// Stat
	Stat = CreateDefaultSubobject<UMyTowerStatComponent>(TEXT("Stat"));

	// WIdget Component
	HpBar = CreateDefaultSubobject<UMyWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWIdgetRef(TEXT("/Game/ThirdPerson/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWIdgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWIdgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(50.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	HpBar->SetHiddenInGame(true);
	TowerLevel = 1;
	AttackType = EAttackType::ENone;
	BlueColorBias = 0.0f;
}

// Called when the game starts or when spawned
void ADragonCharacter::BeginPlay()
{
	Super::BeginPlay();
	Stat->SetLevelStat(TowerLevel, true);
}

void ADragonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Stat->OnHpZero.AddUObject(this, &ADragonCharacter::SetDead);
}

// Called every frame
void ADragonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADragonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADragonCharacter::ProcessAttack()
{
	ComboActionBegin();
}

void ADragonCharacter::ComboActionBegin()
{
	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = Stat->GetAttackSpeed();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ClawActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADragonCharacter::ComboACtionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ClawActionMontage);
}

void ADragonCharacter::ComboACtionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyAttackEnd();
}

void ADragonCharacter::NotifyAttackEnd()
{
	OnAttackFinished.ExecuteIfBound();
}


void ADragonCharacter::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = Stat->GetAttackRadius() * 2;
	const float AttackRadius = Stat->GetAttackRadius() * 2;
	const float AttackDamage = Stat->GetAttack();
	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	if (Target)
	{
		Start = Target->GetActorLocation();
	}
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDectected = false;
	TArray<FOverlapResult> OverlapResults;

	switch (AttackType)
	{
	case EAttackType::ESingle:
		{
			HitDectected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANEL_ACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
			if (HitDectected)
			{
				FDamageEvent DamageEvent;
				OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
			}
		}
		break;
	case EAttackType::EMulty:
		{
			FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Attack), false, this);
			HitDectected = GetWorld()->OverlapMultiByChannel(
				OverlapResults,
				GetActorLocation(),
				FQuat::Identity,
				CCHANEL_ACTION,
				FCollisionShape::MakeSphere(GetAIDetectRange()),
				CollisionQueryParam);
			if (HitDectected)
			{
				for (auto const& OverlapResult : OverlapResults)
				{
					FDamageEvent DamageEvent;
					OverlapResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
				}
			}

		}
		break;
	}

	

//#if ENABLE_DRAW_DEBUG
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsuleHalfHeight = AttackRange * 0.5f;
//	FColor DrawColor = HitDectected ? FColor::Green : FColor::Red;
//
//
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
//
//#endif
}

float ADragonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AttackType == EAttackType::ENone)
	{
		Stat->ApplyDamage(DamageAmount);
	}
	return DamageAmount;
}

void ADragonCharacter::SetDead()
{
	Effect->bAutoDestroy = true;
	Effect->Activate(true);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	HpBar->SetHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ADragonCharacter::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->Montage_Play(DeadMontage, 1.0f);
	}
}
void ADragonCharacter::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

void ADragonCharacter::SetupCharacterWidget(UMyUserWidget* UserWidget)
{
	UMyHpBarWidget* HpBarWidget = Cast<UMyHpBarWidget>(UserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UMyHpBarWidget::UpdateHpBar);
	}
}

int ADragonCharacter::IsUpgradeable(const ADragonCharacter* Other)
{
	if ((AttackType == Other->GetAttackType()) && (TowerLevel == Other->GetTowerLevel()))
	{
		return TowerLevel+1;
	}

	return 0;
}

void ADragonCharacter::SetTowerLevel(int _Level)
{
	TowerLevel = _Level;
	Stat->SetLevelStat(TowerLevel, true);
	BlueColorBias = Stat->GetLevelColor();
	SetAttackType(true);
	UE_LOG(LogClass, Log, TEXT("SetTowerLevel : %d"), TowerLevel);

}

void ADragonCharacter::SetAttackType(bool SetLevel/* = false*/)
{
	if(!SetLevel)
	{
		AttackType = static_cast<EAttackType>(FMath::RandRange(0, 1));
	}

	switch (AttackType)
	{
	case EAttackType::ESingle:
		ChangeColor(FVector(1.0f, 0.0f, BlueColorBias), 1.0f);
		break;
	case EAttackType::EMulty:
		ChangeColor(FVector(0.0f, 1.0f, BlueColorBias), 1.0f);
		break;
	case EAttackType::ENone:
		ChangeColor();
		break;
	}
}

void ADragonCharacter::SetOwnedCube(AActor* Cube)
{
	OwnedCube = CastChecked<AMyColorActor>(Cube);
}

float ADragonCharacter::GetAIDetectRange()
{
	return 200.0f;
}

void ADragonCharacter::SetAIAttackDelegate(const FAITowerAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ADragonCharacter::AttackByAI(APawn* InTarget)
{
	Target = InTarget;
	ProcessAttack();
}

void ADragonCharacter::ChangeColor(FVector Color, float Alpha)
{
	GetMesh()->SetVectorParameterValueOnMaterials("Color", Color);

}

