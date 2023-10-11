// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MaterialTestGameMode.generated.h"


// 현재 게임플레이 상태를 저장하는 enum
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

	// 현재 상태 
	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState();

	// 새 플레이상태로 설정
	void SetCurrentState(EBatteryPlayState NewState);

protected:

	// 캐릭터 파워가 줄어드는 정도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float DecayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float PowerToWin;

	// HUD 화면에 사용할 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// HUD의 인스턴스
	UPROPERTY()
	class UUserWidget*		CurrentWidget;

private:
	EBatteryPlayState		CurrentState;

	TArray<class ASpawnVolume*> SpawnVolumeActors;

	// 게임 플레이 상태에 영향을 주는 모든 함수의 호출을 담당
	void HandleNewState(EBatteryPlayState NewState);
};



