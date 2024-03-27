import subprocess

while True:
    command = "echo '3E 80' | isotpsend -p 00 -s 7E0 -d 7E8 vcan0"
    subprocess.run(command, shell=True)