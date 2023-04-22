// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Color_Button.h"
#include "CPP_Cobot.h"

// Sets default values
ACPP_Color_Button::ACPP_Color_Button()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	center = CreateDefaultSubobject<UArrowComponent>(TEXT("Center"));
	red = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("red"));
	redCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("redCollision"));
	green = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("green"));
	greenCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("greenCollision"));
	blue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("blue"));
	blueCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("blueCollision"));
	black = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("black"));
	blackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("blackCollision"));

	RootComponent = center;

	red->SetupAttachment(RootComponent);
	green->SetupAttachment(RootComponent);
	blue->SetupAttachment(RootComponent);
	black->SetupAttachment(RootComponent);
	redCollision->SetupAttachment(red);
	greenCollision->SetupAttachment(green);
	blueCollision->SetupAttachment(blue);
	blackCollision->SetupAttachment(black);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTTON(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SM_BUTTON.Succeeded()) {
		red->SetStaticMesh(SM_BUTTON.Object);
		green->SetStaticMesh(SM_BUTTON.Object);
		blue->SetStaticMesh(SM_BUTTON.Object);
		black->SetStaticMesh(SM_BUTTON.Object);
	}

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FOOTHOLD(TEXT("/Engine/BasicShapes/Cube.Cube"));
	//if (SM_FOOTHOLD.Succeeded()) {
	//	Foothold->SetStaticMesh(SM_FOOTHOLD.Object);
	//}

	color = FVector(0.f, 1.f, 0.f);
}

// Called when the game starts or when spawned
void ACPP_Color_Button::BeginPlay()
{
	Super::BeginPlay();
	red->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.0f, 0.0f, 0.0f)); 
	green->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 1.0f, 0.0f));
	blue->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 1.0f));
	black->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));

}

void ACPP_Color_Button::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	redCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_redCollision);
	greenCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_greenCollision);
	blueCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_blueCollision);
	blackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_blackCollision);
}

// Called every frame
void ACPP_Color_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Color_Button::OnComponentBeginOverlap_redCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	color.X = 1.0f;
	//이부분은 서버 통신 구현때 변경 예정
	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), color);
}

void ACPP_Color_Button::OnComponentBeginOverlap_greenCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	color.Y = 1.0f;
	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), color);
}

void ACPP_Color_Button::OnComponentBeginOverlap_blueCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	color.Z = 1.0f;
	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), color);
}

void ACPP_Color_Button::OnComponentBeginOverlap_blackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	color.X = 0.0f;
	color.Y = 0.0f;
	color.Z = 0.0f;
	(Cast<ACPP_Cobot>(GetWorld()->GetFirstPlayerController()->GetPawn()))->GetMesh()->SetVectorParameterValueOnMaterials(TEXT("cobot_color"), color);
}

