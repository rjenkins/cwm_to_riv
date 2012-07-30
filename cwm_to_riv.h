#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
**	Data Structures 
*/

typedef struct node_t {
        int node_id;
        char *node_name;
        char *node_type;
        int class_name;
        char *snmp_access;
        char *inap_ne_life_counter;
        char *parent;
	char *ifindex;
        int slot;
        int bay;
        int line;
        int port;
        struct node_t *next;
} node_t;

typedef struct farside_node_t {
        int node_id;
        char *node_name;
        int class_name;
} farside_node_t;

typedef struct process_list_t {
        int node_id;
        char *node_parent;
        int is_processed;
        struct process_list_t *next;
} process_list_t;

/*
**	Function Protypes Here
*/

void print_usage(void);
void process_node(void);
void print_topo(node_t *);
void set_node_processed(int);
void add_node_to_process_list(int , char *, process_list_t *);
void fetch_interfaces(int, char *, node_t *);
void free_farside_parent_info(farside_node_t *);
void free_process_list(process_list_t *);
void free_node_list(node_t *);

node_t *fetch_node(int, char *, char *);
node_t *add_node(int, char *, int, char *);
node_t *add_interface(int, int, int, int, node_t *);

int check_process_list(int, process_list_t *);
int lookup_node(int, node_t *);
int lookup_interface(char *, node_t *);

process_list_t *get_next_node_to_process(process_list_t *);

char *add_farside_interface(int, int, int, int, node_t *, farside_node_t *);

farside_node_t *get_farside_parent_info(int, char *);

