// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Pickup.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	whereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = whereToSpawn;

	_spawn_delay_min = 1.0f;
	_spawn_delay_max = 4.5f;

	
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();


	// GameMode ���� ��Ʈ���ϱ����� �ּ�
	// _cur_delay = FMath::RandRange(_spawn_delay_min, _spawn_delay_max);
	// GetWorldTimerManager().SetTimer(_timer_handle, this, &ASpawnVolume::SpawnPickup, _cur_delay, false);
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	FVector SpawnOrigin = whereToSpawn->Bounds.Origin;
	FVector SpawnExtent = whereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void ASpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
	if (bShouldSpawn)
	{
		_cur_delay = FMath::RandRange(_spawn_delay_min, _spawn_delay_max);
		GetWorldTimerManager().SetTimer(_timer_handle, this, &ASpawnVolume::SpawnPickup, _cur_delay, false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(_timer_handle);
	}
}

void ASpawnVolume::SpawnPickup()
{
	// ������ �� �� �ִٸ�
	if (whatToSpawn != NULL)
	{
		// ���尡 ��ȿ���� Ȯ��
		UWorld* const World = GetWorld();
		if (World)
		{
			// ���� �Ķ���͸� ����
			FActorSpawnParameters SpawnParams;

			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// ������ġ�� ������ ��ġ�� ����
			FVector SpawnLocation = GetRandomPointInVolume();

			// ������ �����ۿ� ������ ȸ������ ��
			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = FMath::FRand() * 360.0f;
			SpawnRotation.Roll = FMath::FRand() * 360.0f;

			// Pickup �� ����
			APickup* const SpawnedPickup = World->SpawnActor<APickup>(whatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			_cur_delay = FMath::RandRange(_spawn_delay_min, _spawn_delay_max);
			GetWorldTimerManager().SetTimer(_timer_handle, this, &ASpawnVolume::SpawnPickup, _cur_delay, false);
		}
	}
		
}

