// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "../../../server/server/protocol.h"
#include "CPP_Cobot_Controller.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "CPP_Color_Button.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Color_Button : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Color_Button();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* center;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* redCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* red;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* greenCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* green;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* blueCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* blue;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* blackCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* black;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* redFoothold; //빨간색 발판

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* redFootholdCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* cyanFoothold; //청록색 발탄

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* cyanFootholdCollision;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* respawnCollision;

	FVector color;

private:
	void CobotButtonSend(packet_type type);

	//버튼의 충돌
	UFUNCTION()
		void OnComponentBeginOverlap_redCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_greenCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_blueCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_blackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//발판의 충돌
	UFUNCTION()
		void OnComponentBeginOverlap_redFootholdCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_cyanCollisionCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//리스폰 충돌
	UFUNCTION()
		void OnComponentBeginOverlap_respawnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
