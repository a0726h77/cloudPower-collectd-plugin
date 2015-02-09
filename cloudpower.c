#include "collectd.h"
#include "common.h"
#include "plugin.h"

#include <stdio.h>
#include "cloudPowerManagerAPI.h"

#define MAX_NODE_NAME 64

#ifndef HOST_NAME_MAX
# define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
#endif

/* CloudPower plugin configuration example:
 *
 * <Plugin cloudpower>
 *   <Node "1FRoom">
 *     Host "192.168.2.100"
 *   </Node>
 * </Plugin>
 */

struct cloudpower_node_s;
typedef struct cloudpower_node_s cloudpower_node_t;
struct cloudpower_node_s
{
    char name[MAX_NODE_NAME];
    char host[HOST_NAME_MAX];

    cloudpower_node_t *next;
};

static cloudpower_node_t *nodes_head = NULL;

static int cloudpower_node_add (const cloudpower_node_t *cpn)
{
    cloudpower_node_t *cpn_copy;
    cloudpower_node_t *cpn_ptr;

    /* Check for duplicates first */
    for (cpn_ptr = nodes_head; cpn_ptr != NULL; cpn_ptr = cpn_ptr->next)
    {
        if (strcmp (cpn->name, cpn_ptr->name) == 0)
        {
            break;
        }
    }

    if (cpn_ptr != NULL)
    {
        ERROR ("cloudpower plugin: A node with the name `%s' already exists.", cpn->name);
        return (-1);
    }

    cpn_copy = malloc (sizeof (*cpn_copy));
    if (cpn_copy == NULL)
    {
        ERROR ("cloudpower plugin: malloc failed adding cloudpower_node to the tree.");
        return (-1);
    }

    memcpy (cpn_copy, cpn, sizeof (*cpn_copy));
    cpn_copy->next = NULL;

    DEBUG ("cloudpower plugin: Adding node \"%s\".", cpn->name);

    if (nodes_head == NULL)
    {
        nodes_head = cpn_copy;
    }
    else
    {
        cpn_ptr = nodes_head;
        while (cpn_ptr->next != NULL)
        {
            cpn_ptr = cpn_ptr->next;
        }
        cpn_ptr->next = cpn_copy;
    }

    return (0);
}

static int cloudpower_config_node (oconfig_item_t *ci)
{
    cloudpower_node_t cpn;
    int i;
    int status;

    memset (&cpn, 0, sizeof (cpn));

    status = cf_util_get_string_buffer (ci, cpn.name, sizeof (cpn.name));
    if (status != 0)
    {
        return (status);
    }

    for (i = 0; i < ci->children_num; i++)
    {
        oconfig_item_t *option = ci->children + i;

        if (strcasecmp ("Host", option->key) == 0)
        {
            status = cf_util_get_string_buffer (option, cpn.host, sizeof (cpn.host));
        }
        else
        {
            WARNING ("cloudpower plugin: Option `%s' not allowed inside a `Node' block. I'll ignore this option.", option->key);
        }

        if (status != 0)
        break;
    }

    if (status != 0)
    {
        return (status);
    }

    return (cloudpower_node_add (&cpn));
}

static int cloudpower_config (oconfig_item_t *ci)
{
    int i;

    for (i = 0; i < ci->children_num; i++)
    {
        oconfig_item_t *option = ci->children + i;

        if (strcasecmp ("Node", option->key) == 0)
        {
            cloudpower_config_node (option);
        }
        else
        {
            WARNING ("cloudpower plugin: Option `%s' not allowed in cloudpower configuration. It will be ignored.", option->key);
        }
    }

    if (nodes_head == NULL)
    {
        ERROR ("cloudpower plugin: No valid node configuration could be found.");
        return (ENOENT);
    }

    return (0);
}

static void cloudpower_submit (char *plugin_instance, char *type, char *type_instance, double value)
{
	value_t values[1];
	value_list_t vl = VALUE_LIST_INIT;

	values[0].gauge = value;

	vl.values = values;
	vl.values_len = 1;
	sstrncpy (vl.host, hostname_g, sizeof (vl.host));
	sstrncpy (vl.plugin, "cloudpower", sizeof (vl.plugin));
	sstrncpy (vl.plugin_instance, plugin_instance, sizeof (vl.plugin_instance));
	sstrncpy (vl.type, type, sizeof (vl.type));
	sstrncpy (vl.type_instance, type_instance, sizeof (vl.type_instance));

	plugin_dispatch_values (&vl);
}

static int cloudpower_read (void)
{
    char* electric_status;
    char* token;
    double electriCurrent, kwhrTotal;

    cloudpower_node_t *cpn;

    for (cpn = nodes_head; cpn != NULL; cpn = cpn->next)
    {
        electric_status = getElectric(cpn->host);

        token = strtok(electric_status, " ");
        electriCurrent = atoi(token) * 0.1;

        token = strtok(NULL, " ");
        kwhrTotal = atoi(token) * 0.1;

        cloudpower_submit (cpn->name, "electric", "A", electriCurrent);
        cloudpower_submit (cpn->name, "electric", "kwhr", kwhrTotal);
    }

	return (0);
}

void module_register (void)
{
    plugin_register_complex_config ("cloudpower", cloudpower_config);
	plugin_register_read ("cloudpower", cloudpower_read);
}
