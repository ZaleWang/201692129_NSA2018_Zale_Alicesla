
//产生消息的函数or生成消息的函数

#include "generate_msg.h"
#include "defs.h"
#include "olsr.h"
#include "build_msg.h"
#include "packet.h"

/*
 * Infomation repositiries
 */
#include "hna_set.h"
#include "mid_set.h"
#include "tc_set.h"
#include "mpr_selector_set.h"
#include "duplicate_set.h"
#include "neighbor_table.h"
#include "link_set.h"
#include "two_hop_neighbor_table.h"
#include "net_olsr.h"

static char pulsedata[] = { '\\', '|', '/', '-' };

#define PULSE_MAX 4
static uint8_t pulse_state = 0;

//用于生成hello消息
void
generate_hello(void *p)
{
  struct hello_message hellopacket;
  struct interface *ifn = (struct interface *)p;
//创建一个消息
  olsr_build_hello_packet(&hellopacket, ifn);
//调用queue_hello函数
//生成带有参数hellopackaet内容的hello消息包
//创建成功则通过net_output函数将消息通过ifn发出
  if (queue_hello(&hellopacket, ifn))
    net_output(ifn);//ifn是发送消息的接口
//释放消息所占内存
  olsr_free_hello_packet(&hellopacket);

}

//用于生成tc消息
//同于hello消息
void
generate_tc(void *p)
{
  struct tc_message tcpacket;
  struct interface *ifn = (struct interface *)p;
  olsr_build_tc_packet(&tcpacket);


  if (queue_tc(&tcpacket, ifn) && TIMED_OUT(ifn->fwdtimer)) {
    set_buffer_timer(ifn);
  }

  olsr_free_tc_packet(&tcpacket);
}

//MID包的生成
void
generate_mid(void *p)

{
  struct interface *ifn = (struct interface *)p;
  //生成MID消息的节点将生成的MID消息包放进一个队列中
  //接收消息的节点依次读取队列中的MID消息
  //当读取到某一消息时
  //首先判断该消息是否过期
  //若过期，则不予读取，直接丢弃该数据包

  if (queue_mid(ifn) && TIMED_OUT(ifn->fwdtimer)) {
    set_buffer_timer(ifn);
  }

}
//同mid包的生成
void
generate_hna(void *p)
{
  struct interface *ifn = (struct interface *)p;

  if (queue_hna(ifn) && TIMED_OUT(ifn->fwdtimer)) {
    set_buffer_timer(ifn);
  }
}

void
generate_stdout_pulse(void *foo __attribute__ ((unused)))
{
  if (olsr_cnf->debug_level == 0)
    return;

  pulse_state = pulse_state == 3 ? 0 : pulse_state + 1;

  printf("%c\r", pulsedata[pulse_state]);

}

/*
 * Local Variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */
