// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/TopDownPawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/MyPrimaryDataAsset.h"
#include "Character/Dragon/DragonCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/MyTowerCubeInterface.h"
#include "UI/TowerHUDWidget.h"
#include "DrawDebugHelpers.h"
#include "Game/ColorGameModeBase.h"
#include "ColorActor/MyColorActor.h"
#include "EngineUtils.h"


// Sets default values
ATopDownPawn::ATopDownPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsBuildMode = false;
	IsCompositedMode = false;
	
	// 카메라 루트
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetRelativeLocationAndRotation(FVector(30.0f, 0.0f, 2220.0f), FRotator(116.56f, -90.0f, -26.56f));
	RootComponent = Scene;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetFieldOfView(51.71f);
	FollowCamera->SetAspectRatio(1.765f);
	FollowCamera->SetRelativeLocation(FVector::ZeroVector);
	FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);
		
	FollowCamera->SetupAttachment(Scene);

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionBuildRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pawn/Input/Actions/IA_Build.IA_Build'"));
	if (IsValid(InputActionBuildRef.Object))
	{
		BuildAction = InputActionBuildRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> CompositeActionBuildRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pawn/Input/Actions/IA_Composite.IA_Composite'"));
	if (IsValid(CompositeActionBuildRef.Object))
	{
		CompositeAction = CompositeActionBuildRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDestroyRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pawn/Input/Actions/IA_Destroy.IA_Destroy'"));
	if (IsValid(InputActionDestroyRef.Object))
	{
		DestroyAction = InputActionDestroyRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLeftClickRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pawn/Input/Actions/IA_BuildRand.IA_BuildRand'"));
	if (IsValid(InputActionLeftClickRef.Object))
	{
		LeftClickAction = InputActionLeftClickRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMyPrimaryDataAsset> ControlDataRef(TEXT("/Script/Port.MyPrimaryDataAsset'/Game/Pawn/Control/MY_TopDown.MY_TopDown'"));
	if (IsValid(ControlDataRef.Object))
	{
		CharacterControl = ControlDataRef.Object;
	}

	static ConstructorHelpers::FClassFinder<ADragonCharacter> DragonTowerRef(TEXT("/Game/CubeCollect/Dragron/DragonTower.DragonTower_C"));
	if (IsValid(DragonTowerRef.Class))
	{
		WhatToSpawn = DragonTowerRef.Class;
	}

	MaxHp = 15;
}

// Called when the game starts or when spawned
void ATopDownPawn::BeginPlay()
{
	Super::BeginPlay();

	Scene->SetRelativeLocation(FVector(30.0f, 0.0f, 2220.0f));
	Scene->SetRelativeRotation(FRotator(-90.0f, -26.56f, 116.56f));

	//Add Input Mapping Context
	if (APlayerController* PlayerController = CastChecked<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterControl->InputMappingContext, 0);
		}
	}

	PrevActor = nullptr;

	for (TActorIterator<AMyColorActor> it(GetWorld()); it; ++it)
	{
		ColorActors.Add(*it);
	}
}

// Called every frame
void ATopDownPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsBuildMode)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (IsValid(PlayerController))
		{
			FHitResult Result;
			PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Result);
			IMyTowerCubeInterface* Cube = Cast<IMyTowerCubeInterface>(Result.GetActor());

			// 선택된게 타워 였었는지 확인 할 변수
			bool IsTower = false;

			ADragonCharacter* Tower = Cast<ADragonCharacter>(Result.GetActor());
			if(IsValid(Tower))
			{
				Cube = Cast<IMyTowerCubeInterface>(Tower->GetOwnedCube());
				if (Cube)
				{
					PrevActor = Tower->GetOwnedCube();
					ChangeColorCube(PrevActor);
					IsTower = true;
					FVector Start = Scene->GetComponentLocation();
					FVector End = Result.ImpactPoint;
					//DrawDebugLine(GetWorld(), Start, End, FColor::Magenta, false, 1, 0, 10);
				}
			}
			else
			{
				PrevActor = Result.GetActor();
				ChangeColorCube(PrevActor);
			}
		}
	}
	else if (IsCompositedMode)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (IsValid(PlayerController))
		{
			FHitResult Result;
			PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Result);
			IMyTowerCubeInterface* Cube = Cast<IMyTowerCubeInterface>(Result.GetActor());

			// 선택된게 타워 였었는지 확인 할 변수
			bool IsTower = false;

			ADragonCharacter* Tower = Cast<ADragonCharacter>(Result.GetActor());
			if(IsValid(Tower))
			{
				Cube = Cast<IMyTowerCubeInterface>(Tower->GetOwnedCube());
				if (Cube)
				{
					PrevActor = Tower->GetOwnedCube();
					ChangeColorCube(PrevActor);
					IsTower = true;
					FVector Start = Scene->GetComponentLocation();
					FVector End = Result.ImpactPoint;
					//DrawDebugLine(GetWorld(), Start, End, FColor::Magenta, false, 1, 0, 10);
				}
			}
			else
			{
				PrevActor = Result.GetActor();
				ChangeColorCube(PrevActor);
			}
		}
	}
	else
	{
		ChangeColorCube(PrevActor);
	}

}

