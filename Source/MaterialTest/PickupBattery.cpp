// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBattery.h"


APickupBattery::APickupBattery()
{
	GetMesh()->SetSimulatePhysics(true);

	BatteryPower = 150.0f;
}

void APickupBattery::wasCollected_Implementation()
{
	Super::wasCollected_Implementation();

	Destroy();
}

float APickupBattery::GetPower()
{
	return BatteryPower;
}
