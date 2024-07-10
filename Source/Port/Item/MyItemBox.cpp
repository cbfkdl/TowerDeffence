// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MyItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interface/MyCharcaterItemInterface.h"
#include "Engine/AssetManager.h"
#include "Item/MyItemDataAsset.h"
#include "Physics/PortCollision.h"
#include "Character/MyCharacterBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMyItemBox::AMyItemBox()
{

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_TRIGGER);
	Trigger->SetBoxExtent(FVector(15.0f, 15.0f, 15.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyItemBox::OnOverlapBegin);
	Trigger->SetSimulatePhysics(true);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ExampleContent/Blueprint_Communication/Meshes/SM_Battery_Medium.SM_Battery_Medium'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}

	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	//Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	//Mesh->SetSimulatePhysics(true);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ExampleContent/Effects/ParticleSystems/P_electricity_arc.P_electricity_arc'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}

	// All Box Start Active
	bIsActive = true;
}

bool AMyItemBox::IsActive()
{
	return bIsActive;
}

void AMyItemBox::SetActive(bool NewPickupState)
{
	bIsActive = NewPickupState;
}

void AMyItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("MyItemDataAsset"), Assets);
	ensure(0 < Assets.Num());
	
	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<UMyItemDataAsset>(AssetPtr.Get());
	ensure(Item);

}

void AMyItemBox::UpdateBeamTargetPoint()
{
	AMyCharacterBase* MyCharacter = Cast<AMyCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Effect->SetBeamTargetPoint(0, MyCharacter->GetMesh()->GetSocketLocation("spine_02"), 0);
	//UE_LOG(LogClass, Log, TEXT("UpdateBeamTargetPoint"));
}

void AMyItemBox::TimerEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
	Effect->Complete();
	Effect->OnSystemFinished.AddDynamic(this, &AMyItemBox::OnEffectFinished);
}

void AMyItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (IsValid(Item) == false)
	{
		Destroy();
		return;
	}

	IMyCharcaterItemInterface* OverlapPawn = Cast<IMyCharcaterItemInterface>(OtherActor);

	//UE_LOG(LogClass, Log, TEXT("Overlap Item %s"), *OtherActor->GetName());

	if (OverlapPawn)
	{
		UE_LOG(LogClass, Log, TEXT("Overlap Pawn %s"), *OtherActor->GetName());

		OverlapPawn->TakeItem(Item);
	}
	else
		return;

	// spine_02
	Effect->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	Effect->bAutoDestroy = true;

	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyItemBox::UpdateBeamTargetPoint, 0.03f, true);

	GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, this, &AMyItemBox::TimerEnd, 1.0f, true);

}

void AMyItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

