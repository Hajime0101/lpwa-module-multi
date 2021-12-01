/*
 * control_cmd.c
 *
 *  Created on: Aug 8, 2021
 *      Author: kazuaki
 */

// LPWA制御用コマンドの処理
// メッセージ "!Cxxx..." の処理を行う

#include "control_cmd.h"

#include "main.h"
#include "console.h"
#include "command.h"
#include "eeprom.h"
#include "packet.h"


extern int LEDCounterLED2;

#define DIGIT(c) ((c)-'0')

// !C000<n>
// 制御のテスト用
// n秒間、LEDを点灯する
static void ControlCommand000(const uint8_t *param)
{
	int t = DIGIT(param[0]);
	if( t>0 && t<=9 ){
		LEDCounterLED2 = DIGIT(param[0]) * 10;
	}
}


// !C002
// 死活監視機能
// バッテリー残量を求め、!S002 のメッセージをHQへ送信する
static void ControlCommand002(const uint8_t *param)
{
	// バッテリー残量を求める
	// TODO: （未実装）
	uint16_t adc = 1023;
	// メッセージを作成し、UPLINKとして送信する
	// メッセージ長は、電圧4+バージョン8+UPLINK6
	uint8_t msg[40];
	sprintf((char *)msg, "\"!S002%04X%s%s%06X\"", adc, MEMORY_MAGIC_STR, MEMORY_VERSION_STR, (unsigned int)ConfigReadUplinkID());
	command_uplink(msg);
}



// 制御コマンドを受け取って、その内容を処理する
// コマンドの先頭（ cmd[0] ）は "!" である
// !C000 の形式である
void ConrtolCommandExecute(const uint8_t *cmd)
{
	// 念の為、エラーチェック
	if( cmd[0] != '!' ) return;
	if( cmd[1] != 'C' ) return;
	// コマンド番号を求める, コマンドは 0〜FFF の範囲
	int cmd_no = (DIGIT(cmd[2]) * 16 + DIGIT(cmd[3])) * 16 + DIGIT(cmd[4]);
	if( cmd_no < 0 || cmd_no > 0xfff ) return;
	//
	SerialOutput("[%d]", cmd_no);
	const uint8_t *param = cmd + 5;
	switch( cmd_no ){
	case 0x000:  // !C000
		ControlCommand000(param);
	   break;
	case 0x002:  // !C002
		ControlCommand002(param);
	   break;
    default:
	// 何もしない
		break;
	}
}



