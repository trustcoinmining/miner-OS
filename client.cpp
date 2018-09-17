#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <jansson.h>
#include <fstream>

#include "src/TCPClient.h"
#include "sys_monitor.h"
#include "jsonparser.h"



TCPClient tcp;
#define MINER "miner"

using namespace std;

void sig_exit(int s)
{
	tcp.exit();
	exit(0);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sig_exit);
	char *message;
	long double cpu_usage_p;
	const char *value;
	const char *wallet;
	const char *pooladdress;
	const char *ncores;
	// map<string, string> device; 
	string str;
	int ndevices;
	// tcp.setup("127.0.0.1",11999);
	tcp.setup("0.tcp.ngrok.io",19988);


	while(1)
	{
		srand(time(NULL));
		// memset(message, 0, sizeof(message)); //clean message
		str = pci_stats();
		cpu_usage(&cpu_usage_p);

		// sleep(1);
		// tcp.Send(to_string(rand()%25000));
		tcp.Send(str);
		string rec = tcp.receive();
		if( rec != "" )
		{
			cout << "Server Response:" << rec << endl;
			const char * c = rec.c_str();
			json_t *root = load_json(c);

			if (root) {
				/* print and release the JSON structure */
				print_json(root);
				
				json_t *boo = json_object_get(root, "function");
				value = json_string_value(boo); //get value of an object based on its key
				// string val(value);
				printf("function : %s\n",value);
				
				//miner configuration
				if(strcmp(value,"miner") == 0){
					//get values
					json_t *json_wallet = json_object_get(root, "wallet");
					json_t *json_pooladdr = json_object_get(root, "pool");
					json_t *json_ncores = json_object_get(root, "ncores");
					json_t *json_ndevices = json_object_get(root, "ndevices");
					json_t *json_devicepci = json_object_get(root, "devices");
					wallet = json_string_value(json_wallet);
					pooladdress = json_string_value(json_pooladdr);
					ncores = json_string_value(json_ncores);
					ndevices = json_integer_value(json_ndevices);


					//check values
					printf("wallet : %s\n", wallet);
					printf("pool address: %s\n", pooladdress);
					printf("ncores : %s\n ndevices : %d\n", ncores, ndevices);

					//set runminer.sh with new wallet and pool
					string miner;
					miner = "echo ./ccminer -a keccak -o stratum+tcp://" + string(pooladdress) + " -u " + string(wallet) + " -p c=MAX,ID=Worker1 > runminer.sh";
					cout << "command : " << miner << endl;
					const char *command = miner.c_str();
					system(command);
					system("cp runminer.sh /root/Downloads/ccminer.linux");

					//set cores.txt and devices.txt
					string cores;
					cores = "echo " + string(ncores) + " > cores.txt";
					const char *core_cmd = cores.c_str();
					system(core_cmd);
					system("cp cores.txt /root/Downloads/ccminer.linux");
					
					ofstream f_device;
					f_device.open ("devices.txt");
					
					for (int i=0; i<ndevices; i++){
						json_t *json_dev = json_array_get(json_devicepci, i);
						const char * devpci;
						devpci = json_string_value(json_dev);
						cout << "dev pci : " << devpci << endl;
						string devs = "/sys/bus/pci/devices/0000:" + string(devpci) + "/resource0";
						f_device << devs << "\n";
						// system("cp devices.txt /root/Miner/ccminer.linux && rm devices.txt"); //for each folder
						
					}
					
					f_device.close();
				}

				//turn on/off miner
				if(strcmp(value,"onoff") == 0){}
				//upload bitstream
				if(strcmp(value,"bit") == 0){
					
				}

				//configuration
				// pseudo code
				// if key x = value y then
				// modify ccminer
				// upload bitstream to x number of board
				// on/off miner

				//monitoring
				//pseudo code
				json_decref(root);

			}

			
		}
		sleep(1);
		// memset(message, 0, sizeof(message)); //clean message
	}
	return 0;
}
