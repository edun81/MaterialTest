// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaterialTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Pickup.h"
#include "PickupBattery.h"


//////////////////////////////////////////////////////////////////////////
// AMaterialTestCharacter

AMaterialTestCharacter::AMaterialTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Create the Collection Sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.0f);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	InitialPower = 2000.f;
	CharacterPower = InitialPower;

	// 파워 레벨에 따라 속도를 설정함
	SpeedFactor = 0.75f;
	BaseSpeed = 10.0f;
}

void AMaterialTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMaterialTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMaterialTestCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMaterialTestCharacter::Look);

		//Collecting
		EnhancedInputComponent->BindAction(Collection, ETriggerEvent::Triggered, this, &AMaterialTestCharacter::CollectPickups);

	}

}

void AMaterialTestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
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

void AMaterialTestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMaterialTestCharacter::CollectPickups()
{
	UE_LOG(LogClass, Log, TEXT("---------- CollectPickups"));
	UE_LOG(LogClass, Log, TEXT("---------- CollectPickups"));
	UE_LOG(LogClass, Log, TEXT("---------- CollectPickups"));
	UE_LOG(LogClass, Log, TEXT("---------- CollectPickups"));


	// Sphere와 오버랩 되는 모든 액터를 배열로 받아와서
	TArray<AActor*> arr_actors;
	CollectionSphere->GetOverlappingActors(arr_actors);

	// 
	float CollectedPower = 0.0f;

	// foreach를 이용해서 Actor를 APickup 으로 형변환
	for (int32 i = 0; i < arr_actors.Num(); ++i)
	{
		APickup* const pickup = Cast<APickup>(arr_actors[i]);

		// 형변환 성공, 아이템 유효하고, 활성화 되어있으면
		if (pickup && !pickup->IsPendingKill() && pickup->IsActive())
		{
			// WasCollected 함수 호출 및 비활성화 상태로 세팅
			pickup->wasCollected();

			APickupBattery* battery = Cast<APickupBattery>(pickup);
			if (battery)
			{
				CollectedPower += battery->GetPower();
			}

			pickup->SetActive(false);
		}
	}

	if (CollectedPower > 0)
	{
		UpdatePower(CollectedPower);
	}
}

float AMaterialTestCharacter::GetInitialPower()
{
	return InitialPower;
}

float AMaterialTestCharacter::GetCurrentPower()
{
	return CharacterPower;
}

// 파워가 늘거나 줄면 호출됨.
void AMaterialTestCharacter::UpdatePower(float PowerChange)
{
	CharacterPower = CharacterPower + PowerChange;

	// 파워에 따라 속도를 변경함.
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed * SpeedFactor * CharacterPower;

	// 시각효과 호출
	EventPowerChangeEffect();

}




