#include <stdio.h>
#include <net/if.h>
#include <linux/can.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int setCanInterface(){
    const char *ifname;
    int s;
    //<net/if.h>, <linux/can.h>를 사용하는구나
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    printf("-> ");
    scanf("%s", ifname, sizeof(ifname));

    if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0){
        perror("Error While Opening Socket!");
        return -1;
    }
    else {
        printf("Can Socket Successfully created!\n");
    }

    //<string.h>, <sys/ioctl.h>
    strcpy(ifr.ifr_name, ifname);
    int ret;
    if((ret = ioctl(s, SIOCGIFINDEX, &ifr)) < 0){
        printf("Error While Retrieve CAN Interface Identifer!");
        return -1;
    }
    else{
        printf("Successfully Retrieved CAN Interface Identifer!\n");
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if((ret = bind(s, (struct sockaddr*)&addr, sizeof(addr))) < 0){
        printf("Error While Binding CAN Socket!");
        return -1;
    }
    else{
        printf("Successfully Binded CAN Socket!\n");
    }

    return s;
}

#define CAN_FRAME_MAX_LEN 8
#define CANID_List_MAX_LINES 5000
int canSniffing(const int s){
    struct can_frame frame;
    //scanf("%9s", listTest[i])
    char canIdList[CANID_List_MAX_LINES][4] = {0};

    
    int orderOfId = 0;

    //<stdbool.h>
    while(true){
        //<unistd.h>
        int rx_bytes;
        if((rx_bytes = read(s, &frame, sizeof(frame))) < 0){
            perror("Fail to receive can frame");
            return -1;
        } else if(rx_bytes < (int)sizeof(struct can_frame)){
            printf("Incomplete can frame is received - rx_bytes: %d\n", rx_bytes);
            return -1;
        } else if(frame.can_dlc > CAN_FRAME_MAX_LEN){
            printf("Invalid dlc: %u\n", frame.can_dlc);
            return -1;
        }


        if(((frame.can_id >> 29) & 1) == 1){
            printf("Error frame is received\n");
        } else if(((frame.can_id >> 30) & 1) == 1){
            printf("RTR frame is received\n");
        } else {
            char frame_can_id_str[4];
            sprintf(frame_can_id_str, "%X", frame.can_id);

            int isOverlapped;
            int overlappedIndex;
            int i;
            //isOverlapped?
            for(i = 0; i < CANID_List_MAX_LINES; i++){
                //Not Overlapped
                if((strcmp(canIdList[i], frame_can_id_str)) != 0){
                    isOverlapped = 0;
                }

                //Overlapped
                else{
                    isOverlapped = 1;
                    overlappedIndex = i;
                    break;
                }
            }

            //NotOverLapped
            if(isOverlapped == 0){
                //Change the font color using ANSI escape code
                printf("\033[1;31m| %3X |\033[0m", frame.can_id);
                printf("\033[1;36m %X\033[0m", frame.can_dlc);
                printf(" ");
                for (int i = 0; i < CAN_FRAME_MAX_LEN - 1; ++i){
                    printf("%02X ", frame.data[i]);
                }
                printf("\n");

                strcpy(canIdList[orderOfId], frame_can_id_str);
                orderOfId += 1;

                for(int i = 0; i < CANID_List_MAX_LINES; i++){
                    printf("%s ", canIdList[i]);
                }
                printf("\n");
            }

            //OverLapped
            else{
                printf("@@@@@@@@@@@@@@@@@@@@@@OVERLAPPED! YOU SHOULD HANDLE THIS.. %s, %s @@@@@@@@@@@@@@@@@@@@@@", canIdList[overlappedIndex], frame_can_id_str);
                printf("\033[1;31m| %3X |\033[0m", frame.can_id);
                printf("\033[1;36m %X\033[0m", frame.can_dlc);
                printf(" ");
                for (int i = 0; i < CAN_FRAME_MAX_LEN - 1; ++i){
                    printf("%02X ", frame.data[i]);
                }
                printf("\n");
            }
            
        }
    }
    
    return 0;
}

int main(){

    //@@@@@startInterface@@@@@
    printf("\n");
    printf("\033[1;31m ██████╗ █████╗ ███╗   ██╗\033[0m █████╗ ██╗  ██╗   ██╗███████╗███████╗\n");
    printf("\033[1;31m██╔════╝██╔══██╗████╗  ██║\033[0m██╔══██╗██║  ╚██╗ ██╔╝╚══███╔╝██╔════╝\n");
    printf("\033[1;31m██║     ███████║██╔██╗ ██║\033[0m███████║██║   ╚████╔╝   ███╔╝ █████╗  \n");
    printf("\033[1;31m██║     ██╔══██║██║╚██╗██║\033[0m██╔══██║██║    ╚██╔╝   ███╔╝  ██╔══╝  \n");
    printf("\033[1;31m╚██████╗██║  ██║██║ ╚████║\033[0m██║  ██║███████╗██║   ███████╗███████╗\n");
    printf("\033[1;31m ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝\033[0m╚═╝  ╚═╝╚══════╝╚═╝   ╚══════╝╚══════╝\n");
    printf("                                                    ©2024 sundaeGAN\n");
    printf("                                    Contact: minjunkim134@gmail.com\n");
    printf("\n");
    printf("  Welcome to CANalyze!\n");
    printf("\n");
    printf("\n");
    printf("\n");

    //@@@@@Network Interface setting@@@@@
    int socket;
    printf("\033[1;33mSet your CAN network Interface.\033[0m\n");
    socket = setCanInterface();

    //@@@@@Choose the option@@@@@
    int choosing;
    printf("\n\033[0;33m[1]\033[0m Packet Sniffing\n");
    // -- many other options.. --

    printf("-> ");
    scanf("%d", &choosing);
    
    //@@@@@Packet Sniffing@@@@@
    if (choosing == 1){
        canSniffing(socket);
    }

    return 0;
}
