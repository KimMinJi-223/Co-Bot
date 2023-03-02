// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "CPP_Checker_5S.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Checker_5S : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Checker_5S();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

private:
	//class ACPP_Cobot_On_rail* Cobot;
	TArray<class ACPP_Cobot_On_rail*> Cobot;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* CheckerMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Move;

	UPROPERTY(VisibleAnywhere)
		UArrowComponent* Factory_start;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* Kill_Zone;

	UFUNCTION()
		void OnMoveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnMoveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnKillZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
