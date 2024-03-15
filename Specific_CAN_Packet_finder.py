import subprocess, re, os

# 1. 사용자가 log 파일이름을 입력
logFileName = input("\nLog File : ")


while True:
    
    # 2. log를 반으로 (len 신경써야함 'wc')
    command = "wc -l " + logFileName
    result = str(subprocess.run(command, shell=True, stdout=subprocess.PIPE, text=True))
    result = re.search(r"stdout='(\d+)", result)
    result = int(result.group(1))
    if result == 1:
        print("\n\nDone!")
        specificLogfileName = input("Name your specific packet : ")
        command = "mv ./" + logFileName + " ./" + specificLogfileName
        subprocess.run(command, shell=True)
        
        print("\n\nOkay, the specific Packet you are looking for is -> " + specificLogfileName)
        print("\nGood Luck ;)")
        break

    divider = result // 2
    command = "split -l " + str(divider) + " " + logFileName
    subprocess.run(command, shell=True)

    # 3. 기존 log 파일 삭제
    command = "rm " + logFileName
    subprocess.run(command, shell=True)

    # 4. 모든 파일 실행
    file_list = os.listdir()
    file_list = [file for file in file_list if file.startswith("xa")]

    try :
        for subFile in file_list:
            while True:
                print("\n@@@Executing " + subFile + "...@@@")
                command = "canplayer -I " + subFile
                subprocess.run(command, shell=True)
                print("\nPut the car back to the original condition.")
                if input("\nNext or ReExecute (n or r) : ") == "n":
                    break;
                
    except : print("\n\nUnexpected Error =(")

    # 5. 사용자가 파일을 선택 (그 파일 이름 바꾸기)
    survivedFile = input("\nWhich file do you want to save? : ")
    logFileName = input("\nRename this file : ")
    command = "mv ./" + survivedFile + " ./" + logFileName
    subprocess.run(command, shell=True)

    #6. 선택한 파일 제외 다 삭제
    file_list = os.listdir()
    file_list = [file for file in file_list if file.startswith("xa")]

    for removeFile in file_list:
        command = "rm " + removeFile
        subprocess.run(command, shell=True)