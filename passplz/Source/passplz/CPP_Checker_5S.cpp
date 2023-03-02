// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Checker_5S.h"
#include "CPP_Cobot_On_rail.h"

// Sets default values
ACPP_Checker_5S::ACPP_Checker_5S()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CheckerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Move = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_Button"));
	Factory_start = CreateDefaultSubobject<UArrowComponent>(TEXT("Factory_start"));
	Kill_Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Kill_Zone"));

	RootComponent = Move;

	CheckerMesh->SetupAttachment(RootComponent);
	Factory_start->SetupAttachment(RootComponent);
	Kill_Zone->SetupAttachment(RootComponent);

	CheckerMesh->SetRelativeLocation(FVector(0.f, 0.f, -190.f));
	Factory_start->SetRelativeLocation(FVector(0.f, -1500.f, -80.f));

	Factory_start->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHECKER(TEXT("/Game/model/STAGE_5/checker.checker"));
	if (SK_CHECKER.Succeeded()) {
		CheckerMesh->SetSkeletalMesh(SK_CHECKER.Object);
	}
}

// Called when the game starts or when spawned
void ACPP_Checker_5S::BeginPlay()
{
	Super::BeginPlay();

	CheckerMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/model/STAGE_5/checker_Red_Green_Anim.checker_Red_Green_Anim"));
	if (AnimAsset != nullptr) {
		CheckerMesh->PlayAnimation(AnimAsset, true);
	}

	Cobot.Init(nullptr, 21);

	FVector actorlocation = GetActorLocation();
	for (int i = 0; i < 21; ++i) {
		Cobot[i] = GetWorld()->SpawnActor<ACPP_Cobot_On_rail>(ACPP_Cobot_On_rail::StaticClass(), FVector(actorlocation.X, actorlocation.Y + i * 150 - 9 * 150, actorlocation.Z - 80.f), FRotator(0.0f, 0.0f, 0.0f));
	}
}

// Called every frame
void ACPP_Checker_5S::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Checker_5S::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Move->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Checker_5S::OnMoveOverlap);
	Move->OnComponentEndOverlap.AddDynamic(this, &ACPP_Checker_5S::OnMoveEndOverlap);
}

//21번째를 지우고 1번쨰에 삽입한다.
void ACPP_Checker_5S::OnMoveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("begin"));
	Cobot[20]->Destroy();
	Cobot.RemoveAt(20);
	Cobot.Insert(nullptr, 0);
	
	FVector Start_Location = Factory_start->GetComponentLocation();
	Cobot[0] = GetWorld()->SpawnActor<ACPP_Cobot_On_rail>(ACPP_Cobot_On_rail::StaticClass(), FVector(Start_Location.X, Start_Location.Y, Start_Location.Z), FRotator(0.0f, 0.0f, 0.0f));
}

void ACPP_Checker_5S::OnMoveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("end"));
	for (int i = 0; i < 21; ++i) {

		FVector cobot_location = Cobot[i]->GetActorLocation();
		FLatentActionInfo Info;
		Info.CallbackTarget = Cobot[i];
		UKismetSystemLibrary::MoveComponentTo(
			Cobot[i]->GetRootComponent(),
			FVector(cobot_location.X, cobot_location.Y + 150.f, cobot_location.Z),
			FRotator(0.f, 0.f, 0.f),
			false,
			false,
			1.0f,
			false,
			EMoveComponentAction::Move,
			Info);
	}
}

//킬존에 닿으면 해당 객체는 삭제 되고 팩토리스타트 위치에 코봇이 생성된다. 
void ACPP_Checker_5S::OnKillZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


