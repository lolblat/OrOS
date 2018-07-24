//
// Created by lolblat on 08/07/18.
//

#ifndef OROS_ATA_H
#define OROS_ATA_H

#include "ATA.h"
#include "ports.h"
#include "../cpu/types.h"
#include "../MemoryAllocation/MemoryManager.h"
#include "../cpu/interrupts/isr.h"
using namespace drivers;

#define ATA_SECTOR_SIZE 512

#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

#define ATA_IDENTIFY_MASTER_CODE 0xA0
#define ATA_IDENTIFY_SLAVE_CODE 0xB0

#define ATA_PRIMARY_IO_BASE_PORT 0x1F0
#define ATA_PRIMARY_CONTROL_PORT 0x3F6

#define ATA_SECONDARY_IO_BASE_PORT 0x170
#define ATA_SECONDARY_IO_CONTROL_PORT 0x376

#define ATA_PRIMARY_IRQ_BUS 14
#define ATA_SECONDARY_IRQ_BUS 15

#define ATA_REG_DATA_OFFSET 0 // read or write pio data bytes
#define ATA_REG_ERROR_OFFSET 1 // read only, retrieve any error
#define ATA_REG_FEATURES_OFFSET 1 // write only control command specific interface features
#define ATA_REG_SECTOR_COUNT_OFFSET 2 // number of sectors to read / write
#define ATA_REG_SECTOR_NUMBER_OFFSET 3 // chs/lba28/lba48 specific
#define ATA_REG_CYLINDER_LOW_OFFSET 4 // partial disk sector address
#define ATA_REG_CYLINDER_HIGH_OFFSET 5 // partial disk sector address
#define ATA_REG_DRIVE_HEAD_OFFSET 6 // used to select a drive and/or a head
#define ATA_REG_STATUS_OFFSET 7 // read only used to read the current status
#define ATA_REG_COMMAND_OFFSET 7 // write only send ata commands to the device
#define ATA_REG_ALT_STATUS_OFFSET 0x0C


// Errors bits, all in error register, offset 1
#define ATA_ERR_BIT_ADDRESS_MARK_NOT_FOUND 0 //AMNF
#define ATA_ERR_BIT_TRACK_ZERO_NOT_FOUND 1 // TKZNF
#define ATA_ERR_BIT_ABORTED_COMMAND 2 // ABRT
#define ATA_ERR_BIT_MEDIA_CHANGE_REQUEST 3 // MCR
#define ATA_ERR_BIT_ID_NOT_FOUND 4 // IDNF
#define ATA_ERR_BIT_MEDIA_CHANGED 5 // MC
#define ATA_ERR_BIT_MEDIA_UNCORRECTABLE_DATA 6 // UNC
#define ATA_ERR_BIT_MEDIA_BAD_BLOCK 7 // BBK

//Drive head register, offset 6
// 0 - 3 bits 24 to 27 of the block number in LBA
#define ATA_DH_BIT_DRV 4
// bit 5 is 1 and bit 7
#define ATA_DH_BIT_LBA 6 // LBA addressing if set

#define ATA_STATUS_BIT_ERR 0
#define ATA_STATUS_BIT_INDEX 1 // always 0
#define ATA_STATUS_BIT_CORRECTED_DATA 2 // always 0
#define ATA_STATUS_BIT_SET_WHEN_DATA_OR_READY 3
#define ATA_STATUS_BIT_OVERLAPPED_MODE 4
#define ATA_STATUS_BIT_DRIVE_FALUT_ERROR 5
#define ATA_STATUS_BIT_SPUN_DOWN 6
#define ATA_STATUS_BIT_BUSY 7


#define ATA_COMMAND_IDENTIFY 0xEC
#define ATA_COMMAND_READ_SECTORS 0x20
#define ATA_COMMAND_WRITE_SECTORS 0x30
#define ATA_COMMAND_CACHE_FLUSH 0xE7
#define ATA_WRITE_DELAY 50
#define ATA_READ_MASTER 0xE0
#define ATA_READ_SLAVE 0xF0
enum ATABus
{
    Primary,
    Secondary
};

enum ATAType
{
    Master,
    Slave
};

enum ATADeviceState
{
    s_Read,
    s_Delay,
    s_Idle
};

struct ATAReadResult
{
    u8* data;
    u32 index_of_sector;
};
class ATA
{
private:
    Ports m_data,m_features_error,m_sector_count,m_sector_number,m_cylinder_low,m_cylinder_high,m_drive_head,m_status_command, m_alt_status;

    ATABus m_ata_bus;
    ATAType m_ata_type;
    ATADeviceState  m_state;
    ATAReadResult m_read_result;

    void SelectDrive();
    void ATADelay();
    void ATAReadSector(u32 lba_address);
    void ATAWriteSector(u8* data, u32 lba_address, u32 size);

public:
    ATA(ATABus, ATAType);
    ~ATA();
    void SetPorts(ATABus, ATAType);
    bool ATAPoll();

    u8* ATARead(u32 lba_address, u32 number_of_sectors);
    void ATAWriteSectors(u8* data, u32 lba_address, u32 size);
    u8* ATAIdentify();
    bool CheckIfError();
    u8* GetReadResult();
};


#endif //OROS_ATA_H
