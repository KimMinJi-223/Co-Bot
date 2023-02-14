// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "CPP_Maze_2S.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Maze_2S : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Maze_2S();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* center;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Maze;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Target;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* forward_target_collision;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* back_target_collision;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* right_target_collision;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* left_target_collision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* plan;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* forward_button;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* forward_button_collision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* back_button;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* back_button_collision;

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
		int32 move_right;

	UFUNCTION()
		void OnForwardButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnForwardButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnBackButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBackButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnRightButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnRightButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnLeftButtonOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnLeftButtonEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
}; 
