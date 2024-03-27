import subprocess

# Read mem from Start Address first
command = 'echo "' + "23 14 C3 F8 00 00 FF" + '" | isotpsend -p 00 -s 7E0 -d 7E8 vcan0'
subprocess.run(command, shell=True)

# Start Address
addr = hex(0xC3F80000)

while True:
    sendAddr_list = []
    
    addr = str(hex(int(addr, 16) + 0xFF))

    for i in range(2, len(addr), 2):
        sendAddr_list.append(addr[i:i+2])
        
    sendAddr = ' '.join(sendAddr_list)
    print(sendAddr)
    
    command = 'echo "' + "23 14 " + sendAddr + " FF" + '" | isotpsend -p 00 -s 7E0 -d 7E8 vcan0'
    subprocess.run(command, shell=True)
    
    
    
