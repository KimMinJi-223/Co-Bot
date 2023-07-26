// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "passplz.h"
#include "GameFramework/Actor.h"
#include "CPP_Color_Forklift.generated.h"

UCLASS()
class PASSPLZ_API ACPP_Color_Forklift : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Color_Forklift();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//기둥 6개
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* pillar1;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* pillar2;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* pillar3;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* pillar4;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* pillar5;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* pillar6;

	UPROPERTY(VisibleAnywhere)
		TArray<UStaticMeshComponent*> Forklifts;

	FVector pillarColor;
	int currentColorForklift; 
	float forklifrsdirection[8]; 
	bool isForkliftsMove[8]; 
	float forkliftsMoveTime[8]; 

	FTimerHandle forkliftsMoveTimer;

	//버튼 사운드
	USoundBase* buttonSound;
	//지게차 사운드
	USoundBase* forkliftSound;

public:
	FVector GetPillarColor() { return pillarColor; };
	void SetPillarColor();
	void FindAndMoveForkliftByColor();
	void ForkliftMoveTimer();
	void RecvColor(int color);
};
