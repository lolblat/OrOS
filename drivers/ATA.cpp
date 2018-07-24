//
// Created by lolblat on 08/07/18.
//
#include "ATA.h"

ATA::ATA(ATABus bus, ATAType type)
{
  SetPorts(bus, type);
}

void ATA::SelectDrive()
{
    if(m_ata_type == Master)
    {
        m_drive_head.port_byte_out(ATA_IDENTIFY_MASTER_CODE);
    }
    else
    {
        m_drive_head.port_byte_out(ATA_IDENTIFY_SLAVE_CODE);
    }
}

u8* ATA::ATAIdentify()
{
    SelectDrive();
    u32 io_port = 0;
    if(m_ata_bus == Primary)
    {
        io_port = ATA_PRIMARY_IO_BASE_PORT;
    }
    else
    {
        io_port = ATA_SECONDARY_IO_BASE_PORT;
    }

    m_sector_count.port_byte_out(0);
    m_sector_number.port_byte_out(0);
    m_cylinder_low.port_byte_out(0);
    m_cylinder_high.port_byte_out(0);

    m_status_command.port_byte_out(ATA_COMMAND_IDENTIFY);

    u8 status = m_status_command.port_byte_in();

    if(status)
    {
        while((m_status_command.port_byte_in() & ATA_STATUS_BIT_BUSY) != 0);
        status = m_status_command.port_byte_in();
        while((status & ATA_STATUS_BIT_ERR != 1) && (status & ATA_STATUS_BIT_SET_WHEN_DATA_OR_READY != 1))
        {
            status = m_status_command.port_byte_in();
        }
        if(status & ATA_STATUS_BIT_ERR)
        {
            return (u8*)0;
        }
        u8* buffer = (u8*)MemoryManager::GetInstance()->kmalloc(512);
        for(u32 i = 0; i < 256; i ++)
        {
            *(u16*)(buffer + i*2) = m_data.port_word_in();
        }
        return buffer;
    }
    else
    {
        return (u8*)0;
    }

}

void ATA::ATAReadSector(u32 lba_address)
{
    m_drive_head.port_byte_out((m_ata_type == Master ? ATA_READ_MASTER : ATA_READ_SLAVE) | ((lba_address >> 24) & 0x0F));
    m_sector_count.port_byte_out(1);
    m_sector_number.port_byte_out((u8)lba_address);
    m_cylinder_low.port_byte_out((u8)(lba_address >> 8));
    m_cylinder_high.port_byte_out((u8)(lba_address >> 16));
    m_status_command.port_byte_out(ATA_COMMAND_READ_SECTORS);
    m_state = s_Read;
}

void ATA::ATADelay()
{
    m_state = s_Delay;
    for(u32 i = 0; i < 4; i++) // four time read the status reg, 100ns for each read.
    {
        m_alt_status.port_byte_in();
    }
}
bool ATA::ATAPoll()
{
    if(m_state == s_Delay)
    {
        return false;
    }

    m_state = s_Read;
    u8 status = m_status_command.port_byte_in();
    if(status & (0x1 << ATA_STATUS_BIT_SET_WHEN_DATA_OR_READY) && (!(status & (0x1 << ATA_STATUS_BIT_BUSY))))
    {
        for(u32 i = 0; i < 256; i++)
        {
            u16 data = m_data.port_word_in();
            *(u16*)((u8*)m_read_result.data + m_read_result.index_of_sector * ATA_SECTOR_SIZE + i*2) = data;
        }
        m_read_result.index_of_sector += 1;
        return true;
    }
    else
    {
        if(status & (0x1 << ATA_STATUS_BIT_ERR))
        {
            Util::printf("[E] Error at ATA !\n");
        }
    }
    return false;

}


