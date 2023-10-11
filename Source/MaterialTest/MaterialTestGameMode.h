// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MaterialTestGameMode.generated.h"


// ���� �����÷��� ���¸� �����ϴ� enum
UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
	EPlaying UMETA(DisplayName = "Meta-Play"),
	EGameOver UMETA(DisplayName = "Meta-GameOver"),
	EWon UMETA(DisplayName = "Meta-Won!!"),
	EUnknown UMETA(DispalyName = "[Meta-Unknown]")
};


UCLASS(minimalapi)
class AMaterialTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMaterialTestGameMode();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	virtual void BeginPlay() override;

	// ���� ���� 
	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState();

	// �� �÷��̻��·� ����
	void SetCurrentState(EBatteryPlayState NewState);

protected:

	// ĳ���� �Ŀ��� �پ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float DecayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float PowerToWin;

	// HUD ȭ�鿡 ����� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// HUD�� �ν��Ͻ�
	UPROPERTY()
	class UUserWidget*		CurrentWidget;

private:
	EBatteryPlayState		CurrentState;

	TArray<class ASpawnVolume*> SpawnVolumeActors;

	// ���� �÷��� ���¿� ������ �ִ� ��� �Լ��� ȣ���� ���
	void HandleNewState(EBatteryPlayState NewState);
};



