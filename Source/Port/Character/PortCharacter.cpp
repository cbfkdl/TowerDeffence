// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyPrimaryDataAsset.h"
#include "CharacaterStat/MyCharacterStatComponent.h"


DEFINE_LOG_CATEGORY(LogMyCharacter);

//////////////////////////////////////////////////////////////////////////
// APortCharacter

APortCharacter::APortCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Temp Value
	/*m_MaxWalkSpeed = 700.0f;
	m_MinWalkSpeed = 200.0f;*/

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'"));
	if (IsValid(InputActionJumpRef.Object))
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'"));
	if (IsValid(InputActionMoveRef.Object))
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
	if (IsValid(InputActionLookRef.Object))
	{
		LookAction = InputActionLookRef.Object;
	}

	/*static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDashRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Dash.IA_Dash'"));
	if (nullptr != InputActionDashRef.Object)
	{
		DashAction = InputActionDashRef.Object;
	}*/

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRollRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Roll.IA_Roll'"));
	if (IsValid(InputActionRollRef.Object))
	{
		RollAction = InputActionRollRef.Object;
	}

	// 
	IsDashEnd = true;

	Stat->InitEnergy(2000.0f);
	//CharacterPower = Stat->GetInitialPower();


}

void APortCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = CastChecked<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APortCharacter::Tick(float fDeltaTime)
{
	Super::Tick(fDeltaTime);

	//float MovementSpeed = GetCharacterMovement()->MaxWalkSpeed;

	//if ( IsDashEnd && static_cast<int32>(MovementSpeed) > static_cast<int32>(m_MinWalkSpeed))
	//{
	//	MovementSpeed = FMath::Lerp<float, float>(MovementSpeed, m_MinWalkSpeed, fDeltaTime*5.0f);
	//	//UE_LOG(LogMyCharacter, Log, TEXT("DashEnd %.2f"), MovementSpeed);
	//	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	//}

}

void APortCharacter::SetCharacterControlData(const UMyPrimaryDataAsset* ControlData)
{
	Super::SetCharacterControlData(ControlData);

	CameraBoom->TargetArmLength = ControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(ControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = ControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = ControlData->bInheritPitch;
	CameraBoom->bInheritYaw = ControlData->bInheritPitch;
	CameraBoom->bInheritRoll = ControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = ControlData->bDoCollisionTest;

}

//////////////////////////////////////////////////////////////////////////
// Input

void APortCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APortCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APortCharacter::Look);

		//Dash
		//EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &APortCharacter::Dash);
		//EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &APortCharacter::DashEnd);

		//Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APortCharacter::Roll);

	}

}

void APortCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APortCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APortCharacter::Dash(const FInputActionValue& Value)
{
	float MovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	IsDashEnd = false;

	if (MovementSpeed >= m_MaxWalkSpeed)
		return;

	MovementSpeed = FMath::Lerp<float, float>(MovementSpeed, m_MaxWalkSpeed, 0.05f);
	
	//UE_LOG(LogMyCharacter, Log, TEXT("Dash %.2f"), MovementSpeed);

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	
}

void APortCharacter::DashEnd(const FInputActionValue& Value)
{
	IsDashEnd = true;
}

void APortCharacter::Roll(const FInputActionValue& Value)
{
	ProcessRollCommand();
	
	FVector MovementVector = Value.Get<FVector>();

	

	if (IsValid(Controller))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Controller
		APlayerController* PlayerController = CastChecked<APlayerController>(Controller);
		if (PlayerController)
		{
			FVector ForwardVector = GetActorForwardVector();
			//SetActorRelativeLocation(GetActorLocation() + ForwardVector * 500.0f, true, nullptr, ETeleportType::None);
			
			SetActorLocation(GetActorLocation() + ForwardVector * 500.0f, true, nullptr, ETeleportType::None);

			
		}



	}


}
