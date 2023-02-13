// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_StartWidget.h"
#include "CPP_Cobot_Controller.h"

bool UCPP_StartWidget::Is_Set_IDPW_Controller()
{
	return Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Is_Set_IDPW(ID, Passward);
	/*서버! 컨트롤러에서 서버에 send후 서버는 컨트롤러에게 bool값을 보내주고 컨트롤러가 이 함수에 bool값을 줘야함*/
}
