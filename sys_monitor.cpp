#include "sys_monitor.h"

void cpu_usage(long double *cpu_usage_percent)
{
    long double a[4], b[4], loadavg;
    FILE *fp;

    //CPU usage
    fp = fopen("/proc/stat","r");
    fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&a[0],&a[1],&a[2],&a[3]);
    fclose(fp);
    sleep(1);//delay

    fp = fopen("/proc/stat","r");
    fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&b[0],&b[1],&b[2],&b[3]);
    fclose(fp);

    *cpu_usage_percent = (((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3])))*100;
    printf("The current CPU utilization is : %Lf %% \n",*cpu_usage_percent);

}

string pci_stats(void)
{
        FILE *fp;
        char line[100];
        //Xilinx board detector
        system("lspci | grep Xilinx > lspci.txt");

        fp = fopen("lspci.txt","r");
        while (!feof (fp)) {
            if (fgets(line, sizeof(line), fp)) {
                printf("lspci= %s\n", line);
            }
        }
      
        fclose(fp);
        
        return line;
}

//for testing
// int main(){

//     long double cpu_usage_p;
//     // char *boards;
//     string boards;
    
//     while(1){
//         cpu_usage(&cpu_usage_p);
//         boards = pci_stats();
        
//     }
//     return 0;
// }