void ATA::SetPorts(ATABus bus, ATAType type)
{
    m_ata_bus = bus;
    m_ata_type = type;
    if(bus == Primary)
    {
        m_data.SetPort(ATA_PRIMARY_IO_BASE_PORT + ATA_REG_DATA_OFFSET);
        m_features_error.SetPort(ATA_PRIMARY_IO_BASE_PORT + ATA_REG_ERROR_OFFSET);
        m_sector_count.SetPort(ATA_PRIMARY_IO_BASE_PORT + ATA_REG_SECTOR_COUNT_OFFSET);
        m_sector_number.SetPort(ATA_PRIMARY_IO_BASE_PORT + ATA_REG_SECTOR_NUMBER_OFFSET);
        m_cylinder_low.SetPort(ATA_PRIMARY_IO_BASE_PORT + ATA_REG_CYLINDER_LOW_OFFSET);
        m_cylinder_high.SetPort(ATA_PRIMARY_IO_BASE_PORT + ATA_REG_CYLINDER_HIGH_OFFSET);
        m_drive_head.SetPort(ATA_PRIMARY_IO_BASE_PORT + ATA_REG_DRIVE_HEAD_OFFSET);
        m_status_command.SetPort(ATA_PRIMARY_IO_BASE_PORT + ATA_REG_STATUS_OFFSET);
        m_alt_status.SetPort(ATA_PRIMARY_IO_BASE_PORT + ATA_REG_ALT_STATUS_OFFSET);
    }
    else
    {
        m_data.SetPort(ATA_SECONDARY_IO_BASE_PORT + ATA_REG_DATA_OFFSET);
        m_features_error.SetPort(ATA_SECONDARY_IO_BASE_PORT + ATA_REG_ERROR_OFFSET);
        m_sector_count.SetPort(ATA_SECONDARY_IO_BASE_PORT + ATA_REG_SECTOR_COUNT_OFFSET);
        m_sector_number.SetPort(ATA_SECONDARY_IO_BASE_PORT + ATA_REG_SECTOR_NUMBER_OFFSET);
        m_cylinder_low.SetPort(ATA_SECONDARY_IO_BASE_PORT + ATA_REG_CYLINDER_LOW_OFFSET);
        m_cylinder_high.SetPort(ATA_SECONDARY_IO_BASE_PORT + ATA_REG_CYLINDER_HIGH_OFFSET);
        m_drive_head.SetPort(ATA_SECONDARY_IO_BASE_PORT + ATA_REG_DRIVE_HEAD_OFFSET);
        m_status_command.SetPort(ATA_SECONDARY_IO_BASE_PORT + ATA_REG_STATUS_OFFSET);
        m_alt_status.SetPort(ATA_SECONDARY_IO_BASE_PORT + ATA_REG_ALT_STATUS_OFFSET);
    }
    m_state = s_Idle;
}

u8* ATA::ATARead(u32 lba_address, u32 number_of_sectors)
{

    m_read_result.data = (u8*)MemoryManager::GetInstance()->kmalloc(number_of_sectors * ATA_SECTOR_SIZE);
    m_read_result.index_of_sector = 0;
    for(u32 i = 0; i < number_of_sectors; i ++)
    {
        ATADelay();
        ATAReadSector(lba_address + i);
        ATAPoll();
    }
    return this->GetReadResult();
}

u8* ATA::GetReadResult()
{
    return m_read_result.data;
}
void ATA::ATAWriteSector(u8 *data, u32 lba_address, u32 size)
{
    u8* replaced_sector = ATARead(lba_address,1);
    u8* res = data;
    if(size != ATA_SECTOR_SIZE) // need to fill up
    {
        res = (u8*)MemoryManager::GetInstance()->kmalloc(ATA_SECTOR_SIZE); // first create a sector size buffer.
        Util::memcopy(data, res, size);
        Util::memcopy(replaced_sector, res + size, ATA_SECTOR_SIZE - size);
    }
    m_drive_head.port_byte_out((m_ata_type == Master ? ATA_READ_MASTER : ATA_READ_SLAVE) | ((lba_address >> 24) & 0x0F)); // select drive;
    u32 number_of_transfers = (ATA_SECTOR_SIZE)/(sizeof(u16));
    m_sector_count.port_byte_out(1);
    m_sector_number.port_byte_out((u8)lba_address);
    m_cylinder_low.port_byte_out((u8)lba_address >> 8);
    m_cylinder_high.port_byte_out((u8)lba_address >> 16);

    m_status_command.port_byte_out(ATA_COMMAND_WRITE_SECTORS);

    for(u32 i = 0; i < number_of_transfers; i++, res += 2)
    {
        u16 short_data = *(u16*)res;
        m_data.port_word_out(short_data);
        for(u32 k = 0; k < ATA_WRITE_DELAY; k++);
    }

    m_status_command.port_byte_out(ATA_COMMAND_CACHE_FLUSH);
}

void ATA::ATAWriteSectors(u8 *data, u32 lba_address, u32 size)
{
    while(size > 0)
    {
        if(size > ATA_SECTOR_SIZE)
        {
            ATAWriteSector(data,lba_address,ATA_SECTOR_SIZE);
            data += ATA_SECTOR_SIZE;
            lba_address += 1;
            size -= ATA_SECTOR_SIZE;
        }
        else
        {
            ATAWriteSector(data, lba_address, size);
            break;
        }
    }

}