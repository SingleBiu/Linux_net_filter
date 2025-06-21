#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
 
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SingleBiu");
MODULE_DESCRIPTION("Netfliter test");

unsigned int packet_filter(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out,int (*okfn)(struct sk_buff *));

static struct nf_hook_ops packet_simple_nf_opt = {
	.hook = (nf_hookfn *)packet_filter,
	.pf = NFPROTO_INET,
	.hooknum = NF_INET_PRE_ROUTING,
	.priority = NF_IP_PRI_FIRST,
};

static int simple_nf_init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,3,0)
	nf_register_net_hook(&init_net,&packet_simple_nf_opt);
#else
	nf_register_hook(&packet_simple_nf_opt);
#endif
	printk("[simple_nf_test] network hook success.\n");
	return 0;
}

unsigned int packet_filter(unsigned int hooknum,struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
	int ret = NF_DROP;
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct udphdr *udph;

	printk("[simple_nf_test] %s.start.....\n",__func__);

	if(skb == NULL)
		return NF_ACCEPT;
	iph = ip_hdr(skb);
	if(iph == NULL)
		return NF_ACCEPT;
	printk("[simple_nf_test] %s.protocol is [%d].\n",__func__,iph->protocol);
	printk("[simple_nf_test] %s.source addr is [%pI4].\n",__func__,&iph->saddr);
	printk("[simple_nf_test] %s.dest addr os [%pI4].\n",__func__,&iph->daddr);

	switch(iph->protocol)
	{
		case IPPROTO_TCP:
			tcph = (struct tcphdr *)(skb->data + (iph->ihl *4));
			printk("[simple_nf_test] %s. tcp source port is [%d].\n",__func__,ntohs(tcph->source));
			printk("[simple_nf_test] %s. tcp dest port is [%d].\n",__func__,ntohs(tcph->dest));
			break;
		case IPPROTO_UDP:
                        udph = (struct udphdr *)(skb->data + (iph->ihl *4));
                        printk("[simple_nf_test] %s. udp source port is [%d].\n",__func__,ntohs(udph->source));
                        printk("[simple_nf_test] %s. udp dest port is [%d].\n",__func__,ntohs(udph->dest));
                        break;
		default:
			return NF_ACCEPT;
	}
	printk("[simple_nf_test] %s. end.\n\n\n",__func__);
	return NF_ACCEPT;
}

static void simple_nf_exit(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,3,0)
	nf_unregister_net_hook(&init_net,&packet_simple_nf_opt);
#else
	nf_unregister_hook(&packet_simple_nf_opt);
#endif
	printk("[simple_nf_test] remove hook lkm success!\n");
}

module_init(simple_nf_init);
module_exit(simple_nf_exit);
