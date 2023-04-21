// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
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

private:
	UFUNCTION()
		void OnComponentBeginOverlap_box_collision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap_box_collision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	int checkNumOfCobot;
	FVector elevatorDoorLeftPos;
	FVector elevatorDoorRightPos;
	FVector elevatorFloorPos;
	int checkMyStage;

public:
	// 문 열리고 닫히고 올라가고 카메라 바꾸고 레벨 바꾸고
	// 컬리전 박스 부모로 바꾸기
};
