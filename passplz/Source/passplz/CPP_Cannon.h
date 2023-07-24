// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"
#include "CPP_Cannon.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Cannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Cannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* cannonStand;

protected:
	//목표 범위
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* damageRadius;

	//목표 지점
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UDecalComponent* targetLocation;

	//발사 위치
	UPROPERTY(BlueprintReadWrite)
		FVector fireStartLocation;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* lavaCollision;

	UPROPERTY(BlueprintReadWrite)
		FVector projectileVelocity;

	UPROPERTY(BlueprintReadWrite)
		FRotator targetRotation;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* clearActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* cannonMuzzle;


	//서버 : 조준점 위치 정해주는 거
	void SetBombDropLocation(int player_number, double value);
	//서버 : 발사하는 함수
	void FireLava();


	UFUNCTION()
		void OnComponentBeginOverlap_lavaCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
