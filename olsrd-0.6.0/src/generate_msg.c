
//������Ϣ�ĺ���or������Ϣ�ĺ���

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

//��������hello��Ϣ
void
generate_hello(void *p)
{
  struct hello_message hellopacket;
  struct interface *ifn = (struct interface *)p;
//����һ����Ϣ
  olsr_build_hello_packet(&hellopacket, ifn);
//����queue_hello����
//���ɴ��в���hellopackaet���ݵ�hello��Ϣ��
//�����ɹ���ͨ��net_output��������Ϣͨ��ifn����
  if (queue_hello(&hellopacket, ifn))
    net_output(ifn);//ifn�Ƿ�����Ϣ�Ľӿ�
//�ͷ���Ϣ��ռ�ڴ�
  olsr_free_hello_packet(&hellopacket);

}

//��������tc��Ϣ
//ͬ��hello��Ϣ
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

//MID��������
void
generate_mid(void *p)

{
  struct interface *ifn = (struct interface *)p;
  //����MID��Ϣ�Ľڵ㽫���ɵ�MID��Ϣ���Ž�һ��������
  //������Ϣ�Ľڵ����ζ�ȡ�����е�MID��Ϣ
  //����ȡ��ĳһ��Ϣʱ
  //�����жϸ���Ϣ�Ƿ����
  //�����ڣ������ȡ��ֱ�Ӷ��������ݰ�

  if (queue_mid(ifn) && TIMED_OUT(ifn->fwdtimer)) {
    set_buffer_timer(ifn);
  }

}
//ͬmid��������
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
