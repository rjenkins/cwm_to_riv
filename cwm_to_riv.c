#include <sqlhdr.h>
#include <sqliapi.h>
static const char *_Cn6 = "farside_parent_info_cursor";
static const char *_Cn5 = "farside_parent_info";
static const char *_Cn4 = "interface_cursor";
static const char *_Cn3 = "demo2id1";
static const char *_Cn2 = "node_cursor";
static const char *_Cn1 = "demo2id";
#line 1 "cwm_to_riv.ec"
/*  Author: Ray Jenkins
**  Date: Fri Jan 11 09:18:29 CST 2002
**  File: cwm_to_riv.ec
**
*/

#include "cwm_to_riv.h"

/*
**      Informix propritiary "define" macros here
*/

/* 
 * EXEC SQL define NODENAME_LEN 32;
 */
#line 14 "cwm_to_riv.ec"

/* 
 * EXEC SQL define NETNAME_LEN 10;
 */
#line 15 "cwm_to_riv.ec"


/*
** 	Global Variables
*/

/*
 * EXEC SQL BEGIN DECLARE SECTION;
 */
#line 21 "cwm_to_riv.ec"
#line 22 "cwm_to_riv.ec"
#line 22 "cwm_to_riv.ec"
  char *dbname;
/*
 * EXEC SQL END DECLARE SECTION;
 */
#line 23 "cwm_to_riv.ec"


node_t *nodelist_start;
process_list_t *my_process_list;

char *ORVMASTER;
char *NETWORK;
char *OUTFILE;
int *ROOTNODE;

void process_node(void)
{

	node_t *mynode;
	process_list_t *my_main_node;

	if((my_main_node = (process_list_t *)get_next_node_to_process(my_process_list)) == NULL)
		return;

	mynode = fetch_node(my_main_node->node_id,NETWORK,my_main_node->node_parent);
	fetch_interfaces(my_main_node->node_id,NETWORK,mynode);

}

node_t *fetch_node(int node, char *netid, char *nodeparent)
{

/*
 *         EXEC SQL BEGIN DECLARE SECTION;
 */
#line 50 "cwm_to_riv.ec"
#line 51 "cwm_to_riv.ec"
#line 51 "cwm_to_riv.ec"
  char node_name[33];
#line 52 "cwm_to_riv.ec"
int node_id;
#line 53 "cwm_to_riv.ec"
int model;
#line 54 "cwm_to_riv.ec"
  char query[80];
/*
 *         EXEC SQL END DECLARE SECTION;
 */
#line 55 "cwm_to_riv.ec"


	node_t *mynode_t;

	snprintf(query, 79, "select node_name, node_id, model from node where netw_id = %s and node_id = %d",netid, node);

/*
 * 	EXEC SQL prepare demo2id from :query;
 */
#line 61 "cwm_to_riv.ec"
  {
#line 61 "cwm_to_riv.ec"
#line 61 "cwm_to_riv.ec"
#line 61 "cwm_to_riv.ec"
  sqli_prep(ESQLINTVERSION, (char *) _Cn1, query,(ifx_literal_t *) 0, (ifx_namelist_t *) 0, -1, 0,0 ); 
#line 61 "cwm_to_riv.ec"
  }
/*
 * 	EXEC SQL declare node_cursor cursor for demo2id;
 */
#line 62 "cwm_to_riv.ec"
  {
#line 62 "cwm_to_riv.ec"
#line 62 "cwm_to_riv.ec"
  sqli_curs_decl_dynm(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn2, 512), (char *) _Cn2, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn1, 513), 0, 0);
#line 62 "cwm_to_riv.ec"
  }

/*
 * 	EXEC SQL open node_cursor;
 */
#line 64 "cwm_to_riv.ec"
  {
#line 64 "cwm_to_riv.ec"
#line 64 "cwm_to_riv.ec"
  sqli_curs_open(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn2, 768), (ifx_sqlda_t *) 0, (char *) 0, (struct value *) 0, 0, 0);
