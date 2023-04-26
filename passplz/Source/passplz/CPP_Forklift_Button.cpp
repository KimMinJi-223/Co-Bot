// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Forklift_Button.h"
#include "CPP_Color_Forklift.h"

// Sets default values
ACPP_Forklift_Button::ACPP_Forklift_Button()
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

	red->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	green->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	blue->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));
	black->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));

	red->SetRelativeLocation(FVector(0.0f, 120.0f, 0.0f));
	green->SetRelativeLocation(FVector(0.0f, 40.0f, 0.0f));
	blue->SetRelativeLocation(FVector(0.0f, -40.0f, 0.0f));
	black->SetRelativeLocation(FVector(0.0f, -120.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTTON(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (SM_BUTTON.Succeeded()) {
		red->SetStaticMesh(SM_BUTTON.Object);
		green->SetStaticMesh(SM_BUTTON.Object);
		blue->SetStaticMesh(SM_BUTTON.Object);
		black->SetStaticMesh(SM_BUTTON.Object);
	}
}

// Called when the game starts or when spawned
void ACPP_Forklift_Button::BeginPlay()
{
	Super::BeginPlay();
	
	red->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.0f, 0.0f, 0.0f));
	green->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 1.0f, 0.0f));
	blue->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 1.0f));
	black->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.0f, 0.0f, 0.0f));
}

// Called every frame
void ACPP_Forklift_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Forklift_Button::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	redCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Forklift_Button::OnComponentBeginOverlap_redCollision);
	greenCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Forklift_Button::OnComponentBeginOverlap_greenCollision);
	blueCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Forklift_Button::OnComponentBeginOverlap_blueCollision);
	blackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Forklift_Button::OnComponentBeginOverlap_blackCollision);
}

void ACPP_Forklift_Button::OnComponentBeginOverlap_redCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OutActor = Cast<ACPP_Color_Forklift>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Color_Forklift::StaticClass()));
	FVector pillarColor = OutActor->GetPillarColor();
	pillarColor.X = 1.0;
	
	OutActor->SetPillarColor(pillarColor);
}

void ACPP_Forklift_Button::OnComponentBeginOverlap_greenCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OutActor = Cast<ACPP_Color_Forklift>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Color_Forklift::StaticClass()));
	FVector pillarColor = OutActor->GetPillarColor();
	pillarColor.Y = 1.0;
	OutActor->SetPillarColor(pillarColor);
}

void ACPP_Forklift_Button::OnComponentBeginOverlap_blueCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OutActor = Cast<ACPP_Color_Forklift>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Color_Forklift::StaticClass()));
	FVector pillarColor = OutActor->GetPillarColor();
	pillarColor.Z = 1.0;
	OutActor->SetPillarColor(pillarColor);
}

void ACPP_Forklift_Button::OnComponentBeginOverlap_blackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OutActor = Cast<ACPP_Color_Forklift>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Color_Forklift::StaticClass()));
	OutActor->SetPillarColor(FVector(0.f, 0.f, 0.f));
}
