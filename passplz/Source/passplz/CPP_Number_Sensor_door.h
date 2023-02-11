// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "CPP_Number_Sensor_door.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Number_Sensor_door : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Number_Sensor_door();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Foothold;
	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Doorframe;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* Trigger_Foothold;

	UPROPERTY(VisibleAnywhere)
		int32 num_Dooropen;

	UPROPERTY(VisibleAnywhere)
		int32 num_currnet_foot;

	/*UPROPERTY(VisibleAnywhere)
		bool is_door_open;*/

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnFootOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnFootEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
