/*
 * Industrial I/O software trigger interface
 *
 * Copyright (c) 2015 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#ifndef __IIO_SW_TRIGGER
#define __IIO_SW_TRIGGER

#include <linux/module.h>
#include <linux/device.h>
#include <linux/iio/iio.h>
#include <linux/configfs.h>

#define module_iio_sw_trigger_driver(__iio_sw_trigger_type) \
	module_driver(__iio_sw_trigger_type, iio_register_sw_trigger_type, \
		      iio_unregister_sw_trigger_type)

struct iio_sw_trigger_ops;

struct iio_sw_trigger_type {
	const char *name;
	struct module *owner;
	const struct iio_sw_trigger_ops *ops;
	struct list_head list;
};

struct iio_sw_trigger {
	struct iio_trigger *trigger;
	struct iio_sw_trigger_type *trigger_type;
#ifdef CONFIG_CONFIGFS_FS
	struct config_group group;
#endif
};

struct iio_sw_trigger_ops {
	struct iio_sw_trigger* (*probe)(const char *);
	int (*remove)(struct iio_sw_trigger *);
};

/**
 * iio_register_sw_trigger_type() - register a software trigger type
 *
 * @tt: software trigger type to be registered
 */
int iio_register_sw_trigger_type(struct iio_sw_trigger_type *tt);

/**
 * iio_unregister_sw_trigger_type() - unregister a software trigger type
 *
 * @tt: software trigger type to be unregistered
 */
int iio_unregister_sw_trigger_type(struct iio_sw_trigger_type *tt);

struct iio_sw_trigger *iio_sw_trigger_create(const char *, const char *);
void iio_sw_trigger_destroy(struct iio_sw_trigger *);

#ifdef CONFIG_CONFIGFS_FS
static inline
struct iio_sw_trigger *to_iio_sw_trigger(struct config_item *item)
{
	return container_of(to_config_group(item), struct iio_sw_trigger,
			    group);
}

static inline
void iio_config_group_init_type_name(struct config_group *group,
				     const char *name,
				     struct config_item_type *type)
{
	config_group_init_type_name(group, name, type);
}
#else
static inline
void iio_config_group_init_type_name(struct config_group *group,
				     const char *name,
				     struct config_item_type *type)
{ }
#endif

#endif /* __IIO_SW_TRIGGER */