#line 64 "cwm_to_riv.ec"
  }

	for (;;)
	{
/*
 * 		EXEC SQL fetch node_cursor into :node_name, :node_id, :model;
 */
#line 68 "cwm_to_riv.ec"
  {
#line 68 "cwm_to_riv.ec"
  static ifx_sqlvar_t _sqobind[] = 
    {
      { 109, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(node_id), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(model), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
#line 68 "cwm_to_riv.ec"
    };
  static ifx_sqlda_t _SD0 = { 3, _sqobind, {0}, 3, 0 };
  static _FetchSpec _FS1 = { 0, 1, 0 };
#line 68 "cwm_to_riv.ec"
  _sqobind[0].sqldata = node_name;
#line 68 "cwm_to_riv.ec"
  _sqobind[1].sqldata = (char *) &node_id;
#line 68 "cwm_to_riv.ec"
  _sqobind[2].sqldata = (char *) &model;
#line 68 "cwm_to_riv.ec"
  sqli_curs_fetch(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn2, 768), (ifx_sqlda_t *) 0, &_SD0, (char *) 0, &_FS1);
#line 68 "cwm_to_riv.ec"
  }
		if (strncmp(SQLSTATE, "00", 2) != 0)
			break;

		mynode_t = add_node(node_id, node_name, model, nodeparent);
	}

	if (strncmp(SQLSTATE, "02", 2) != 0)
		printf("SQLSTATE after fetch is %s\n", SQLSTATE);

		
/*
 * 	EXEC SQL close node_cursor;
 */
#line 79 "cwm_to_riv.ec"
  {
#line 79 "cwm_to_riv.ec"
#line 79 "cwm_to_riv.ec"
  sqli_curs_close(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn2, 768));
#line 79 "cwm_to_riv.ec"
  }
/*
 * 	EXEC SQL free node_cursor;
 */
#line 80 "cwm_to_riv.ec"
  {
#line 80 "cwm_to_riv.ec"
#line 80 "cwm_to_riv.ec"
  sqli_curs_free(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn2, 770));
#line 80 "cwm_to_riv.ec"
  }
	
	set_node_processed(node_id);
	return(mynode_t);
}

void fetch_interfaces(int my_node_id, char *net_id, node_t *mynode_t) {

	node_t *my_farsidenode_t;
	farside_node_t *my_far_info;	
	char *farside_int_name;

/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 92 "cwm_to_riv.ec"
#line 93 "cwm_to_riv.ec"
#line 93 "cwm_to_riv.ec"
int l_node_id;
#line 94 "cwm_to_riv.ec"
int l_slot;
#line 95 "cwm_to_riv.ec"
int l_bay;
#line 96 "cwm_to_riv.ec"
int l_line;
#line 97 "cwm_to_riv.ec"
int l_port;
#line 98 "cwm_to_riv.ec"
int r_node_id;
#line 99 "cwm_to_riv.ec"
int r_slot;
#line 100 "cwm_to_riv.ec"
int r_bay;
#line 101 "cwm_to_riv.ec"
int r_line;
#line 102 "cwm_to_riv.ec"
int r_port;
#line 103 "cwm_to_riv.ec"
  char query[1024];
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 104 "cwm_to_riv.ec"



        snprintf(query, 1023, "select l_node_id, l_slot, l_bay, l_line, l_port, r_node_id, r_slot, r_bay, r_line, r_port  from packet_line where l_node_id = %d or r_node_id = %d and l_network_id = %s", my_node_id, my_node_id, net_id);

/*
 *         EXEC SQL prepare demo2id1 from :query;
 */
#line 109 "cwm_to_riv.ec"
  {
#line 109 "cwm_to_riv.ec"
#line 109 "cwm_to_riv.ec"
#line 109 "cwm_to_riv.ec"
  sqli_prep(ESQLINTVERSION, (char *) _Cn3, query,(ifx_literal_t *) 0, (ifx_namelist_t *) 0, -1, 0,0 ); 
#line 109 "cwm_to_riv.ec"
  }
/*
 *         EXEC SQL declare interface_cursor cursor for demo2id1;
 */
#line 110 "cwm_to_riv.ec"
  {
#line 110 "cwm_to_riv.ec"
#line 110 "cwm_to_riv.ec"
  sqli_curs_decl_dynm(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn4, 512), (char *) _Cn4, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn3, 513), 0, 0);
#line 110 "cwm_to_riv.ec"
  }

/*
 *         EXEC SQL open interface_cursor;
 */
#line 112 "cwm_to_riv.ec"
  {
#line 112 "cwm_to_riv.ec"
#line 112 "cwm_to_riv.ec"
  sqli_curs_open(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn4, 768), (ifx_sqlda_t *) 0, (char *) 0, (struct value *) 0, 0, 0);
