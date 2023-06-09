#include "GameEngineInput.h"
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineDebug.h>

// 편하게 사용하기 위해 static 
std::map<std::string, GameEngineInput::GameEngineKey> GameEngineInput::Keys;
bool GameEngineInput::IsAnyKeyValue = false;

void GameEngineInput::GameEngineKey::Update(float _DeltaTime)
{
	if (true == KeyCheck())
	{
		// 눌렸기 때문에 시간 누적
		PressTime += _DeltaTime;

		// 만약 키가 아무런 상태가 아니라면 ( 눌리지도, 눌려져 있지도 않음 ) 
		if (true == Free)
		{
			// 아무상태가 아닐 때 눌렸다면 이제 눌린 것
			Down = true;
			// 눌렸다면 눌린 상태인 것
			Press = true;
			// 키를 떼지 않았기 때문에 false 
			Up = false;
			// 현재 눌린상태이기 때문에 false 
			Free = false;
		}
		// 키가 눌렸을 때 Down 상태가 true 라면 이전 프레임에서도 눌려 있던 것이다. 
		else if (true == Down)
		{
			// 이번 프레임에서 눌린 것이 아니기 때문에 false
			Down = false;
			// 현재 눌린상태이기 때문에 true 
			Press = true;
			// 뗀 상태가 아니기 때문에 false 
			Up = false;
			// 눌린 상태이기 때문에 false 
			Free = false;
		}
	}

	// 키가 눌린 상태가 아니라면 
	else
	{
		// 눌린 시간 초기화
		PressTime = 0.0f;

		// 이전 프레임에 키가 눌려있던 상태라면 
		if (true == Press)
		{
			// 현재 키가 눌린 상태가 아니기 때문에 false 
			Down = false;
			// 마찬가지 
			Press = false;
			// 이전 프레임에 키가 눌려있었고 현재 눌린상태가 아니라면 키를 뗐다는 의미
			Up = true;
			Free = true;
		}

		// 키가 눌린 상태가 아닐때 현재 UP 이 true라면 
		else if (true == Up)
		{
			// 원래 아무런 상태도 아니었다는 의미 
			Down = false;
			Press = false;
			Up = false;
			Free = true;
		}
	}
}

// 문자열을 키값으로 하여 키를 생성했기 때문에
// 키가 눌렸다면 그 키에 해당하는 문자열을 확인하여 
// 동일한 문자열을 가진 키의 상태를 반환한다. 
bool GameEngineInput::IsDown(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	if (Keys.end() == Keys.find(UpperName))
	{
		MsgAssert("존재하지 않는 키를 사용하려고 했습니다." + UpperName);
	}

	// 문자열 키값에 해당 하는 GameEngineKey 클래스 의 변수의 값을 반환한다. 
	return Keys[UpperName].Down;
}
bool GameEngineInput::IsUp(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	if (Keys.end() == Keys.find(UpperName))
	{
		MsgAssert("존재하지 않는 키를 사용하려고 했습니다." + UpperName);
	}


	return Keys[UpperName].Up;
}
bool GameEngineInput::IsPress(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	if (Keys.end() == Keys.find(UpperName))
	{
		MsgAssert("존재하지 않는 키를 사용하려고 했습니다." + UpperName);
	}


	return Keys[UpperName].Press;
}
bool GameEngineInput::IsFree(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	if (Keys.end() == Keys.find(UpperName))
	{
		MsgAssert("존재하지 않는 키를 사용하려고 했습니다." + UpperName);
	}

	return Keys[UpperName].Free;
}

float GameEngineInput::GetPressTime(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	if (Keys.end() == Keys.find(UpperName))
	{
		MsgAssert("존재하지 않는 키를 사용하려고 했습니다." + UpperName);
	}

	return Keys[UpperName].PressTime;
}

bool GameEngineInput::IsKey(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	// 키가 end 가 아니라면 문자열 키값으로 저장되어 있는 것
	return Keys.end() != Keys.find(UpperName);
}

// 문자열을 키값으로 하여 원하는 키 생성
void GameEngineInput::CreateKey(const std::string_view& _Name, int _Key)
{
	// 키값을 대문자로 변환
	std::string UpperName = GameEngineString::ToUpper(_Name);

	// 동일한 키값으로 이미 저장되어있다면
	if (Keys.end() != Keys.find(UpperName))
	{
		// assert
		MsgAssert("이미 존재하는 이름의 키를 또 만들려고 했습니다.");
	}

	// 소문자를 대문자로 변환하여 반환 , 다른 문자는 그대로 반환
	_Key = toupper(_Key);

	// 인자로 들어온 문자열을 키값으로 하여 어떤 키를 저장한다.
	Keys[UpperName].Key = _Key;
}

/// GameEngineInput
GameEngineInput::GameEngineInput()
{
}

GameEngineInput::~GameEngineInput()
{
}

void GameEngineInput::Update(float _DeltaTime)
{
	// 현재 map에 저장되어 있는 키를 순회하며 업데이트 진행
	// 
	std::map<std::string, GameEngineKey>::iterator StartKeyIter = Keys.begin();
	std::map<std::string, GameEngineKey>::iterator EndKeyIter = Keys.end();

	for (; StartKeyIter != EndKeyIter; ++StartKeyIter)
	{
		StartKeyIter->second.Update(_DeltaTime);
	}
}