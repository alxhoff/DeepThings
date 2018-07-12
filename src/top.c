#include "darkiot.h"
#include "configure.h"
#include "cmd_line_parser.h"
#include <string.h>
#include "deepthings_edge.h"
#include "deepthings_gateway.h"
/*
./deepthings -mode start
./deepthings -mode gateway -total_edge 6 -n 5 -m 5 -l 16
./deepthings -mode data_src -edge_id 0 -n 5 -m 5 -l 16
./deepthings -mode non_data_src -edge_id 1 -n 5 -m 5 -l 16
./deepthings -mode <execution mode> 
             -total_edge <total edge number> 
             -edge_id <edge device ID>
             -n <FTP dimension N> 
             -m <FTP dimension M> 
             -l <numder of fused layers>
*/

/*"models/yolo.cfg", "models/yolo.weights"*/

int main(int argc, char **argv){
   total_cli_num = 0;
   this_cli_id = 0;

   uint32_t partitions_h = get_int_arg(argc, argv, "-n", 5);
   uint32_t partitions_w = get_int_arg(argc, argv, "-m", 5);
   uint32_t fused_layers = get_int_arg(argc, argv, "-l", 16);

   char network_file[30] = "models/yolo.cfg";
   char weight_file[30] = "models/yolo.weights";

   if(0 == strcmp(get_string_arg(argc, argv, "-mode", "none"), "start")){  
      printf("start\n");
      exec_start_gateway(START_CTRL, TCP);
   }else if(0 == strcmp(get_string_arg(argc, argv, "-mode", "none"), "gateway")){
      printf("Gateway device\n");
      printf("We have %d edge devices now\n", get_int_arg(argc, argv, "-total_edge", 0));
      total_cli_num = get_int_arg(argc, argv, "-total_edge", 0);
      deepthings_gateway(partitions_h, partitions_w, fused_layers, network_file, weight_file);
   }else if(0 == strcmp(get_string_arg(argc, argv, "-mode", "none"), "data_src")){
      printf("Data source edge device\n");
      printf("This client ID is %d\n", get_int_arg(argc, argv, "-edge_id", 0));
      this_cli_id = get_int_arg(argc, argv, "-edge_id", 0);
      deepthings_victim_edge(partitions_h, partitions_w, fused_layers, network_file, weight_file);
   }else if(0 == strcmp(get_string_arg(argc, argv, "-mode", "none"), "non_data_src")){
      printf("Idle edge device\n");
      printf("This client ID is %d\n", get_int_arg(argc, argv, "-edge_id", 0));
      this_cli_id = get_int_arg(argc, argv, "-edge_id", 0);
      deepthings_stealer_edge(partitions_h, partitions_w, fused_layers, network_file, weight_file);
   }
   return 0;
}

