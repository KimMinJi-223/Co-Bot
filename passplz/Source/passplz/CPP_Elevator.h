// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "Components/RectLightComponent.h"
#include "Blueprint/UserWidget.h"
#include "CPP_Elevator.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Elevator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPP_Elevator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PostInitializeComponents() override;	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* doorLeft;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* doorRight;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* doorFloor;
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* boxCollision;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* camera;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* doorFloorForSeekButton;

	UPROPERTY(VisibleAnywhere)
		URectLightComponent* evLightAfterClearTutorial;

private:
	UFUNCTION()
		void OnComponentBeginOverlap_boxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap_boxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintReadWrite)
	int32 checkMyStage;

private:
	int32 checkNumOfCobot;
	FTimerHandle elevatorDoorOpenTimer;
	FTimerHandle elevatorDoorCloseTimer;
	FString levelName;
	int32 choose;
public:
	UFUNCTION(BlueprintCallable)
	void LightOnEvDoorOpen();

	// 문 열리고 닫히고 올라가고 카메라 바꾸고 레벨 바꾸고
	void ElevatorOperateCameraMoveLevelChange();

private:
	void DoorOpenTimer();
	void DoorCloseFloorUpTimer();
};
