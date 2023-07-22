// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_StartWidget.h"


//UCPP_StartWidget::UCPP_StartWidget()
//
//{
//	
//}

void UCPP_StartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(LogTemp, Warning, TEXT("startwidget()"));

	instance = Cast<UCPP_CobotGameInstance>(GetWorld()->GetGameInstance());
	sock = instance->GetSocketMgr()->GetSocket();
}

bool UCPP_StartWidget::SendLoginIDPW()
{
	//서버! ID, Passward->이거 서버로 보내서 로그인 여부확인
	//서버에서 받은거를 리턴값으로 넘겨준다.
	UE_LOG(LogTemp, Warning, TEXT("SendLoginIDPW()"));

	u_long BlockingMode = 0;
	ioctlsocket(*sock, FIONBIO, &BlockingMode); // sock을 논블로킹 모드로 설정

	wchar_t* id = TCHAR_TO_WCHAR(*ID);
	wchar_t* pw = TCHAR_TO_WCHAR(*Passward);

	// 서버한테 들어왔다고 알려주는 거
	cs_login_packet login_pack;
	login_pack.size = sizeof(login_pack);
	login_pack.type = static_cast<char>(packet_type::cs_login);
	wcscpy_s(login_pack.id, MAX_NAME, id);
	wcscpy_s(login_pack.passward, MAX_NAME, pw);

	int ret = send(*sock, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);

	char buff[BUF_SIZE];
	ret = recv(*sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);

	u_long nonBlockingMode = 1;
	ioctlsocket(*sock, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정

	if (ret != buff[0]) {
		UE_LOG(LogTemp, Warning, TEXT("%d start widget signup recv err"), ret);
		return false;
	}

	switch (buff[1])
	{
	case static_cast<int>(packet_type::sc_login_fail): 
		UE_LOG(LogTemp, Warning, TEXT("recv login fail"));
		return false;
	case static_cast<int>(packet_type::sc_login_success): 
		UE_LOG(LogTemp, Warning, TEXT("recv login success"));
		return true;
	}

	return false;
}

//bool UCPP_StartWidget::CallEventSuccessLogin(bool loginResult)
//{
//	isSuccessLogin = loginResult;
//
//	if(isSuccessLogin)
//		Cast<ACPP_Cobot_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->SetWidget(nullptr); //로그인이 성공하면 현재 위젯을 nullptr로 바꾼다.
//
//	FOutputDeviceNull pAR;
//	CallFunctionByNameWithArguments(TEXT("Success_Login"), pAR, nullptr, true);
//}

bool UCPP_StartWidget::SendSingupIDPW()
{
	//서버! signupID, signupPassward->이거 서버로 보내서 로그인 여부확인
	//서버에서 받은거를 리턴값으로 넘겨준다.
	UE_LOG(LogTemp, Warning, TEXT("SendSignupIDPW()"));

	u_long BlockingMode = 0;
	ioctlsocket(*sock, FIONBIO, &BlockingMode); // sock을 논블로킹 모드로 설정

	wchar_t* id = TCHAR_TO_WCHAR(*signupID);
	wchar_t* pw = TCHAR_TO_WCHAR(*signupPassward);

	// 서버한테 들어왔다고 알려주는 거
	cs_login_packet login_pack;
	login_pack.size = sizeof(login_pack);
	login_pack.type = static_cast<char>(packet_type::cs_signup);
	wcscpy_s(login_pack.id, MAX_NAME, id);
	wcscpy_s(login_pack.passward, MAX_NAME, pw);

	int ret = send(*sock, reinterpret_cast<char*>(&login_pack), sizeof(login_pack), 0);

	char buff[BUF_SIZE];
	ret = recv(*sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);

	u_long nonBlockingMode = 1;
	ioctlsocket(*sock, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정

	if (ret != buff[0]) {
		UE_LOG(LogTemp, Warning, TEXT("start widget signup recv err"));
		return false;
	}

	switch (buff[1])
	{
	case static_cast<int>(packet_type::sc_signup_fail): return false;
	case static_cast<int>(packet_type::sc_signup_success): return true;
	}

	return false;
}

//void UCPP_StartWidget::CallEventSuccessSignup(bool signupResult)
//{
//	isSuccessSignup = signupResult; //bool값 설정하고 커스텀이벤트 호출
//
//	FOutputDeviceNull pAR;
//	CallFunctionByNameWithArguments(TEXT("Success_Signup"), pAR, nullptr, true);
//}

void UCPP_StartWidget::CreateRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateAndWait()"));

	wchar_t* room_name = TCHAR_TO_WCHAR(*roomName);

	cs_create_room_packet pack;
	pack.size = sizeof(pack);
	pack.type = static_cast<char>(packet_type::cs_create_room);
	wcscpy_s(pack.room_name, MAX_NAME, room_name);
	pack.room_mode = roomMode;

	//서버에 방이름과 스테이지 번호를 보낸다. (room_name, stageNum)
	int ret = send(*sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);
	if (ret <= 0) UE_LOG(LogTemp, Warning, TEXT("CreateRoom() send err"));

	u_long BlockingMode = 0;
	ioctlsocket(*sock, FIONBIO, &BlockingMode); // sock을 블로킹 모드로 설정

	char buff[BUF_SIZE];
	ret = recv(*sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);

	u_long nonBlockingMode = 1;
	ioctlsocket(*sock, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정

	if (ret != buff[0])
		UE_LOG(LogTemp, Warning, TEXT("create room recv err"));

	switch (buff[1])
	{
	case static_cast<int>(packet_type::sc_create_room_ok):
	{
		sc_create_room_ok_packet* recv_pack = reinterpret_cast<sc_create_room_ok_packet*>(&buff);
		roomID = recv_pack->room_id;
		roomName = WCHAR_TO_TCHAR(recv_pack->room_name);
		roomMode = recv_pack->room_mode;

		FOutputDeviceNull pAR;
		CallFunctionByNameWithArguments(TEXT("wait"), pAR, nullptr, true);
	} break;
	}
}

void UCPP_StartWidget::CallEventSuccessAddRoom(FString name, int mode, int id)
{
	roomName = name;
	roomMode = mode;
	roomID = id;

	FOutputDeviceNull pAR;
	CallFunctionByNameWithArguments(TEXT("add_Room"), pAR, nullptr, true);
}

void UCPP_StartWidget::NormalModeRefresh()
{
	UE_LOG(LogTemp, Warning, TEXT("normal mode refresh()"));
	//서버에 데이터 요청 요청하는 패킷 보내기
	//서버에 방이름이랑 스테이지 번호를 보낸다.
	cs_show_room_list_packet send_pack;
	send_pack.size = sizeof(send_pack);
	send_pack.type = static_cast<char>(packet_type::cs_show_room_list);
	send_pack.room_mode = 0;

	int ret = send(*sock, reinterpret_cast<char*>(&send_pack), sizeof(send_pack), 0);

	//계속 대기
	u_long BlockingMode = 0;
	ioctlsocket(*sock, FIONBIO, &BlockingMode); // sock을 블로킹 모드로 설정

	while (true)
	{
		char buff[BUF_SIZE];
		ret = recv(*sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);

		if (ret != buff[0])
			UE_LOG(LogTemp, Warning, TEXT("normal mode refresh recv err"));

		switch (buff[1])
		{
		case static_cast<int>(packet_type::sc_show_room_list):
		{
			sc_show_room_list_packet* recv_pack = reinterpret_cast<sc_show_room_list_packet*>(&buff);

			roomName = WCHAR_TO_TCHAR(recv_pack->room_name);
			userName = WCHAR_TO_TCHAR(recv_pack->host_name);
			roomID = recv_pack->room_id;
			//stageNum = recv_pack->stage;

			FOutputDeviceNull pAR;
			CallFunctionByNameWithArguments(TEXT("show_Room"), pAR, nullptr, true);
		} break;
		case static_cast<int>(packet_type::sc_show_room_list_end): 
			UE_LOG(LogTemp, Warning, TEXT("show normal list end"));

			u_long nonBlockingMode = 1;
			ioctlsocket(*sock, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정

			return;
		}
	}

	u_long nonBlockingMode = 1;
	ioctlsocket(*sock, FIONBIO, &nonBlockingMode); // sock을 논블로킹 모드로 설정
}

void UCPP_StartWidget::PlayGame(int roomId)
{
	//서버에 해당 아이디를 가지는 방 게임을 시작하라는 send한다
	// 방장과 나머지 플레이어 모두 이곳을 지나면 서버는 게임을 시작하라는 패킷을 보낸다.
	// 방장은 나머지 플레이어가 들어오기 전에 여기서 계속 대기한다.
	//해당 패킷이 잘 도착하면 true를 리턴 아니면 false를 리턴

	if (!prev_enter) {
		cs_enter_room_packet pack;
		pack.size = sizeof(pack);
		pack.type = static_cast<char>(packet_type::cs_enter_room);
		pack.room_id = roomId;
		pack.room_mode = 0;

		int ret = send(*sock, reinterpret_cast<char*>(&pack), sizeof(pack), 0);

		if (ret != -1) {
			UE_LOG(LogTemp, Warning, TEXT("send room enter packet"));
			prev_enter = true;
		} else if (-1 == ret) {
			UE_LOG(LogTemp, Warning, TEXT("send enter room packet err"));
		} else {
			UE_LOG(LogTemp, Warning, TEXT("enter room packet err ret: %d"), ret);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("call recv()"));
	char buff[BUF_SIZE];
	int ret = recv(*sock, reinterpret_cast<char*>(&buff), BUF_SIZE, 0);
	UE_LOG(LogTemp, Warning, TEXT("ret: %d, game start packet size: %d"), ret, sizeof(sc_game_start_packet));

	if (-1 == ret) {

		FTimerHandle waitTimer;
		GetWorld()->GetTimerManager().SetTimer(waitTimer, FTimerDelegate::CreateLambda([&]()
			{
				PlayGame(roomId);
				UE_LOG(LogTemp, Warning, TEXT("Timer"));

			}), 1.f, false);

	} else if (buff[0] == ret) {
		UE_LOG(LogTemp, Warning, TEXT("recv game start packet"));
		
		sc_game_start_packet* pack = reinterpret_cast<sc_game_start_packet*>(&buff);

		// stageNum = pack->stage; // 여기에 시작해야 하는 스테이지가 담겨있다.

		FOutputDeviceNull pAR;
		CallFunctionByNameWithArguments(TEXT("open_lavel"), pAR, nullptr, true);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("errerrerr ret: %d"), ret);
	}
}

