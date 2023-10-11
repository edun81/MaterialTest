// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "PickupBattery.generated.h"

/**
 * 
 */
UCLASS()
class MATERIALTEST_API APickupBattery : public APickup
{
	GENERATED_BODY()

public:
	APickupBattery();

	// 컬렉션 함수 오버라이딩
	void wasCollected_Implementation() override;


	float GetPower();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Power", Meta = (BlueprintProtected = "true"))
	float BatteryPower;
	
};
