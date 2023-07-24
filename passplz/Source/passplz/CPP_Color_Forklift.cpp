// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Color_Forklift.h"

// Sets default values
ACPP_Color_Forklift::ACPP_Color_Forklift()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	pillar1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 1"));
	pillar2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 2"));
	pillar3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 3"));
	pillar4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 4"));
	pillar5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 5"));
	pillar6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar 6"));
	Forklifts.Init(nullptr, 8);
	Forklifts[0] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("blackForklift"));
	Forklifts[1] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("blueForklift"));
	Forklifts[2] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("greenForklift"));
	Forklifts[3] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cyanForklift"));
	Forklifts[4] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("redForklift"));
	Forklifts[5] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("magentaForklift"));
	Forklifts[6] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("yellowForklift"));
	Forklifts[7] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("whiteForklift"));

	RootComponent = pillar1;

	pillar2->SetupAttachment(RootComponent);
	pillar3->SetupAttachment(RootComponent);
	pillar4->SetupAttachment(RootComponent);
	pillar5->SetupAttachment(RootComponent);
	pillar6->SetupAttachment(RootComponent);

	Forklifts[0]->SetupAttachment(RootComponent);
	Forklifts[1]->SetupAttachment(RootComponent);
	Forklifts[2]->SetupAttachment(RootComponent);
	Forklifts[3]->SetupAttachment(RootComponent);
	Forklifts[4]->SetupAttachment(RootComponent);
	Forklifts[5]->SetupAttachment(RootComponent);
	Forklifts[6]->SetupAttachment(RootComponent);
	Forklifts[7]->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTTON(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SM_BUTTON.Succeeded()) {
		pillar1->SetStaticMesh(SM_BUTTON.Object);
		pillar2->SetStaticMesh(SM_BUTTON.Object);
		pillar3->SetStaticMesh(SM_BUTTON.Object);
		pillar4->SetStaticMesh(SM_BUTTON.Object);
		pillar5->SetStaticMesh(SM_BUTTON.Object);
		pillar6->SetStaticMesh(SM_BUTTON.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FORKLIFT(TEXT("/Game/model/STAGE_1/forklift.forklift"));
	if (SM_BUTTON.Succeeded()) {
		Forklifts[0]->SetStaticMesh(SM_FORKLIFT.Object);
		Forklifts[1]->SetStaticMesh(SM_FORKLIFT.Object);
		Forklifts[2]->SetStaticMesh(SM_FORKLIFT.Object);
		Forklifts[3]->SetStaticMesh(SM_FORKLIFT.Object);
		Forklifts[4]->SetStaticMesh(SM_FORKLIFT.Object);
		Forklifts[5]->SetStaticMesh(SM_FORKLIFT.Object);
		Forklifts[6]->SetStaticMesh(SM_FORKLIFT.Object);
		Forklifts[7]->SetStaticMesh(SM_FORKLIFT.Object);
	}

	pillarColor = FVector(0.f, 0.f, 0.f);
	for (int i = 0; i < 8; ++i) {
		isForkliftsMove[i] = false;
		forklifrsdirection[i] = 1.f;
		forkliftsMoveTime[i] = 0.f;
	}
}

// Called when the game starts or when spawned
void ACPP_Color_Forklift::BeginPlay()
{
	Super::BeginPlay();

	pillar1->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));
	pillar2->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));
	pillar3->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));
	pillar4->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));
	pillar5->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));
	pillar6->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));

	Forklifts[0]->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));
	Forklifts[1]->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 1.0f));
	Forklifts[2]->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 1.0f, 0.0f));
	Forklifts[3]->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 1.0f, 1.0f));
	Forklifts[4]->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.0f, 0.0f, 0.0f));;
	Forklifts[5]->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.0f, 0.0f, 1.0f));;
	Forklifts[6]->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.0f, 1.0f, 0.0f));;
	Forklifts[7]->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.0f, 1.0f, 1.0f));;

}

// Called every frame
void ACPP_Color_Forklift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Color_Forklift::SetPillarColor()
{
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/game_sound/stage_1/button_click_Cue.button_click_Cue"));
	UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation(), GetActorRotation());

	pillar1->SetVectorParameterValueOnMaterials(TEXT("color"), pillarColor);
	pillar2->SetVectorParameterValueOnMaterials(TEXT("color"), pillarColor);
	pillar3->SetVectorParameterValueOnMaterials(TEXT("color"), pillarColor);
	pillar4->SetVectorParameterValueOnMaterials(TEXT("color"), pillarColor);
	pillar5->SetVectorParameterValueOnMaterials(TEXT("color"), pillarColor);
	pillar6->SetVectorParameterValueOnMaterials(TEXT("color"), pillarColor);
	FindAndMoveForkliftByColor();
}

void ACPP_Color_Forklift::FindAndMoveForkliftByColor()
{
	currentColorForklift = (int)((pillarColor.X * 2 * 2) + (pillarColor.Y * 2) + (pillarColor.Z * 1));

	isForkliftsMove[currentColorForklift] = true;

	if (!GetWorldTimerManager().IsTimerActive(forkliftsMoveTimer)) {
		GetWorld()->GetTimerManager().SetTimer(forkliftsMoveTimer, this, &ACPP_Color_Forklift::ForkliftMoveTimer, 0.03f, true);
		
	}
}

void ACPP_Color_Forklift::ForkliftMoveTimer()
{
	bool stopTimer = true;
	for (int i = 0; i < 8; ++i) {
		if (isForkliftsMove[i]) {
			stopTimer = false;

			USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/game_sound/stage_1/forklift_movement_Cue.forklift_movement_Cue"));
			UGameplayStatics::PlaySoundAtLocation(this, Sound, Forklifts[i]->GetComponentLocation(), GetActorRotation());

			Forklifts[i]->AddLocalOffset(FVector(forklifrsdirection[i], 0.0f, 0.0f));
			forkliftsMoveTime[i] += 0.03;
			if (forkliftsMoveTime[i] > 3.f) {
				isForkliftsMove[i] = false;
				forklifrsdirection[i] *= -1;
				forkliftsMoveTime[i] = 0.f;
			
			}
		}
	}


	if (stopTimer)
		GetWorldTimerManager().ClearTimer(forkliftsMoveTimer);
}

//버튼이 눌림 사운드를 울리고 색을 바꾼다.
void ACPP_Color_Forklift::RecvColor(int color)
{
	switch (color) {
	case 0: //레드
		UE_LOG(LogTemp, Warning, TEXT("Forklift::Red"));
		pillarColor.X = 1.0;
		break;
	case 1: //그린
		UE_LOG(LogTemp, Warning, TEXT("Forklift::Green"));
		pillarColor.Y = 1.0;
		break;
	case 2: //블루
		UE_LOG(LogTemp, Warning, TEXT("Forklift::Blue"));
		pillarColor.Z = 1.0;
		break;
	case 3: //블랙
		UE_LOG(LogTemp, Warning, TEXT("Forklift::Black"));
		pillarColor = FVector(0.0f, 0.0f, 0.0f);
		break;

	}
	SetPillarColor();
}
