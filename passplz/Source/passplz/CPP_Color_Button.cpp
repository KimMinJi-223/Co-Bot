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

	redFoothold = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("redFoothold"));
	cyanFoothold = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cyanFoothold"));
	redFootholdCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("redFootholdCollision"));
	cyanFootholdCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("cyanFootholdCollision"));

	respawnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("respawnCollision"));

	RootComponent = center;

	red->SetupAttachment(RootComponent);
	green->SetupAttachment(RootComponent);
	blue->SetupAttachment(RootComponent);
	black->SetupAttachment(RootComponent);
	redFoothold->SetupAttachment(RootComponent);
	cyanFoothold->SetupAttachment(RootComponent);

	redCollision->SetupAttachment(red);
	greenCollision->SetupAttachment(green);
	blueCollision->SetupAttachment(blue);
	blackCollision->SetupAttachment(black);
	redFootholdCollision->SetupAttachment(redFoothold);
	cyanFootholdCollision->SetupAttachment(cyanFoothold);

	respawnCollision->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTTON(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SM_BUTTON.Succeeded()) {
		red->SetStaticMesh(SM_BUTTON.Object);
		green->SetStaticMesh(SM_BUTTON.Object);
		blue->SetStaticMesh(SM_BUTTON.Object);
		black->SetStaticMesh(SM_BUTTON.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FOOTHOLD(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (SM_FOOTHOLD.Succeeded()) {
		redFoothold->SetStaticMesh(SM_FOOTHOLD.Object);
		cyanFoothold->SetStaticMesh(SM_FOOTHOLD.Object);
	}

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
	redFoothold->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.0f, 0.0f, 0.0f));
	cyanFoothold->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 1.0f, 1.0f));
}

void ACPP_Color_Button::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	redCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_redCollision);
	greenCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_greenCollision);
	blueCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_blueCollision);
	blackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_blackCollision);
	redFootholdCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_redFootholdCollision);
	cyanFootholdCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_cyanCollisionCollision);
	respawnCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Color_Button::OnComponentBeginOverlap_respawnCollision);

}

// Called every frame
void ACPP_Color_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Color_Button::CobotButtonSend(packet_type type)
{
	SOCKET* sock = Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetSocket();

	cs_button_packet button_pack;
	button_pack.size = sizeof(button_pack);
	button_pack.type = static_cast<char>(type);
	UE_LOG(LogTemp, Warning, TEXT("CobotButtonSend"));
	send(*sock, reinterpret_cast<char*>(&button_pack), sizeof(button_pack), 0);
}

//버튼 충돌===================================================
void ACPP_Color_Button::OnComponentBeginOverlap_redCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("red_button"));
	CobotButtonSend(packet_type::cs_push_button_cobot_red);
}

void ACPP_Color_Button::OnComponentBeginOverlap_greenCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("green_button"));
	CobotButtonSend(packet_type::cs_push_button_cobot_green);
}

void ACPP_Color_Button::OnComponentBeginOverlap_blueCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("blue_button"));
	CobotButtonSend(packet_type::cs_push_button_cobot_blue);
}

void ACPP_Color_Button::OnComponentBeginOverlap_blackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("black_button"));
	CobotButtonSend(packet_type::cs_push_button_cobot_black);
}

//코봇이 발판과 충돌했을때 컬러를 확인해야한다.
void ACPP_Color_Button::OnComponentBeginOverlap_redFootholdCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Cobot* cobot = Cast<ACPP_Cobot>(OtherActor);
	if (cobot) {

		FVector cobotColor = cobot->GetColor();
		FTimerHandle WaitHandle;
		float WaitTime = 5.0;

		if (!cobotColor.Equals(FVector(1.0f, 0.0f, 0.0f))) {
			//같지 않으면 콜리전을 없앤다.
			redFoothold->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			redFootholdCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{

					redFoothold->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					redFootholdCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


				}), WaitTime, false);
		}
	}
}

void ACPP_Color_Button::OnComponentBeginOverlap_cyanCollisionCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ACPP_Cobot* cobot = Cast<ACPP_Cobot>(OtherActor);
	if (cobot) {
		FVector cobotColor = cobot->GetColor();
		FTimerHandle WaitHandle;
		float WaitTime = 5.0;

		if (!cobotColor.Equals(FVector(0.0f, 1.0f, 1.0f))) {
			//같지 않으면 콜리전을 없앤다.
			cyanFoothold->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			cyanFootholdCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{

					cyanFoothold->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					cyanFootholdCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

				}), WaitTime, false);
		}
	}
}

//리스폰
void ACPP_Color_Button::OnComponentBeginOverlap_respawnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Cobot* cobot = Cast<ACPP_Cobot>(OtherActor);
	if (cobot) {
		cobot->SetActorLocation(GetActorLocation());
	}
}

