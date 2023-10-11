// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class MATERIALTEST_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }



	// BlueprintPure 와 BlueprintCallable 의 차이점
	// 둘다 코드로 작성되었고 블루프린트에서 호출이 가능함
	// BlueprintPure 형은 기본적으로 값의 변경이 가능하지만 일반적으로는 그렇게 하면 안됨
	// 반환값을 달라고 했을때 마다 실행이됨
	// 반복문이나 서로다른 두개의 노드등에서 두번 호출 되었을경우

	// BlueprintCallable 형은 실행순서 핀에서 실행 신호가 입력되었을때에만 실행이 됨.
	// 그리고 결과값을 아웃풋 핀에 저장해둠.
	// 값을 변경하지 않는 함수로도 사용될 수 있음

	// 그래서 자주 변경하지 않을 것으로 예상되는 코스트가 많이 들어가는 결과값에 BlueprintPure 함수 보다는 BlueprintCallable 함수가 더 적합함
	// 계산이 빠르고 자주 변경되는 값에는 BlueprintPure 함수가 더 적합함


	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool newState);


	// 블루프린트 네이티브 이벤트일때
	// 코드 임플리먼테이션, 가상함수여야 하고, 자손 클래스에서 오버라이딩이 가능
	// 블루프린트에서 동작 구현 + 추가 코드동작
	UFUNCTION(BlueprintNativeEvent)
	void wasCollected();
	virtual void wasCollected_Implementation();	

	

protected:
	bool	bIsActive;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

};
