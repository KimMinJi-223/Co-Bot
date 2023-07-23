// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "CPP_GearKey_2S.generated.h"

UCLASS()
class PASSPLZ_API ACPP_GearKey_2S : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_GearKey_2S();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	// 서버에서 받은 패킷 타입에 따라 불리는 함수 target의 위치를 옮긴다.
	void target_forward();
	void target_back();
	void target_right();
	void target_left();


private:
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* clear;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* center_gear;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* child_gear1;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* child_gear2;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* child_gear3;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Key;



	UPROPERTY(VisibleAnywhere)
		UBoxComponent* forward_Key_collision;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* back_Key_collision;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* right_Key_collision;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* left_Key_collision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Key_frame;
	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* blockCube;

	FTimerHandle bridgeTimer;
	float bridgeTime = 0.f;

	UFUNCTION()
		void OnClearOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void BridgeTimer();
};