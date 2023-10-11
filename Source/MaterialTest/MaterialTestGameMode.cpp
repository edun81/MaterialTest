// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaterialTestGameMode.h"
#include "MaterialTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "GameFramework/PawnMovementComponent.h"


AMaterialTestGameMode::AMaterialTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;

	// DecayRate 기본값 설정
	DecayRate = 0.01f;
}

void AMaterialTestGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 모든 스폰볼륨액터를 찾아냄
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

	AMaterialTestCharacter* pMyChar = Cast<AMaterialTestCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (pMyChar)
	{
		PowerToWin = (pMyChar->GetInitialPower()) * 1.25f;
	}

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

	UE_LOG(LogClass, Log, TEXT("-- GetInitialPower : %f"), pMyChar->GetInitialPower());
	UE_LOG(LogClass, Log, TEXT("-- Power To Win : %f"), PowerToWin );
}

void AMaterialTestGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMaterialTestCharacter* pMyChar = Cast<AMaterialTestCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (pMyChar)
	{
		if (pMyChar->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(EBatteryPlayState::EWon);
		}
		else if (pMyChar->GetCurrentPower() > 0)
		{
			pMyChar->UpdatePower(-DeltaTime * DecayRate * (pMyChar->GetInitialPower()));
		}
		else
		{
			SetCurrentState(EBatteryPlayState::EGameOver);
		}
	}
}

float AMaterialTestGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

EBatteryPlayState AMaterialTestGameMode::GetCurrentState()
{
	return CurrentState;
}

void AMaterialTestGameMode::SetCurrentState(EBatteryPlayState NewState)
{
	CurrentState = NewState;

	HandleNewState(CurrentState);
}

void AMaterialTestGameMode::HandleNewState(EBatteryPlayState NewState)
{
	switch (NewState)
	{
		case EBatteryPlayState::EPlaying:
			{
				// Spawn volumes active
				for (ASpawnVolume* pVolume : SpawnVolumeActors)
				{
					pVolume->SetSpawningActive(true);
				}
			}
			break;		
		case EBatteryPlayState::EWon:
			{
				for (ASpawnVolume* pVolume : SpawnVolumeActors)
				{
					pVolume->SetSpawningActive(false);
				}
			}
			break;
		case EBatteryPlayState::EGameOver:
			{
				// Spawn volumes inactive
				for (ASpawnVolume* pVolume : SpawnVolumeActors)
				{
					pVolume->SetSpawningActive(false);
				}

				// block player input
				APlayerController* pPlayerCon = UGameplayStatics::GetPlayerController(this, 0);
				if (pPlayerCon)
				{
					pPlayerCon->SetCinematicMode(true, false, false, true, true);
				}

				ACharacter* pCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
				if (pCharacter)
				{
					pCharacter->GetMesh()->SetSimulatePhysics(true);
					pCharacter->GetMovementComponent()->MovementState.bCanJump = false;
				}
			}
			break;

		default:
		case EBatteryPlayState::EUnknown:
			{
				// do nothing
			}
			break;
	}
}
