// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower/DestroyZoneActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/PortCollision.h"
#include "Interface/MyEnemyStatInterface.h"
#include "Game/ColorGameModeBase.h"
#include "Game/TowerEnemySpawnActor.h"

// Sets default values
ADestroyZoneActor::ADestroyZoneActor()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Trigger;
	//Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_TRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 40.0f, 40.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADestroyZoneActor::OnOverlapBegin);


	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ExampleContent/Blueprint_Communication/Meshes/SM_Battery_Medium.SM_Battery_Medium'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));*/

}

void ADestroyZoneActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ADestroyZoneActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	//UE_LOG(LogClass, Log, TEXT("Destroy Actor In"))
	IMyEnemyStatInterface* HitActor = Cast<IMyEnemyStatInterface>(OtherActor);
	if (HitActor)
	{
		AColorGameModeBase* pGmaeMode = GetWorld()->GetAuthGameMode<AColorGameModeBase>();
		if (pGmaeMode)
		{
			pGmaeMode->UpdateHP(HitActor->GetDamage());
			pGmaeMode->UpdateEnemyCount();
			//HitActor->UpdateEnemyCount();
			OtherActor->Destroy();
		}
	}


}
