#include "collectd.h"
#include "common.h"
#include "plugin.h"

#include <stdio.h>
#include "cloudPowerManagerAPI.h"

static const char *config_keys[] =
{
    "Host"
};

static int config_keys_num = STATIC_ARRAY_SIZE (config_keys);

static char *config_host = NULL;

static int cloudpower_config (const char *key, const char *value)
{
	if (strcasecmp ("Host", key) == 0)
	{
        char *temp;

        temp = strdup (value);
        if (temp == NULL)
        {
            ERROR("CloudPower plugin: Host strdup failed.");
            return (1);
        }
        sfree (config_host);
        config_host = temp;
	}

	return (0);
}

static void cloudpower_submit (char *type, char *type_instance, double value)
{
	value_t values[1];
	value_list_t vl = VALUE_LIST_INIT;

	values[0].gauge = value;

	vl.values = values;
	vl.values_len = 1;
	sstrncpy (vl.host, hostname_g, sizeof (vl.host));
	sstrncpy (vl.plugin, "cloudpower", sizeof (vl.plugin));
	sstrncpy (vl.plugin_instance, "", sizeof (vl.plugin_instance));
	sstrncpy (vl.type, type, sizeof (vl.type));
	sstrncpy (vl.type_instance, type_instance, sizeof (vl.type_instance));

	plugin_dispatch_values (&vl);
}

static int cloudpower_read (void)
{
    char* electric_status;
    char* token;
    double electriCurrent, kwhrTotal;

    electric_status = getElectric(config_host);

    token = strtok(electric_status, " ");
    electriCurrent = atoi(token) * 0.1;

    token = strtok(NULL, " ");
    kwhrTotal = atoi(token) * 0.1;

	cloudpower_submit ("electric", "A", electriCurrent);
	cloudpower_submit ("electric", "kwhr", kwhrTotal);

	return (0);
}

static int cloudpower_shutdown (void)
{
    sfree (config_host);

    return (0);
}

void module_register (void)
{
	plugin_register_config ("cloudpower", cloudpower_config, config_keys, config_keys_num);
	plugin_register_read ("cloudpower", cloudpower_read);
    plugin_register_shutdown ("cloudpower", cloudpower_shutdown);
}
