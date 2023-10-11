// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class MATERIALTEST_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return whereToSpawn; }	

	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawningActive(bool bShouldSpawn);

protected:
	// TSubclassOf�� ����Ѵٴ°��� APickup ��ӹ޴� Ŭ������ �������� �Ѵٴ� �ǹ�
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APickup> whatToSpawn;

	FTimerHandle	_timer_handle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float _spawn_delay_min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float _spawn_delay_max;

	float _cur_delay;

private:
	// ������ ������ġ�� ������ �ڽ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent*	whereToSpawn;

	void SpawnPickup();

};
