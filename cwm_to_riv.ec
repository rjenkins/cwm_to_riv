/*  Author: Ray Jenkins
**  Date: Fri Jan 11 09:18:29 CST 2002
**  File: cwm_to_riv.ec
*/

#include "cwm_to_riv.h"

/*
**      Informix propritiary "define" macros here
*/

EXEC SQL define NODENAME_LEN 32;
EXEC SQL define NETNAME_LEN 10;

/*
** 	Global Variables
*/

EXEC SQL BEGIN DECLARE SECTION; 
	string *dbname;
EXEC SQL END DECLARE SECTION; 

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

        EXEC SQL BEGIN DECLARE SECTION; 
                string node_name[NODENAME_LEN + 1];
                int node_id;
		int model;
                char query[80];
        EXEC SQL END DECLARE SECTION;

	node_t *mynode_t;

	snprintf(query, 79, "select node_name, node_id, model from node where netw_id = %s and node_id = %d",netid, node);

	EXEC SQL prepare demo2id from :query;
	EXEC SQL declare node_cursor cursor for demo2id;

	EXEC SQL open node_cursor;

	for (;;) 
	{ 
		EXEC SQL fetch node_cursor into :node_name, :node_id, :model; 
		if (strncmp(SQLSTATE, "00", 2) != 0) 
			break;

		mynode_t = add_node(node_id, node_name, model, nodeparent);
	}

	if (strncmp(SQLSTATE, "02", 2) != 0) 
		printf("SQLSTATE after fetch is %s\n", SQLSTATE); 

		
	EXEC SQL close node_cursor; 
	EXEC SQL free node_cursor;
	
	set_node_processed(node_id);
	return(mynode_t);
}

void fetch_interfaces(int my_node_id, char *net_id, node_t *mynode_t) {

	node_t *my_farsidenode_t;
	farside_node_t *my_far_info;	
	char *farside_int_name;

	EXEC SQL BEGIN DECLARE SECTION;
		int l_node_id;
                int l_slot;
                int l_bay;
                int l_line;
                int l_port;
                int r_node_id;
                int r_slot;
                int r_bay;
                int r_line;
                int r_port;
        	char query[1024];
	EXEC SQL END DECLARE SECTION;


        snprintf(query, 1023, "select l_node_id, l_slot, l_bay, l_line, l_port, r_node_id, r_slot, r_bay, r_line, r_port  from packet_line where l_node_id = %d or r_node_id = %d and l_network_id = %s", my_node_id, my_node_id, net_id);

        EXEC SQL prepare demo2id1 from :query;
        EXEC SQL declare interface_cursor cursor for demo2id1;

        EXEC SQL open interface_cursor;

	for (;;) 
	{ 
		EXEC SQL fetch interface_cursor into :l_node_id, :l_slot, :l_bay, :l_line, :l_port, :r_node_id, :r_slot, :r_bay, :r_line, :r_port; 
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

	EXEC SQL close interface_cursor; 
	EXEC SQL free interface_cursor;

	process_node();
	
}

farside_node_t *get_farside_parent_info(int node, char *netid)
{

        EXEC SQL BEGIN DECLARE SECTION; 
            	string node_name[NODENAME_LEN + 1];
		int model;
                char query[80];
        EXEC SQL END DECLARE SECTION;


	farside_node_t *my_farside_node_t;
	my_farside_node_t = (farside_node_t *) malloc(sizeof(farside_node_t));
	
	snprintf(query, 79, "select node_name, model from node where netw_id = %s and node_id = %d",netid, node);

	EXEC SQL prepare farside_parent_info from :query;
	EXEC SQL declare farside_parent_info_cursor cursor for farside_parent_info;

	EXEC SQL open farside_parent_info_cursor;

	for (;;) 
	{ 
		EXEC SQL fetch farside_parent_info_cursor into :node_name, :model; 
		if (strncmp(SQLSTATE, "00", 2) != 0) 
			break;
	}

	if (strncmp(SQLSTATE, "02", 2) != 0) 
		printf("SQLSTATE after fetch is %s\n", SQLSTATE); 

	my_farside_node_t->node_id = node;
	my_farside_node_t->node_name = (char *) strdup(node_name);
	my_farside_node_t->class_name = model;

	EXEC SQL close node_cursor; 
	EXEC SQL free node_cursor;
	
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

	EXEC SQL connect to :dbname;
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
