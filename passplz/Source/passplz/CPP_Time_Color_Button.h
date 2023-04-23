// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "CPP_Time_Color_Button.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Time_Color_Button : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Time_Color_Button();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* center;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* timeColorFootholdCollision;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* timeColorFoothold;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* redCollision1;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* red1;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* greenCollision1;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* green1;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* blueCollision1;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* blue1;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* blackCollision1;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* black1;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* redCollision2;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* red2;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* greenCollision2;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* green2;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* blueCollision2;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* blue2;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* blackCollision2;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* black2;

	FVector footholdColor;

	FTimerHandle footholdColorChageTimer;

private:
	//버튼1의 충돌
	UFUNCTION()
		void OnComponentBeginOverlap_redCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_greenCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_blueCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_blackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentBeginOverlap_timeColorFootholdCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void FootholdColorChageTimer();
};
