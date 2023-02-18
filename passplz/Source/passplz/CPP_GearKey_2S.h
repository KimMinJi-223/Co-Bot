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
		UStaticMeshComponent* lever;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* forward_lever_collision;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* back_lever_collision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* right_button;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* right_button_collision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* left_button;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* left_button_collision;

	UPROPERTY(VisibleAnywhere)
		bool Is_forward;

	UPROPERTY(VisibleAnywhere)
		bool Is_back;

	UPROPERTY(VisibleAnywhere)
		bool Is_right;

	UPROPERTY(VisibleAnywhere)
		bool Is_left;

	UPROPERTY(VisibleAnywhere)
		int32 move_forward;

	UPROPERTY(VisibleAnywhere)
		int32 turn_right;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* floor;

	UFUNCTION()
		void OnForwardLeverOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnForwardLeverEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnBackLeverOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBackLeverEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnRightButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnRightButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnLeftButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnLeftButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnClearOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
