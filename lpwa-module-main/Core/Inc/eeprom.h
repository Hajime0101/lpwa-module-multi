/*
 * eeprom.h
 *
 *  Created on: 2021/06/07
 *      Author: kazuaki
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#define MEMORY_MAGIC_STR     "CMN/IGSS"
#define MEMORY_VERSION_STR   "20210829"

#define MEMOEY_BASEADDR      0x08080000
#define MEMORY_MAGIC         MEMOEY_BASEADDR+0x00	// 8bytes
#define MEMORY_DEVICE_ID     MEMOEY_BASEADDR+0x08	// 3bytes
#define MEMORY_UPLINK_ID     MEMOEY_BASEADDR+0x0C	// 3bytes
#define MEMORY_TX_REPEAT     MEMOEY_BASEADDR+0x10	// 1bytes

#define MEMORY_FEATURES      MEMOEY_BASEADDR+0x20   // 0020〜005F の 0x40 Bytes


// 機能番号の定義（ATFコマンド）
#define FCONF_LOWER_BOUNDARY 0x00
#define FCONF_MODULE_MODE    0x00   // 0:中継機, 1:ブリッジ
#define FCONF_TELEMETRY_ON   0x01   // 0:起動しない, 1以上:データ送信間隔
#define FCONF_KEEP_UPLINK    0x3F   // （開発者用）0:UPLINK変更あり, 1:UPLINK変更なし
#define FCONF_HIGHER_BOUNDARY 0x3F



void ConfigClearEEPROM(void);
void ConfigReadVersion(uint8_t product[]);

uint32_t ConfigReadDeviceID(void);
void ConfigWriteDeviceID(uint32_t id);

uint32_t ConfigReadUplinkID(void);
void ConfigWriteUplinkID(uint32_t id);

uint16_t ConfigReadSeq(void);
void ConfigWriteSeq(uint16_t seq);

uint8_t ConfigReadTxRepeat(void);
void ConfigWriteTxRepeat(uint8_t t);

uint8_t ConfigReadFeature(uint8_t no);
void ConfigWriteFeature(uint8_t no, uint8_t value);


#endif /* INC_EEPROM_H_ */
