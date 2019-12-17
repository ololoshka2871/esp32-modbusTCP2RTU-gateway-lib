target remote :3333
monitor reset halt
monitor program_esp32 esp32-modbusTCP2RTU-gateway.bin 0x10000 verify exit
quit

