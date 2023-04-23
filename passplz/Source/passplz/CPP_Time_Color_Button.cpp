// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Time_Color_Button.h"
#include "CPP_Cobot.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
// Sets default values
ACPP_Time_Color_Button::ACPP_Time_Color_Button()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	center = CreateDefaultSubobject<UArrowComponent>(TEXT("Center"));
	timeColorFoothold = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("timeColorFoothold"));
	timeColorFootholdCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("timeColorFootholdCollision"));

	//첫번째 컬러 버튼
	red1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("red1"));
	redCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("redCollision1"));
	green1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("green1"));
	greenCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("greenCollision1"));
	blue1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("blue1"));
	blueCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("blueCollision1"));
	black1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("black1"));
	blackCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("blackCollision1"));
	//첫번째 컬러 버튼
	red2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("red2"));
	redCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("redCollision2"));
	green2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("green2"));
	greenCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("greenCollision2"));
	blue2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("blue2"));
	blueCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("blueCollision2"));
	black2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("black2"));
	blackCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("blackCollision2"));

	RootComponent = center;

	timeColorFoothold->SetupAttachment(RootComponent);
	timeColorFootholdCollision->SetupAttachment(timeColorFoothold);

	red1->SetupAttachment(RootComponent);
	green1->SetupAttachment(RootComponent);
	blue1->SetupAttachment(RootComponent);
	black1->SetupAttachment(RootComponent);

	redCollision1->SetupAttachment(red1);
	greenCollision1->SetupAttachment(green1);
	blueCollision1->SetupAttachment(blue1);
	blackCollision1->SetupAttachment(black1);

	red2->SetupAttachment(RootComponent);
	green2->SetupAttachment(RootComponent);
	blue2->SetupAttachment(RootComponent);
	black2->SetupAttachment(RootComponent);

	redCollision2->SetupAttachment(red2);
	greenCollision2->SetupAttachment(green2);
	blueCollision2->SetupAttachment(blue2);
	blackCollision2->SetupAttachment(black2);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FOOTHOLD(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (SM_FOOTHOLD.Succeeded()) {
		timeColorFoothold->SetStaticMesh(SM_FOOTHOLD.Object);
		red1->SetStaticMesh(SM_FOOTHOLD.Object);
		green1->SetStaticMesh(SM_FOOTHOLD.Object);
		blue1->SetStaticMesh(SM_FOOTHOLD.Object);
		black1->SetStaticMesh(SM_FOOTHOLD.Object);

		red2->SetStaticMesh(SM_FOOTHOLD.Object);
		green2->SetStaticMesh(SM_FOOTHOLD.Object);
		blue2->SetStaticMesh(SM_FOOTHOLD.Object);
		black2->SetStaticMesh(SM_FOOTHOLD.Object);
	}
}

// Called when the game starts or when spawned
void ACPP_Time_Color_Button::BeginPlay()
{
	Super::BeginPlay();

	/*UMaterialParameterCollection* MPC = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/material/MPC_baseColor.MPC_baseColor"));
	UMaterialParameterCollectionInstance* MyMPCInstance = GetWorld()->GetParameterCollectionInstance(MPC);
	MyMPCInstance->SetVectorParameterValue(FName("Color"), FVector(1.0f, 0.0f, 0.0f));*/

	red1->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.0f, 0.0f, 0.0f));
	green1->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 1.0f, 0.0f));
	blue1->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 1.0f));
	black1->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));
	
	red2->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.0f, 0.0f, 0.0f));
	green2->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 1.0f, 0.0f));
	blue2->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 1.0f));
	black2->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));

	GetWorldTimerManager().SetTimer(footholdColorChageTimer, this, &ACPP_Time_Color_Button::FootholdColorChageTimer, 3.0f, true);

}

// Called every frame
void ACPP_Time_Color_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Time_Color_Button::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	redCollision1->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Time_Color_Button::OnComponentBeginOverlap_redCollision);
	greenCollision1->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Time_Color_Button::OnComponentBeginOverlap_greenCollision);
	blueCollision1->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Time_Color_Button::OnComponentBeginOverlap_blueCollision);
	blackCollision1->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Time_Color_Button::OnComponentBeginOverlap_blackCollision);
	//버튼1과 버튼2가 호출하는 함수는 같다.
	redCollision2->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Time_Color_Button::OnComponentBeginOverlap_redCollision);
	greenCollision2->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Time_Color_Button::OnComponentBeginOverlap_greenCollision);
	blueCollision2->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Time_Color_Button::OnComponentBeginOverlap_blueCollision);
	blackCollision2->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Time_Color_Button::OnComponentBeginOverlap_blackCollision);

	timeColorFootholdCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Time_Color_Button::OnComponentBeginOverlap_timeColorFootholdCollision);

}

//버튼1의 충돌==========================================================================================
void ACPP_Time_Color_Button::OnComponentBeginOverlap_redCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	footholdColor.X = 1.0f;
	//이부분은 서버 통신 구현때 변경 예정
	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), footholdColor);
	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->SetColor(footholdColor);
}

void ACPP_Time_Color_Button::OnComponentBeginOverlap_greenCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	footholdColor.Y = 1.0f;

	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), footholdColor);
	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->SetColor(footholdColor);
}

void ACPP_Time_Color_Button::OnComponentBeginOverlap_blueCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	footholdColor.Z = 1.0f;

	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), footholdColor);
	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->SetColor(footholdColor);
}

void ACPP_Time_Color_Button::OnComponentBeginOverlap_blackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	footholdColor.X = 0.0f;
	footholdColor.Y = 0.0f;
	footholdColor.Z = 0.0f;

	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), footholdColor);
	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->SetColor(footholdColor);
}

void ACPP_Time_Color_Button::OnComponentBeginOverlap_timeColorFootholdCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Cobot* cobot = Cast<ACPP_Cobot>(OtherActor);
	if (cobot) {
		FVector cobotColor = cobot->GetColor();
		FTimerHandle WaitHandle;
		float WaitTime = 5.0;

		if (!cobotColor.Equals(FVector(0.0f, 1.0f, 1.0f)/*여기에 발판색을 넣는다.*/)) {
			//같지 않으면 콜리전을 없앤다.
			timeColorFoothold->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			timeColorFootholdCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{

					timeColorFoothold->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					timeColorFootholdCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

				}), WaitTime, false);
		}
	}
}

void ACPP_Time_Color_Button::FootholdColorChageTimer()
{
}