#line 112 "cwm_to_riv.ec"
  }

	for (;;)
	{
/*
 * 		EXEC SQL fetch interface_cursor into :l_node_id, :l_slot, :l_bay, :l_line, :l_port, :r_node_id, :r_slot, :r_bay, :r_line, :r_port;
 */
#line 116 "cwm_to_riv.ec"
  {
#line 116 "cwm_to_riv.ec"
  static ifx_sqlvar_t _sqobind[] = 
    {
      { 102, sizeof(l_node_id), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(l_slot), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(l_bay), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(l_line), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(l_port), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(r_node_id), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(r_slot), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(r_bay), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(r_line), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(r_port), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
#line 116 "cwm_to_riv.ec"
    };
  static ifx_sqlda_t _SD0 = { 10, _sqobind, {0}, 10, 0 };
  static _FetchSpec _FS1 = { 0, 1, 0 };
#line 116 "cwm_to_riv.ec"
  _sqobind[0].sqldata = (char *) &l_node_id;
#line 116 "cwm_to_riv.ec"
  _sqobind[1].sqldata = (char *) &l_slot;
#line 116 "cwm_to_riv.ec"
  _sqobind[2].sqldata = (char *) &l_bay;
#line 116 "cwm_to_riv.ec"
  _sqobind[3].sqldata = (char *) &l_line;
#line 116 "cwm_to_riv.ec"
  _sqobind[4].sqldata = (char *) &l_port;
#line 116 "cwm_to_riv.ec"
  _sqobind[5].sqldata = (char *) &r_node_id;
#line 116 "cwm_to_riv.ec"
  _sqobind[6].sqldata = (char *) &r_slot;
#line 116 "cwm_to_riv.ec"
  _sqobind[7].sqldata = (char *) &r_bay;
#line 116 "cwm_to_riv.ec"
  _sqobind[8].sqldata = (char *) &r_line;
#line 116 "cwm_to_riv.ec"
  _sqobind[9].sqldata = (char *) &r_port;
#line 116 "cwm_to_riv.ec"
  sqli_curs_fetch(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn4, 768), (ifx_sqlda_t *) 0, &_SD0, (char *) 0, &_FS1);
#line 116 "cwm_to_riv.ec"
  }
		if (strncmp(SQLSTATE, "00", 2) != 0)
			break;
		
		if (l_node_id == -1 || r_node_id == -1)
			continue;

		if(l_node_id == my_node_id)
		{
			if(!(my_farsidenode_t = add_interface(l_slot, l_bay, l_line, l_port, mynode_t)))
				continue;
			my_far_info = get_farside_parent_info(r_node_id, net_id);
			farside_int_name = (char *)add_farside_interface(r_slot, r_bay, r_line, r_port, my_farsidenode_t, my_far_info);	
			if(!(check_process_list(my_far_info->node_id, my_process_list)))
			{
				add_node_to_process_list(my_far_info->node_id, farside_int_name, my_process_list);
				free_farside_parent_info(my_far_info);
			}
		}

		else
		{
			if(!(my_farsidenode_t = add_interface(r_slot, r_bay, r_line, r_port, mynode_t)))
				continue;
			my_far_info = get_farside_parent_info(l_node_id, net_id);
			farside_int_name = (char *)add_farside_interface(l_slot, l_bay, l_line, l_port, my_farsidenode_t, my_far_info);	
			if(!(check_process_list(my_far_info->node_id, my_process_list)))
			{
				add_node_to_process_list(my_far_info->node_id, farside_int_name, my_process_list);
				free_farside_parent_info(my_far_info);
			}	
		}	
		
			
	}

	if (strncmp(SQLSTATE, "02", 2) != 0)
		printf("SQLSTATE after fetch is %s\n", SQLSTATE);

/*
 * 	EXEC SQL close interface_cursor;
 */
#line 155 "cwm_to_riv.ec"
  {
#line 155 "cwm_to_riv.ec"
#line 155 "cwm_to_riv.ec"
  sqli_curs_close(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn4, 768));
#line 155 "cwm_to_riv.ec"
  }
/*
 * 	EXEC SQL free interface_cursor;
 */
#line 156 "cwm_to_riv.ec"
  {
#line 156 "cwm_to_riv.ec"
#line 156 "cwm_to_riv.ec"
  sqli_curs_free(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn4, 770));
#line 156 "cwm_to_riv.ec"
  }

	process_node();
	
}