// Called to bind functionality to input
void ATopDownPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Build
		EnhancedInputComponent->BindAction(BuildAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Build);

		// Composite
		EnhancedInputComponent->BindAction(CompositeAction, ETriggerEvent::Triggered, this, &ATopDownPawn::CompositeActor);

		// Destroy
		EnhancedInputComponent->BindAction(DestroyAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Destroy);

		// LefteClick
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &ATopDownPawn::BuildRnadomActor);
		//EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &ATopDownPawn::BuildRnadomActorReleased);

	}
}

void ATopDownPawn::Build(const FInputActionValue& Value)
{
	IsBuildMode = IsBuildMode ? false : true;
	IsCompositedMode = false;
}

void ATopDownPawn::CompositeActor(const FInputActionValue& Value)
{
	IsCompositedMode = IsCompositedMode ? false : true;
	IsBuildMode = false;
}

void ATopDownPawn::BuildRnadomActor(const FInputActionValue& Value)
{
	//IsBuildMode = false;
	if (IsBuildMode && PrevActor)
	{
		IsBuildMode = false;

		IMyTowerCubeInterface* PrevCube = Cast<IMyTowerCubeInterface>(PrevActor);
		if (PrevCube)
		{
			PrevCube->ChangeColor();

			UWorld* const World = GetWorld();
			if (!PrevCube->GetIsOnTurret() && World)
			{
				AColorGameModeBase* pGameMode = GetWorld()->GetAuthGameMode<AColorGameModeBase>();
				if (IsValid(pGameMode) && pGameMode->IsValidBuild())
				{
					pGameMode->SetGold();
				}
				else
				{
					// 골드가 부족함
					return;
				}

				UStaticMeshComponent* CubeMesh = PrevActor->FindComponentByClass<UStaticMeshComponent>();
				FBoxSphereBounds CubeBounds = CubeMesh->CalcBounds(CubeMesh->GetComponentTransform());
				FVector SpawnLocation = PrevActor->GetActorLocation();
				SpawnLocation = SpawnLocation + FVector(0.0f, 0.0f, CubeBounds.BoxExtent.Z);
				ADragonCharacter* const SpawnedActor = World->SpawnActor<ADragonCharacter>(WhatToSpawn, SpawnLocation, FRotator::ZeroRotator, FActorSpawnParameters());
				SpawnedActor->SetAttackType();
				SpawnedActor->SetOwnedCube(PrevActor);
				PrevCube->SetOnTower(SpawnedActor);
				AllTowers.Add(SpawnedActor);
			}
			PrevActor = nullptr;
		}
	}
	else if (IsCompositedMode)
	{
		IsCompositedMode = false;
		IMyTowerCubeInterface* PrevCube = Cast<IMyTowerCubeInterface>(PrevActor);
		if (!PrevCube && IsValid(PrevActor))
		{
			ADragonCharacter* Tower = Cast<ADragonCharacter>(PrevActor);
			if (IsValid(Tower))
			{
				PrevCube = Cast<IMyTowerCubeInterface>(Tower->GetOwnedCube());
			}
		}
		
		UWorld* const World = GetWorld();
		if (PrevCube && IsValid(World))
		{
			PrevCube->ChangeColor();
			ADragonCharacter* Tower = Cast<ADragonCharacter>(PrevCube->GetOnTower());
			if(IsValid(Tower))
			{
				// 첫번째  순서로 옮기기
				int TowerCount = 0;
				int32 Index = AllTowers.Find(Tower);
				AllTowers.RemoveAt(Index);
				AllTowers.Insert(Tower, 0);
			}

			EAttackType _AttackType = PrevCube->GetAttackType();
			int UpgradeLevel = 0;
			ADragonCharacter* DeleteActor = nullptr;
			for (auto& _Tower : AllTowers)
			{
				if (_Tower == PrevCube->GetOnTower())
				{
					continue;
				}

				UpgradeLevel = Tower->IsUpgradeable(_Tower);
				if (UpgradeLevel)
				{
					UE_LOG(LogClass, Log, TEXT("Same Grade _Tower : %s"), *_Tower->GetFName().ToString());
					IMyTowerCubeInterface* _Cube = Cast<IMyTowerCubeInterface>(_Tower->GetOwnedCube());
					if(_Cube)
					{
						_Cube->SetOnTower(nullptr);
						_Tower->Destroy();
						DeleteActor = _Tower;
					}
					break;
				}
			}
			if (IsValid(DeleteActor))
			{
				AllTowers.Remove(DeleteActor);
				DeleteActor = nullptr;
			}

			if(UpgradeLevel)
			{
				FVector SpawnLocation = Tower->GetActorLocation();
				AllTowers.Remove(Tower);
				Tower->Destroy();
				ADragonCharacter* const SpawnedActor = World->SpawnActor<ADragonCharacter>(WhatToSpawn, SpawnLocation, FRotator::ZeroRotator, FActorSpawnParameters());
				SpawnedActor->SetAttackType();
				SpawnedActor->SetTowerLevel(UpgradeLevel);
				PrevCube->SetOnTower(SpawnedActor);
				PrevCube->ChangeColor();
				PrevActor = nullptr;
				AllTowers.Add(SpawnedActor);
			}
		}

		
	}
	
}

