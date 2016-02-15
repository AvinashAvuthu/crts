#include <stdio.h>
#include <liquid/liquid.h>
#include "CRTS.hpp"
#include "ECR.hpp"
#include "SC_Control_and_Feedback_Test.hpp"

// constructor
SC_Control_and_Feedback_Test::SC_Control_and_Feedback_Test() {}

// destructor
SC_Control_and_Feedback_Test::~SC_Control_and_Feedback_Test() {}

// setup feedback enables for each node
void SC_Control_and_Feedback_Test::initialize_node_fb() {
  printf("Sending control for fb enables\n");
  int fb_enables = CRTS_TX_STATE_FB_EN | CRTS_TX_GAIN_FB_EN | CRTS_RX_STATS_FB_EN;
  set_node_parameters(1, CRTS_FB_EN, (void*) &fb_enables);
  set_node_parameters(2, CRTS_FB_EN, (void*) &fb_enables);

  double rx_stats_period = 1.0;
  set_node_parameters(1, CRTS_RX_STATS, (void*) &rx_stats_period);
  set_node_parameters(2, CRTS_RX_STATS, (void*) &rx_stats_period);
  
  set_node_parameters(1, CRTS_RX_STATS_FB, (void*) &rx_stats_period);
  set_node_parameters(2, CRTS_RX_STATS_FB, (void*) &rx_stats_period);
}

// execute function
void SC_Control_and_Feedback_Test::execute(int node, char fb_type, void *_arg) {

  printf("\n");
  
  // handle all possible feedback types
  switch (fb_type) {
    case CRTS_TX_STATE:
      if (*(int*)_arg == TX_STOPPED)
        printf("Node %i has stopped transmitting\n", node);
      if (*(int*)_arg == TX_CONTINUOUS)
        printf("Node %i has started transmitting\n", node);
      break;
    case CRTS_TX_FREQ:
      printf("Node %i has updated it's transmit frequency to %.1e\n", node, *(double*)_arg);
      break;
    case CRTS_TX_RATE:
      printf("Node %i has updated it's transmit rate to %.3e\n", node, *(double*)_arg);
      break;
    case CRTS_TX_GAIN:
      printf("Node %i has updated it's transmit gain to %.3f\n", node, *(double*)_arg);
      break;
    case CRTS_TX_MOD:
      printf("Node %i has updated it's transmit modulation to %s\n", node, 
             modulation_types[*(int*)_arg].name);
      break;
    case CRTS_TX_FEC0:
      printf("Node %i has updated it's inner FEC scheme  to %s\n", node, 
             fec_scheme_str[*(int*)_arg][0]);
      break;
    case CRTS_TX_FEC1:
      printf("Node %i has updated it's outter FEC scheme to %s\n", node, 
             fec_scheme_str[*(int*)_arg][0]);
      break;
    
    case CRTS_RX_STATE:
      if (*(int*)_arg == RX_STOPPED)
        printf("Node %i has stopped receiving\n", node);
      if (*(int*)_arg == RX_CONTINUOUS)
        printf("Node %i has started receiving\n", node);
      break;
    case CRTS_RX_FREQ:
      printf("Node %i has updated it's receive frequency to %.3e\n", node, *(double*)_arg);
      break;
    case CRTS_RX_RATE:
      printf("Node %i has updated it's receive rate to %.3e\n", node, *(double*)_arg);
      break;
    case CRTS_RX_GAIN:
      printf("Node %i has updated it's receive gain to %.1f\n", node, *(double*)_arg);
      break;
    case CRTS_RX_STATS:
      struct ExtensibleCognitiveRadio::rx_statistics rx_stats = 
        *(struct ExtensibleCognitiveRadio::rx_statistics*) _arg;
      printf("Node %i has sent updated receive statistics:\n", node);
      printf("  Number of frames received: %i\n", rx_stats.frames_received);
      printf("  Average EVM:               %.3f\n", rx_stats.avg_evm);
      printf("  Average RSSI:              %.3f\n", rx_stats.avg_rssi);
      printf("  Average PER:               %.3f\n", rx_stats.avg_per);
      printf("  Average throughput:        %.3e\n", rx_stats.avg_throughput);
      break;
   }
}