farside_node_t *get_farside_parent_info(int node, char *netid)
{

/*
 *         EXEC SQL BEGIN DECLARE SECTION;
 */
#line 165 "cwm_to_riv.ec"
#line 166 "cwm_to_riv.ec"
#line 166 "cwm_to_riv.ec"
  char node_name[33];
#line 167 "cwm_to_riv.ec"
int model;
#line 168 "cwm_to_riv.ec"
  char query[80];
/*
 *         EXEC SQL END DECLARE SECTION;
 */
#line 169 "cwm_to_riv.ec"



	farside_node_t *my_farside_node_t;
	my_farside_node_t = (farside_node_t *) malloc(sizeof(farside_node_t));
	
	snprintf(query, 79, "select node_name, model from node where netw_id = %s and node_id = %d",netid, node);

/*
 * 	EXEC SQL prepare farside_parent_info from :query;
 */
#line 177 "cwm_to_riv.ec"
  {
#line 177 "cwm_to_riv.ec"
#line 177 "cwm_to_riv.ec"
#line 177 "cwm_to_riv.ec"
  sqli_prep(ESQLINTVERSION, (char *) _Cn5, query,(ifx_literal_t *) 0, (ifx_namelist_t *) 0, -1, 0,0 ); 
#line 177 "cwm_to_riv.ec"
  }
/*
 * 	EXEC SQL declare farside_parent_info_cursor cursor for farside_parent_info;
 */
#line 178 "cwm_to_riv.ec"
  {
#line 178 "cwm_to_riv.ec"
#line 178 "cwm_to_riv.ec"
  sqli_curs_decl_dynm(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn6, 512), (char *) _Cn6, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn5, 513), 0, 0);
#line 178 "cwm_to_riv.ec"
  }

/*
 * 	EXEC SQL open farside_parent_info_cursor;
 */
#line 180 "cwm_to_riv.ec"
  {
#line 180 "cwm_to_riv.ec"
#line 180 "cwm_to_riv.ec"
  sqli_curs_open(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn6, 768), (ifx_sqlda_t *) 0, (char *) 0, (struct value *) 0, 0, 0);
#line 180 "cwm_to_riv.ec"
  }

	for (;;)
	{
/*
 * 		EXEC SQL fetch farside_parent_info_cursor into :node_name, :model;
 */
#line 184 "cwm_to_riv.ec"
  {
#line 184 "cwm_to_riv.ec"
  static ifx_sqlvar_t _sqobind[] = 
    {
      { 109, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 102, sizeof(model), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
#line 184 "cwm_to_riv.ec"
    };
  static ifx_sqlda_t _SD0 = { 2, _sqobind, {0}, 2, 0 };
  static _FetchSpec _FS1 = { 0, 1, 0 };
#line 184 "cwm_to_riv.ec"
  _sqobind[0].sqldata = node_name;
#line 184 "cwm_to_riv.ec"
  _sqobind[1].sqldata = (char *) &model;
#line 184 "cwm_to_riv.ec"
  sqli_curs_fetch(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn6, 768), (ifx_sqlda_t *) 0, &_SD0, (char *) 0, &_FS1);
#line 184 "cwm_to_riv.ec"
  }
		if (strncmp(SQLSTATE, "00", 2) != 0)
			break;
	}

	if (strncmp(SQLSTATE, "02", 2) != 0)
		printf("SQLSTATE after fetch is %s\n", SQLSTATE);

	my_farside_node_t->node_id = node;
	my_farside_node_t->node_name = (char *) strdup(node_name);
	my_farside_node_t->class_name = model;

/*
 * 	EXEC SQL close node_cursor;
 */
#line 196 "cwm_to_riv.ec"
  {
#line 196 "cwm_to_riv.ec"
#line 196 "cwm_to_riv.ec"
  sqli_curs_close(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn2, 768));
#line 196 "cwm_to_riv.ec"
  }
/*
 * 	EXEC SQL free node_cursor;
 */
#line 197 "cwm_to_riv.ec"
  {
#line 197 "cwm_to_riv.ec"
#line 197 "cwm_to_riv.ec"
  sqli_curs_free(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, (char *) _Cn2, 770));
#line 197 "cwm_to_riv.ec"
  }
	
	return(my_farside_node_t);
}



