// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorActor/MyColorActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "physics/PortCollision.h"
#include "Interface/MyTowerCubeInterface.h"
#include "Character/Dragon/DragonCharacter.h"


// Sets default values
AMyColorActor::AMyColorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	IsOnTurret = false;
	ColorIndex = 1.0f;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_TOWER);
	Trigger->SetBoxExtent(FVector(25.0f, 25.0f, 50.0f));
	//Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyColorActor::OnOverlapBegin);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}

	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	// 안해도 되는데 색깔 바뀌는지 확인차
	ChangeColor(FVector(0.0f, 0.0f, 1.0f), 0.7f);
}

// Called when the game starts or when spawned
void AMyColorActor::BeginPlay()
{
	Super::BeginPlay();

	ChangeColor(FVector(1.0f, 1.0f, 1.0f), 1.0f);
}


void AMyColorActor::SetOnTower(ADragonCharacter* _Tower)
{
	Tower = _Tower;
	IsOnTurret = true;

	if (IsValid(Tower) == false)
	{
		IsOnTurret = false;
		return;
	}
	Tower->SetOwnedCube(this);
}

EAttackType AMyColorActor::GetAttackType()
{
	if (IsValid(Tower) == false)
	{
		return EAttackType::ENone;
	}
	return Tower->GetAttackType();
}

void AMyColorActor::ChangeColor(FVector Color/* = FVector(1.0f, 1.0f, 1.0f)*/, float Alpha/* = 1.0f*/)
{
	Mesh->SetVectorParameterValueOnMaterials("Color", Color);
	Mesh->SetScalarParameterValueOnMaterials("Color", Alpha);
}


