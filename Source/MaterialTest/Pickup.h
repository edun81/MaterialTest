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



	// BlueprintPure �� BlueprintCallable �� ������
	// �Ѵ� �ڵ�� �ۼ��Ǿ��� �������Ʈ���� ȣ���� ������
	// BlueprintPure ���� �⺻������ ���� ������ ���������� �Ϲ������δ� �׷��� �ϸ� �ȵ�
	// ��ȯ���� �޶�� ������ ���� �����̵�
	// �ݺ����̳� ���δٸ� �ΰ��� ����� �ι� ȣ�� �Ǿ������

	// BlueprintCallable ���� ������� �ɿ��� ���� ��ȣ�� �ԷµǾ��������� ������ ��.
	// �׸��� ������� �ƿ�ǲ �ɿ� �����ص�.
	// ���� �������� �ʴ� �Լ��ε� ���� �� ����

	// �׷��� ���� �������� ���� ������ ����Ǵ� �ڽ�Ʈ�� ���� ���� ������� BlueprintPure �Լ� ���ٴ� BlueprintCallable �Լ��� �� ������
	// ����� ������ ���� ����Ǵ� ������ BlueprintPure �Լ��� �� ������


	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool newState);


	// �������Ʈ ����Ƽ�� �̺�Ʈ�϶�
	// �ڵ� ���ø������̼�, �����Լ����� �ϰ�, �ڼ� Ŭ�������� �������̵��� ����
	// �������Ʈ���� ���� ���� + �߰� �ڵ嵿��
	UFUNCTION(BlueprintNativeEvent)
	void wasCollected();
	virtual void wasCollected_Implementation();	

	

protected:
	bool	bIsActive;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

};