char *add_farside_interface(int slot, int bay, int line, int port, node_t *parent_node_t, farside_node_t *main_node_info)
{
	node_t *new;
	node_t *start = nodelist_start;

	char int_name[80];
	char ifindex[80];

	snprintf(int_name,79,"%s.%d%d%d%d",main_node_info->node_name, slot, bay, line, port);
	snprintf(ifindex,79,"%d%d%d%d", slot, bay, line, port);

	while(start->next != NULL)
		start = start->next;

		new = (node_t *) malloc(sizeof(node_t));
		new->node_id =  (int) main_node_info->node_id;
		new->node_name = (char *) strdup(int_name);
		new->node_type = (char *) strdup("1");
		new->class_name = main_node_info->class_name;
		new->snmp_access = (char *) strdup("NoAccess");
		new->inap_ne_life_counter = (char *) strdup("1");
		new->parent = (char *) strdup(parent_node_t->node_name);
		new->ifindex = (char *) strdup(ifindex);
		new->slot = slot;
		new->bay =  bay;
		new->line =  line;
		new->port =  port;
		new->next = 0x00;
		start->next = new;
		return(new->node_name);
}

node_t *add_interface(int slot, int bay, int line, int port, node_t *parent_node_t)
{
	node_t *new;
	node_t *start = nodelist_start;

	char int_name[80];
	char ifindex[80];

	snprintf(int_name,79,"%s.%d%d%d%d",parent_node_t->node_name, slot, bay, line, port);
	snprintf(ifindex,79,"%d%d%d%d", slot, bay, line, port);
	
	if(lookup_interface(int_name, start))
		return(NULL);
	
	while(start->next != NULL)
		start = start->next;

		new = (node_t *) malloc(sizeof(node_t));
		new->node_id = parent_node_t->node_id;
		new->node_name = (char *) strdup(int_name);
		new->node_type = (char *) strdup("1");
		new->class_name = parent_node_t->class_name;
		new->snmp_access = (char *) strdup("NoAccess");
		new->inap_ne_life_counter = (char *) strdup("1");
		new->parent = (char *) strdup(parent_node_t->node_name);
		new->ifindex = (char *) strdup(ifindex);
		new->slot = slot;
		new->bay =  bay;
		new->line =  line;
		new->port =  port;
		new->next = 0x00;
		start->next = new;
		return(new);
}

node_t *add_node(int node_id, char *node_name, int model, char *nodeparent)
{
	node_t *start = (node_t *)nodelist_start;
	node_t *new;

	if(start->node_name == NULL)		/* First time around, eh? */
	{
		start->node_id = node_id;
		start->node_name = (char *) strdup(node_name);
		start->node_type = (char *) strdup("0");
		start->class_name = model;
		start->snmp_access = (char *) strdup("NoAccess");
		start->inap_ne_life_counter = (char *) strdup("1");
		start->parent = (char *) strdup(nodeparent);
		start->slot = -1;
		start->bay = -1;
		start->line = -1;
		start->port = -1;
		start->next = 0x00;
		return(start);
	}

	else
	{
		while(start->next != NULL)
			start = start->next;

		new = (node_t *) malloc(sizeof(node_t));
		new->node_id = node_id;
		new->node_name = (char *) strdup(node_name);
		new->node_type = (char *) strdup("0");
		new->class_name = model;
		new->snmp_access = (char *) strdup("NoAccess");
		new->inap_ne_life_counter = (char *) strdup("1");
		new->parent = (char *) strdup(nodeparent);
		new->slot = -1;
		new->bay = -1;
		new->line = -1;
		new->port = -1;
		new->next = 0x00;
		start->next = new;
		return(new);
	}
}

int check_process_list(int node_id, process_list_t *start)
{
		for(start; start != NULL; start = start->next)
			if(node_id == start->node_id)
				return 1;
		return NULL;

}

void add_node_to_process_list(int node_id, char *node_parent, process_list_t *start)
{
	process_list_t *new;

	if(start->node_parent == NULL)		/* First time around, eh? */
	{
		start->node_id = node_id;
		start->is_processed = 0;
		start->node_parent = (char *) strdup(node_parent);
		start->next = 0x00;
	}

	else
	{
		while(start->next != NULL)
			start = start->next;

		new = (process_list_t *) malloc(sizeof(process_list_t));
		new->node_id = node_id;
		new->is_processed = 0;
		new->node_parent = (char *) strdup(node_parent);
		new->next = 0x00;
		start->next = new;
	}
}