void ATopDownPawn::BuildRnadomActorReleased(const FInputActionValue& Value)
{
	UE_LOG(LogClass, Log, TEXT("Left click Complete"));

}

void ATopDownPawn::Destroy(const FInputActionValue& Value)
{
	UE_LOG(LogClass, Log, TEXT("Destroy In"));
	// e눌러서 타워 지우게 하기
}

void ATopDownPawn::ChangeColorCube(AActor* SelectedCube)
{
	if (IsValid(SelectedCube))
	{
		for (auto& Cube : ColorActors)
		{
			if (Cube != SelectedCube)
			{
				Cube->ChangeColor();
			}
		}
		IMyTowerCubeInterface* SelectCube = Cast<IMyTowerCubeInterface>(SelectedCube);
		if (SelectCube)
		{
			FVector Color;
			float Alpha = 0.7f;
			if(IsBuildMode)
			{
				Color = SelectCube->GetIsOnTurret() ? FVector(1.0f, 0.0f, 0.0f) : FVector(0.0f, 0.0f, 1.0f);
			}
			else if(IsCompositedMode)
			{
				Color = SelectCube->GetIsOnTurret() ? FVector(1.0f, 0.0f, 1.0f) : FVector(1.0f, 0.0f, 0.0f);
			}
			else
			{
				Color = FVector(1.0f, 1.0f, 1.0f);
				Alpha = 1.0f;
			}

			SelectCube->ChangeColor(Color, Alpha);
		}

		//SelectedCube = nullptr;
	}
}

void ATopDownPawn::SetupHUDWIdget(UTowerHUDWidget* HUDWidget)
{
	if (IsValid(HUDWidget))
	{
		AColorGameModeBase* pGameMode = GetWorld()->GetAuthGameMode<AColorGameModeBase>();
		if (IsValid(pGameMode))
		{
			HUDWidget->UpdateRound(pGameMode->GetCurrentRound());
			HUDWidget->UpdateHp(pGameMode->GetHp());
			HUDWidget->UpdateGold(pGameMode->GetGold());
			//CountDown
			HUDWidget->UpdateCountdown(3);
			pGameMode->OnRoundChanged.AddUObject(HUDWidget, &UTowerHUDWidget::UpdateRound);
			pGameMode->OnHpChanged.AddUObject(HUDWidget, &UTowerHUDWidget::UpdateHp);
			pGameMode->OnCountChanged.AddUObject(HUDWidget, &UTowerHUDWidget::UpdateCountdown);
			pGameMode->OnGoldChanged.AddUObject(HUDWidget, &UTowerHUDWidget::UpdateGold);
		}
	}
}