void set_node_processed(int node_id)
{

	process_list_t *start = my_process_list;

	for(start; start != NULL; start = start->next)
		if(start->node_id == node_id)
			start->is_processed = 1;

}


process_list_t *get_next_node_to_process(process_list_t *start)
{

	for(start; start != NULL; start = start->next)
		if(start->is_processed == 0)
			return(start);		
	return NULL;
}

int lookup_interface(char *node_name, node_t *start)
{

	for(start; start != NULL; start = start->next)
		if(strcmp(start->node_name,node_name) == 0)
				return 1;
	return NULL;
}

int lookup_node(int node_id, node_t *start)
{
	if (start->node_name == NULL)
		return NULL;

	for(start; start != NULL; start = start->next)
		if(node_id == start->node_id)
			if(strcmp(start->node_type,"0") == 0)
				return 1;
	return NULL;
}

void free_farside_parent_info(farside_node_t *myinfo)
{
        free(myinfo->node_name);
	free(myinfo);
}

void free_process_list(process_list_t *start)
{
	for(start; start != NULL; start = start->next)
	{
        	free(start->node_parent);
        	free(start->next);
		free(start);
	}
}

void free_node_list(node_t *start)
{
	for(start; start != NULL; start = start->next)
	{
		free(start->node_name);
		free(start->node_type);
		free(start->snmp_access);	
		free(start->inap_ne_life_counter);
		free(start->parent);
		free(start->next);
		free(start);
	}
}
		

void print_topo(node_t *start)
{

	FILE *fp;

	fp = (FILE *) fopen(OUTFILE,"w");

	for(start; start != NULL; start = start->next)
	{	
		fprintf(fp,"%s:", start->node_name);
		fprintf(fp,"%d:",start->node_id);
		fprintf(fp,"%s:", start->node_type);
		fprintf(fp,"%d:", start->class_name);
		fprintf(fp,"%s:", start->snmp_access);
		fprintf(fp,"%s:",start->inap_ne_life_counter);
		fprintf(fp,"%s",start->parent);
		if(strcmp(start->node_type,"1") == 0)
		{
			fprintf(fp,":%s:",start->ifindex);
			fprintf(fp,"%d:",start->slot);
			fprintf(fp,"%d:",start->bay);
			fprintf(fp,"%d:",start->line);
			fprintf(fp,"%d\n",start->port);
		}

		else
		{
			fprintf(fp,":-1");
			fprintf(fp,":-1");
			fprintf(fp,":-1");
			fprintf(fp,":-1");
			fprintf(fp,":-1");
			fprintf(fp,"\n");	
		}
	}
}

void print_usage()
{
	printf("orv_icwmtoriv: Version 1.0 rjenkins.\n");
	printf("orv_icwmtoriv: \t Usage: <orvMaster hostname> <cwm rootnode ID> <cwm network ID> <CWM database> <outputfile>\n");
	printf("Example - orv_icwmtoriv cwmserver 0 1 stratacom /tmp/cwmimport.DOMAIN.cfg\n");
	exit(10);
}

int main(int argc, char **argv)
{

	if (argc < 6)
	{
		printf("Not enough arguments!\n");
		print_usage();
	}

	if (strcmp(argv[1],"-help") == 0)
		print_usage();

	ORVMASTER = argv[1];
	ROOTNODE = (int *) atoi(argv[2]);
	NETWORK = argv[3];
	dbname = argv[4];
	OUTFILE = argv[5];

/*
 * 	EXEC SQL connect to :dbname;
 */
#line 487 "cwm_to_riv.ec"
  {
#line 487 "cwm_to_riv.ec"
  sqli_connect_open(ESQLINTVERSION, 0, dbname, (char *) 0, (ifx_conn_t *) 0, 0);
#line 487 "cwm_to_riv.ec"
  }
	if(!(strncmp(SQLSTATE, "IX", 2)))
	{
		printf("Error Connecting to %s CWM Database\n",dbname);
		printf("SQLSTATE after attempting to connect is %s\n", SQLSTATE);
		exit(11);
	}
		
	nodelist_start = (node_t *) calloc(1,sizeof(node_t));
	my_process_list = (process_list_t *) calloc(1,sizeof(process_list_t));

	add_node_to_process_list((int)ROOTNODE,ORVMASTER,my_process_list);
	process_node();
	print_topo(nodelist_start);
	free_process_list(my_process_list);
	free_node_list(nodelist_start);
	return 0;
}

#line 504 "cwm_to_riv.ec"
